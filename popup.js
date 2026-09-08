const appIdInput = document.getElementById("appId");
const tabStatus = document.getElementById("tabStatus");
const logBox = document.getElementById("log");
const detectBtn = document.getElementById("detectBtn");
const downloadBtn = document.getElementById("downloadBtn");
const toggleAdvanced = document.getElementById("toggleAdvanced");
const advancedPanel = document.getElementById("advancedPanel");
const pathsInput = document.getElementById("paths");
const detectPathsBtn = document.getElementById("detectPathsBtn");
const downloadByPathsBtn = document.getElementById("downloadByPathsBtn");

let activeTab = null;

// Generic seed list used only as a starting point for "Emergency mode".
// "Detect paths" overwrites this with whatever the current project's file
// tree actually contains, so this list doesn't need to be exhaustive.
const DEFAULT_PATHS = [
  ".gitignore",
  "package.json",
  "vite.config.js",
  "jsconfig.json",
  "index.html",
  "tailwind.config.js",
  "postcss.config.js",
  "eslint.config.js",
  "components.json",
  "src/main.jsx",
  "src/App.jsx",
  "src/index.css",
  "src/api/base44Client.js",
  "src/lib/utils.js",
  "src/pages/Home.jsx",
  "base44/config.jsonc"
];

init();

async function init() {
  activeTab = await getActiveTab();
  const appId = extractAppId(activeTab?.url || "");
  tabStatus.textContent = appId ? "Base44" : "No Base44";
  if (appId) appIdInput.value = appId;
  pathsInput.value = DEFAULT_PATHS.join("\n");
  log(appId ? "Detected a project from the current tab." : "Open a Base44 editor tab, or paste a Project ID.");
}

detectBtn.addEventListener("click", async () => {
  activeTab = await getActiveTab();
  const appId = extractAppId(activeTab?.url || "");
  if (!appId) return log("I can't see a project ID in the active tab.");
  appIdInput.value = appId;
  tabStatus.textContent = "Base44";
  log(`Project ID: ${appId}`);
});

downloadBtn.addEventListener("click", async () => {
  await withBusy(downloadBtn, async () => {
    const appId = requireAppId();

    log("Requesting the ZIP export from Base44...");
    const zip = await runInPage(downloadProjectZip, [appId]);
    if (zip.ok) {
      await saveZipBase64(zip.base64, `base44-${appId}.zip`);
      log(`Done: downloaded the official ZIP export (${formatBytes(zip.byteLength)}).`);
      return;
    }

    log(`${zip.error}\n\nFalling back to the code endpoint...`);
    const result = await runInPage(downloadProjectFromApi, [appId]);
    if (result.ok) {
      await saveZip(result.files, `base44-${appId}.zip`);
      log(`Done: ${Object.keys(result.files).length} files saved as a ZIP.`);
      return;
    }

    let paths = parsePaths(pathsInput.value);
    try {
      const detected = await runInPage(detectPathsFromPage, []);
      if (detected.paths?.length) {
        paths = detected.paths;
        pathsInput.value = paths.join("\n");
      }
    } catch {}

    log(`${result.error}\n\nBase44 is blocking direct reads. Switching automatically to emergency mode: reading ${paths.length} files through the editor...`);
    const fallback = await runInPage(downloadProjectByPaths, [appId, paths]);
    if (!fallback.ok) {
      log(`${fallback.error}\n\nNot downloaded:\n${(fallback.failures || []).map((x) => `- ${x.path}`).join("\n")}`);
      return;
    }
    await saveZip(fallback.files, `base44-${appId}-editor.zip`);
    const failed = fallback.failures?.length ? `\nSkipped: ${fallback.failures.length}` : "";
    log(`Done via fallback: ${Object.keys(fallback.files).length} files saved as a ZIP.${failed}`);
  });
});

toggleAdvanced.addEventListener("click", () => {
  advancedPanel.hidden = !advancedPanel.hidden;
});

detectPathsBtn.addEventListener("click", async () => {
  await withBusy(detectPathsBtn, async () => {
    const detected = await runInPage(detectPathsFromPage, []);
    const paths = detected.paths?.length ? detected.paths : DEFAULT_PATHS;
    pathsInput.value = paths.join("\n");
    log(`Detected ${paths.length} paths. You can edit them before downloading.`);
  });
});

