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

const DEFAULT_PATHS = [
  ".github/workflows/ci.yml",
  ".github/workflows/dependabot-auto-merge.yml",
  ".github/dependabot.yml",
  "base44/config.jsonc",
  "src/api/base44Client.js",
  "src/components/AuthLayout.jsx",
  "src/components/ChatPanel.jsx",
  "src/components/FileMessage.jsx",
  "src/components/GlassOrbs.jsx",
  "src/components/GoogleIcon.jsx",
  "src/components/IncomingCallModal.jsx",
  "src/components/Layout.jsx",
  "src/components/LoadingScreen.jsx",
  "src/components/NetworkStatus.jsx",
  "src/components/NetworkStatusToggle.jsx",
  "src/components/ProfileSettingsModal.jsx",
  "src/components/ProtectedRoute.jsx",
  "src/components/ScrollToTop.jsx",
  "src/components/TypingIndicator.jsx",
  "src/components/UpdateBanner.jsx",
  "src/components/UserNotRegisteredError.jsx",
  "src/components/VideoPanel.jsx",
  "src/hooks/use-mobile.jsx",
  "src/hooks/use-size.jsx",
  "src/hooks/useNetworkStatus.js",
  "src/lib/app-params.js",
  "src/lib/AuthContext.jsx",
  "src/lib/fileUtils.js",
  "src/lib/PageNotFound.jsx",
  "src/lib/query-client.js",
  "src/lib/sounds.js",
  "src/lib/utils.js",
  "src/pages/DirectCall.jsx",
  "src/pages/ForgotPassword.jsx",
  "src/pages/Home.jsx",
  "src/pages/Login.jsx",
  "src/pages/MessengerApp.jsx",
  "src/pages/Register.jsx",
  "src/pages/ResetPassword.jsx",
  "src/pages/Room.jsx",
  "src/pages/ServerPage.jsx",
  "src/utils/index.ts",
  "src/App.jsx",
  "src/index.css",
  "src/main.jsx",
  ".gitignore",
  "AGENTS.md",
  "CLAUDE.md",
  "components.json",
  "eslint.config.js",
  "index.html",
  "jsconfig.json",
  "package.json",
  "postcss.config.js",
  "README.md",
  "tailwind.config.js",
  "vite.config.js"
];

init();

async function init() {
  activeTab = await getActiveTab();
  const appId = extractAppId(activeTab?.url || "");
  tabStatus.textContent = appId ? "Base44" : "Brak Base44";
  if (appId) appIdInput.value = appId;
  pathsInput.value = DEFAULT_PATHS.join("\n");
  log(appId ? "Wykryto projekt z aktualnej karty." : "Otwórz kartę edytora Base44 albo wklej Project ID.");
}

detectBtn.addEventListener("click", async () => {
  activeTab = await getActiveTab();
  const appId = extractAppId(activeTab?.url || "");
  if (!appId) return log("Nie widzę ID projektu w aktywnej karcie.");
  appIdInput.value = appId;
  tabStatus.textContent = "Base44";
  log(`Project ID: ${appId}`);
});

downloadBtn.addEventListener("click", async () => {
  await withBusy(downloadBtn, async () => {
    const appId = requireAppId();
    log("Pobieram kod przez API Base44...");
    const result = await runInPage(downloadProjectFromApi, [appId]);
    if (!result.ok) {
      const paths = parsePaths(pathsInput.value);
      log(`${result.error}\n\nBase44 blokuje bezpośredni odczyt. Przechodzę automatycznie na tryb awaryjny: ${paths.length} plików przez Monaco...`);
      const fallback = await runInPage(downloadProjectByPaths, [appId, paths]);
      if (!fallback.ok) {
        log(`${fallback.error}\n\nNiepobrane:\n${(fallback.failures || []).map((x) => `- ${x.path}`).join("\n")}`);
        return;
      }
      await saveZip(fallback.files, `base44-${appId}-monaco.zip`);
      const failed = fallback.failures?.length ? `\nPominięto: ${fallback.failures.length}` : "";
      log(`Gotowe fallbackiem: ${Object.keys(fallback.files).length} plików zapisanych jako ZIP.${failed}`);
      return;
    }
    await saveZip(result.files, `base44-${appId}.zip`);
    log(`Gotowe: ${Object.keys(result.files).length} plików zapisanych jako ZIP.`);
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
    log(`Wykryto ${paths.length} ścieżek. Możesz je poprawić przed pobraniem.`);
  });
});

