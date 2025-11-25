#!/bin/bash
# Universal build script for Road Trip Adventure RPG
# Универсальный скрипт сборки для "69 Crossroads"
# Works on macOS and Linux / Работает на macOS и Linux

set -e  # Exit on error

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"

echo "=========================================="
echo "Building 69 Crossroads - Россия 90-х"
echo "=========================================="
echo ""

# Detect OS
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macOS"
    echo "Detected OS: macOS"
else
    OS="Linux"
    echo "Detected OS: Linux"
fi

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Configure CMake with SFML path detection
echo ""
echo "Configuring CMake..."
echo "--------------------"

if [[ "$OS" == "macOS" ]]; then
    # Try to find SFML in common macOS locations
    SFML_PATHS=(
        "/opt/homebrew/opt/sfml@2/lib/cmake/SFML"
        "/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML"
        "/usr/local/opt/sfml@2/lib/cmake/SFML"
        "/usr/local/lib/cmake/SFML"
    )
    
    SFML_FOUND=false
    for SFML_PATH in "${SFML_PATHS[@]}"; do
        if [ -d "$SFML_PATH" ]; then
            echo -e "${GREEN}✓ Found SFML at: $SFML_PATH${NC}"
            SFML_FOUND=true
            cmake .. -DSFML_DIR="$SFML_PATH"
            break
        fi
    done
    
    if [ "$SFML_FOUND" = false ]; then
        echo -e "${RED}✗ SFML not found in common locations${NC}"
        echo ""
        echo "Please install SFML using Homebrew:"
        echo "  brew install sfml@2"
        echo ""
        echo "Or specify SFML path manually:"
        echo "  cmake .. -DSFML_DIR=/path/to/SFML"
        exit 1
    fi
else
    # Linux - let CMake find SFML automatically
    echo "Configuring for Linux..."
    cmake ..
fi

# Build
echo ""
echo "Building project..."
echo "-------------------"
cmake --build . --config Release

echo ""
echo -e "${GREEN}=========================================="
echo "Build completed successfully!"
echo "==========================================${NC}"
echo ""
echo "Executable location: $BUILD_DIR/kkurs"
echo ""
echo "To run the game:"
echo "  cd $BUILD_DIR"
echo "  ./kkurs"
echo ""
echo "To run tests:"
echo "  cd $BUILD_DIR"
echo "  ctest --output-on-failure"
echo ""
