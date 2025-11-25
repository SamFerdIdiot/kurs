#!/bin/bash
# Clean build directory
# Очистить директорию сборки
# Note: This removes the entire build directory including copied assets.
# Assets will be re-copied during the next build.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"

echo "=========================================="
echo "Cleaning build directory"
echo "Очистка директории сборки"
echo "=========================================="

if [ -d "$BUILD_DIR" ]; then
    echo "Removing $BUILD_DIR..."
    rm -rf "$BUILD_DIR"
    echo "✓ Build directory cleaned"
    echo "Note: Assets will be re-copied during the next build"
else
    echo "Build directory does not exist. Nothing to clean."
fi

echo ""
echo "Done!"
