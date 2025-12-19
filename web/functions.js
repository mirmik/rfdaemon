// Application state storage
const appTexts = [];

// Available themes
const THEMES = [
    { id: 'retrowave', name: 'Retrowave', icon: '◐' },
    { id: 'neon-orange', name: 'Neon Orange', icon: '◑' }
];
let currentThemeIndex = 0;

/**
 * Initializes theme from localStorage or default
 */
function initTheme() {
    const savedTheme = localStorage.getItem('rfdaemon-theme');
    if (savedTheme) {
        const index = THEMES.findIndex(t => t.id === savedTheme);
        if (index !== -1) {
            currentThemeIndex = index;
        }
    }
    applyTheme();
}

/**
 * Applies the current theme to the document
 */
function applyTheme() {
    const theme = THEMES[currentThemeIndex];
    document.documentElement.setAttribute('data-theme', theme.id);
    
    const label = document.getElementById('theme-label');
    const icon = document.querySelector('.theme-switcher-icon');
    if (label) label.textContent = theme.name;
    if (icon) icon.textContent = theme.icon;
    
    localStorage.setItem('rfdaemon-theme', theme.id);
}

/**
 * Toggles to the next theme
 */
function toggleTheme() {
    currentThemeIndex = (currentThemeIndex + 1) % THEMES.length;
    applyTheme();
}

/**
 * Creates a styled button element
 */
function makeButton(text, onClick, className = '') {
    const button = document.createElement("button");
    button.textContent = text;
    button.className = `btn btn-sm ${className}`;
    button.onclick = onClick;
    return button;
}

/**
 * Performs an async HTTP GET request
 */
function httpGet(url, async = true) {
    return new Promise((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        xhr.open("GET", url, async);
        xhr.onload = () => {
            if (xhr.status >= 200 && xhr.status < 300) {
                resolve(xhr.responseText);
            } else {
                reject(new Error(`HTTP ${xhr.status}: ${xhr.statusText}`));
            }
        };
        xhr.onerror = () => reject(new Error("Network error"));
        xhr.send(null);
    });
}

/**
 * Performs an async HTTP POST request
 */
function httpPost(url, data) {
    return new Promise((resolve, reject) => {
        const xhr = new XMLHttpRequest();
        xhr.open("POST", url, true);
        xhr.setRequestHeader("Content-Type", "application/json");
        xhr.onload = () => {
            if (xhr.status >= 200 && xhr.status < 300) {
                resolve(xhr.responseText);
            } else {
                reject(new Error(`HTTP ${xhr.status}: ${xhr.statusText}`));
            }
        };
        xhr.onerror = () => reject(new Error("Network error"));
        xhr.send(JSON.stringify(data));
    });
}

/**
 * Updates the status text with appropriate styling
 */
function updateStatusDisplay(textElement, name, state) {
    textElement.textContent = `${name}: ${state}`;
    textElement.className = 'app-status';
    
    const stateLower = state.toLowerCase();
    if (stateLower === 'running' || stateLower === 'started') {
        textElement.classList.add('status-running');
    } else if (stateLower === 'stopped' || stateLower === 'exited') {
        textElement.classList.add('status-stopped');
    } else {
        textElement.classList.add('status-starting');
    }
}

/**
 * Initializes the application list from server
 */
