# YellowDocs

A native Haiku OS office suite.

## Overview

YellowDocs is a future native office-suite monorepo for Haiku OS. It aims to provide a compact, responsive, local-first set of productivity applications that feel native to Haiku.

## Applications

- **YellowDocs Writer**: Rich-text word processor (MVP - this implementation)
- YellowDocs Sheets: Spreadsheet (future)
- YellowDocs Slides: Presentation editor (future)
- YellowDocs Draw: Vector drawing and diagram editor (future)
- YellowDocs Math: Equation editor (future)
- YellowDocs Data: Lightweight SQLite database front end (future)
- YellowDocs Home: Optional suite launcher/document hub (future)

## Current Status

This repository contains the first application: **YellowDocs Writer** - a polished, functional rich-text document editor.

## Architecture

```
YellowDocs/
├── apps/
│   └── writer/           # YellowDocs Writer application
│       ├── src/          # Application source code
│       └── Jamfile       # Application build rules
├── libs/
│   ├── core/             # Shared core library (document models, types)
│   │   └── src/
│   ├── ui/               # Shared UI components library
│   │   └── src/
│   └── utils/            # Shared utilities (file I/O, MIME, helpers)
│       └── src/
├── resources/
│   ├── icons/            # Application and document icons
│   └── fonts/            # Optional bundled fonts
├── package/              # HPKG package layout
└── Jamfile               # Root build rules
```

## Building

```bash
# From Haiku terminal
cd /path/to/YellowDocs
jam -q
```

## Installation

The built package will be created in the `build` directory and can be installed via:

```bash
pkgman install /path/to/YellowDocs/build/YellowDocs-Writer-*.hpkg
```

## Branding

- **Suite Name**: YellowDocs
- **Application**: YellowDocs Writer
- **Executable**: `yellowdocs-writer`
- **Document Extension**: `.ywrite`
- **MIME Type**: `application/x-vnd.yellowdocs-writer`
- **Application MIME Type**: `application/x-vnd.yellowdocs-writer-app`
- **Accent Color**: Warm yellow/amber

## License

MIT License - see LICENSE file for details.