downloadByPathsBtn.addEventListener("click", async () => {
  await withBusy(downloadByPathsBtn, async () => {
    const appId = requireAppId();
    const paths = parsePaths(pathsInput.value);
    if (!paths.length) throw new Error("The path list is empty.");
    log(`Reading ${paths.length} files through the editor...`);
    const result = await runInPage(downloadProjectByPaths, [appId, paths]);
    if (!result.ok) {
      log(`${result.error}\n\nNot downloaded:\n${(result.failures || []).map((x) => `- ${x.path}`).join("\n")}`);
      return;
    }
    await saveZip(result.files, `base44-${appId}-editor.zip`);
    const failed = result.failures?.length ? `\nSkipped: ${result.failures.length}` : "";
    log(`Done: ${Object.keys(result.files).length} files saved as a ZIP.${failed}`);
  });
});

async function getActiveTab() {
  const [tab] = await chrome.tabs.query({ active: true, currentWindow: true });
  return tab;
}

function extractAppId(url) {
  const match = String(url).match(/app\.base44\.com\/apps\/([^/]+)/);
  return match?.[1] || "";
}

function requireAppId() {
  const appId = appIdInput.value.trim();
  if (!/^[a-zA-Z0-9_-]{8,}$/.test(appId)) throw new Error("Paste a valid Project ID.");
  return appId;
}

function parsePaths(text) {
  return [...new Set(text.split(/\r?\n/).map((line) => line.trim()).filter(Boolean))];
}

function log(message) {
  logBox.textContent = message;
}

