#!/bin/bash
# Run unit tests only
# Запустить только модульные тесты

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"

echo "=========================================="
echo "Running Unit Tests / Запуск модульных тестов"
echo "=========================================="

cd "$BUILD_DIR"
if [ -f "./unit_tests" ]; then
    ./unit_tests
else
    echo "Unit tests executable not found. Run 'cmake --build . --target unit_tests' first."
    exit 1
fi

echo ""
echo "Unit tests completed / Модульные тесты завершены"
