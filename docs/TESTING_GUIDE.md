# Testing Guide / Руководство по Тестированию
# Road Trip Adventure RPG - "Автоподвиг 90-х"

**Version:** 1.0  
**Date:** 2025-11-12  
**Status:** Active

---

## Table of Contents / Оглавление

1. [Introduction](#introduction--введение)
2. [Getting Started](#getting-started--начало-работы)
3. [Writing Tests](#writing-tests--написание-тестов)
4. [Running Tests](#running-tests--запуск-тестов)
5. [Best Practices](#best-practices--лучшие-практики)
6. [Debugging Failed Tests](#debugging-failed-tests--отладка-неуспешных-тестов)
7. [CI/CD Integration](#cicd-integration--интеграция-cicd)
8. [FAQ](#faq--часто-задаваемые-вопросы)

---

## Introduction / Введение

### Purpose / Назначение

This guide provides comprehensive instructions for writing, running, and maintaining tests for the Road Trip Adventure RPG project. The testing strategy ensures code quality, catches bugs early, and maintains the 60 FPS performance target.

Это руководство предоставляет полные инструкции по написанию, запуску и поддержке тестов для проекта Road Trip Adventure RPG. Стратегия тестирования обеспечивает качество кода, раннее обнаружение ошибок и поддержку целевой производительности 60 FPS.

### Testing Philosophy / Философия Тестирования

Our testing approach follows these principles:

1. **Test First**: Write tests before or alongside code
2. **Fast Feedback**: Tests should run quickly
3. **Isolation**: Each test is independent
4. **Clarity**: Tests serve as documentation
5. **Automation**: Tests run automatically in CI/CD

Наш подход к тестированию следует этим принципам:

1. **Тесты сначала**: Писать тесты до или одновременно с кодом
2. **Быстрая обратная связь**: Тесты должны выполняться быстро
3. **Изоляция**: Каждый тест независим
4. **Ясность**: Тесты служат документацией
5. **Автоматизация**: Тесты запускаются автоматически в CI/CD

---

## Getting Started / Начало Работы

### Prerequisites / Предварительные Требования

#### Required Software

- **CMake** >= 3.10
- **C++ Compiler** with C++17 support (GCC, Clang, MSVC)
- **SFML** 2.5+ 
- **Google Test** (installed automatically via FetchContent)

#### Mac OS Setup

```bash
# Install dependencies via Homebrew
brew install cmake sfml

# Clone repository
git clone https://github.com/barzikol/kkurs.git
cd kkurs

# Build with tests
mkdir build && cd build
cmake ..
cmake --build .

# Run tests
ctest --output-on-failure
```

#### Linux Setup

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install cmake g++ libsfml-dev

# Clone and build
git clone https://github.com/barzikol/kkurs.git
cd kkurs
mkdir build && cd build
cmake ..
cmake --build .

# Run tests
ctest --output-on-failure
```

### Project Structure / Структура Проекта

```
kkurs/
├── include/              # Header files
├── src/                  # Source files
├── tests/                # Test directory
│   ├── unit/            # Unit tests
│   ├── integration/     # Integration tests
│   ├── mocks/           # Mock objects
│   ├── utils/           # Test utilities
│   └── CMakeLists.txt   # Test configuration
├── scripts/             # Helper scripts
│   ├── run_all_tests.sh
│   ├── run_unit_tests.sh
│   └── ...
└── CMakeLists.txt       # Main build configuration
```

---

## Writing Tests / Написание Тестов

### Unit Tests / Модульные Тесты

Unit tests verify individual classes and functions in isolation.

#### Basic Structure

```cpp
#include <gtest/gtest.h>
#include "YourClass.h"

// Simple test
TEST(TestSuiteName, TestName) {
    // Arrange - set up test data
    YourClass obj;
    
    // Act - perform the action
    int result = obj.someMethod();
    
    // Assert - verify the result
    EXPECT_EQ(result, expectedValue);
}
```

#### Example: Testing Character Class

```cpp
// tests/unit/test_character.cpp
#include <gtest/gtest.h>
#include "Character.h"

class CharacterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Runs before each test
    }
    
    void TearDown() override {
        // Runs after each test
    }
};

TEST_F(CharacterTest, ExRacerArchetypeHasCorrectSkipChance) {
    Character character(CharacterArchetype::EX_RACER, "Иван", 30);
    
    EXPECT_EQ(character.getName(), "Иван");
    EXPECT_EQ(character.getAge(), 30);
    EXPECT_FLOAT_EQ(character.getEventSkipChance(), 0.16f);
}

TEST_F(CharacterTest, StudentArchetypeBoostsMorale) {
    Character character(CharacterArchetype::STUDENT, "Мария", 22);
    
    EXPECT_TRUE(character.hasAbility("joy_aura"));
    EXPECT_GT(character.getMoraleBonus(), 0.0f);
}

TEST_F(CharacterTest, UnemployedHasHardDifficulty) {
    Character character(CharacterArchetype::UNEMPLOYED, "Петр", 35);
    
    EXPECT_EQ(character.getDifficulty(), Difficulty::HARD);
    EXPECT_LT(character.getInitialMorale(), 50.0f);
}
```

### Integration Tests / Интеграционные Тесты

Integration tests verify that multiple components work together correctly.

#### Example: Scene Transition Test

```cpp
// tests/integration/test_scene_transitions.cpp
#include <gtest/gtest.h>
#include "SceneManager.h"
#include <SFML/Graphics.hpp>

class SceneTransitionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a window for testing
        window.create(sf::VideoMode(800, 600), "Test", sf::Style::None);
        manager = std::make_unique<SceneManager>(window);
    }
    
    void TearDown() override {
        window.close();
    }
    
    sf::RenderWindow window;
    std::unique_ptr<SceneManager> manager;
};

TEST_F(SceneTransitionTest, MainMenuToCharacterCreation) {
    // Start at main menu
    ASSERT_EQ(manager->getCurrentSceneType(), SceneType::MAIN_MENU);
    
    // Simulate selecting "New Game"
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::Enter;
    
    manager->getCurrentScene()->handleInput(event);
    manager->update(0.016f);
    
    // Should transition to character creation
    EXPECT_EQ(manager->getNextSceneType(), SceneType::CHARACTER_CREATION);
}

TEST_F(SceneTransitionTest, DataTransferBetweenScenes) {
    // Create character in creation scene
    Character testCharacter(CharacterArchetype::EX_RACER, "Test", 25);
    
    // Store in player state
    PlayerState::getInstance().setCharacter(testCharacter);
    
    // Transition to next scene
    manager->transitionTo(SceneType::ROUTE_SELECTION);
    
    // Verify data is accessible in new scene
    const Character& retrieved = PlayerState::getInstance().getCharacter();
    EXPECT_EQ(retrieved.getName(), "Test");
    EXPECT_EQ(retrieved.getArchetype(), CharacterArchetype::EX_RACER);
}
```

### Test Fixtures / Тестовые Фикстуры

Fixtures provide common setup for multiple tests.

```cpp
class ResourceManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        rm = &ResourceManager::getInstance();
        rm->reset(); // Start with clean state
    }
    
    ResourceManager* rm;
};

TEST_F(ResourceManagerTest, FuelConsumption) {
    rm->setFuel(100.0f);
    rm->modifyFuel(-25.0f);
    EXPECT_FLOAT_EQ(rm->getFuel(), 75.0f);
}

TEST_F(ResourceManagerTest, CannotExceedMaxFuel) {
    rm->setFuel(100.0f);
    rm->modifyFuel(50.0f);  // Try to add fuel beyond max
    EXPECT_LE(rm->getFuel(), 100.0f);
}
```

### Assertions / Утверждения

#### Basic Assertions

```cpp
// Boolean
EXPECT_TRUE(condition);
EXPECT_FALSE(condition);

// Equality
EXPECT_EQ(actual, expected);
EXPECT_NE(actual, notExpected);

// Comparison
EXPECT_LT(val1, val2);  // Less than
EXPECT_LE(val1, val2);  // Less or equal
EXPECT_GT(val1, val2);  // Greater than
EXPECT_GE(val1, val2);  // Greater or equal

// Floating point
EXPECT_FLOAT_EQ(actual, expected);
EXPECT_DOUBLE_EQ(actual, expected);
EXPECT_NEAR(actual, expected, tolerance);

// String
EXPECT_STREQ(str1, str2);
EXPECT_STRNE(str1, str2);

// Pointers
EXPECT_EQ(ptr, nullptr);
EXPECT_NE(ptr, nullptr);
```

#### EXPECT vs ASSERT

- **EXPECT_***: Test continues after failure (non-fatal)
- **ASSERT_***: Test stops immediately after failure (fatal)

Use ASSERT when continuing would be unsafe or meaningless.

```cpp
TEST(MyTest, Example) {
    MyClass* obj = createObject();
    ASSERT_NE(obj, nullptr);  // Must pass to continue
    
    EXPECT_EQ(obj->getValue(), 42);  // Can fail but test continues
    EXPECT_TRUE(obj->isValid());
}
```

---

## Running Tests / Запуск Тестов

### Using CTest / Использование CTest

```bash
# Run all tests
cd build
ctest --output-on-failure

# Run with verbose output
ctest --output-on-failure --verbose

# Run specific test by name
ctest -R UnitTests

# Run tests matching pattern
ctest -R Character

# Run tests with label
ctest -L unit
ctest -L integration
```

### Using Test Executables Directly

```bash
# Run unit tests
cd build
./unit_tests

# Run specific test suite
./unit_tests --gtest_filter=CharacterTest.*

# Run specific test
./unit_tests --gtest_filter=CharacterTest.ExRacerArchetype

# List all tests without running
./unit_tests --gtest_list_tests

# Run tests multiple times
./unit_tests --gtest_repeat=10

# Run with output to file
./unit_tests --gtest_output=xml:test_results.xml
```

### Using Scripts / Использование Скриптов

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

### Test Options / Опции Тестов

```bash
# Show help
./unit_tests --help

# Useful options
--gtest_filter=<pattern>     # Run matching tests
--gtest_repeat=<n>           # Repeat tests n times
--gtest_shuffle             # Randomize test order
--gtest_random_seed=<n>     # Set random seed
--gtest_output=xml:<file>   # Output to XML file
--gtest_break_on_failure    # Break debugger on failure
--gtest_catch_exceptions=0  # Don't catch exceptions
```

---

## Best Practices / Лучшие Практики

### 1. Test Naming / Именование Тестов

**Pattern**: `TEST(TestSuiteName, What_Scenario_ExpectedResult)`

Good examples:
```cpp
TEST(CharacterTest, ExRacer_InitialState_HasSkipChance)
TEST(ResourceManager, ModifyFuel_NegativeAmount_DecreasesValue)
TEST(SceneManager, TransitionToInvalidScene_RemainsAtCurrentScene)
```

Bad examples:
```cpp
TEST(Test1, Test)           // Too vague
TEST(CharacterTest, Test2)  // No description
TEST(A, B)                  // Meaningless names
```

### 2. AAA Pattern / Паттерн AAA

Structure tests in three clear sections:

```cpp
TEST(ResourceManager, ModifyMoney_PositiveAmount_IncreasesBalance) {
    // Arrange - подготовка
    ResourceManager rm;
    rm.setMoney(100);
    
    // Act - действие
    rm.modifyMoney(50);
    
    // Assert - проверка
    EXPECT_EQ(rm.getMoney(), 150);
}
```

### 3. One Assertion Per Test? / Одна проверка на тест?

**Guideline**: One *logical* assertion per test, but multiple EXPECT statements for the same concept are OK.

Good:
```cpp
TEST(Character, Initialization_SetsAllProperties) {
    Character c(CharacterArchetype::STUDENT, "Иван", 20);
    
    // All these verify the same logical concept: proper initialization
    EXPECT_EQ(c.getName(), "Иван");
    EXPECT_EQ(c.getAge(), 20);
    EXPECT_EQ(c.getArchetype(), CharacterArchetype::STUDENT);
}
```

Bad:
```cpp
TEST(Character, VariousTests) {
    Character c(CharacterArchetype::STUDENT, "Иван", 20);
    
    EXPECT_EQ(c.getName(), "Иван");  // Testing initialization
    
    c.setMorale(75.0f);
    EXPECT_EQ(c.getMorale(), 75.0f);  // Testing morale - different concept!
}
```

### 4. Test Independence / Независимость Тестов

Each test must be independent and not rely on other tests.

Good:
```cpp
TEST(PlayerState, SetAndGetFuel) {
    PlayerState& ps = PlayerState::getInstance();
    ps.reset();  // Start with clean state
    
    ps.setFuel(80.0f);
    EXPECT_FLOAT_EQ(ps.getFuel(), 80.0f);
}

TEST(PlayerState, SetAndGetMoney) {
    PlayerState& ps = PlayerState::getInstance();
    ps.reset();  // Independent of previous test
    
    ps.setMoney(200);
    EXPECT_EQ(ps.getMoney(), 200);
}
```

### 5. Use Fixtures for Common Setup

```cpp
class GridInventoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        inventory = std::make_unique<GridInventorySystem>(6, 4);
        testItem = Item{"Wrench", "Tool", 1.5f, 50, ItemRarity::COMMON};
    }
    
    std::unique_ptr<GridInventorySystem> inventory;
    Item testItem;
};

