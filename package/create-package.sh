#!/bin/sh

# YellowDocs Writer package creation script
# This script creates the HPKG package for YellowDocs Writer

set -e

# Script directory
SCRIPT_DIR="$(dirname "$0")"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Package information
PACKAGE_NAME="YellowDocs-Writer"
PACKAGE_VERSION="1.0.0"
PACKAGE_REVISION="1"

# Build directory
BUILD_DIR="$PROJECT_DIR/build"
PACKAGE_DIR="$BUILD_DIR/package"
HPKG_FILE="$BUILD_DIR/YellowDocs-Writer-$PACKAGE_VERSION-$PACKAGE_REVISION-any.hpkg"

# Create build directory
mkdir -p "$BUILD_DIR"
mkdir -p "$PACKAGE_DIR"

# Create package directory structure
mkdir -p "$PACKAGE_DIR/bin"
mkdir -p "$PACKAGE_DIR/lib"
mkdir -p "$PACKAGE_DIR/share/yellowdocs/writer/resources"
mkdir -p "$PACKAGE_DIR/share/mime_db/application"
mkdir -p "$PACKAGE_DIR/share/desktop_db"
mkdir -p "$PACKAGE_DIR/share/doc/yellowdocs/writer"

# Copy application executable
if [ -f "$PROJECT_DIR/apps/writer/yellowdocs-writer" ]; then
    cp "$PROJECT_DIR/apps/writer/yellowdocs-writer" "$PACKAGE_DIR/bin/"
else
    echo "Error: Application executable not found"
    exit 1
fi

# Copy libraries
if [ -f "$PROJECT_DIR/libs/core/libyellowdocs-core.so" ]; then
    cp "$PROJECT_DIR/libs/core/libyellowdocs-core.so" "$PACKAGE_DIR/lib/"
fi

if [ -f "$PROJECT_DIR/libs/ui/libyellowdocs-ui.so" ]; then
    cp "$PROJECT_DIR/libs/ui/libyellowdocs-ui.so" "$PACKAGE_DIR/lib/"
fi

if [ -f "$PROJECT_DIR/libs/utils/libyellowdocs-utils.so" ]; then
    cp "$PROJECT_DIR/libs/utils/libyellowdocs-utils.so" "$PACKAGE_DIR/lib/"
fi

# Copy resources
if [ -f "$PROJECT_DIR/resources/app_icon.png" ]; then
    cp "$PROJECT_DIR/resources/app_icon.png" "$PACKAGE_DIR/share/yellowdocs/writer/resources/"
fi

if [ -f "$PROJECT_DIR/resources/document_icon.png" ]; then
    cp "$PROJECT_DIR/resources/document_icon.png" "$PACKAGE_DIR/share/yellowdocs/writer/resources/"
fi

# Create MIME database entry
cat > "$PACKAGE_DIR/share/mime_db/application/x-vnd.yellowdocs-writer" << 'EOF'
application/x-vnd.yellowdocs-writer
  short_description: "YellowDocs Writer Document"
  long_description: "YellowDocs Writer rich text document"
  file_extensions: ywrite
  preferred_app: application/x-vnd.yellowdocs-writer-app
  sniffer_rule: 0:4:0x5900445752 / 0x00000000
  sniffer_priority: 100
EOF

# Create desktop file for Tracker
cat > "$PACKAGE_DIR/share/desktop_db/YellowDocs Writer" << 'EOF'
application/x-vnd.yellowdocs-writer-app
EOF

# Copy documentation
cp "$PROJECT_DIR/README.md" "$PACKAGE_DIR/share/doc/yellowdocs/writer/"
cp "$PROJECT_DIR/LICENSE" "$PACKAGE_DIR/share/doc/yellowdocs/writer/"

# Create package using packagefs
package create \
    -name "$PACKAGE_NAME" \
    -version "$PACKAGE_VERSION" \
    -revision "$PACKAGE_REVISION" \
    -arch any \
    -provider "YellowDocs Contributors" \
    -packager "YellowDocs Team" \
    -summary "A native Haiku OS word processor" \
    -description "YellowDocs Writer is a compact, responsive, local-first rich-text document editor designed specifically for Haiku OS. It provides a native Haiku interface with familiar keyboard shortcuts and Tracker integration." \
    -copyright "Copyright 2024 YellowDocs Contributors" \
    -license "MIT License" \
    -url "https://github.com/YellowDocs/YellowDocs" \
    -depends haiku>=r1~beta4 \
    -depends lib:libbe \
    -depends lib:libtracker \
    -provides "YellowDocs Writer 1.0.0" \
    -provides "application/x-vnd.yellowdocs-writer 1.0.0" \
    -provides "application/x-vnd.yellowdocs-writer-app 1.0.0" \
    "$PACKAGE_DIR" \
    "$HPKG_FILE"

# Clean up
echo "Package created: $HPKG_FILE"
rm -rf "$PACKAGE_DIR"

echo "YellowDocs Writer package creation complete!"
