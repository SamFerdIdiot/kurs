#!/bin/bash
# Run tests for Road Trip Adventure RPG
# Запустить тесты для проекта "69 Crossroads"

set -e  # Exit on error

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"

echo "=========================================="
echo "Running Tests / Запуск тестов"
echo "=========================================="
echo ""

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${RED}Build directory not found.${NC}"
    echo "Please run ./scripts/build.sh first"
    exit 1
fi

cd "$BUILD_DIR"

# Check if tests were built
if [ ! -f "./tests/unit_tests" ]; then
    echo -e "${RED}Tests not found.${NC}"
    echo "Building tests..."
    cmake --build . --target unit_tests
fi

# Run tests
echo "Running unit tests..."
echo "--------------------"
./tests/unit_tests

echo ""
echo "Running CTest..."
echo "----------------"
ctest --output-on-failure

echo ""
echo -e "${GREEN}=========================================="
echo "All tests passed! / Все тесты прошли!"
echo "==========================================${NC}"