function formatBytes(bytes) {
  if (!Number.isFinite(bytes)) return "unknown size";
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(1)} KB`;
  return `${(bytes / (1024 * 1024)).toFixed(1)} MB`;
}

async function withBusy(button, task) {
  const buttons = [detectBtn, downloadBtn, detectPathsBtn, downloadByPathsBtn];
  try {
    buttons.forEach((btn) => (btn.disabled = true));
    await task();
  } catch (error) {
    log(error?.message || String(error));
  } finally {
    buttons.forEach((btn) => (btn.disabled = false));
  }
}

async function runInPage(func, args) {
  activeTab = activeTab || await getActiveTab();
  if (!activeTab?.id) throw new Error("Can't read the active tab.");
  const [result] = await chrome.scripting.executeScript({
    target: { tabId: activeTab.id },
    world: "MAIN",
    func,
    args
  });
  return result.result;
}

// Primary path: Base44's official export endpoint. This is the same
// endpoint the "Export code" button in the Base44 editor uses, and it
// returns a ready-made ZIP archive of the app's currently saved code, so
// no local ZIP assembly or file-by-file reading is needed.
// Docs: https://docs.base44.com/api-reference/export-app-source-code
async function downloadProjectZip(appId) {
  try {
    const token = localStorage.getItem("base44_access_token") || localStorage.getItem("token");
    const headerVariants = [{}, token ? { Authorization: `Bearer ${token}` } : null].filter(Boolean);

    let lastError = "";
    for (const extraHeaders of headerVariants) {
      const response = await fetch(`/api/apps/${appId}/coding/export-to-zip`, {
        method: "GET",
        credentials: "include",
        headers: { Accept: "application/zip", ...extraHeaders }
      });

      if (response.ok) {
        const buffer = await response.arrayBuffer();
        return { ok: true, base64: arrayBufferToBase64(buffer), byteLength: buffer.byteLength };
      }

      const text = await response.text();
      let detail = text;
      try {
        const parsed = JSON.parse(text);
        detail = parsed.detail || parsed.message || text;
      } catch {}
      lastError = `Base44 export API: ${response.status} ${detail}`;

      // Only worth retrying with a different auth header on auth failures.
      if (response.status !== 401 && response.status !== 403) break;
    }
    return { ok: false, error: lastError || "The ZIP export request failed." };
  } catch (error) {
    return { ok: false, error: error?.message || String(error) };
  }

  function arrayBufferToBase64(buffer) {
    const bytes = new Uint8Array(buffer);
    let binary = "";
    const chunkSize = 0x8000;
    for (let i = 0; i < bytes.length; i += chunkSize) {
      binary += String.fromCharCode(...bytes.subarray(i, i + chunkSize));
    }
    return btoa(binary);
  }
}

// Secondary path: an older, undocumented JSON endpoint that some Base44
// projects still expose. Kept as a fallback in case the official ZIP
// export above is unavailable for a given app or account.
async function downloadProjectFromApi(appId) {
  try {
    const token = localStorage.getItem("base44_access_token") || localStorage.getItem("token");
    if (!token) return { ok: false, error: "No Base44 token found in this tab. Log in at app.base44.com." };
    const response = await fetch(`/api/apps/${appId}/code`, {
      headers: {
        "Accept": "application/json",
        "Authorization": `Bearer ${token}`
      }
    });
    const text = await response.text();
    if (!response.ok) {
      let detail = text;
      try {
        const parsed = JSON.parse(text);
        detail = parsed.detail || parsed.message || text;
      } catch {}
      return { ok: false, error: `Base44 API: ${response.status} ${detail}` };
    }
    const payload = JSON.parse(text);
    const files = normalizeFiles(payload.files || payload);
    return { ok: true, files };
  } catch (error) {
    return { ok: false, error: error?.message || String(error) };
  }

  function normalizeFiles(input) {
    if (Array.isArray(input)) {
      return Object.fromEntries(input.map((file) => [
        file.path || file.filePath || file.file_path || file.name,
        String(file.content ?? file.code ?? "")
      ]).filter(([path]) => path));
    }
    const out = {};
    for (const [path, value] of Object.entries(input || {})) {
      out[path] = typeof value === "string" ? value : String(value?.content ?? value?.code ?? JSON.stringify(value, null, 2));
    }
    return out;
  }
}

async function downloadProjectByPaths(appId, paths) {
  const files = {};
  const failures = [];

  for (const path of paths) {
    try {
      files[path] = await readEditorFile(path);
    } catch (error) {
      failures.push({ path, error: error?.message || String(error) });
    }
  }

  if (!Object.keys(files).length) {
    return { ok: false, error: "Couldn't read any file through the editor.", failures };
  }
  return { ok: true, files, failures };

  // Reads one file's code out of the Monaco editor. Base44's editor route
  // accepts the target file as a `filePath` query param, e.g.
  // /apps/{id}/editor/workspace/code?filePath=src%2Fpages%2FOAuthConsent.jsx
  // so the fastest, most reliable way to open a file is to update the URL
  // directly (via pushState + a synthetic popstate so the app's client-side
  // router picks it up) instead of clicking through the file tree one node
  // at a time. If that doesn't make the editor switch in time — different
  // router setup, file not indexed yet, etc. — fall back to clicking the
  // tree node for that path, which is slower but works even when direct
  // URL navigation doesn't.
  async function readEditorFile(path) {
    const normalizedPath = normalizePath(path);

    let opened = await navigateToFileByUrl(appId, normalizedPath);
    if (!opened) {
      await openPathInTree(normalizedPath);
      await waitForEditorPath(normalizedPath);
    }
    return readEditorText();
  }

  async function navigateToFileByUrl(appId, path) {
    if (!appId) return false;
    const targetUrl = `/apps/${appId}/editor/workspace/code?filePath=${encodeURIComponent(path)}`;
    if (`${location.pathname}${location.search}` !== targetUrl) {
      history.pushState(null, "", targetUrl);
      window.dispatchEvent(new PopStateEvent("popstate"));
    }
    try {
      await waitForEditorPath(path, 40);
      return true;
    } catch {
      return false;
    }
  }

  async function openPathInTree(path) {
    const segments = normalizePath(path).split("/").filter(Boolean);
    if (!segments.length) throw new Error("The file path is empty.");

    const navigation = findNavigationRoot();
    if (!navigation) throw new Error("Couldn't find the Base44 file tree.");
    let container = findTreeContainer(navigation);
    if (!container) throw new Error("Couldn't find the contents of the Base44 file tree.");

    for (let i = 0; i < segments.length; i++) {
      const segment = segments[i];
      const button = findTreeButton(container, segment);
      if (!button) {
        throw new Error(`Couldn't find the segment "${segment}" in the file tree.`);
      }

      const isLast = i === segments.length - 1;
      if (isLast) {
        if (getEditorPath().endsWith(`/${path}`)) return;
        activate(button);
        return;
      }

      if (button.getAttribute("data-state") !== "open") {
        activate(button);
      }
      container = await waitForTreeSegment(button, segments[i + 1]);
    }
  }

  async function waitForEditorPath(path, attempts = 60) {
    const targetSuffix = `/${path}`;
    for (let i = 0; i < attempts; i++) {
      if (getEditorPath().endsWith(targetSuffix)) return;
      await sleep(i < 5 ? 100 : 150);
    }
    throw new Error(`Couldn't open the file "${path}".`);
  }

  async function waitForTreeSegment(folderButton, segment) {
    const wanted = normalizeButtonText(segment);
    for (let i = 0; i < 30; i++) {
      const container = getFolderContents(folderButton);
      if (container) {
        const button = findTreeButton(container, wanted);
        if (button) return container;
      }
      await sleep(100);
    }
    throw new Error(`The segment "${segment}" never appeared in the file tree.`);
  }

  function findNavigationRoot() {
    return document.querySelector('[role="navigation"]') || document.querySelector("nav");
  }

  function findTreeContainer(navigation) {
    return Array.from(navigation.querySelectorAll("div")).find((container) => {
      const nodes = Array.from(container.children);
      return nodes.length > 0 && nodes.every((node) => getNodeButton(node));
    }) || null;
  }

  function findTreeButton(container, label) {
    const wanted = normalizeButtonText(label);
    for (const node of Array.from(container.children)) {
      const button = getNodeButton(node);
      if (button && normalizeButtonText(button.textContent) === wanted && isVisible(button)) return button;
    }
    return null;
  }

  function getNodeButton(node) {
    return Array.from(node?.children || []).find((child) => child.tagName === "BUTTON") || null;
  }

  function getFolderContents(folderButton) {
    const folderNode = folderButton?.parentElement;
    return Array.from(folderNode?.children || []).filter((child) => child !== folderButton && child.tagName === "DIV").pop() || null;
  }

  function activate(button) {
    if (typeof button.click === "function") {
      button.click();
      return;
    }
    button.dispatchEvent(new MouseEvent("click", { bubbles: true, cancelable: true, view: window }));
  }

  function findEditorRoot() {
    const editors = Array.from(document.querySelectorAll(".monaco-editor"));
    if (!editors.length) return null;
    return editors.find((editor) => editor.classList.contains("focused")) || editors[0];
  }

  function getEditorPath() {
    return findEditorRoot()?.dataset?.uri || "";
  }

  function readEditorText() {
    const editor = findEditorRoot();
    if (!editor) throw new Error("Couldn't find the code editor.");
    const viewLines = editor.querySelector(".view-lines");
    if (!viewLines) throw new Error("Couldn't read the code from the editor.");
    return viewLines.innerText.replace(/\u00a0/g, " ").replace(/\r\n/g, "\n").trimEnd();
  }

  function normalizePath(value) {
    return String(value).replace(/\\/g, "/").replace(/^\/+/, "").trim();
  }

  function normalizeButtonText(value) {
    return String(value).replace(/\u00a0/g, " ").replace(/\s+/g, " ").trim();
  }

  function isVisible(element) {
    const rect = element.getBoundingClientRect();
    const style = getComputedStyle(element);
    return rect.width > 0 && rect.height > 0 && style.display !== "none" && style.visibility !== "hidden";
  }

  function sleep(ms) {
    return new Promise((resolve) => setTimeout(resolve, ms));
  }
}