TEST_F(GridInventoryTest, AddItem_ValidPosition_Success) {
    int id = inventory->addItem(testItem, 1, 1, 0, 0);
    EXPECT_GE(id, 0);
}

TEST_F(GridInventoryTest, AddItem_OutOfBounds_Fails) {
    int id = inventory->addItem(testItem, 1, 1, 10, 10);
    EXPECT_EQ(id, -1);
}
```

### 6. Test Error Conditions

Don't just test the happy path!

```cpp
TEST(ResourceManager, ModifyFuel_BelowZero_ClampsToZero) {
    ResourceManager rm;
    rm.setFuel(20.0f);
    rm.modifyFuel(-50.0f);  // Try to go negative
    EXPECT_GE(rm.getFuel(), 0.0f);
}

TEST(GridInventory, RotateItem_NoSpace_Fails) {
    GridInventorySystem inv(2, 2);
    
    // Fill grid so rotation won't fit
    inv.addItem(item1, 2, 1, 0, 0);
    inv.addItem(item2, 2, 1, 0, 1);
    
    int id1 = inv.getItemIdAt(0, 0);
    bool rotated = inv.rotateItem(id1);
    
    EXPECT_FALSE(rotated);  // Should fail due to no space
}
```

### 7. Avoid Magic Numbers

```cpp
// Bad
TEST(Character, AgeValidation) {
    Character c(CharacterArchetype::STUDENT, "Test", 25);
    EXPECT_EQ(c.getAge(), 25);
    EXPECT_GT(c.getAge(), 18);
    EXPECT_LT(c.getAge(), 100);
}

