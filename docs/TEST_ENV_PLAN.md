# План Организации Тестовой Среды / Test Environment Plan
# Проект "Автоподвиг 90-х" / Project "Autopodvig 90s"

**Версия:** 1.0  
**Дата создания:** 2025-11-12  
**Статус:** В разработке / In Development

---

## Оглавление / Table of Contents

1. [Обзор](#обзор--overview)
2. [Анализ Структуры](#анализ-структуры--structure-analysis)
3. [Компоненты Тестовой Инфраструктуры](#компоненты-тестовой-инфраструктуры--test-infrastructure-components)
4. [Поэтапный План Реализации](#поэтапный-план-реализации--implementation-plan)
5. [Интеграция с CI/CD](#интеграция-с-cicd--cicd-integration)
6. [Поддержка Mac OS](#поддержка-mac-os--mac-os-support)
7. [Критерии Приемки](#критерии-приемки--acceptance-criteria)
8. [Ответственные и Сроки](#ответственные-и-сроки--responsibilities-and-timeline)
9. [Ссылки на Документацию](#ссылки-на-документацию--documentation-references)

---

## Обзор / Overview

### Цель / Purpose

Организовать полноценную тестовую среду для проекта 'Автоподвиг 90-х', обеспечивающую:
- Автоматизированное тестирование кода (unit, integration)
- Непрерывную интеграцию (CI/CD)
- Поддержку разработки на Mac OS
- Соответствие требованиям из PROMPT_TEMPLATE.md

Create a comprehensive testing environment for the 'Autopodvig 90s' project, providing:
- Automated code testing (unit, integration)
- Continuous integration (CI/CD)
- Mac OS development support
- Compliance with PROMPT_TEMPLATE.md requirements

### Принципы / Principles

1. **Немедленная Работоспособность** - все тесты должны работать сразу после создания
2. **Независимость** - тесты не должны зависеть от внешних сервисов
3. **Автоматизация** - тесты запускаются автоматически при изменениях
4. **Документированность** - каждый компонент имеет документацию
5. **Кросс-платформенность** - поддержка Linux, Mac OS, Windows

---

## Анализ Структуры / Structure Analysis

### Текущее Состояние / Current State

#### Существующие Компоненты / Existing Components

```
kkurs/
├── docs/
│   ├── tasks.md                    ✅ Существует / Exists
│   ├── PROMPT_TEMPLATE.md          ✅ Существует / Exists
│   ├── progress.md                 ✅ Существует / Exists
│   └── tasks/
│       └── test-strategy.md        ✅ Существует / Exists
├── include/                        ✅ Заголовочные файлы / Headers
├── src/                            ✅ Исходный код / Source
├── CMakeLists.txt                  ✅ Система сборки / Build system
└── tests/                          ❌ ОТСУТСТВУЕТ / MISSING
```

#### Недостающие Компоненты / Missing Components

- ❌ Директория tests/ с unit и integration тестами
- ❌ GitHub Actions workflows для CI/CD
- ❌ Mocks и test utilities
- ❌ Скрипты автоматизации тестирования
- ❌ TESTING_GUIDE.md документация
- ❌ CMake конфигурация для тестов

### Анализ Требований из docs/tasks.md

Согласно [docs/tasks.md](tasks.md):

**Developer Stories:**
- **US-D-004:** Comprehensive Testing
  - Unit tests for all core systems ✅ Планируется
  - Integration tests for scene transitions ✅ Планируется
  - Test coverage > 70% 🎯 Цель
  - Tests run automatically in CI/CD ✅ Планируется
  - Test failures are clearly reported ✅ Планируется

**Фаза 8:** Testing & QA (из MASTER_IMPLEMENTATION_PLAN)
- Automated test suite ✅ Планируется
- Manual QA checklist ✅ Планируется
- CI/CD pipeline ✅ Планируется
- Code review process ✅ Планируется
- Performance benchmarking ✅ Планируется

---

## Компоненты Тестовой Инфраструктуры / Test Infrastructure Components

### 1. Unit Tests (Модульные Тесты)

**Цель:** Тестирование отдельных классов и функций в изоляции

**Структура:**
```
tests/unit/
├── test_character.cpp           # Тесты класса Character
├── test_car.cpp                 # Тесты класса Car
├── test_player_state.cpp        # Тесты PlayerState
├── test_resource_manager.cpp    # Тесты ResourceManager
├── test_grid_inventory.cpp      # Тесты GridInventorySystem
├── test_event_bus.cpp           # Тесты EventBus
├── test_npc_manager.cpp         # Тесты NPCManager
└── test_scene_manager.cpp       # Тесты SceneManager
```

**Фреймворк:** Google Test (gtest/gmock)

**Примеры Тестов:**
- Инициализация объектов
- Валидация входных данных
- Корректность вычислений
- Управление состоянием
- Обработка ошибок

### 2. Integration Tests (Интеграционные Тесты)

**Цель:** Тестирование взаимодействия между компонентами

**Структура:**
```
tests/integration/
├── test_scene_transitions.cpp   # Переходы между сценами
├── test_event_bus_integration.cpp # Интеграция EventBus
├── test_resource_flow.cpp       # Поток ресурсов через системы
├── test_inventory_integration.cpp # Инвентарь + UI + PlayerState
├── test_npc_interactions.cpp    # NPC + Диалоги + Квесты
└── test_save_load.cpp           # Сохранение/загрузка игры
```

**Сценарии:**
- Полный цикл игры (меню → игра → выход)
- Обновление ресурсов → HUD отображение
- Добавление предмета → обновление инвентаря → UI refresh
- Начало квеста → прогресс → завершение
- Scene A → EventBus → Scene B transfer data

### 3. Mocks & Stubs (Моки и Заглушки)

**Цель:** Изолировать тестируемый код от зависимостей

**Структура:**
```
tests/mocks/
├── mock_sfml.h                  # Заглушки для SFML (если нужно)
├── mock_event_bus.h             # Мок EventBus для изоляции
├── mock_scene.h                 # Мок Scene для тестов менеджера
├── mock_resource_manager.h      # Мок ResourceManager
└── test_fixtures.h              # Общие фикстуры для тестов
```

**Использование:**
- Тестирование без графики (headless)
- Симуляция событий SFML
- Контроль времени (deltaTime)
- Предсказуемое поведение зависимостей

### 4. Test Utilities (Утилиты)

**Структура:**
```
tests/utils/
├── test_helpers.h/cpp           # Общие вспомогательные функции
├── assertion_macros.h           # Кастомные макросы проверок
├── test_data_generator.h/cpp    # Генерация тестовых данных
└── performance_timer.h/cpp      # Измерение производительности
```

**Функции:**
- Создание тестовых объектов (builders)
- Сравнение плавающих чисел с погрешностью
- Генерация случайных тестовых данных
- Измерение времени выполнения
- Логирование результатов тестов

### 5. Test Scripts (Скрипты)

**Структура:**
```
scripts/
├── run_all_tests.sh             # Запуск всех тестов
├── run_unit_tests.sh            # Только unit тесты
├── run_integration_tests.sh     # Только integration тесты
├── generate_coverage.sh         # Генерация отчёта о покрытии
├── check_test_requirements.sh   # Проверка зависимостей
└── ci_test_runner.sh            # Скрипт для CI/CD
```

**Возможности:**
- Быстрый запуск конкретных наборов тестов
- Автоматическая генерация отчётов
- Интеграция с CI/CD
- Проверка окружения перед запуском

### 6. CMake Configuration (Конфигурация)

**Дополнения в CMakeLists.txt:**
```cmake
# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)

# Create test library with common code
add_library(kkurs_lib STATIC
    ${SOURCES}  # Все исходники без main.cpp
)

# Unit tests executable
add_executable(unit_tests
    tests/unit/test_character.cpp
    tests/unit/test_car.cpp
    # ... more unit tests
)
target_link_libraries(unit_tests
    kkurs_lib
    GTest::GTest
    GTest::Main
    sfml-graphics sfml-window sfml-system
)
add_test(NAME UnitTests COMMAND unit_tests)

# Integration tests executable
add_executable(integration_tests
    tests/integration/test_scene_transitions.cpp
    # ... more integration tests
)
target_link_libraries(integration_tests
    kkurs_lib
    GTest::GTest
    GTest::Main
    sfml-graphics sfml-window sfml-system
)
add_test(NAME IntegrationTests COMMAND integration_tests)

# Code coverage (optional, for development)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(kkurs_lib PRIVATE --coverage)
        target_link_options(kkurs_lib PRIVATE --coverage)
    endif()
endif()
```

---

## Поэтапный План Реализации / Implementation Plan

### Этап 1: Инфраструктура (Неделя 1)

**Ответственный:** DevOps / Lead Developer  
**Продолжительность:** 5 дней

#### Задачи:

1. **День 1: Создание Структуры**
   - Создать директории: tests/{unit,integration,mocks,utils}
   - Создать scripts/ директорию
   - Добавить .gitignore для тестовых артефактов
   - Файлы: 
     - `tests/README.md` - описание структуры тестов
     - `tests/CMakeLists.txt` - конфигурация тестов

2. **День 2: CMake Конфигурация**
   - Обновить CMakeLists.txt для поддержки тестов
   - Настроить линковку с Google Test
   - Создать kkurs_lib библиотеку
   - Добавить targets для unit и integration тестов
   - Файл: `CMakeLists.txt` (обновлён)

3. **День 3: Google Test Integration**
   - Установить Google Test через FetchContent или system package
   - Создать базовый тест для проверки setup
   - Настроить test discovery в CMake
   - Файлы:
     - `tests/unit/test_sanity.cpp` - проверка работоспособности
     - `cmake/GoogleTest.cmake` - helper для интеграции

4. **День 4: Скрипты Автоматизации**
   - Создать run_all_tests.sh
   - Создать run_unit_tests.sh
   - Создать run_integration_tests.sh
   - Добавить check_test_requirements.sh
   - Файлы: `scripts/*.sh` (4 скрипта)

5. **День 5: Документация**
   - Создать docs/TESTING_GUIDE.md
   - Обновить docs/TEST_ENV_PLAN.md (этот файл)
   - Документировать команды запуска тестов
   - Обновить README.md с секцией Testing
   - Файлы: `docs/TESTING_GUIDE.md`, обновления в `README.md`

**Критерии Завершения:**
- ✅ Структура директорий создана
- ✅ CMake успешно конфигурируется с тестами
- ✅ Базовый тест компилируется и запускается
- ✅ Скрипты работают на Linux и Mac OS
- ✅ Документация актуальна

---

### Этап 2: Unit Tests (Неделя 2)

**Ответственный:** Development Team  
**Продолжительность:** 5 дней

#### Приоритет Тестирования:

**High Priority (Дни 1-3):**
1. **Character & Car Classes**
   - test_character.cpp (20-30 тестов)
   - test_car.cpp (15-20 тестов)
   
2. **PlayerState**
   - test_player_state.cpp (25-35 тестов)
   - Тестирование архетипов, ресурсов, состояния

3. **ResourceManager**
   - test_resource_manager.cpp (20-25 тестов)
   - Fuel, money, energy management

**Medium Priority (Дни 4-5):**
4. **GridInventorySystem**
   - test_grid_inventory.cpp (30-40 тестов)
   - Размещение, ротация, коллизии

5. **EventBus**
   - test_event_bus.cpp (25-30 тестов)
   - Pub/sub, очередь событий, типобезопасность

#### Пример Теста:

```cpp
// tests/unit/test_character.cpp
#include <gtest/gtest.h>
#include "Character.h"

class CharacterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Подготовка перед каждым тестом
    }
};

TEST_F(CharacterTest, ArchetypeExRacerHasCorrectBonus) {
    Character character(CharacterArchetype::EX_RACER, "Testov", 30);
    EXPECT_EQ(character.getEventSkipChance(), 0.16f);
    EXPECT_FLOAT_EQ(character.getSkillBonus("driving"), 1.2f);
}

TEST_F(CharacterTest, StudentArchetypeBoostsMorale) {
    Character character(CharacterArchetype::STUDENT, "Ivanov", 25);
    EXPECT_TRUE(character.hasAbility("joy_aura"));
    EXPECT_FLOAT_EQ(character.getMoraleMultiplier(), 1.1f);
}

TEST_F(CharacterTest, UnemployedStartsWithLowMorale) {
    Character character(CharacterArchetype::UNEMPLOYED, "Sidorov", 35);
    EXPECT_LT(character.getInitialMorale(), 50.0f);
    EXPECT_EQ(character.getDifficulty(), Difficulty::HARD);
}
```

**Критерии Завершения:**
- ✅ Минимум 5 классов покрыты unit тестами
- ✅ Покрытие кода > 60% (измерено)
- ✅ Все тесты проходят успешно
- ✅ Документация каждого теста (что проверяется)

---

### Этап 3: Integration Tests (Неделя 3)

**Ответственный:** QA Team + Developers  
**Продолжительность:** 5 дней

#### Приоритетные Сценарии:

**День 1-2: Scene Transitions**
```cpp
// tests/integration/test_scene_transitions.cpp
TEST(SceneIntegration, MainMenuToCharacterCreation) {
    SceneManager manager(window);
    
    // Начинаем с главного меню
    ASSERT_EQ(manager.getCurrentScene(), SceneType::MAIN_MENU);
    
    // Симулируем выбор "New Game"
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::Enter;
    
    manager.handleInput(event);
    manager.update(0.016f);
    
    // Должны перейти к созданию персонажа
    EXPECT_EQ(manager.getNextScene(), SceneType::CHARACTER_CREATION);
}

TEST(SceneIntegration, DataTransferBetweenScenes) {
    // Проверяем передачу данных при переходе WorldMap -> Cinematic
    // ...
}
```

**День 3-4: EventBus Integration**
```cpp
// tests/integration/test_event_bus_integration.cpp
TEST(EventBusIntegration, ResourceChangeUpdatesHUD) {
    // ResourceManager изменяет топливо
    // → EventBus получает событие
    // → HUD обновляет отображение
    // → UI показывает новое значение
}

TEST(EventBusIntegration, InventoryChangeTriggersMultipleSystems) {
    // InventorySystem добавляет предмет
    // → EventBus рассылает событие
    // → UI обновляет сетку инвентаря
    // → QuestSystem проверяет квестовые предметы
    // → PlayerState обновляет вес
}
```

**День 5: Resource Flow**
```cpp
// tests/integration/test_resource_flow.cpp
TEST(ResourceFlow, FuelConsumptionDuringTravel) {
    // Начать путешествие с полным баком
    // Проехать определённое расстояние
    // Проверить корректное уменьшение топлива
    // Убедиться, что HUD обновился
}
```

**Критерии Завершения:**
- ✅ 10+ интеграционных тестов
- ✅ Все критические пути протестированы
- ✅ EventBus integration полностью покрыта
- ✅ Тесты стабильны (не flaky)

---

### Этап 4: CI/CD Integration (Неделя 4)

**Ответственный:** DevOps  
**Продолжительность:** 5 дней

#### День 1-2: GitHub Actions Setup

**Файл:** `.github/workflows/test.yml`

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
        run: |
          sudo apt-get update
          sudo apt-get install -y cmake g++ libsfml-dev libgtest-dev
      
      - name: Configure CMake
        run: |
          mkdir build
          cd build
          cmake .. -DCMAKE_BUILD_TYPE=Debug
      
      - name: Build Tests
        run: cmake --build build --target all
      
      - name: Run Unit Tests
        run: cd build && ctest --output-on-failure -R UnitTests
      
      - name: Run Integration Tests
        run: cd build && ctest --output-on-failure -R IntegrationTests
      
      - name: Generate Coverage Report
        if: success()
        run: |
          cd build
          lcov --capture --directory . --output-file coverage.info
          lcov --remove coverage.info '/usr/*' --output-file coverage.info
          lcov --list coverage.info
      
      - name: Upload Coverage to Codecov
        uses: codecov/codecov-action@v3
        with:
          file: ./build/coverage.info
          fail_ci_if_error: false

  test-macos:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Dependencies
        run: |
          brew install cmake sfml googletest
      
      - name: Configure CMake
        run: |
          mkdir build
          cd build
          cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml/lib/cmake/SFML
      
      - name: Build Tests
        run: cmake --build build --target all
      
      - name: Run Tests
        run: cd build && ctest --output-on-failure
```

#### День 3: Code Coverage Integration

**Файл:** `.github/workflows/coverage.yml`

```yaml
name: Code Coverage

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  coverage:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install Dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y cmake g++ libsfml-dev libgtest-dev lcov
      
      - name: Build with Coverage
        run: |
          mkdir build && cd build
          cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
          cmake --build .
      
      - name: Run Tests
        run: cd build && ctest --output-on-failure
      
      - name: Generate Coverage Report
        run: |
          cd build
          lcov --capture --directory . --output-file coverage.info
          lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage.info
          genhtml coverage.info --output-directory coverage_html
      
      - name: Upload to Codecov
        uses: codecov/codecov-action@v3
        with:
          file: ./build/coverage.info
      
      - name: Archive Coverage Report
        uses: actions/upload-artifact@v3
        with:
          name: coverage-report
          path: build/coverage_html
```

#### День 4-5: Quality Checks

**Файл:** `.github/workflows/quality.yml`

```yaml
name: Code Quality

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  lint:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Install clang-tidy
        run: sudo apt-get install -y clang-tidy
      
      - name: Run clang-tidy
        run: |
          find src include -name '*.cpp' -o -name '*.h' | xargs clang-tidy -p build
  
  codeql:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Initialize CodeQL
        uses: github/codeql-action/init@v2
        with:
          languages: cpp
      
      - name: Build
        run: |
          mkdir build && cd build
          cmake .. && cmake --build .
      
      - name: Perform CodeQL Analysis
        uses: github/codeql-action/analyze@v2
```

**Критерии Завершения:**
- ✅ CI/CD pipeline настроен для Linux и Mac OS
- ✅ Тесты запускаются автоматически на каждый push
- ✅ Coverage отчёты генерируются
- ✅ Quality checks проходят успешно

---

### Этап 5: Документация и Финализация (Неделя 5)

**Ответственный:** Technical Writer + Team  
**Продолжительность:** 5 дней

#### День 1-2: TESTING_GUIDE.md

**Структура:**
1. Введение в тестовую среду
2. Как запустить тесты локально
3. Как писать новые тесты
4. Лучшие практики тестирования
5. Отладка неуспешных тестов
6. CI/CD процесс
7. Метрики и покрытие кода

#### День 3: Обновление progress.md

Добавить секцию:
```markdown
### Тестовая Инфраструктура (2025-11-12)
- ✅ Создана структура tests/ с unit и integration тестами
- ✅ Интегрирован Google Test framework
- ✅ Настроен CI/CD pipeline (GitHub Actions)
- ✅ Поддержка Mac OS и Linux
- ✅ Coverage > 70% достигнуто
- ✅ Документация (TESTING_GUIDE.md) создана
```

#### День 4: Примеры и Best Practices

**Файл:** `tests/EXAMPLES.md`

Содержание:
- Примеры unit тестов для различных паттернов
- Примеры integration тестов
- Использование mocks и fixtures
- Параметризованные тесты
- Тестирование асинхронных операций
- Performance тесты

#### День 5: Финальная Верификация

**Чек-лист:**
- [ ] Все тесты проходят локально
- [ ] CI/CD pipeline работает
- [ ] Документация актуальна
- [ ] Coverage > 70%
- [ ] Mac OS поддержка проверена
- [ ] progress.md обновлён
- [ ] TEST_ENV_PLAN.md финализирован
- [ ] Code review завершён

**Критерии Завершения:**
- ✅ TESTING_GUIDE.md создан и проверен
- ✅ progress.md обновлён
- ✅ Все acceptance criteria выполнены
- ✅ Документация актуальна и полная

---

## Интеграция с CI/CD / CI/CD Integration

### GitHub Actions Workflows

#### 1. Основной Тестовый Pipeline

**Триггеры:**
- Push в ветки: main, develop, copilot/**
- Pull requests в main, develop

**Этапы:**
1. Checkout кода
2. Установка зависимостей (SFML, Google Test)
3. Конфигурация CMake
4. Сборка проекта и тестов
5. Запуск unit tests
6. Запуск integration tests
7. Генерация coverage отчёта
8. Загрузка результатов

#### 2. Coverage Pipeline

**Триггеры:**
- Push в main, develop
- Pull requests в main

**Цель:**
- Измерить покрытие кода тестами
- Убедиться, что coverage > 70%
- Генерировать HTML отчёт
- Загружать на Codecov

#### 3. Quality Checks Pipeline

**Триггеры:**
- Pull requests в main

**Проверки:**
- clang-tidy для статического анализа
- CodeQL для поиска уязвимостей
- clang-format для проверки стиля (опционально)

### Badge для README

Добавить в README.md:
```markdown
![Tests](https://github.com/barzikol/kkurs/workflows/Tests/badge.svg)
![Coverage](https://codecov.io/gh/barzikol/kkurs/branch/main/graph/badge.svg)
![Quality](https://github.com/barzikol/kkurs/workflows/Code%20Quality/badge.svg)
```

---

## Поддержка Mac OS / Mac OS Support

### Специфичные Требования

#### 1. Установка Зависимостей через Homebrew

```bash
# Google Test
brew install googletest

# SFML
brew install sfml

# CMake (если не установлен)
brew install cmake

# Coverage tools (опционально)
brew install lcov
```

#### 2. CMake Configuration для Mac OS

```cmake
# В CMakeLists.txt
if(APPLE)
    # Автоматический поиск SFML через Homebrew
    if(NOT DEFINED SFML_DIR)
        execute_process(
            COMMAND brew --prefix sfml
            OUTPUT_VARIABLE SFML_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        set(SFML_DIR "${SFML_PREFIX}/lib/cmake/SFML")
    endif()
    
    # Поиск Google Test
    if(NOT DEFINED GTest_DIR)
        execute_process(
            COMMAND brew --prefix googletest
            OUTPUT_VARIABLE GTEST_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        set(GTest_DIR "${GTEST_PREFIX}/lib/cmake/GTest")
    endif()
endif()
```

#### 3. Запуск Тестов на Mac OS

```bash
# Настройка проекта
mkdir build && cd build
cmake ..

# Сборка
cmake --build .

# Запуск всех тестов
ctest --output-on-failure

# Запуск конкретного набора
./unit_tests
./integration_tests
```

#### 4. GitHub Actions для Mac OS

```yaml
test-macos:
  runs-on: macos-latest
  steps:
    - uses: actions/checkout@v3
    
    - name: Install Dependencies
      run: |
        brew install cmake sfml googletest
    
    - name: Configure and Build
      run: |
        mkdir build && cd build
        cmake ..
        cmake --build .
    
    - name: Run Tests
      run: cd build && ctest --output-on-failure
```

### Известные Проблемы и Решения

**Проблема 1:** SFML не находится автоматически
**Решение:** Указать путь явно через `-DSFML_DIR`

**Проблема 2:** Google Test не найден
**Решение:** Установить через Homebrew или использовать FetchContent

**Проблема 3:** Разные пути на Intel и Apple Silicon Mac
**Решение:** Использовать `brew --prefix` для динамического определения путей

---

## Критерии Приемки / Acceptance Criteria

### Обязательные Требования

#### 1. Структура

- ✅ Создана директория tests/ с поддиректориями:
  - tests/unit/ - модульные тесты
  - tests/integration/ - интеграционные тесты
  - tests/mocks/ - моки и заглушки
  - tests/utils/ - утилиты для тестирования

#### 2. Unit Tests

- ✅ Минимум 5 классов покрыты unit тестами
- ✅ Покрытие кода > 70% (измеряется через lcov/gcov)
- ✅ Все тесты проходят успешно
- ✅ Тесты независимы друг от друга
- ✅ Быстрое выполнение (< 5 секунд для unit тестов)

#### 3. Integration Tests

- ✅ Протестированы все переходы между сценами
- ✅ Протестирована интеграция EventBus
- ✅ Протестирован поток данных между системами
- ✅ Минимум 10 интеграционных тестов
- ✅ Тесты стабильны (не flaky)

#### 4. CMake Integration

- ✅ CMakeLists.txt обновлён для поддержки тестов
- ✅ Команда `cmake --build build --target all` собирает тесты
- ✅ Команда `ctest` запускает все тесты
- ✅ Поддержка Mac OS (автопоиск SFML через Homebrew)
- ✅ Отдельные targets для unit и integration тестов

#### 5. CI/CD

- ✅ GitHub Actions workflow создан и работает
- ✅ Тесты запускаются автоматически на каждый push
- ✅ Тесты запускаются на Linux и Mac OS
- ✅ Coverage отчёты генерируются и загружаются
- ✅ Статусы тестов отображаются в PR

#### 6. Скрипты

- ✅ scripts/run_all_tests.sh - запускает все тесты
- ✅ scripts/run_unit_tests.sh - только unit тесты
- ✅ scripts/run_integration_tests.sh - только integration
- ✅ scripts/generate_coverage.sh - генерирует coverage отчёт
- ✅ Скрипты работают на Linux и Mac OS

#### 7. Документация

- ✅ docs/TEST_ENV_PLAN.md (этот файл) создан и актуален
- ✅ docs/TESTING_GUIDE.md создан с инструкциями
- ✅ docs/progress.md обновлён с информацией о тестах
- ✅ README.md содержит секцию Testing
- ✅ tests/README.md описывает структуру тестов

#### 8. Качество

- ✅ Все тесты имеют понятные названия
- ✅ Каждый тест документирован (что проверяется)
- ✅ Используются assertion macros (EXPECT_*, ASSERT_*)
- ✅ Нет дублирования кода между тестами (используются fixtures)
- ✅ Тесты следуют паттерну Arrange-Act-Assert

### Дополнительные Критерии (Nice to Have)

- ⭐ Performance тесты (60 FPS, < 512MB памяти)
- ⭐ Параметризованные тесты для различных сценариев
- ⭐ Mocks для SFML (headless testing)
- ⭐ Автоматическая генерация тестовых данных
- ⭐ Benchmarking инструменты
- ⭐ Visual regression тесты для UI

---

## Ответственные и Сроки / Responsibilities and Timeline

### Роли

| Роль | Ответственность | Персона |
|------|----------------|---------|
| **Lead Developer** | Общее руководство, архитектура тестов | TBD |
| **DevOps Engineer** | CI/CD pipeline, Mac OS поддержка | TBD |
| **QA Engineer** | Integration тесты, manual testing | TBD |
| **Developers** | Unit тесты для своих модулей | Team |
| **Technical Writer** | Документация (TESTING_GUIDE.md) | TBD |

### График Реализации

```
Неделя 1: Инфраструктура
┌─────────────────────────────────────────┐
│ День 1-2: Структура + CMake             │
│ День 3: Google Test integration         │
│ День 4: Скрипты                         │
│ День 5: Базовая документация            │
└─────────────────────────────────────────┘

Неделя 2: Unit Tests
┌─────────────────────────────────────────┐
│ День 1-3: Высокоприоритетные тесты      │
│ День 4-5: Средний приоритет             │
└─────────────────────────────────────────┘

Неделя 3: Integration Tests
┌─────────────────────────────────────────┐
│ День 1-2: Scene transitions             │
│ День 3-4: EventBus integration          │
│ День 5: Resource flow                   │
└─────────────────────────────────────────┘

Неделя 4: CI/CD
┌─────────────────────────────────────────┐
│ День 1-2: GitHub Actions setup          │
│ День 3: Coverage integration            │
│ День 4-5: Quality checks                │
└─────────────────────────────────────────┘

Неделя 5: Документация и Финализация
┌─────────────────────────────────────────┐
│ День 1-2: TESTING_GUIDE.md              │
│ День 3: progress.md update              │
│ День 4: Примеры и best practices        │
│ День 5: Финальная верификация           │
└─────────────────────────────────────────┘
```

### Milestone Dependencies

```
Этап 1 (Инфраструктура)
    ↓
    ├─→ Этап 2 (Unit Tests)
    └─→ Этап 3 (Integration Tests)
         ↓
         Этап 4 (CI/CD)
              ↓
              Этап 5 (Документация)
```

### Оценка Трудозатрат

| Этап | Человеко-дни | FTE | Календарные дни |
|------|--------------|-----|-----------------|
| Этап 1 | 5 | 1.0 | 5 |
| Этап 2 | 10 | 2.0 | 5 |
| Этап 3 | 8 | 1.6 | 5 |
| Этап 4 | 5 | 1.0 | 5 |
| Этап 5 | 5 | 1.0 | 5 |
| **Итого** | **33** | **1.3** | **25** |

*Примечание: При команде из 2-3 разработчиков реализация займёт 2-3 недели*

---

## Ссылки на Документацию / Documentation References

### Внутренние Документы

| Документ | Описание | Статус |
|----------|----------|--------|
| [docs/tasks.md](tasks.md) | Общие задачи проекта | ✅ Существует |
| [docs/PROMPT_TEMPLATE.md](PROMPT_TEMPLATE.md) | Шаблон для задач | ✅ Существует |
| [docs/progress.md](progress.md) | Отслеживание прогресса | ✅ Обновляется |
| [docs/tasks/test-strategy.md](tasks/test-strategy.md) | Стратегия тестирования | ✅ Существует |
| [docs/TESTING_GUIDE.md](TESTING_GUIDE.md) | Руководство по тестированию | 📝 Создаётся |
| [tests/README.md](../tests/README.md) | Структура тестов | 📝 Создаётся |
| [tests/EXAMPLES.md](../tests/EXAMPLES.md) | Примеры тестов | 📝 Создаётся |

### Внешние Ресурсы

| Ресурс | URL | Описание |
|--------|-----|----------|
| Google Test | https://github.com/google/googletest | Фреймворк для C++ тестирования |
| SFML | https://www.sfml-dev.org/ | Simple and Fast Multimedia Library |
| CMake | https://cmake.org/documentation/ | Система сборки |
| GitHub Actions | https://docs.github.com/en/actions | CI/CD платформа |
| Codecov | https://about.codecov.io/ | Coverage reporting |
| lcov | https://github.com/linux-test-project/lcov | Coverage visualization |

---

## Мониторинг Прогресса / Progress Monitoring

### Метрики Отслеживания

1. **Code Coverage**
   - Цель: > 70%
   - Измерение: lcov/gcov
   - Частота: каждый PR

2. **Test Success Rate**
   - Цель: 100%
   - Измерение: CTest output
   - Частота: каждый commit

3. **Test Execution Time**
   - Unit тесты: < 5 секунд
   - Integration тесты: < 30 секунд
   - Измерение: CTest timing
   - Частота: еженедельно

4. **CI/CD Pipeline Success**
   - Цель: > 95% success rate
   - Измерение: GitHub Actions
   - Частота: continuous

### Отчётность

**Еженедельные Отчёты:**
- Количество добавленных тестов
- Текущее покрытие кода
- Статус CI/CD pipeline
- Обнаруженные проблемы
- План на следующую неделю

**Обновления в progress.md:**
- После каждого завершённого этапа
- При достижении milestone
- При изменении acceptance criteria

---

## Риски и Митигация / Risks and Mitigation

### Технические Риски

| Риск | Вероятность | Влияние | Митигация |
|------|-------------|---------|-----------|
| Google Test не работает на Mac OS | Низкая | Высокое | Использовать Homebrew установку, тестировать на Mac OS рано |
| Низкое покрытие кода (< 70%) | Средняя | Среднее | Приоритизация критичных компонентов, code review |
| CI/CD pipeline медленный | Средняя | Среднее | Кеширование зависимостей, параллельные jobs |
| Flaky tests | Высокая | Высокое | Изолировать тесты, избегать времени/random, retry механизм |
| SFML требует графику | Средняя | Среднее | Headless mode, Xvfb на Linux, моки для SFML |

### Организационные Риски

| Риск | Вероятность | Влияние | Митигация |
|------|-------------|---------|-----------|
| Недостаток времени | Средняя | Высокое | Приоритизация, MVP подход, расширение команды |
| Недостаточная экспертиза | Низкая | Среднее | Обучение, документация, pair programming |
| Изменение требований | Средняя | Среднее | Гибкий план, регулярные sync-ups |

### План Контингенции

**Если не удаётся достичь 70% coverage:**
1. Сфокусироваться на критичных компонентах (ResourceManager, GridInventory, EventBus)
2. Снизить временно цель до 60%, но обязательно покрыть core logic
3. Создать backlog для оставшихся тестов

**Если CI/CD не работает на Mac OS:**
1. Сфокусироваться на Linux pipeline
2. Документировать manual процесс для Mac OS
3. Использовать self-hosted runner для Mac OS

---

## Следующие Шаги / Next Steps

### Немедленные Действия (Сегодня)

1. ✅ Создать docs/TEST_ENV_PLAN.md (этот файл)
2. 📝 Создать структуру директорий tests/
3. 📝 Обновить CMakeLists.txt (добавить enable_testing())
4. 📝 Создать базовый test_sanity.cpp
5. 📝 Обновить progress.md

### На Этой Неделе

1. Завершить Этап 1 (Инфраструктура)
2. Настроить Google Test integration
3. Создать первые 3-5 unit тестов
4. Написать базовую документацию в TESTING_GUIDE.md
5. Подготовить GitHub Actions workflow (draft)

### В Следующем Месяце

1. Завершить Этапы 2-3 (Unit + Integration тесты)
2. Достичь coverage > 70%
3. Запустить CI/CD pipeline
4. Финализировать документацию
5. Провести team training по тестированию

---

## Изменения и Версионирование / Changes and Versioning

### История Изменений

| Версия | Дата | Автор | Описание |
|--------|------|-------|----------|
| 1.0 | 2025-11-12 | AI Agent | Первоначальная версия плана |

### Процесс Обновления

**Когда обновлять этот документ:**
- При завершении каждого этапа
- При изменении acceptance criteria
- При обнаружении новых рисков
- При изменении timeline

**Процесс:**
1. Создать PR с изменениями
2. Code review от team lead
3. Обновить версию документа
4. Добавить запись в историю изменений

---

## Глоссарий / Glossary

| Термин | Описание |
|--------|----------|
| **Unit Test** | Тест одного модуля/класса в изоляции |
| **Integration Test** | Тест взаимодействия между компонентами |
| **Mock** | Заглушка для зависимости с предсказуемым поведением |
| **Fixture** | Общая настройка для группы тестов |
| **Coverage** | Процент кода, выполненного тестами |
| **CI/CD** | Continuous Integration / Continuous Deployment |
| **Flaky Test** | Тест с нестабильным результатом |
| **Headless** | Режим без графического интерфейса |
| **Assertion** | Проверка условия в тесте |
| **Test Suite** | Набор связанных тестов |

---

**Статус Документа:** 📝 В разработке / In Development  
**Последнее Обновление:** 2025-11-12  
**Следующее Ревью:** После Этапа 1  
**Владелец:** Development Team  
**Утверждено:** Pending
