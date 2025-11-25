# План Фазы 4: UI/HUD без моделей / Phase 4 Plan: Raw UI without Assets

**Дата / Date**: 2025-11-16  
**Приоритет / Priority**: 🔴 КРИТИЧЕСКИЙ / CRITICAL  
**Статус / Status**: Не начато / Not Started

---

## 🎯 ЦЕЛЬ / OBJECTIVE

Создать функциональный HUD (Heads-Up Display) без использования графических ассетов (спрайтов, текстур). Использовать только примитивы (прямоугольники, текст, Unicode символы).

---

## 📋 ЗАДАЧИ / TASKS

### 1. HUDPanel компонент ✅
**Файлы**: `include/UI/HUDPanel.h`, `src/UI/HUDPanel.cpp`

#### Функционал:
- [ ] Отображение топлива (progress bar)
  - Иконка: ⛽ (U+26FD)
  - Цвет: желтый → оранжевый → красный
  - Позиция: верхний левый угол
  
- [ ] Отображение денег (число)
  - Иконка: 💰 (U+1F4B0) или ₽
  - Цвет: зеленый
  - Формат: "1,234 ₽"
  
- [ ] Отображение энергии (progress bar)
  - Иконка: ⚡ (U+26A1) или ❤️
  - Цвет: синий → желтый → красный
  - Позиция: под топливом
  
- [ ] Время суток (опционально)
  - Иконка: 🕐 (U+1F550)
  - Формат: "14:30"

#### Технические требования:
```cpp
class HUDPanel {
public:
    HUDPanel();
    
    void update(const PlayerState& state);
    void render(sf::RenderWindow& window);
    
    void setPosition(float x, float y);
    void setSize(float width, float height);
    
private:
    // Resource displays
    ResourceBar m_fuelBar;
    ResourceBar m_energyBar;
    sf::Text m_moneyText;
    sf::Text m_timeText;
    
    // Icons (Unicode)
    sf::Text m_fuelIcon;
    sf::Text m_energyIcon;
    sf::Text m_moneyIcon;
    
    // Layout
    sf::Vector2f m_position;
    sf::RectangleShape m_background;
    sf::Font m_font;
};
```

---

### 2. ResourceBar компонент
**Файлы**: `include/UI/ResourceBar.h`, `src/UI/ResourceBar.cpp`

#### Функционал:
- [ ] Progress bar с рамкой
- [ ] Цветовой градиент (high → low)
- [ ] Анимация изменения
- [ ] Текст с процентами (опционально)

#### Спецификация:
```cpp
class ResourceBar {
public:
    ResourceBar(const std::string& label, 
                const sf::Color& highColor,
                const sf::Color& lowColor);
    
    void setValue(float current, float max);
    void render(sf::RenderWindow& window);
    void setPosition(float x, float y);
    
private:
    sf::RectangleShape m_background;  // Темный фон
    sf::RectangleShape m_fill;        // Заполнение
    sf::RectangleShape m_border;      // Рамка
    sf::Text m_label;
    
    float m_currentValue;
    float m_maxValue;
    sf::Color m_highColor;
    sf::Color m_lowColor;
};
```

---

### 3. Интеграция в сцены

#### 3.1 NodeScene
- [ ] Добавить HUDPanel member
- [ ] Обновлять в update()
- [ ] Рендерить в render()
- [ ] Позиция: (20, 20)

#### 3.2 TravelCinematicScene  
- [ ] Интегрировать HUD
- [ ] Добавить текущую скорость
- [ ] Добавить расстояние до цели
- [ ] Позиция: (20, 20)

#### 3.3 LocationScene
- [ ] Показать HUD
- [ ] Выделить изменяющиеся ресурсы
- [ ] Анимация при транзакции

#### 3.4 MapScene
- [ ] Упрощенный HUD (только основные ресурсы)
- [ ] Позиция: верхний правый угол

---

### 4. UI Компоненты (базовые)

#### 4.1 InfoPanel
**Цель**: Всплывающие информационные панели

```cpp
class InfoPanel {
public:
    void show(const std::string& title, 
              const std::string& message);
    void hide();
    bool isVisible() const;
    void render(sf::RenderWindow& window);
    
private:
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    sf::Text m_messageText;
    bool m_visible;
};
```

#### 4.2 ProgressIndicator
**Цель**: Индикатор для долгих операций

```cpp
class ProgressIndicator {
public:
    void setProgress(float percent);  // 0.0 - 1.0
    void render(sf::RenderWindow& window);
    
private:
    sf::RectangleShape m_bar;
    sf::Text m_percentText;
};
```

---

### 5. Цветовая схема

#### Основная палитра:
```cpp
// Ресурсы
const sf::Color FUEL_HIGH(255, 215, 0);      // Золотой
const sf::Color FUEL_LOW(255, 69, 0);        // Красно-оранжевый
const sf::Color ENERGY_HIGH(100, 149, 237);  // Синий
const sf::Color ENERGY_LOW(255, 215, 0);     // Желтый
const sf::Color MONEY_COLOR(50, 205, 50);    // Зеленый

// UI элементы
const sf::Color UI_BG(40, 40, 45, 200);      // Темно-серый полупрозрачный
const sf::Color UI_BORDER(200, 200, 200);    // Светло-серый
const sf::Color UI_TEXT(255, 255, 255);      // Белый

// Состояния
const sf::Color DISABLED_COLOR(128, 128, 128); // Серый
const sf::Color HOVER_COLOR(255, 255, 200);    // Светло-желтый
const sf::Color ACTIVE_COLOR(100, 255, 100);   // Светло-зеленый
```

