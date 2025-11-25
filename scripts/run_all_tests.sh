#!/bin/bash
# Run all tests for Road Trip Adventure RPG
# Запустить все тесты для проекта "Автоподвиг 90-х"

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"

echo "=========================================="
echo "Running All Tests / Запуск всех тестов"
echo "=========================================="

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Creating and configuring..."
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    cmake ..
fi

# Build tests
echo ""
echo "Building tests / Сборка тестов..."
cd "$BUILD_DIR"
cmake --build . --target all

# Run all tests using CTest
echo ""
echo "Running tests / Запуск тестов..."
ctest --output-on-failure --verbose

echo ""
echo "=========================================="
echo "All tests completed / Все тесты завершены"
echo "=========================================="
