# Mock SFML Library for Testing
# Мок-библиотека SFML для Тестирования

This directory contains mock implementations of SFML types that allow testing SFML-dependent code without requiring SFML to be installed.

**Эта директория содержит мок-реализации типов SFML, которые позволяют тестировать код, зависящий от SFML, без необходимости установки SFML.**

## Purpose / Назначение

The mock SFML library enables:
- Running tests in CI/CD environments without graphics libraries
- Testing business logic without requiring SFML installation
- Faster test execution (no graphics rendering)
- Platform-independent testing

**Мок-библиотека SFML позволяет:**
- **Запускать тесты в CI/CD окружениях без графических библиотек**
- **Тестировать бизнес-логику без установки SFML**
- **Быстрое выполнение тестов (без отрисовки графики)**
- **Платформонезависимое тестирование**

## Structure / Структура

```
sfml/
└── SFML/
    ├── System.hpp      # Vector2, Time, Clock, Rect types
    ├── Window.hpp      # Event, Keyboard, Mouse types
    └── Graphics.hpp    # Color, Shape, Text, Texture, RenderWindow types
```

## Implementation Details / Детали Реализации

### Supported Types / Поддерживаемые Типы

**System.hpp:**
- `Vector2<T>` (Vector2f, Vector2i, Vector2u)
- `Rect<T>` (FloatRect, IntRect)
- `Time`
- `Clock`
- Type alias: `Uint32`

**Window.hpp:**
- `Event` and all event types
- `Keyboard` and keyboard keys
- `Mouse` and mouse buttons

**Graphics.hpp:**
- `Color` with common colors (Red, Green, Blue, etc.)
- `RectangleShape`
- `CircleShape`
- `Font`
- `Text`
- `Vertex`
- `PrimitiveType` enum
- `Texture`
- `Sprite`
- `RenderWindow`

### Design Philosophy / Философия Дизайна

The mock implementations provide:
1. **Minimal functionality** - Only what's needed for testing
2. **No rendering** - Drawing methods are no-ops
3. **State tracking** - Store values for verification in tests
4. **API compatibility** - Match SFML's public interface

**Мок-реализации предоставляют:**
1. **Минимальная функциональность** - только то, что нужно для тестирования
2. **Без отрисовки** - методы рисования ничего не делают
3. **Отслеживание состояния** - сохраняют значения для проверки в тестах
4. **Совместимость API** - соответствуют публичному интерфейсу SFML

## Usage / Использование

The mock SFML is used automatically by CMake when real SFML is not found:

**Мок SFML автоматически используется CMake, когда реальная SFML не найдена:**

```cmake
# In tests/CMakeLists.txt
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)

if(SFML_FOUND)
    # Use real SFML
    target_link_libraries(kkurs_lib PUBLIC sfml-graphics sfml-window sfml-system)
else()
    # Use mock SFML
    target_include_directories(kkurs_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks/sfml)
endif()
```

## Limitations / Ограничения

The mock library does NOT support:
- Actual graphics rendering
- Window event generation
- Image/texture loading from files
- Audio functionality
- Network functionality

**Мок-библиотека НЕ поддерживает:**
- **Реальную отрисовку графики**
- **Генерацию событий окна**
- **Загрузку изображений/текстур из файлов**
- **Функциональность аудио**
- **Функциональность сети**

## Extending / Расширение

To add new SFML types or methods:

1. Add the type/method to the appropriate header (System.hpp, Window.hpp, or Graphics.hpp)
2. Provide a minimal implementation that stores state
3. Ensure the public API matches SFML's interface
4. Test that existing tests continue to work

**Чтобы добавить новые типы или методы SFML:**

1. **Добавьте тип/метод в соответствующий заголовок**
2. **Предоставьте минимальную реализацию, которая хранит состояние**
3. **Убедитесь, что публичный API соответствует интерфейсу SFML**
4. **Протестируйте, что существующие тесты продолжают работать**

## Maintenance / Поддержка

- Keep mock types in sync with SFML's public API
- Only add types/methods that are actually used in tests
- Prefer simple implementations over complex ones
- Document any significant deviations from SFML's behavior

**Поддерживайте моки в синхронизации с публичным API SFML**  
**Добавляйте только типы/методы, которые действительно используются в тестах**  
**Предпочитайте простые реализации сложным**  
**Документируйте любые значительные отклонения от поведения SFML**

---

**Last Updated**: 2025-11-18  
**SFML Version Compatibility**: 2.5+  
**Maintainers**: Development Team