---

### 6. Unicode иконки

#### Ресурсы:
- ⛽ Топливо (U+26FD FUEL PUMP)
- 💰 Деньги (U+1F4B0 MONEY BAG)
- ⚡ Энергия (U+26A1 HIGH VOLTAGE)
- ❤️ Здоровье (U+2764 HEAVY BLACK HEART)

#### Навигация:
- 🗺️ Карта (U+1F5FA WORLD MAP)
- 🏠 Дом (U+1F3E0 HOUSE)
- 🏪 Магазин (U+1F3EA CONVENIENCE STORE)
- ⛽ Заправка (U+26FD FUEL PUMP)
- 🏨 Мотель (U+1F3E8 HOTEL)

#### Машины:
- 🚗 Автомобиль (U+1F697 AUTOMOBILE)
- 🔧 Ремонт (U+1F527 WRENCH)
- ⚙️ Настройки (U+2699 GEAR)

#### Квесты:
- ✅ Выполнено (U+2705 CHECK MARK)
- ❌ Провалено (U+274C CROSS MARK)
- 🎯 Цель (U+1F3AF DIRECT HIT)
- 📦 Предмет (U+1F4E6 PACKAGE)

---

## 🧪 ТЕСТИРОВАНИЕ / TESTING

### Unit тесты:
- [ ] test_hud_panel.cpp
  - Проверка обновления значений
  - Проверка рендеринга
  - Проверка цветовых переходов
  
- [ ] test_resource_bar.cpp
  - Проверка setValue()
  - Проверка градиента
  - Проверка bounds (0-max)

### Интеграционные тесты:
- [ ] HUD в NodeScene
- [ ] HUD в TravelCinematicScene
- [ ] HUD в LocationScene
- [ ] Обновление при изменении ресурсов

### Ручное тестирование:
- [ ] Визуальная проверка всех баров
- [ ] Проверка читаемости текста
- [ ] Проверка позиционирования
- [ ] Проверка на разных разрешениях

---

## 📐 LAYOUT СПЕЦИФИКАЦИЯ

### HUD Layout (20, 20):
```
┌─────────────────────────────────┐
│ ⛽ Fuel:  ████████░░░░  80/100L │
│ ⚡ Energy:████████████  100/100 │
│ 💰 Money: 1,234 ₽               │
│ 🕐 Time:  14:30                 │
└─────────────────────────────────┘
```

### Размеры:
- Width: 300px
- Height: 120px
- Padding: 10px
- Bar Height: 20px
- Gap between bars: 5px

---

## 🎨 MOCK-UP

### NodeScene с HUD:
```
┌────────────────────────────────────────┐
│ ⛽80L ⚡100 💰1,234₽          [NodeName]│
│                                        │
│                                        │
│          🚗 (машина игрока)            │
│                                        │
│                                        │
│                                        │
│                       🏪 (знак E)      │
│                                        │
│                                        │
│ [WASD] Move  [E] Interact  [ESC] Menu │
└────────────────────────────────────────┘
```

### TravelCinematicScene с HUD:
```
┌────────────────────────────────────────┐
│ ⛽75L ⚡95 💰1,234₽   Progress: ████░░░│
│                                        │
│        [   SKY   ]                     │
│    [  MOUNTAINS  ]                     │
│  [ TREES & BUILDINGS ]                 │
│ ══════ ROAD ══════  🚗                 │
│                                        │
│                                        │
│ Distance: 120/200 km  Speed: 50km/s   │
└────────────────────────────────────────┘
```

---

## 📝 КРИТЕРИИ ГОТОВНОСТИ / DONE CRITERIA

### Код:
- [x] HUDPanel.h/cpp реализованы
- [x] ResourceBar.h/cpp реализованы
- [x] Интеграция во все сцены
- [x] Unit тесты написаны и проходят
- [x] Нет compiler warnings

### Функциональность:
- [x] HUD отображается корректно
- [x] Ресурсы обновляются в реальном времени
- [x] Цветовые градиенты работают
- [x] Unicode иконки отображаются
- [x] Layout корректен на 1366x768

### Документация:
- [x] Inline комментарии
- [x] Обновлен PROJECT_STATUS.md
- [x] Создан UI_GUIDE.md (опционально)

---

## 🚀 СЛЕДУЮЩИЙ ПР / NEXT PR

После завершения этой фазы:
1. Добавить спрайты машин
2. Добавить parallax фоны
3. Добавить иконки ресурсов
4. Улучшить визуальные эффекты

---

## 📊 ОЦЕНКА / ESTIMATION

**Время разработки**: 3-5 дней  
**Сложность**: 🟡 Средняя  
**Риски**: 🟢 Низкие (не ломает существующий код)

**Разбивка по задачам**:
- ResourceBar: 0.5 дня
- HUDPanel: 1 день
- Интеграция: 1 день
- Тестирование: 0.5 дня
- Доработка и баг-фиксы: 1 день

**Итого**: 4 дня

---

## ✅ ЧЕКЛИСТ ПЕРЕД КОММИТОМ

Перед созданием PR убедиться:
- [ ] Все тесты проходят (новые + старые)
- [ ] Нет compiler warnings
- [ ] Код отформатирован
- [ ] Комментарии добавлены
- [ ] PROJECT_STATUS.md обновлен
- [ ] Screenshot'ы UI сделаны
- [ ] Commit messages понятны

---

**Создано**: 2025-11-16  
**Автор**: @copilot  
**Для**: @denisov2956151998_barzikol
