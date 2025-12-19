let apps = [];
let selectedIndex = null;

// Initialize the application
async function init() {
    await loadApps();
    renderApps();
    if (apps.length > 0) {
        selectApp(0);
    }
    startStatusLoop();
}

// Load apps from server
async function loadApps() {
    try {
        const resp = await fetch('/apps_full_state.json');
        const data = await resp.json();
        apps = data.apps || [];
    } catch (e) {
        console.error('Failed to load apps:', e);
        apps = [];
    }
}

// Render the apps list
function renderApps() {
    const container = document.getElementById('apps-list');
    if (!container) return;

    if (apps.length === 0) {
        container.innerHTML = '<div class="empty-state"><p>No applications configured</p></div>';
        return;
    }

    container.innerHTML = apps.map((app, i) => `
        <div class="app-card ${app.state === 'running' ? 'running' : 'stopped'} ${i === selectedIndex ? 'selected' : ''}"
             onclick="selectApp(${i})">
            <div class="app-header">
                <span class="status-indicator">${app.state === 'running' ? '●' : '○'}</span>
                <span class="app-name">${escapeHtml(app.name)}</span>
            </div>
            <div class="app-info">
                ${app.state}${app.pid > 0 ? ' | pid: ' + app.pid : ''}
            </div>
            <div class="app-command" title="${escapeHtml(app.command)}">
                ${escapeHtml(app.command)}
            </div>
            <div class="app-buttons" onclick="event.stopPropagation()">
                ${app.state === 'running'
                    ? `<button class="secondary" onclick="stopApp(${i})">Stop</button>`
                    : `<button class="success" onclick="startApp(${i})">Start</button>`}
                <button class="secondary" onclick="restartApp(${i})">Restart</button>
                <button class="danger" onclick="deleteApp(${i})">Delete</button>
            </div>
        </div>
    `).join('');
}

// Select an app and show details
function selectApp(index) {
    selectedIndex = index;
    renderApps();
    renderDetails();
}

// Render details panel
function renderDetails() {
    const emptyState = document.getElementById('empty-state');
    const appDetails = document.getElementById('app-details');

    if (selectedIndex === null || selectedIndex >= apps.length) {
        emptyState.style.display = 'flex';
        appDetails.style.display = 'none';
        return;
    }

    emptyState.style.display = 'none';
    appDetails.style.display = 'block';

    const app = apps[selectedIndex];
    document.getElementById('detail-name').value = app.name || '';
    document.getElementById('detail-command').value = app.command || '';
    document.getElementById('detail-restart').value = 'always'; // Default, as we don't get this from API yet

    const statusEl = document.getElementById('detail-status');
    statusEl.textContent = app.state;
    statusEl.className = 'value ' + app.state;

    document.getElementById('detail-pid').textContent = app.pid > 0 ? app.pid : '-';
}

// Apply changes to the selected app
async function applyChanges() {
    if (selectedIndex === null) return;

    const body = JSON.stringify({
        index: selectedIndex,
        name: document.getElementById('detail-name').value,
        command: document.getElementById('detail-command').value,
        restart: document.getElementById('detail-restart').value
    });

    try {
        await fetch('/app_update.action', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: body
        });
        await loadApps();
        renderApps();
        renderDetails();
    } catch (e) {
        console.error('Failed to update app:', e);
        alert('Failed to update application');
    }
}

// Add a new app
async function addApp() {
    const name = prompt('Application name:');
    if (!name) return;

    const command = prompt('Command to execute:');
    if (!command) return;

    try {
        await fetch('/app_add.action', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                name: name,
                command: command,
                restart: 'always'
            })
        });
        await loadApps();
        renderApps();
        selectApp(apps.length - 1);
    } catch (e) {
        console.error('Failed to add app:', e);
        alert('Failed to add application');
    }
}

// Delete an app
async function deleteApp(index) {
    const app = apps[index];
    if (!confirm(`Delete "${app.name}"?`)) return;

    try {
        await fetch('/app_delete.action?index=' + index);
        await loadApps();

        if (selectedIndex >= apps.length) {
            selectedIndex = apps.length > 0 ? apps.length - 1 : null;
        }

        renderApps();
        renderDetails();
    } catch (e) {
        console.error('Failed to delete app:', e);
        alert('Failed to delete application');
    }
}

// Start an app
async function startApp(index) {
    try {
        await fetch('/start.action?index=' + index);
    } catch (e) {
        console.error('Failed to start app:', e);
    }
}

// Stop an app
async function stopApp(index) {
    try {
        await fetch('/stop.action?index=' + index);
    } catch (e) {
        console.error('Failed to stop app:', e);
    }
}

// Restart an app
async function restartApp(index) {
    try {
        await fetch('/restart.action?index=' + index);
    } catch (e) {
        console.error('Failed to restart app:', e);
    }
}

// Stop all apps
async function stopAll() {
    try {
        await fetch('/stop_all.action');
    } catch (e) {
        console.error('Failed to stop all:', e);
    }
}

// Start all apps
async function startAll() {
    try {
        await fetch('/start_all.action');
    } catch (e) {
        console.error('Failed to start all:', e);
    }
}

// Save config to disk
async function saveConfig() {
    try {
        await fetch('/save_config.action');
        alert('Configuration saved!');
    } catch (e) {
        console.error('Failed to save config:', e);
        alert('Failed to save configuration');
    }
}

// Refresh logs for selected app
async function refreshLogs() {
    if (selectedIndex === null) return;

    try {
        const resp = await fetch('/get_logs.action?index=' + selectedIndex);
        const data = await resp.json();
        const logs = atob(data.stdout || '');
        document.getElementById('logs-area').value = logs;

        // Scroll to bottom
        const textarea = document.getElementById('logs-area');
        textarea.scrollTop = textarea.scrollHeight;
    } catch (e) {
        console.error('Failed to load logs:', e);
        document.getElementById('logs-area').value = 'Failed to load logs';
    }
}

// Status update loop
function startStatusLoop() {
    setInterval(async () => {
        try {
            const resp = await fetch('/apps_state.json');
            const data = await resp.json();

            // Update apps state
            for (let i = 0; i < data.apps.length && i < apps.length; i++) {
                apps[i].state = data.apps[i].state;
                apps[i].pid = data.apps[i].pid;
            }

            // Re-render to reflect state changes
            renderApps();

            // Update details panel if an app is selected
            if (selectedIndex !== null && selectedIndex < apps.length) {
                const app = apps[selectedIndex];
                const statusEl = document.getElementById('detail-status');
                if (statusEl) {
                    statusEl.textContent = app.state;
                    statusEl.className = 'value ' + app.state;
                }
                const pidEl = document.getElementById('detail-pid');
                if (pidEl) {
                    pidEl.textContent = app.pid > 0 ? app.pid : '-';
                }
            }
        } catch (e) {
            console.error('Status update failed:', e);
        }
    }, 500);
}

// Utility: escape HTML
function escapeHtml(text) {
    if (!text) return '';
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}
