const statusEl = document.getElementById("status");
const errorEl = document.getElementById("error");
const fileInput = document.getElementById("fileInput");
const openBtn = document.getElementById("openBtn");
const tablesEl = document.getElementById("tables");

let api = null;

function setStatus(message) {
  statusEl.textContent = message;
}

function showError(message) {
  errorEl.style.display = "block";
  errorEl.textContent = message;
}

function clearError() {
  errorEl.style.display = "none";
  errorEl.textContent = "";
}

function clearTables() {
  tablesEl.replaceChildren();
}

function renderTables(tableNames) {
  clearTables();
  for (const name of tableNames) {
    const li = document.createElement("li");
    li.textContent = String(name);
    tablesEl.appendChild(li);
  }
}

function bindApi(Module) {
  return {
    open: Module.cwrap("onecd_open", "number", ["string"]),
    listTablesJsonPtr: Module.cwrap("onecd_list_tables_json", "number", []),
    freeString: Module.cwrap("onecd_free_string", null, ["number"]),
    utf8ToString: Module.UTF8ToString.bind(Module),
    FS: Module.FS,
  };
}

async function initModule() {
  if (typeof Module === "undefined") {
    throw new Error("parser.js did not define global Module");
  }

  const runtimeReady =
    typeof Module.ready === "object" && typeof Module.ready.then === "function"
      ? Module.ready
      : new Promise((resolve) => {
          const previous = Module.onRuntimeInitialized;
          Module.onRuntimeInitialized = () => {
            if (typeof previous === "function") previous();
            resolve();
          };
        });

  await runtimeReady;
  api = bindApi(Module);
}

async function fileToUint8Array(file) {
  const buf = await file.arrayBuffer();
  return new Uint8Array(buf);
}

function readTablesJson() {
  const ptr = api.listTablesJsonPtr();
  if (!ptr) {
    throw new Error("onecd_list_tables_json returned null pointer");
  }

  try {
    const jsonText = api.utf8ToString(ptr);
    return JSON.parse(jsonText);
  } finally {
    api.freeString(ptr);
  }
}

async function onOpenClick() {
  clearError();
  clearTables();

  const file = fileInput.files?.[0];
  if (!file) {
    showError("Please choose a .1CD file first.");
    return;
  }

  const wasmPath = `/tmp/${file.name}`;

  try {
    setStatus("Reading selected file...");
    const bytes = await fileToUint8Array(file);

    setStatus("Writing file to WASM FS...");
    api.FS.writeFile(wasmPath, bytes);

    setStatus("Opening 1CD file...");
    const openResult = api.open(wasmPath);
    if (openResult !== 0) {
      throw new Error(`onecd_open failed with code ${openResult}`);
    }

    setStatus("Loading table list...");
    const parsed = readTablesJson();
    const tableNames = Array.isArray(parsed) ? parsed : parsed.tables;

    if (!Array.isArray(tableNames)) {
      throw new Error("Table JSON format is unsupported. Expected array or {tables:[...]}");
    }

    renderTables(tableNames);
    setStatus(`Loaded ${tableNames.length} tables from ${file.name}`);
  } catch (err) {
    const message = err instanceof Error ? err.message : String(err);
    showError(message);
    setStatus("Failed to open file.");
  } finally {
    try {
      api.FS.unlink(wasmPath);
    } catch (_) {
      // Ignore cleanup errors.
    }
  }
}

async function main() {
  clearError();
  clearTables();
  openBtn.disabled = true;

  try {
    await initModule();
    setStatus("WASM runtime is ready.");
    openBtn.disabled = false;
  } catch (err) {
    const message = err instanceof Error ? err.message : String(err);
    showError(`WASM init error: ${message}`);
    setStatus("Runtime initialization failed.");
  }
}

fileInput.addEventListener("change", () => {
  clearError();
});

openBtn.addEventListener("click", () => {
  void onOpenClick();
});

void main();
