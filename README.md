# Base44 Code Downloader

Download your Base44 project source code as a ZIP directly from the Chrome tab you already have open.

This extension is built for makers who want a quick local backup, a handoff to another developer, or a starting point for moving a Base44 app into a normal Git workflow.

## Version 1.1.0

- Now uses Base44's official `GET /api/apps/{app_id}/coding/export-to-zip` export endpoint as the primary download path. This returns a real ZIP archive built by Base44 itself, so the download is complete and accurate.
- Falls back to an older JSON code endpoint, and finally to the editor-DOM reading mode, only if the official export endpoint isn't available for a given app or account.
- Full English UI and log messages.

## Why People Use It

- One-click project detection from the active Base44 editor tab.
- Uses Base44's own export endpoint for a reliable, complete ZIP.
- Automatic fallback when the official export endpoint isn't reachable.
- Editor-DOM recovery mode for projects that can only be read through the editor.
- No backend, no account, no external server. Everything runs locally in your browser.

## Install

1. Download or clone this repository.
2. Open `chrome://extensions`.
3. Enable `Developer mode`.
4. Click `Load unpacked`.
5. Select this extension folder.

## How To Download A Project

1. Open your project in `app.base44.com`.
2. Click the Base44 Code Downloader icon.
3. Click `Detect project`.
4. Click `Download ZIP`.

The extension first requests Base44's own export archive at
`/api/apps/{app_id}/coding/export-to-zip`. If that request fails (for
example if it returns `401`/`403`, or the endpoint isn't available for your
plan), the extension automatically falls back to an older code endpoint, and
if that also fails, to reading files through the editor DOM.

## Fallback Mode

Some Base44 projects or accounts can't reach the official export endpoint.
In that case the extension can switch files in the editor and read the code
from the Monaco DOM.

You can also open `Emergency mode: file list`, detect paths from the current
page, edit the list, and download only the files you want.

## Privacy

The extension reads code from the active Base44 tab and generates (or saves)
the ZIP locally in Chrome. It does not upload your code to any third-party
service.

## Contributing

Pull requests are welcome. Good first ideas:

- Better automatic file tree detection.
- Support for more Base44 project layouts.
- Progress bar for large projects.
- Export metadata such as app name and timestamp.
- Better UI states and error messages.

If this helped you rescue or back up a project, star the repo so more builders can find it.
