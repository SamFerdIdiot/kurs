#!/bin/bash
# Run the game
# Запустить игру

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"

echo "=========================================="
echo "Running 69 Crossroads - Россия 90-х"
echo "=========================================="
echo ""

# Check if executable exists
if [ ! -f "$BUILD_DIR/kkurs" ]; then
    echo "Executable not found. Building..."
    "$SCRIPT_DIR/build.sh"
fi

# Run from build directory (assets are copied there during build)
cd "$BUILD_DIR"
./kkurs
