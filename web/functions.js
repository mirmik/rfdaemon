const appTexts = [];
const appStateCache = [];

const THEMES = [
    { id: "retrowave", name: "Retrowave", icon: "◐" },
    { id: "neon-orange", name: "Neon Orange", icon: "◑" }
];

let currentThemeIndex = 0;
let selectedAppIndex = null;
let logMode = "snapshot";
let logPollingTimer = null;

function initTheme() {
    const savedTheme = localStorage.getItem("rfdaemon-theme");
    if (savedTheme) {
        const index = THEMES.findIndex((theme) => theme.id === savedTheme);
        if (index !== -1) {
            currentThemeIndex = index;
        }
    }
    applyTheme();
}

function applyTheme() {
    const theme = THEMES[currentThemeIndex];
    document.documentElement.setAttribute("data-theme", theme.id);

    const label = document.getElementById("theme-label");
    const icon = document.querySelector(".theme-switcher-icon");
    if (label) label.textContent = theme.name;
    if (icon) icon.textContent = theme.icon;

    localStorage.setItem("rfdaemon-theme", theme.id);
}

function toggleTheme() {
    currentThemeIndex = (currentThemeIndex + 1) % THEMES.length;
    applyTheme();
}

function makeButton(text, onClick, className = "") {
    const button = document.createElement("button");
    button.textContent = text;
    button.className = `btn btn-sm ${className}`;
    button.onclick = (event) => {
        event.stopPropagation();
        onClick();
    };
    return button;
}

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

function stateClassFor(state) {
    const stateLower = String(state || "unknown").toLowerCase();
    if (stateLower === "running" || stateLower === "started") {
        return "status-running";
    }
    if (stateLower === "stopped" || stateLower === "exited") {
        return "status-stopped";
    }
    return "status-starting";
}

function updateStatusDisplay(textElement, name, state) {
    textElement.textContent = state || "unknown";
    textElement.className = `app-status ${stateClassFor(state)}`;
    textElement.dataset.name = name || "";
}

function updateMetaSummary() {
    const appsCount = document.getElementById("apps-count");
    const runningCount = document.getElementById("running-count");
    const selectionSummary = document.getElementById("selection-summary");
    const logModeSummary = document.getElementById("log-mode-summary");

    const running = appStateCache.filter((app) =>
        String(app.state || "").toLowerCase() === "running"
    ).length;

    if (appsCount) appsCount.textContent = String(appStateCache.length);
    if (runningCount) runningCount.textContent = String(running);

    if (selectionSummary) {
        if (selectedAppIndex === null || !appStateCache[selectedAppIndex]) {
            selectionSummary.textContent = "No application selected";
        } else {
            const app = appStateCache[selectedAppIndex];
            selectionSummary.textContent = `${app.name} / ${app.state || "unknown"}`;
        }
    }

    if (logModeSummary) {
        logModeSummary.textContent = logMode === "live" ? "Live Tail (polling)" : "Snapshot";
    }
}

function renderApps() {
    const container = document.getElementById("divtext");
    const filterValue = (document.getElementById("app-filter")?.value || "").trim().toLowerCase();

    container.innerHTML = "";
    appTexts.length = 0;

    const visibleApps = appStateCache
        .map((app, index) => ({ app, index }))
        .filter(({ app }) => {
            if (!filterValue) return true;
            return `${app.name || ""} ${app.command || ""}`.toLowerCase().includes(filterValue);
        });

    if (visibleApps.length === 0) {
        const empty = document.createElement("div");
        empty.className = "app-card";
        empty.innerHTML = '<div class="app-name">No matches</div><div class="app-status status-stopped">Adjust filter or reload application list.</div>';
        container.appendChild(empty);
        return;
    }

    visibleApps.forEach(({ app, index }) => {
        const card = document.createElement("article");
        card.className = "app-card";
        if (index === selectedAppIndex) {
            card.classList.add("is-selected");
        }
        card.onclick = () => selectApp(index);

        const top = document.createElement("div");
        top.className = "app-card-top";

        const info = document.createElement("div");
        const title = document.createElement("div");
        title.className = "app-name";
        title.textContent = app.name || `App ${index}`;
        info.appendChild(title);

        const status = document.createElement("div");
        updateStatusDisplay(status, app.name, app.state);
        info.appendChild(status);
        appTexts[index] = status;

        const meta = document.createElement("div");
        meta.className = "app-meta";
        meta.textContent = `PID ${app.pid || 0}`;

        top.appendChild(info);
        top.appendChild(meta);

        const bottom = document.createElement("div");
        bottom.className = "app-card-bottom";

        const command = document.createElement("div");
        command.className = "app-command";
        command.textContent = app.command || "";
        command.title = app.command || "";

        const controls = document.createElement("div");
        controls.className = "app-controls";
        controls.appendChild(makeButton("Stop", () => stopApp(index), "btn-danger"));
        controls.appendChild(makeButton("Start", () => startApp(index), "btn-success"));
        controls.appendChild(makeButton("Restart", () => restartApp(index), "btn-warning"));
        controls.appendChild(makeButton("Logs", () => showSnapshot(index), "btn-info"));
        controls.appendChild(makeButton("Delete", () => deleteApp(index), "btn-danger"));

        bottom.appendChild(command);
        bottom.appendChild(controls);

        card.appendChild(top);
        card.appendChild(bottom);
        container.appendChild(card);
    });
}