async function detectPathsFromPage() {
  const navigation = document.querySelector('[role="navigation"]') || document.querySelector("nav");
  if (!navigation) return { paths: [] };

  const root = findTreeContainer(navigation);
  if (!root) return { paths: [] };

  const paths = [];
  await collectFiles(root, []);
  return { paths: [...new Set(paths)] };

  async function collectFiles(container, parentPath) {
    const nodes = Array.from(container.children);
    for (const node of nodes) {
      const button = getNodeButton(node);
      if (!button) continue;

      const name = normalizeText(button.textContent);
      if (!name) continue;

      if (!button.hasAttribute("data-state")) {
        paths.push([...parentPath, name].join("/"));
        continue;
      }

      if (button.getAttribute("data-state") !== "open") activate(button);
      const contents = await waitForFolderContents(button);
      if (contents) await collectFiles(contents, [...parentPath, name]);
    }
  }

  function findTreeContainer(rootElement) {
    return Array.from(rootElement.querySelectorAll("div")).find((container) => {
      const nodes = Array.from(container.children);
      return nodes.length > 0 && nodes.every((node) => getNodeButton(node));
    }) || null;
  }

  function getNodeButton(node) {
    return Array.from(node?.children || []).find((child) => child.tagName === "BUTTON") || null;
  }

  function getFolderContents(folderButton) {
    const folderNode = folderButton?.parentElement;
    return Array.from(folderNode?.children || []).filter((child) => child !== folderButton && child.tagName === "DIV").pop() || null;
  }

  async function waitForFolderContents(folderButton) {
    for (let i = 0; i < 40; i++) {
      const contents = getFolderContents(folderButton);
      if (contents && (folderButton.getAttribute("data-state") === "open" || contents.children.length > 0)) return contents;
      await sleep(100);
    }
    return getFolderContents(folderButton);
  }

  function activate(button) {
    if (typeof button.click === "function") {
      button.click();
      return;
    }
    button.dispatchEvent(new MouseEvent("click", { bubbles: true, cancelable: true, view: window }));
  }

  function normalizeText(value) {
    return String(value).replace(/\u00a0/g, " ").replace(/\s+/g, " ").trim();
  }

  function sleep(ms) {
    return new Promise((resolve) => setTimeout(resolve, ms));
  }
}

