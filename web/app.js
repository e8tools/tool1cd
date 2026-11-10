const statusEl = document.getElementById("status");
const errorEl = document.getElementById("error");
const fileInput = document.getElementById("fileInput");
const openBtn = document.getElementById("openBtn");
const tablesEl = document.getElementById("tables");
const sortFieldEl = document.getElementById("sortField");
const sortDirEl = document.getElementById("sortDir");
const tableMetaEl = document.getElementById("tableMeta");
const tableContentHeadEl = document.getElementById("tableContentHead");
const tableContentBodyEl = document.getElementById("tableContentBody");

let worker = null;
let requestSeq = 1;
const pendingRequests = new Map();
let currentTables = [];
let selectedTableName = "";
const TABLE_PREVIEW_LIMIT = 100;

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

function clearTableContent() {
  tableContentHeadEl.replaceChildren();
  tableContentBodyEl.replaceChildren();
  tableMetaEl.textContent = "Select a table to preview first 100 rows.";
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
    tr.classList.toggle("active", table.name === selectedTableName);

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
    tr.addEventListener("click", () => {
      void loadTableContent(table.name);
    });
    tablesEl.appendChild(tr);
  }
}

function renderTableContent(payload) {
  tableContentHeadEl.replaceChildren();
  tableContentBodyEl.replaceChildren();

  const fields = Array.isArray(payload?.fields) ? payload.fields : [];
  const rows = Array.isArray(payload?.rows) ? payload.rows : [];

  const headerRow = document.createElement("tr");
  const rowNumTh = document.createElement("th");
  rowNumTh.className = "rownum";
  rowNumTh.textContent = "#";
  headerRow.appendChild(rowNumTh);

  for (const field of fields) {
    const th = document.createElement("th");
    th.textContent = String(field?.name ?? "");
    headerRow.appendChild(th);
  }
  tableContentHeadEl.appendChild(headerRow);

  for (const row of rows) {
    const tr = document.createElement("tr");
    if (row?.deleted) {
      tr.classList.add("deleted");
    }

    const rowNumTd = document.createElement("td");
    rowNumTd.className = "rownum";
    rowNumTd.textContent = String(row?.row ?? "");
    tr.appendChild(rowNumTd);

    const values = Array.isArray(row?.values) ? row.values : [];
    for (const value of values) {
      const td = document.createElement("td");
      td.textContent = String(value ?? "");
      tr.appendChild(td);
    }

    tableContentBodyEl.appendChild(tr);
  }

  const offset = Number(payload?.offset) || 0;
  const totalRows = Number(payload?.totalRows) || 0;
  const shownTo = Math.min(offset + rows.length, totalRows);
  tableMetaEl.textContent =
    `${payload?.table ?? ""}: showing ${offset}-${shownTo} of ${totalRows} rows`;
}

function initWorker() {
  worker = new Worker("./wasm_worker.js");

  worker.addEventListener("message", (event) => {
    const message = event.data || {};
    const request = pendingRequests.get(message.id);
    if (!request) {
      return;
    }
    pendingRequests.delete(message.id);
    if (message.ok) {
      request.resolve(message.result);
    } else {
      request.reject(new Error(String(message.error || "Unknown worker error")));
    }
  });

  worker.addEventListener("error", (event) => {
    const message = event.message || "WASM worker crashed";
    for (const request of pendingRequests.values()) {
      request.reject(new Error(message));
    }
    pendingRequests.clear();
  });
}

function workerCall(method, params = {}) {
  if (!worker) {
    return Promise.reject(new Error("WASM worker is not initialized"));
  }

  return new Promise((resolve, reject) => {
    const id = requestSeq++;
    pendingRequests.set(id, { resolve, reject });
    worker.postMessage({ id, method, params });
  });
}

async function loadTableContent(tableName) {
  clearError();
  setStatus(`Loading rows from ${tableName}...`);
  try {
    const payload = await workerCall("getTableRows", {
      tableName,
      offset: 0,
      limit: TABLE_PREVIEW_LIMIT,
    });
    selectedTableName = tableName;
    renderTables();
    renderTableContent(payload);
    setStatus(`Loaded ${tableName} preview`);
  } catch (err) {
    const message = err instanceof Error ? err.message : String(err);
    showError(message);
    setStatus("Failed to load table content.");
  }
}

async function onOpenClick() {
  clearError();
  clearTables();
  clearTableContent();

  const file = fileInput.files?.[0];
  if (!file) {
    showError("Please choose a .1CD file first.");
    return;
  }

  openBtn.disabled = true;
  try {
    setStatus("Mounting selected file in WORKERFS...");
    await workerCall("openFile", { file });

    setStatus("Loading table list...");
    const parsed = await workerCall("listTables");
    currentTables = normalizeTables(parsed);
    selectedTableName = "";
    renderTables();
    clearTableContent();
    setStatus(`Loaded ${currentTables.length} tables from ${file.name}`);
  } catch (err) {
    const message = err instanceof Error ? err.message : String(err);
    showError(message);
    setStatus("Failed to open file.");
  } finally {
    openBtn.disabled = false;
  }
}

async function main() {
  clearError();
  clearTables();
  clearTableContent();
  openBtn.disabled = true;

  try {
    initWorker();
    await workerCall("init");
    setStatus("WASM worker runtime is ready.");
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

window.addEventListener("beforeunload", () => {
  if (worker) {
    worker.terminate();
  }
});

void main();
