const statusEl = document.getElementById("status");
const errorEl = document.getElementById("error");
const fileInput = document.getElementById("fileInput");
const openBtn = document.getElementById("openBtn");
const tablesEl = document.getElementById("tables");
const sortFieldEl = document.getElementById("sortField");
const sortDirEl = document.getElementById("sortDir");

let api = null;
let currentTables = [];

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

function formatSize(bytes) {
  if (!Number.isFinite(bytes) || bytes < 0) return "0 B";

  const units = ["B", "KiB", "MiB", "GiB", "TiB"];
  let value = bytes;
  let unitIndex = 0;

  while (value >= 1024 && unitIndex < units.length - 1) {
    value /= 1024;
    unitIndex += 1;
  }

  return `${value.toFixed(unitIndex === 0 ? 0 : 2)} ${units[unitIndex]}`;
}

function extractName(value, depth = 0) {
  if (depth > 10 || value == null) return "";
  if (typeof value === "string") return value;

  if (Array.isArray(value)) {
    for (const item of value) {
      const candidate = extractName(item, depth + 1);
      if (candidate) return candidate;
    }
    return "";
  }

  if (typeof value === "object") {
    const preferredKeys = [
      "name",
      "Name",
      "tableName",
      "table_name",
      "table",
      "title",
      "value",
    ];
    for (const key of preferredKeys) {
      if (key in value) {
        const candidate = extractName(value[key], depth + 1);
        if (candidate) return candidate;
      }
    }

    for (const nestedValue of Object.values(value)) {
      const candidate = extractName(nestedValue, depth + 1);
      if (candidate) return candidate;
    }
  }

  return "";
}

function extractSize(value, depth = 0) {
  if (depth > 3 || value == null) return 0;
  if (typeof value === "number" && Number.isFinite(value)) return value;
  if (typeof value === "string") {
    const parsed = Number(value);
    return Number.isFinite(parsed) ? parsed : 0;
  }

  if (Array.isArray(value)) {
    for (const item of value) {
      const candidate = extractSize(item, depth + 1);
      if (candidate > 0) return candidate;
    }
    return 0;
  }

  if (typeof value === "object") {
    const preferredKeys = ["size", "totalSize", "bytes", "value"];
    for (const key of preferredKeys) {
      if (key in value) {
        const candidate = extractSize(value[key], depth + 1);
        if (candidate > 0) return candidate;
      }
    }
  }

  return 0;
}

function normalizeTables(parsed) {
  if (!Array.isArray(parsed)) {
    if (parsed && Array.isArray(parsed.tables)) {
      return normalizeTables(parsed.tables);
    }
    throw new Error("Table JSON format is unsupported. Expected array or {tables:[...]}");
  }

  return parsed.map((item) => {
    if (typeof item === "string") {
      return { name: item, size: 0, raw: item };
    }

    const rawName = item?.name ?? item;
    const rawSize = item?.size ?? item;
    const normalizedName = extractName(rawName) || extractName(item) || "(unknown)";
    return {
      name: normalizedName === "[object Object]" ? "(unknown)" : normalizedName,
      size: extractSize(rawSize),
      raw: item,
    };
  });
}

function sortedTables() {
  const sortField = sortFieldEl.value;
  const isAsc = sortDirEl.value === "asc";
  const sign = isAsc ? 1 : -1;
  const rows = [...currentTables];

  rows.sort((a, b) => {
    if (sortField === "size") {
      if (a.size !== b.size) {
        return (a.size - b.size) * sign;
      }
      return a.name.localeCompare(b.name) * sign;
    }

    const nameCmp = a.name.localeCompare(b.name);
    if (nameCmp !== 0) {
      return nameCmp * sign;
    }
    return (a.size - b.size) * sign;
  });

  return rows;
}

function renderTables() {
  clearTables();

  for (const table of sortedTables()) {
    const tr = document.createElement("tr");

    const nameTd = document.createElement("td");
    const fallbackName =
      extractName(table.raw) ||
      (table.raw && typeof table.raw === "object" ? JSON.stringify(table.raw) : "");
    const displayName =
      typeof table.name === "string" && table.name !== "[object Object]"
        ? table.name
        : (fallbackName || "(unknown)");
    nameTd.textContent = displayName;

    const sizeTd = document.createElement("td");
    sizeTd.textContent = formatSize(table.size);
    sizeTd.className = "size";

    tr.appendChild(nameTd);
    tr.appendChild(sizeTd);
    tablesEl.appendChild(tr);
  }
}

function bindApi(Module) {
  const hasCwrap = typeof Module.cwrap === "function";
  const wrap = (name, returnType, argTypes) => {
    if (hasCwrap) {
      const fn = Module.cwrap(name, returnType, argTypes);
      if (typeof fn === "function") return fn;
    }
    const direct = Module[`_${name}`];
    return typeof direct === "function" ? direct : null;
  };

  const open = wrap("onecd_open", "number", ["string"]);
  const listTablesJsonPtr = wrap("onecd_list_tables_json", "number", []);
  const freeString = wrap("onecd_free_string", null, ["number"])
    || (typeof Module._free === "function" ? Module._free : null);

  const missing = [];
  if (!open) missing.push("onecd_open");
  if (!listTablesJsonPtr) missing.push("onecd_list_tables_json");
  if (!freeString) missing.push("onecd_free_string");
  if (missing.length) {
    throw new Error(
      `Missing WASM exports: ${missing.join(", ")}. ` +
      "Rebuild and refresh parser.js/parser.wasm."
    );
  }

  return {
    open,
    listTablesJsonPtr,
    freeString,
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
    currentTables = normalizeTables(parsed);
    renderTables();
    setStatus(`Loaded ${currentTables.length} tables from ${file.name}`);
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

sortFieldEl.addEventListener("change", () => {
  renderTables();
});

sortDirEl.addEventListener("change", () => {
  renderTables();
});

void main();
