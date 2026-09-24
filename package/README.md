# YellowDocs Writer Package

This directory contains files for creating the HPKG package for YellowDocs Writer.

## Package Creation

### Using the Script

The `create-package.sh` script automates the package creation process:

```bash
chmod +x create-package.sh
./create-package.sh
```

This will:
1. Create the package directory structure
2. Copy all necessary files
3. Create MIME database entries
4. Create the HPKG package file
5. Place the package in the `build/` directory

### Manual Package Creation

Alternatively, you can manually create the package:

```bash
# Create package directory structure
mkdir -p build/package/bin
mkdir -p build/package/lib
mkdir -p build/package/share/yellowdocs/writer/resources

# Copy files
cp apps/writer/yellowdocs-writer build/package/bin/
cp libs/core/libyellowdocs-core.so build/package/lib/
cp libs/ui/libyellowdocs-ui.so build/package/lib/
cp libs/utils/libyellowdocs-utils.so build/package/lib/
cp resources/app_icon.png build/package/share/yellowdocs/writer/resources/
cp resources/document_icon.png build/package/share/yellowdocs/writer/resources/

# Create the package
package create \
    -name "YellowDocs-Writer" \
    -version "1.0.0" \
    -revision "1" \
    -arch any \
    -provider "YellowDocs Contributors" \
    -summary "A native Haiku OS word processor" \
    build/package \
    build/YellowDocs-Writer-1.0.0-1-any.hpkg
```

## Package Contents

The HPKG package contains:

- **Executable**: `bin/yellowdocs-writer`
- **Libraries**: `lib/libyellowdocs-*.so`
- **Resources**: Icons and other resources
- **MIME Database**: MIME type registration
- **Desktop Files**: Tracker integration
- **Documentation**: README and LICENSE files

## Package Information

- **Name**: YellowDocs Writer
- **Version**: 1.0.0
- **Architecture**: any
- **Provider**: YellowDocs Contributors
- **License**: MIT License
- **Dependencies**: Haiku >= r1~beta4, libbe, libtracker

## Installation

After creating the package, install it using:

```bash
pkgman install build/YellowDocs-Writer-1.0.0-1-any.hpkg
```

## HaikuPorts Submission

To submit to HaikuPorts:

1. Create the HPKG package
2. Test the package on a clean Haiku installation
3. Create a recipe file for HaikuPorts
4. Submit a pull request to the HaikuPorts repository

## Package Testing

Test the package before distribution:

```bash
# Install the package
pkgman install build/YellowDocs-Writer-1.0.0-1-any.hpkg

# Verify installation
pkgman list | grep YellowDocs

# Test the application
yellowdocs-writer

# Uninstall
pkgman remove yellowdocs-writer
```