downloadByPathsBtn.addEventListener("click", async () => {
  await withBusy(downloadByPathsBtn, async () => {
    const appId = requireAppId();
    const paths = parsePaths(pathsInput.value);
    if (!paths.length) throw new Error("Lista ścieżek jest pusta.");
    log(`Pobieram ${paths.length} plików przez edytor Monaco...`);
    const result = await runInPage(downloadProjectByPaths, [appId, paths]);
    if (!result.ok) {
      log(`${result.error}\n\nNiepobrane:\n${(result.failures || []).map((x) => `- ${x.path}`).join("\n")}`);
      return;
    }
    await saveZip(result.files, `base44-${appId}-monaco.zip`);
    const failed = result.failures?.length ? `\nPominięto: ${result.failures.length}` : "";
    log(`Gotowe: ${Object.keys(result.files).length} plików zapisanych jako ZIP.${failed}`);
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
  if (!/^[a-zA-Z0-9_-]{8,}$/.test(appId)) throw new Error("Wklej poprawny Project ID.");
  return appId;
}

function parsePaths(text) {
  return [...new Set(text.split(/\r?\n/).map((line) => line.trim()).filter(Boolean))];
}

function log(message) {
  logBox.textContent = message;
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
  if (!activeTab?.id) throw new Error("Nie mogę odczytać aktywnej karty.");
  const [result] = await chrome.scripting.executeScript({
    target: { tabId: activeTab.id },
    world: "MAIN",
    func,
    args
  });
  return result.result;
}

async function downloadProjectFromApi(appId) {
  try {
    const token = localStorage.getItem("base44_access_token") || localStorage.getItem("token");
    if (!token) return { ok: false, error: "Brak tokenu Base44 w tej karcie. Zaloguj się w app.base44.com." };
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
      files[path] = await readMonacoFile(appId, path);
    } catch (error) {
      failures.push({ path, error: error?.message || String(error) });
    }
  }

  if (!Object.keys(files).length) {
    return { ok: false, error: "Nie udało się pobrać żadnego pliku przez Monaco.", failures };
  }
  return { ok: true, files, failures };

  async function readMonacoFile(appId, path) {
    const url = `/apps/${appId}/editor/workspace/code?filePath=${encodeURIComponent(path)}`;
    history.pushState(null, "", url);
    window.dispatchEvent(new PopStateEvent("popstate"));

    for (let i = 0; i < 60; i++) {
      await sleep(i < 3 ? 180 : 300);
      const models = window.monaco?.editor?.getModels?.() || [];
      const hit = models.find((model) => String(model.uri).endsWith(`/${path}`))
        || models.find((model) => String(model.uri).includes(`/${path}`));
      if (hit) return hit.getValue();
    }
    throw new Error("Nie znaleziono modelu Monaco dla pliku.");
  }

  function sleep(ms) {
    return new Promise((resolve) => setTimeout(resolve, ms));
  }
}

function detectPathsFromPage() {
  const text = document.body?.innerText || "";
  const block = text.split("Code files")[1]?.split("Split view")[0] || "";
  const lines = block.split(/\r?\n/).map((line) => line.trim()).filter(Boolean);
  const files = [];
  const knownTopFiles = new Set([
    ".gitignore", "AGENTS.md", "CLAUDE.md", "components.json", "eslint.config.js",
    "index.html", "jsconfig.json", "package.json", "postcss.config.js", "README.md",
    "tailwind.config.js", "vite.config.js"
  ]);
  const srcFolders = new Set(["api", "components", "hooks", "lib", "pages", "utils"]);
  let top = "";
  let srcFolder = "";
  let inGithubWorkflows = false;

  for (const line of lines) {
    if (line === ".github" || line === "base44" || line === "src") {
      top = line;
      srcFolder = "";
      inGithubWorkflows = false;
      continue;
    }
    if (top === ".github" && line === "workflows") {
      inGithubWorkflows = true;
      continue;
    }
    if (top === "src" && srcFolders.has(line)) {
      srcFolder = line;
      continue;
    }
    if (line === "entities" || line === "ui") continue;
    if (!looksLikeFile(line)) continue;

    if (knownTopFiles.has(line)) files.push(line);
    else if (top === ".github" && inGithubWorkflows && line !== "dependabot.yml") files.push(`.github/workflows/${line}`);
    else if (top === ".github") files.push(`.github/${line}`);
    else if (top === "base44") files.push(`base44/${line}`);
    else if (top === "src" && srcFolder) files.push(`src/${srcFolder}/${line}`);
    else if (top === "src") files.push(`src/${line}`);
  }

  return { paths: [...new Set(files)] };

  function looksLikeFile(value) {
    return value.startsWith(".") || /\.[a-z0-9]+$/i.test(value);
  }
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