async function init_function() {
    try {
        const response = await httpGet("apps_full_state.json");
        const json = JSON.parse(response);
        const container = document.getElementById("divtext");
        container.innerHTML = "";
        appTexts.length = 0;

        json.apps.forEach((app, index) => {
            const row = document.createElement("div");

            // Status label
            const statusLabel = document.createElement("span");
            statusLabel.className = 'app-status';
            updateStatusDisplay(statusLabel, app.name || `App ${index}`, app.state || 'unknown');
            appTexts.push(statusLabel);
            row.appendChild(statusLabel);

            // Control buttons
            const controls = document.createElement("div");
            controls.className = 'app-controls';
            controls.appendChild(makeButton("Stop", () => stopApp(index), 'btn-danger'));
            controls.appendChild(makeButton("Start", () => startApp(index), 'btn-success'));
            controls.appendChild(makeButton("Restart", () => restartApp(index), 'btn-warning'));
            controls.appendChild(makeButton("Logs", () => getStdout(index), 'btn-info'));
            controls.appendChild(makeButton("×", () => deleteApp(index), 'btn-danger'));
            row.appendChild(controls);

            // Command label
            const commandLabel = document.createElement("span");
            commandLabel.className = 'app-command';
            commandLabel.textContent = app.command || '';
            commandLabel.title = app.command || '';
            row.appendChild(commandLabel);

            container.appendChild(row);
        });
    } catch (error) {
        console.error("Failed to initialize apps:", error);
        const container = document.getElementById("divtext");
        container.innerHTML = '<div style="color: #e74c3c; padding: 20px;">Failed to load applications. Check server connection.</div>';
    }
}

/**
 * Periodically updates application states
 */
function state_update_loop() {
    setInterval(async () => {
        try {
            const response = await httpGet("apps_state.json");
            const json = JSON.parse(response);

            json.apps.forEach((app, index) => {
                if (appTexts[index]) {
                    updateStatusDisplay(appTexts[index], app.name, app.state);
                }
            });
        } catch (error) {
            console.error("Failed to update state:", error);
        }
    }, 500);
}

/**
 * Stops all applications
 */
async function stop_all() {
    try {
        await httpGet("stop_all.action");
    } catch (error) {
        console.error("Failed to stop all:", error);
    }
}

/**
 * Starts all applications
 */
async function start_all() {
    try {
        await httpGet("start_all.action");
    } catch (error) {
        console.error("Failed to start all:", error);
    }
}

/**
 * Stops a specific application by index
 */
async function stopApp(index) {
    try {
        await httpGet(`stop.action?index=${index}`);
    } catch (error) {
        console.error(`Failed to stop app ${index}:`, error);
    }
}

/**
 * Starts a specific application by index
 */
async function startApp(index) {
    try {
        await httpGet(`start.action?index=${index}`);
    } catch (error) {
        console.error(`Failed to start app ${index}:`, error);
    }
}

/**
 * Restarts a specific application by index
 */
async function restartApp(index) {
    try {
        await httpGet(`restart.action?index=${index}`);
    } catch (error) {
        console.error(`Failed to restart app ${index}:`, error);
    }
}

/**
 * Retrieves and displays stdout logs for an application
 */
async function getStdout(index) {
    try {
        const response = await httpGet(`get_logs.action?index=${index}`);
        const json = JSON.parse(response);
        const log = atob(json.stdout);
        const logArea = document.getElementById("log_area");
        logArea.value = log;
        logArea.scrollTop = logArea.scrollHeight;
    } catch (error) {
        console.error(`Failed to get logs for app ${index}:`, error);
        const logArea = document.getElementById("log_area");
        logArea.value = `Error: Failed to retrieve logs for application ${index}`;
    }
}

/**
 * Adds a new application
 */
async function addApp() {
    const name = prompt("Application name:");
    if (!name) return;

    const command = prompt("Command to execute:");
    if (!command) return;

    try {
        await httpPost("app_add.action", {
            name: name,
            command: command,
            restart: "always"
        });
        await init_function();
    } catch (error) {
        console.error("Failed to add app:", error);
        alert("Failed to add application");
    }
}

/**
 * Deletes an application by index
 */
async function deleteApp(index) {
    if (!confirm("Delete this application?")) return;

    try {
        await httpGet(`app_delete.action?index=${index}`);
        await init_function();
    } catch (error) {
        console.error(`Failed to delete app ${index}:`, error);
        alert("Failed to delete application");
    }
}

/**
 * Saves current configuration to disk
 */
async function saveConfig() {
    try {
        await httpGet("save_config.action");
        alert("Configuration saved!");
    } catch (error) {
        console.error("Failed to save config:", error);
        alert("Failed to save configuration");
    }
}