// Good
TEST(Character, AgeValidation) {
    const int TEST_AGE = 25;
    const int MIN_AGE = 18;
    const int MAX_AGE = 100;
    
    Character c(CharacterArchetype::STUDENT, "Test", TEST_AGE);
    EXPECT_EQ(c.getAge(), TEST_AGE);
    EXPECT_GT(c.getAge(), MIN_AGE);
    EXPECT_LT(c.getAge(), MAX_AGE);
}
```

### 8. Performance Tests

```cpp
#include <chrono>

TEST(PerformanceTest, UpdateLoop_RunsAt60FPS) {
    const float TARGET_FRAME_TIME = 1.0f / 60.0f;  // 16.67ms
    const int NUM_ITERATIONS = 100;
    
    Scene* scene = createTestScene();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        scene->update(TARGET_FRAME_TIME);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    float averageFrameTime = duration.count() / (float)NUM_ITERATIONS;
    
    EXPECT_LT(averageFrameTime, TARGET_FRAME_TIME * 1000);  // Should be under 16.67ms
}
```

---

## Debugging Failed Tests / Отладка Неуспешных Тестов

### Reading Test Output / Чтение Вывода Тестов

```
[==========] Running 5 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 3 tests from CharacterTest
[ RUN      ] CharacterTest.ExRacerHasSkipChance
[       OK ] CharacterTest.ExRacerHasSkipChance (0 ms)
[ RUN      ] CharacterTest.StudentBoostsMorale
[       OK ] CharacterTest.StudentBoostsMorale (0 ms)
[ RUN      ] CharacterTest.UnemployedHardMode
test_character.cpp:45: Failure
Expected equality of these values:
  character.getDifficulty()
    Which is: 1
  Difficulty::HARD
    Which is: 2
