# SFML 3.x Migration Plan

**Цель**: Полная миграция проекта на SFML 3.x + переписать UI с нуля под MacBook Air M1 (1440x900)

---

## 🔄 Ключевые Изменения SFML 3.x API

### 1. Event System
**SFML 2.x:**
```cpp
sf::Event event;
while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) { }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) { }
    }
}
```

**SFML 3.x:**
```cpp
while (const std::optional<sf::Event> event = window.pollEvent()) {
    if (const auto* closed = event->getIf<sf::Event::Closed>()) { }
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Up) { }
    }
}
```

### 2. Font Loading
**SFML 2.x:**
```cpp
sf::Font font;
if (font.loadFromFile("font.ttf")) { }
```

**SFML 3.x:**
```cpp
sf::Font font("font.ttf"); // throws on failure
// OR
if (auto font = sf::Font::openFromFile("font.ttf")) { }
```

### 3. VideoMode
**SFML 2.x:**
```cpp
sf::RenderWindow window(sf::VideoMode(1440, 900), "Title");
```

**SFML 3.x:**
```cpp
sf::RenderWindow window(sf::VideoMode({1440, 900}), "Title");
```

### 4. Text (NO setLineSpacing)
**SFML 2.x:**
```cpp
text.setLineSpacing(1.2f);
```

**SFML 3.x:**
```cpp
// setLineSpacing() удалён - использовать \n и ручное позиционирование
```

### 5. Keyboard Keys
**SFML 2.x:**
```cpp
sf::Keyboard::Up, sf::Keyboard::Down
```

**SFML 3.x:**
```cpp
sf::Keyboard::Key::Up, sf::Keyboard::Key::Down
```

---

## 📋 План Миграции

### Phase 1: Базовая Инфраструктура ✅
- [x] Создать UIConstants.h с фиксированными значениями
- [x] Обновить CMakeLists.txt для SFML 3.x
- [ ] Создать базовый wrapper для Font loading
- [ ] Создать базовый wrapper для Event handling

### Phase 2: Core Files
- [ ] **main.cpp** - обновить создание окна
- [ ] **SceneManager.cpp** - обновить event loop
- [ ] **Scene.h** - обновить интерфейс handleInput()

### Phase 3: Scene Files (по приоритету)
1. [ ] **MainMenuScene** (критично)
2. [ ] **NodeScene** (критично)
3. [ ] **CharacterCreationScene**
4. [ ] **MapScene**
5. [ ] **InventoryScene**
6. [ ] **CharacterScene**
7. [ ] **CompanionsScene**
8. [ ] **QuestsScene**
9. [ ] **AbilityTreeScene**
10. [ ] **LocationScene**
11. [ ] **CinematicEventScene**
12. [ ] **TravelCinematicScene**
13. [ ] **RestScene**

### Phase 4: UI Components
- [ ] **HUDPanel** - переписать без масштабирования
- [ ] **ResourceBar** - переписать без масштабирования
- [ ] Удалить **UILayout** систему полностью

### Phase 5: Support Files
- [ ] **CharacterCreationMenu.cpp**
- [ ] Все файлы с event handling

### Phase 6: Testing
- [ ] Протестировать каждую сцену
- [ ] Проверить все переходы
- [ ] Проверить font loading
- [ ] Проверить все клавиши

---

## 🎨 Новая UI Система

### Принципы
1. **Фиксированные размеры** - всё под 1440x900, без масштабирования
2. **Жёсткие координаты** - все позиции в константах UIConstants.h
3. **Простота** - никаких сложных layout систем
4. **Консистентность** - единый стиль для всех сцен

### Структура
```
UIConstants.h  - все константы
  ├─ SCREEN_WIDTH/HEIGHT
  ├─ Color::*
  ├─ FONT_SIZE_*
  ├─ PADDING_*/SPACING_*
  ├─ Menu::* (позиции для меню)
  ├─ HUD::* (позиции для HUD)
  └─ Fonts::* (пути к шрифтам)
```

### Использование
```cpp
#include "UIConstants.h"

// Создание окна
sf::RenderWindow window(
    sf::VideoMode({UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT}),
    "KKURS"
);

// Позиционирование
text.setPosition(UI::Menu::START_X, UI::Menu::START_Y);
text.setCharacterSize(UI::FONT_SIZE_NORMAL);
text.setFillColor(UI::Color::TEXT_PRIMARY);

// Spacing
float y = UI::Menu::START_Y;
for (auto& option : options) {
    option.setPosition(UI::Menu::START_X, y);
    y += UI::Menu::OPTION_SPACING;
}
```

---

## 🔧 Helper Functions

### FontLoader (для упрощения)
```cpp
// include/FontLoader.h
class FontLoader {
public:
    static std::optional<sf::Font> load() {
        // Try primary
        if (auto font = sf::Font::openFromFile(UI::Fonts::PRIMARY)) {
            return font;
        }
        // Try fallback 1
        if (auto font = sf::Font::openFromFile(UI::Fonts::FALLBACK_1)) {
            return font;
        }
        // Try fallback 2
        if (auto font = sf::Font::openFromFile(UI::Fonts::FALLBACK_2)) {
            return font;
        }
        return std::nullopt;
    }
};
```

### EventHelper (для упрощения event handling)
```cpp
// include/EventHelper.h
class EventHelper {
public:
    static bool isKeyPressed(const sf::Event& event, sf::Keyboard::Key key) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            return keyPressed->code == key;
        }
        return false;
    }

    static bool isClosed(const sf::Event& event) {
        return event.is<sf::Event::Closed>();
    }
};
```

---

## 📝 Порядок Реализации

### Сегодня (День 1)
1. ✅ Создать UIConstants.h
2. [ ] Создать FontLoader.h
3. [ ] Создать EventHelper.h
4. [ ] Обновить main.cpp
5. [ ] Обновить SceneManager
6. [ ] Обновить MainMenuScene
7. [ ] Тест - запустить игру

### День 2
1. [ ] Обновить NodeScene (критично)
2. [ ] Обновить CharacterCreationScene
3. [ ] Обновить MapScene
4. [ ] Тест всего flow

### День 3
1. [ ] Обновить InventoryScene
2. [ ] Обновить CharacterScene
3. [ ] Обновить CompanionsScene
4. [ ] Обновить QuestsScene

### День 4
1. [ ] Обновить AbilityTreeScene
2. [ ] Обновить LocationScene
3. [ ] Обновить CinematicEventScene
4. [ ] Обновить TravelCinematicScene

### День 5
1. [ ] Переписать HUDPanel
2. [ ] Переписать ResourceBar
3. [ ] Удалить UILayout систему
4. [ ] Финальное тестирование

---

## ⚠️ Breaking Changes

### Что удаляем
- ❌ UILayout.h / UILayout.cpp
- ❌ Все макросы SCALE_*
- ❌ ScreenResolution enum
- ❌ Весь responsive код

### Что добавляем
- ✅ UIConstants.h
- ✅ FontLoader.h
- ✅ EventHelper.h

---

## 🎯 Результат

После миграции:
- ✅ Работает с SFML 3.x
- ✅ Фиксированный UI 1440x900
- ✅ Чистый простой код
- ✅ Быстрая разработка
- ✅ Нет сложных систем масштабирования
- ✅ Всё работает стабильно

---

**Начинаем прямо сейчас!**
