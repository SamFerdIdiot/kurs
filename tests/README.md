# Test Suite for Road Trip Adventure RPG
# Набор Тестов для Проекта "Автоподвиг 90-х"

This directory contains all tests for the project.

## Structure / Структура

```
tests/
├── unit/                   # Unit tests (модульные тесты)
│   ├── test_character.cpp
│   ├── test_car.cpp
│   └── ...
├── integration/            # Integration tests (интеграционные тесты)
│   ├── test_scene_transitions.cpp
│   └── ...
├── mocks/                  # Mock objects (моки и заглушки)
│   ├── sfml/              # Mock SFML for testing without SFML installation
│   │   └── SFML/
│   │       ├── System.hpp
│   │       ├── Window.hpp
│   │       └── Graphics.hpp
│   ├── mock_scene.h
│   └── ...
├── utils/                  # Test utilities (утилиты)
│   ├── test_helpers.h
│   └── ...
├── README.md              # This file
└── CMakeLists.txt         # Test build configuration
```

## Mock SFML Infrastructure / Инфраструктура Моков SFML

The project includes a mock SFML library that allows tests to run without having SFML installed. This is particularly useful for CI/CD environments and developers who want to run tests without installing graphics libraries.

**Проект включает мок-библиотеку SFML, которая позволяет запускать тесты без установки SFML. Это особенно полезно для CI/CD окружений и разработчиков, которые хотят запускать тесты без установки графических библиотек.**

### How it works / Как это работает

- When SFML is found, tests use the real SFML library
- When SFML is not found, tests use mock SFML from `tests/mocks/sfml/`
- Mock SFML provides stub implementations of SFML types (Vector2f, RenderWindow, etc.)
- Mock implementations focus on testing business logic, not graphics rendering

**Когда SFML найдена, тесты используют реальную библиотеку SFML**  
**Когда SFML не найдена, тесты используют мок SFML из `tests/mocks/sfml/`**  
**Мок SFML предоставляет заглушки для типов SFML (Vector2f, RenderWindow и т.д.)**  
**Моки фокусируются на тестировании бизнес-логики, а не отрисовки графики**

## Running Tests / Запуск Тестов

### Using CMake

```bash
# Build tests
mkdir build && cd build
cmake ..
cmake --build . --target all

# Run all tests
ctest --output-on-failure

# Run specific test suite
./unit_tests
./integration_tests
```

### Using Scripts

```bash
# Run all tests
./scripts/run_all_tests.sh

# Run unit tests only
./scripts/run_unit_tests.sh

# Run integration tests only
./scripts/run_integration_tests.sh

# Generate coverage report
./scripts/generate_coverage.sh
```

## Writing Tests / Написание Тестов

### Example Unit Test

```cpp
#include <gtest/gtest.h>
#include "Character.h"

TEST(CharacterTest, InitializationWithExRacer) {
    Character character(CharacterArchetype::EX_RACER, "Test", 30);
    
    EXPECT_EQ(character.getName(), "Test");
    EXPECT_EQ(character.getAge(), 30);
    EXPECT_FLOAT_EQ(character.getEventSkipChance(), 0.16f);
}
```

### Example Integration Test

```cpp
#include <gtest/gtest.h>
#include "SceneManager.h"

TEST(SceneIntegration, MainMenuTransition) {
    // Setup
    sf::RenderWindow window;
    SceneManager manager(window);
    
    // Action
    manager.transitionTo(SceneType::CHARACTER_CREATION);
    
    // Assert
    EXPECT_EQ(manager.getCurrentScene(), SceneType::CHARACTER_CREATION);
}
```

## Test Guidelines / Руководство по Тестам

1. **Naming**: Use descriptive names - `TEST(ClassName, MethodName_Scenario_ExpectedBehavior)`
2. **Isolation**: Each test should be independent
3. **AAA Pattern**: Arrange, Act, Assert
4. **No Magic Numbers**: Use constants with meaningful names
5. **Fast**: Unit tests should run in < 5 seconds
6. **Documentation**: Add comments for complex tests

## Coverage Requirements / Требования к Покрытию

- **Overall**: > 70%
- **Core Systems**: > 80%
  - ResourceManager
  - GridInventorySystem
  - EventBus
  - PlayerState
  - SceneManager

## Continuous Integration / Непрерывная Интеграция

Tests run automatically on:
- Every push to main, develop, copilot/** branches
- Every pull request
- Platforms: Linux, Mac OS

See `.github/workflows/test.yml` for CI configuration.

## Troubleshooting / Решение Проблем

### Test Fails Locally But Passes in CI

- Check environment differences
- Verify dependencies versions
- Look for timing-dependent tests

### SFML Not Found (Optional)

Tests can run without SFML using the mock library. However, if you want to test with real SFML:

**Тесты могут работать без SFML, используя мок-библиотеку. Однако, если вы хотите тестировать с реальной SFML:**

```bash
# Mac OS
cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml/lib/cmake/SFML

# Linux
sudo apt-get install libsfml-dev
```

## Resources / Ресурсы

- [Google Test Documentation](https://google.github.io/googletest/)
- [Testing Best Practices](../docs/TESTING_GUIDE.md)
- [Test Strategy](../docs/tasks/test-strategy.md)
- [Test Environment Plan](../docs/TEST_ENV_PLAN.md)

---

**Last Updated**: 2025-11-12  
**Maintainers**: Development Team