[  FAILED  ] CharacterTest.UnemployedHardMode (1 ms)
[----------] 3 tests from CharacterTest (1 ms total)
```

Key information:
- Test name that failed
- File and line number of failure
- Expected vs actual values
- Time taken

### Using Debugger / Использование Отладчика

```bash
# GDB (Linux/Mac)
gdb ./unit_tests
(gdb) run --gtest_filter=CharacterTest.UnemployedHardMode
# When it fails:
(gdb) backtrace
(gdb) print character
(gdb) print character.getDifficulty()

# LLDB (Mac)
lldb ./unit_tests
(lldb) run --gtest_filter=CharacterTest.UnemployedHardMode
(lldb) bt
(lldb) frame variable
```

### Adding Debug Output

```cpp
TEST(MyTest, DebugExample) {
    int value = calculate();
    
    // Print debug info
    std::cout << "Calculated value: " << value << std::endl;
    
    EXPECT_EQ(value, 42);
}

// Or use custom message
TEST(MyTest, CustomMessage) {
    int actual = calculate();
    int expected = 42;
    
    EXPECT_EQ(actual, expected) << "Calculation failed. Expected: " 
                                << expected << ", Got: " << actual;
}
```

### Common Issues / Распространённые Проблемы

#### 1. Floating Point Comparison

```cpp
// ❌ Bad - will fail due to precision
float result = 0.1f + 0.2f;
EXPECT_EQ(result, 0.3f);