async function saveZipBase64(base64, filename) {
  const binary = atob(base64);
  const bytes = new Uint8Array(binary.length);
  for (let i = 0; i < binary.length; i++) bytes[i] = binary.charCodeAt(i);
  const url = URL.createObjectURL(new Blob([bytes], { type: "application/zip" }));
  await chrome.downloads.download({ url, filename, saveAs: true });
  setTimeout(() => URL.revokeObjectURL(url), 30_000);
}

async function saveZip(files, filename) {
  const zip = createZip(files);
  const url = URL.createObjectURL(new Blob([zip], { type: "application/zip" }));
  await chrome.downloads.download({
    url,
    filename,
    saveAs: true
  });
  setTimeout(() => URL.revokeObjectURL(url), 30_000);
}

function createZip(files) {
  const encoder = new TextEncoder();
  const entries = Object.entries(files).map(([name, content]) => ({
    name: sanitizeZipPath(name),
    data: encoder.encode(String(content ?? ""))
  })).filter((entry) => entry.name);

  const localParts = [];
  const centralParts = [];
  let offset = 0;
  const now = new Date();
  const dosTime = ((now.getHours() & 31) << 11) | ((now.getMinutes() & 63) << 5) | ((now.getSeconds() / 2) & 31);
  const dosDate = (((now.getFullYear() - 1980) & 127) << 9) | (((now.getMonth() + 1) & 15) << 5) | (now.getDate() & 31);

  for (const entry of entries) {
    const nameBytes = encoder.encode(entry.name);
    const crc = crc32(entry.data);
    const localHeader = concatBytes(
      u32(0x04034b50), u16(20), u16(0), u16(0), u16(dosTime), u16(dosDate),
      u32(crc), u32(entry.data.length), u32(entry.data.length), u16(nameBytes.length), u16(0), nameBytes
    );
    localParts.push(localHeader, entry.data);
    const centralHeader = concatBytes(
      u32(0x02014b50), u16(20), u16(20), u16(0), u16(0), u16(dosTime), u16(dosDate),
      u32(crc), u32(entry.data.length), u32(entry.data.length), u16(nameBytes.length), u16(0), u16(0),
      u16(0), u16(0), u32(0), u32(offset), nameBytes
    );
    centralParts.push(centralHeader);
    offset += localHeader.length + entry.data.length;
  }

  const centralSize = centralParts.reduce((sum, part) => sum + part.length, 0);
  const end = concatBytes(
    u32(0x06054b50), u16(0), u16(0), u16(entries.length), u16(entries.length),
    u32(centralSize), u32(offset), u16(0)
  );
  return concatBytes(...localParts, ...centralParts, end);
}

function sanitizeZipPath(path) {
  return String(path).replace(/\\/g, "/").replace(/^\/+/, "").split("/").filter((part) => part && part !== "." && part !== "..").join("/");
}

function concatBytes(...parts) {
  const total = parts.reduce((sum, part) => sum + part.length, 0);
  const out = new Uint8Array(total);
  let offset = 0;
  for (const part of parts) {
    out.set(part, offset);
    offset += part.length;
  }
  return out;
}

function u16(value) {
  return new Uint8Array([value & 255, (value >>> 8) & 255]);
}

function u32(value) {
  return new Uint8Array([value & 255, (value >>> 8) & 255, (value >>> 16) & 255, (value >>> 24) & 255]);
}

function crc32(bytes) {
  let crc = 0xffffffff;
  for (let i = 0; i < bytes.length; i++) {
    crc ^= bytes[i];
    for (let j = 0; j < 8; j++) crc = (crc >>> 1) ^ (0xedb88320 & -(crc & 1));
  }
  return (crc ^ 0xffffffff) >>> 0;
}