function updateSelectionPanel() {
    const name = document.getElementById("selected-app-name");
    const state = document.getElementById("selected-app-state");
    const service = document.getElementById("selected-app-service");
    const pid = document.getElementById("selected-app-pid");
    const command = document.getElementById("selected-app-command");
    const hint = document.getElementById("log-hint");

    const buttons = [
        document.getElementById("selected-start"),
        document.getElementById("selected-stop"),
        document.getElementById("selected-restart"),
        document.getElementById("selected-refresh")
    ];

    const snapshotButton = document.getElementById("snapshot-button");
    const liveButton = document.getElementById("live-button");

    if (snapshotButton) snapshotButton.classList.toggle("is-active", logMode === "snapshot");
    if (liveButton) liveButton.classList.toggle("is-active", logMode === "live");

    if (selectedAppIndex === null || !appStateCache[selectedAppIndex]) {
        if (name) name.textContent = "Terminal Output";
        if (state) state.textContent = "Choose an application from the left panel.";
        if (service) service.textContent = "-";
        if (pid) pid.textContent = "-";
        if (command) command.textContent = "Select an application to inspect its command line and logs.";
        if (hint) hint.textContent = "Snapshot loads the latest journal slice.";
        buttons.forEach((button) => {
            if (button) button.disabled = true;
        });
        updateMetaSummary();
        return;
    }

    const app = appStateCache[selectedAppIndex];
    if (name) name.textContent = app.name || `App ${selectedAppIndex}`;
    if (state) {
        state.textContent = app.state || "unknown";
        state.className = `selection-state ${stateClassFor(app.state)}`;
    }
    if (service) service.textContent = `rfd-${app.name || selectedAppIndex}`;
    if (pid) pid.textContent = String(app.pid || 0);
    if (command) command.textContent = app.command || "";
    if (hint) {
        hint.textContent =
            logMode === "live"
                ? "Live Tail uses periodic HTTP refresh of the current journal slice."
                : "Snapshot loads the latest journal slice on demand.";
    }

    buttons.forEach((button) => {
        if (button) button.disabled = false;
    });
    updateMetaSummary();
}

function writeLog(text) {
    const logArea = document.getElementById("log_area");
    if (!logArea) return;

    const stickToBottom =
        logArea.scrollTop + logArea.clientHeight >= logArea.scrollHeight - 24;

    logArea.value = text;

    if (stickToBottom || logMode === "live") {
        logArea.scrollTop = logArea.scrollHeight;
    }
}

async function fetchLogText(index) {
    const response = await httpGet(`get_logs.action?index=${index}`);
    const json = JSON.parse(response);
    return atob(json.stdout);
}

async function refreshSelectedLogs() {
    if (selectedAppIndex === null) return;

    try {
        const log = await fetchLogText(selectedAppIndex);
        writeLog(log);
    } catch (error) {
        console.error(`Failed to retrieve logs for application ${selectedAppIndex}:`, error);
        writeLog(`Error: Failed to retrieve logs for application ${selectedAppIndex}`);
    }
}

function stopLogPolling() {
    if (logPollingTimer !== null) {
        clearInterval(logPollingTimer);
        logPollingTimer = null;
    }
}