// ✅ Good - allows tolerance
EXPECT_FLOAT_EQ(result, 0.3f);
// or
EXPECT_NEAR(result, 0.3f, 0.0001f);
```

#### 2. Uninitialized Variables

```cpp
// ❌ Bad
TEST(MyTest, BadInit) {
    int value;  // Uninitialized!
    EXPECT_GT(value, 0);  // Undefined behavior
}

// ✅ Good
TEST(MyTest, GoodInit) {
    int value = 0;  // Initialized
    EXPECT_GE(value, 0);
}
```

#### 3. Test Order Dependencies

```cpp
// ❌ Bad - Test2 depends on Test1
TEST(BadTest, Test1) {
    globalCounter = 5;
}

TEST(BadTest, Test2) {
    EXPECT_EQ(globalCounter, 5);  // Assumes Test1 ran first!
}

// ✅ Good - Each test is independent
TEST(GoodTest, Test1) {
    int counter = 5;
    EXPECT_EQ(counter, 5);
}

TEST(GoodTest, Test2) {
    int counter = 5;  // Own setup
    EXPECT_EQ(counter, 5);
}
```

---

## CI/CD Integration / Интеграция CI/CD

### GitHub Actions Workflow

Tests run automatically on every push and pull request.

**Configuration**: `.github/workflows/test.yml`

```yaml
name: Tests

on:
  push:
    branches: [ main, develop, copilot/** ]
  pull_request:
    branches: [ main, develop ]

jobs:
  test-linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install Dependencies
        run: sudo apt-get install -y cmake g++ libsfml-dev
      - name: Build and Test
        run: |
          mkdir build && cd build
          cmake ..
          cmake --build .
          ctest --output-on-failure

  test-macos:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install Dependencies
        run: brew install cmake sfml
      - name: Build and Test
        run: |
          mkdir build && cd build
          cmake ..
          cmake --build .
          ctest --output-on-failure
```

### Viewing Test Results

1. Go to your pull request on GitHub
2. Click on "Checks" tab
3. View test results for each platform
4. Click on failed tests to see details

### Coverage Reports

Coverage reports are generated and uploaded to Codecov.

View at: `https://codecov.io/gh/barzikol/kkurs`

---

## FAQ / Часто Задаваемые Вопросы

### Q: How do I add a new test file?

A: 
1. Create file in `tests/unit/` or `tests/integration/`
2. Add to `tests/CMakeLists.txt`:
   ```cmake
   set(UNIT_TEST_SOURCES
       ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_sanity.cpp
       ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_mynewfile.cpp  # Add here
   )
   ```
3. Rebuild and run tests

### Q: Tests pass locally but fail in CI

A:
- Check for environment-specific assumptions
- Verify all dependencies are installed in CI
- Look for timing-dependent tests
- Check for absolute vs relative paths

### Q: How do I test SFML rendering?

A:
- Use headless mode (no window creation) where possible
- For rendering tests, use `sf::RenderTexture` instead of `sf::RenderWindow`
- Verify logic, not visual output
- Consider integration tests for full rendering

### Q: Should I test private methods?

A:
No, test through public interface. If you need to test private methods, they might belong in a separate class.

### Q: How do I handle random behavior in tests?

A:
1. Seed random number generators with fixed value
2. Mock random functions to return predictable values
3. Test statistical properties over many runs

### Q: What's the difference between Mock and Stub?

A:
- **Stub**: Simple replacement with hardcoded responses
- **Mock**: Tracks calls and can verify behavior

### Q: How often should I run tests?

A:
- **Unit tests**: After every code change (seconds)
- **Integration tests**: Before committing (minutes)
- **Full suite**: In CI/CD (automatic)

---

## Additional Resources / Дополнительные Ресурсы

### Documentation

- [Google Test Primer](https://google.github.io/googletest/primer.html)
- [Google Test Advanced Guide](https://google.github.io/googletest/advanced.html)
- [Test Strategy](tasks/test-strategy.md)
- [Test Environment Plan](TEST_ENV_PLAN.md)

### Tools

- **CTest**: CMake's test runner
- **lcov**: Code coverage visualization
- **Codecov**: Coverage reporting service
- **Valgrind**: Memory leak detection
- **AddressSanitizer**: Memory error detection

### Tips for Success

1. Write tests as you code
2. Run tests frequently
3. Keep tests fast
4. Maintain test code quality
5. Review test failures immediately
6. Update tests when requirements change

---

**Document Status**: Active  
**Last Updated**: 2025-11-12  
**Maintainers**: Development Team  
**Questions**: Open an issue on GitHub
