# Contributing

Thanks for wanting to improve Base44 Code Downloader.

## Local Development

1. Open `chrome://extensions`.
2. Enable `Developer mode`.
3. Click `Load unpacked`.
4. Select this repository folder.
5. After changes, click the extension reload button in Chrome.

## How Editor-DOM Fallback Reading Works

When the official `export-to-zip` API and the older JSON `/code` endpoint
are both unavailable, the extension reconstructs the project by reading
files straight out of the Base44 editor UI:

1. **Discover every file.** `detectPathsFromPage` walks the file tree in the
   editor sidebar recursively, expanding every folder it finds (pages,
   components, hooks, lib, api, etc.), so the resulting path list covers the
   whole project, not just top-level files.
2. **Open each file directly by URL.** Base44's editor route accepts the
   open file as a query param, e.g.
   `https://app.base44.com/apps/{app_id}/editor/workspace/code?filePath=src%2Fpages%2FOAuthConsent.jsx`.
   For each discovered path, the extension updates the URL with
   `history.pushState` and dispatches a synthetic `popstate` event so
   Base44's client-side router opens that file — no clicking through nested
   folders required. This is the primary strategy because it's fast and
   works regardless of how deep a file is nested.
3. **Fall back to tree clicks if URL navigation doesn't stick.** If the
   editor doesn't switch to the requested file in time (different router
   behavior, file not indexed yet, etc.), the extension falls back to
   expanding folders and clicking the matching tree node instead, the same
   way earlier versions always did.
4. **Read the code from Monaco** once the target file is open, keyed by its
   full path.
5. **Zip with the original directory structure preserved**, so
   `src/pages/OAuthConsent.jsx`, `src/components/ui/button.jsx`, etc. all
   land in the right folders in the downloaded archive.

## Pull Request Ideas

- Improve fallback file discovery (e.g. detecting lazily-rendered tree nodes).
- Make URL-based navigation the default even in "Emergency mode: file list" downloads, with a visible per-file progress indicator.
- Add export progress (current file / total files) in the popup log.
- Add tests for ZIP creation and for the URL-based file navigation path.
- Add screenshots or a short demo GIF.
- Improve accessibility and keyboard navigation.
- Harden auth handling for the official `export-to-zip` endpoint (e.g. surfacing clearer messages for expired sessions).

Keep changes focused and explain what Base44 page state you tested against.