function startLogPolling() {
    stopLogPolling();
    if (logMode !== "live" || selectedAppIndex === null) {
        return;
    }

    refreshSelectedLogs();
    logPollingTimer = setInterval(() => {
        if (selectedAppIndex !== null) {
            refreshSelectedLogs();
        }
    }, 1200);
}

function setLogMode(mode) {
    logMode = mode;
    updateSelectionPanel();
    if (mode === "live") {
        startLogPolling();
    } else {
        stopLogPolling();
    }
}

async function showSnapshot(index) {
    selectApp(index);
    setLogMode("snapshot");
    await refreshSelectedLogs();
}

function selectApp(index) {
    selectedAppIndex = index;
    renderApps();
    updateSelectionPanel();
    if (logMode === "live") {
        startLogPolling();
    }
}

function triggerSelectedAction(action) {
    if (selectedAppIndex === null) return;

    if (action === "start") return startApp(selectedAppIndex);
    if (action === "stop") return stopApp(selectedAppIndex);
    if (action === "restart") return restartApp(selectedAppIndex);
}

async function init_function() {
    try {
        const response = await httpGet("apps_full_state.json");
        const json = JSON.parse(response);

        appStateCache.length = 0;
        json.apps.forEach((app) => appStateCache.push(app));

        if (selectedAppIndex !== null && !appStateCache[selectedAppIndex]) {
            selectedAppIndex = null;
            stopLogPolling();
        }

        renderApps();
        updateSelectionPanel();
    } catch (error) {
        console.error("Failed to initialize apps:", error);
        const container = document.getElementById("divtext");
        container.innerHTML = '<div class="app-card"><div class="app-name">Connection Error</div><div class="app-status status-stopped">Failed to load applications. Check HTTP server.</div></div>';
    }
}

function state_update_loop() {
    setInterval(async () => {
        try {
            const response = await httpGet("apps_state.json");
            const json = JSON.parse(response);

            json.apps.forEach((app, index) => {
                if (!appStateCache[index]) {
                    appStateCache[index] = app;
                } else {
                    appStateCache[index].name = app.name;
                    appStateCache[index].state = app.state;
                    appStateCache[index].pid = app.pid;
                }

                if (appTexts[index]) {
                    updateStatusDisplay(appTexts[index], app.name, app.state);
                }
            });

            updateSelectionPanel();
        } catch (error) {
            console.error("Failed to update state:", error);
        }
    }, 750);
}

async function stop_all() {
    try {
        await httpGet("stop_all.action");
    } catch (error) {
        console.error("Failed to stop all:", error);
    }
}

async function start_all() {
    try {
        await httpGet("start_all.action");
    } catch (error) {
        console.error("Failed to start all:", error);
    }
}

async function stopApp(index) {
    try {
        await httpGet(`stop.action?index=${index}`);
    } catch (error) {
        console.error(`Failed to stop app ${index}:`, error);
    }
}

async function startApp(index) {
    try {
        await httpGet(`start.action?index=${index}`);
    } catch (error) {
        console.error(`Failed to start app ${index}:`, error);
    }
}

async function restartApp(index) {
    try {
        await httpGet(`restart.action?index=${index}`);
    } catch (error) {
        console.error(`Failed to restart app ${index}:`, error);
    }
}

async function addApp() {
    const name = prompt("Application name:");
    if (!name) return;

    const command = prompt("Command to execute:");
    if (!command) return;

    try {
        await httpPost("app_add.action", {
            name,
            command,
            restart: "always"
        });
        await init_function();
    } catch (error) {
        console.error("Failed to add app:", error);
        alert("Failed to add application");
    }
}

async function deleteApp(index) {
    if (!confirm("Delete this application?")) return;

    try {
        await httpGet(`app_delete.action?index=${index}`);
        if (selectedAppIndex === index) {
            selectedAppIndex = null;
            stopLogPolling();
            writeLog("");
        } else if (selectedAppIndex !== null && selectedAppIndex > index) {
            selectedAppIndex -= 1;
        }
        await init_function();
    } catch (error) {
        console.error(`Failed to delete app ${index}:`, error);
        alert("Failed to delete application");
    }
}

async function saveConfig() {
    try {
        await httpGet("save_config.action");
        alert("Configuration saved.");
    } catch (error) {
        console.error("Failed to save config:", error);
        alert("Failed to save configuration");
    }
}
