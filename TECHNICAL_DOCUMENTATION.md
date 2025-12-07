# ТЕХНИЧЕСКАЯ ДОКУМЕНТАЦИЯ ПРОЕКТА
# Keep Driving → Journal-Based Adventure Game

**Версия:** 1.0
**Дата:** 2025-12-07
**Статус:** В разработке (переход к журнальной системе)

---

## СОДЕРЖАНИЕ

1. [Обзор проекта](#обзор-проекта)
2. [Текущая архитектура](#текущая-архитектура)
3. [Журнальная система (Journal System)](#журнальная-система-journal-system)
4. [Анализ кодовой базы](#анализ-кодовой-базы)
5. [Планируемая трансформация (60 Seconds Style)](#планируемая-трансформация-60-seconds-style)
6. [Дорожная карта миграции](#дорожная-карта-миграции)
7. [Стандарты разработки](#стандарты-разработки)
8. [Сборка и зависимости](#сборка-и-зависимости)

---

## ОБЗОР ПРОЕКТА

### Концепция

**Исходная концепция (Keep Driving):**
- Визуальная игра с реальным временем
- Путешествие через российские города в стиле 90-х
- RPG-элементы: прокачка, инвентарь, квесты
- Графический интерфейс и анимации

**Новая концепция (Journal-Based / 60 Seconds Style):**
- Текстовая игра с тактическими паузами
- Дневник путешественника от первого лица
- Выживание с управлением ресурсами
- Минималистичный UI, фокус на повествовании
- Система последствий и рисков
- Рогалик-элементы

### Технологический стек

- **Язык:** C++17
- **Графическая библиотека:** SFML 3.x
- **Система сборки:** CMake 3.10+
- **Тестирование:** Google Test (GoogleTest)
- **Платформа:** macOS (основная), кроссплатформенный дизайн
- **Шрифты:** DejaVu Sans (с поддержкой кириллицы)

---

## ТЕКУЩАЯ АРХИТЕКТУРА

### Основные компоненты

```
┌─────────────────────────────────────────────────────┐
│                   SceneManager                      │
│  (Управление переключением сцен и игровым циклом)   │
└────────────────┬────────────────────────────────────┘
                 │
    ┌────────────┼────────────────────────┐
    │            │                        │
┌───▼────┐  ┌───▼────────┐  ┌────────────▼──────────┐
│ Scene  │  │GameState   │  │  PlayerState /        │
│ (Base) │  │Manager     │  │  GameStateManager     │
└───┬────┘  └────────────┘  └───────────────────────┘
    │
    ├── MainMenuScene
    ├── CharacterCreationScene
    ├── JournalScene ⭐ (NEW - унифицированная система)
    ├── NodeScene
    ├── MapScene
    ├── LocationScene
    ├── TravelCinematicScene
    ├── TravelSelectionScene
    ├── NotesScene
    ├── CharacterScene
    ├── CompanionsScene
    ├── QuestsScene
    ├── InventoryScene
    ├── AbilityTreeScene
    ├── CinematicEventScene
    └── EndGameScene
```

### Scene Interface (include/Scene.h)

Базовый интерфейс для всех сцен:

```cpp
class Scene {
public:
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual SceneType getNextScene() const = 0;
    virtual bool isFinished() const = 0;
};
```

**Доступные SceneType:**
```cpp
enum class SceneType {
    MAIN_MENU,           // Главное меню
    CHARACTER_CREATION,  // Создание персонажа
    CINEMATIC_EVENT,     // Кинематографические события
    NODE,                // Узлы на карте
    CHARACTER,           // Экран персонажа
    COMPANIONS,          // Экран спутников
    QUESTS,              // Квесты
    ABILITY_TREE,        // Дерево способностей
    MAP,                 // Карта мира
    INVENTORY,           // Инвентарь
    LOCATION,            // Локации (заправка, мотель)
    TRAVEL_SELECTION,    // Выбор маршрута
    TRAVEL_CINEMATIC,    // Кинематика путешествия
    NOTES,               // Заметки
    JOURNAL,             // ⭐ Унифицированная журнальная система
    END_GAME,            // Экран окончания игры
    EXIT                 // Выход
};
```

### Управление состоянием

**GameStateManager (Singleton)**
- Хранит глобальное состояние игры
- Управляет PlayerState
- Отслеживает текущую локацию, город
- Время игры (playtime tracking)

**PlayerState**
- Ресурсы: топливо, энергия, деньги
- Характеристики персонажа
- Инвентарь
- Статистика

### Системы

1. **InventorySystem / GridInventorySystem**
   - Управление предметами
   - Сетка инвентаря

2. **ExperienceSystem**
   - Опыт и уровни

3. **AbilityTreeSystem**
   - Дерево навыков
   - Узлы способностей (AbilityNode)

4. **QuestManager**
   - Управление квестами

5. **DialogueManager**
   - Диалоги с NPC

6. **EventManager / EventFactory**
   - Игровые события
   - Фабрика событий

7. **SaveSystem**
   - Сохранение/загрузка игры

8. **FuelSystem**
   - Управление топливом

---

## ЖУРНАЛЬНАЯ СИСТЕМА (JOURNAL SYSTEM)

### Концепция

Унифицированная система текстовых записок (Notes), заменяющая множество отдельных сцен одной универсальной JournalScene. Вдохновлена игрой "60 Seconds!" и дневниковым повествованием.

### Архитектура

```
┌──────────────────────────────────────┐
│          JournalScene                │
│  (Единый контроллер игрового цикла)  │
└──────────────┬───────────────────────┘
               │
   ┌───────────┼────────────┐
   │           │            │
┌──▼──────┐ ┌─▼────────┐ ┌─▼──────────┐
│CityNote │ │TravelNote│ │ EventNote  │
│         │ │(TODO)    │ │ (TODO)     │
└─────────┘ └──────────┘ └────────────┘
```

### Файлы системы

**Базовые классы:**
- `include/Journal/Note.h` - Абстрактный базовый класс записки
- `src/Journal/Note.cpp` - Базовая реализация рендеринга

**Типы записок:**
- `include/Journal/CityNote.h` - Записка города
- `src/Journal/CityNote.cpp` - Реализация городской логики

**Сцена:**
- `include/Journal/JournalScene.h` - Главная сцена журнала
- `src/Journal/JournalScene.cpp` - Управление записками и HUD

### Note - базовый класс

```cpp
struct NoteChoice {
    std::string text;                              // Текст выбора
    std::string outcomeText;                       // Результат выбора
    std::function<void(PlayerState*)> action;      // Действие при выборе
    std::function<bool(PlayerState*)> condition;   // Условие доступности
    bool isDisabled;                               // Недоступен?
    std::string disabledReason;                    // Причина недоступности
};

class Note {
public:
    enum class Type { CITY, TRAVEL, EVENT, LOCATION };

    virtual std::string getId() const = 0;
    virtual Type getType() const = 0;
    virtual std::string getTitle() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::vector<NoteChoice> getChoices() const = 0;
    virtual void executeChoice(int choiceIndex, PlayerState* playerState) = 0;
    virtual void update(float deltaTime) {}
    virtual void render(sf::RenderWindow& window, const sf::Font& font, int selectedChoiceIndex = -1);

    bool isFinished() const { return m_isFinished; }
    void setFinished(bool finished) { m_isFinished = finished; }
};
```

### CityNote - записка города

**Функциональность:**
- Генерирует описание города от первого лица
- Предоставляет выборы: заправка, работа, мотель, магазин, путешествие
- Проверяет условия доступности (деньги, топливо, энергия)
- Применяет эффекты выбора (изменение ресурсов)

**Действия (Actions):**
```cpp
enum class Action {
    NONE,
    GAS_STATION,  // Заправиться (500₽ → топливо = 100%)
    WORK,         // Поискать работу (TODO)
    MOTEL,        // Снять номер (1000₽ → энергия = 100%)
    SHOP,         // Зайти в магазин (TODO)
    TRAVEL        // Ехать дальше (требует >20L топлива)
};
```

**Пример описания:**
```
Я въехал в МОСКВУ. День в разгаре. Шум, движение, суета.
Типичный российский город.

Надо заправиться, бензина почти не осталось. Я устал. Нужно отдохнуть.

Что мне делать?

ВАШ ВЫБОР:
▸ [1] Заправиться (500₽)
  [2] Поискать работу
  [3] Снять номер в мотеле (1000₽)
    ⚠️ Не хватает денег
  [4] Зайти в магазин
  [5] Ехать дальше
    ⚠️ Бензина не хватит
```

### JournalScene - главная сцена

**Ответственность:**
- Управление стеком записок (Note stack)
- Отображение текущей записки
- Обработка ввода (стрелки, цифры 1-9, Enter)
- Переключение между записками
- Минимальный HUD (топливо, энергия, деньги)

**Управление:**
- **↑/↓** - навигация между вариантами
- **1-9** - быстрый выбор варианта
- **Enter** - подтвердить выбор
- **Escape** - выход в главное меню

**HUD:**
```
┌─────────────────────────────────────────────────────┐
│ ТОПЛИВО: 45L    ЭНЕРГИЯ: 60%    ДЕНЬГИ: 1200 ₽     │
└─────────────────────────────────────────────────────┘
```

### Визуальный стиль

**Цветовая палитра:**
- Фон экрана: `rgb(230, 220, 210)` - старая бумага
- Фон записки: `rgb(245, 235, 220)` - светлая бумага
- Текст: `rgb(50, 40, 30)` - темно-коричневый
- HUD фон: `rgba(40, 35, 30, 200)` - темный полупрозрачный
- HUD текст: `rgb(200, 180, 120)` - золотистый
- Подсветка выбора: `rgba(255, 240, 200, 150)` - светло-желтая
- Недоступные варианты: `rgb(150, 140, 130)` - серый
- Предупреждения: `rgb(180, 50, 50)` - красный

**Шрифт:** DejaVu Sans (assets/fonts/font.ttf)
- Поддержка кириллицы
- UTF-8 кодировка через `sf::String::fromUtf8()`

### Интеграция с SFML 3.x

**Изменения API относительно SFML 2.x:**

1. **Конструктор текста:**
```cpp
// SFML 2.x (НЕПРАВИЛЬНО):
sf::Text text(font, "Hello", 24);

// SFML 3.x (ПРАВИЛЬНО):
sf::Text text(font);
text.setString("Hello");
text.setCharacterSize(24);
```

2. **Позиционирование:**
```cpp
// SFML 2.x (НЕПРАВИЛЬНО):
shape.setPosition(100, 200);

// SFML 3.x (ПРАВИЛЬНО):
shape.setPosition(sf::Vector2f(100, 200));
```

3. **Обработка событий:**
```cpp
// SFML 2.x (НЕПРАВИЛЬНО):
if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Up) { ... }
}

// SFML 3.x (ПРАВИЛЬНО):
if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->code == sf::Keyboard::Key::Up) { ... }
}
```

4. **Enum имена:**
```cpp
// SFML 2.x: sf::Keyboard::Num1
// SFML 3.x: sf::Keyboard::Key::Num1
```

5. **Poll events:**
```cpp
// SFML 3.x возвращает std::optional<sf::Event>
while (const std::optional<sf::Event> event = window.pollEvent()) {
    // обработка *event
}
```

---

## АНАЛИЗ КОДОВОЙ БАЗЫ

### Структура файлов

**Всего файлов:**
- Header files (`.h`): ~52
- Source files (`.cpp`): ~46

### Файлы в CMakeLists.txt (компилируются)

**Основные сцены:**
- MainMenuScene.cpp
- CharacterCreationScene.cpp
- CharacterCreationMenu.cpp
- NodeScene.cpp
- MapScene.cpp
- CharacterScene.cpp
- CompanionsScene.cpp
- QuestsScene.cpp
- AbilityTreeScene.cpp
- InventoryScene.cpp
- LocationScene.cpp
- TravelCinematicScene.cpp
- TravelSelectionScene.cpp
- NotesScene.cpp
- CinematicEventScene.cpp
- EndGameScene.cpp

**Журнальная система:**
- Journal/Note.cpp ⭐
- Journal/CityNote.cpp ⭐
- Journal/JournalScene.cpp ⭐

**Системы:**
- SceneManager.cpp
- PlayerState.cpp
- Character.cpp
- Car.cpp
- InventorySystem.cpp
- GridInventorySystem.cpp
- NPC.cpp
- DialogueManager.cpp
- GameTypes.cpp
- ExperienceSystem.cpp
- AbilityNode.cpp
- AbilityTreeSystem.cpp
- AbilityEffectManager.cpp
- EventManager.cpp
- EventFactory.cpp
- QuestManager.cpp
- FuelSystem.cpp
- Structure.cpp
- GameStateManager.cpp
- SaveSystem.cpp

**UI компоненты:**
- UI/UIComponent.cpp
- UI/HUDPanel.cpp
- UI/ResourceBar.cpp

**Утилиты:**
- main.cpp

### Файлы НЕ в CMakeLists.txt

**Существуют, но не компилируются:**
- `src/MotelScene.cpp` - существует, но не подключен

**Причина:** Вероятно, незавершенная функциональность или замененная другой системой.

### Неиспользуемые сцены (кандидаты на удаление после миграции)

После полного перехода на журнальную систему, следующие сцены могут стать избыточными:

**Высокий приоритет удаления:**
- `NodeScene` - замена на CityNote в JournalScene
- `LocationScene` - замена на LocationNote (TODO)
- `TravelCinematicScene` - замена на TravelNote (TODO)
- `TravelSelectionScene` - замена на TravelNote (TODO)
- `NotesScene` - похожая функциональность с Journal
- `MotelScene` - функциональность в CityNote

**Средний приоритет:**
- `MapScene` - может быть интегрирован в Journal или оставлен
- `CharacterScene` - может быть записка CharacterNote
- `CompanionsScene` - может быть записка CompanionsNote
- `QuestsScene` - может быть записка QuestsNote
- `InventoryScene` - может быть записка InventoryNote
- `AbilityTreeScene` - может быть записка AbilityTreeNote

**Низкий приоритет (сохранить):**
- `MainMenuScene` - необходим
- `CharacterCreationScene` - необходим для старта игры
- `CinematicEventScene` - может использоваться для событий
- `EndGameScene` - необходим для финала

### Git статус (неотслеживаемые файлы)

**Новые файлы (untracked):**
```
?? NotesScene.h
?? TravelSelectionScene.h
?? src/MotelScene.cpp
?? src/NotesScene.cpp
?? src/TravelSelectionScene.cpp
```

**Удаленные файлы (staged for deletion):**
```
D ExampleResponsiveScene.h
D GarageService.h
D InteractionMenu.h
D LocationGenerator.h
D POIManager.h
D PauseMenuScene.h
D RestScene.h
D RouteSelectionScene.h
D StartZone.h
D StoreService.h
D StructurePlacement.h
D TownMenu.h
D TownScene.h
D TutorialSystem.h
D WorkAgencyService.h
D WorldMapScene.h
D src/RestScene.cpp
```

Эти файлы уже удалены из проекта и ожидают коммита.

### Рекомендации по очистке

**Шаг 1: Удалить неиспользуемые файлы**
```bash
# Удалить MotelScene.cpp (не в CMakeLists.txt)
rm src/MotelScene.cpp
rm include/MotelScene.h

# Закоммитить уже удаленные файлы
git add -u
git commit -m "Удаление устаревших файлов сервисов и сцен"
```

**Шаг 2: Организация файлов**
- Все файлы журнальной системы в `include/Journal/` и `src/Journal/`
- Системы в `include/Systems/` и `src/Systems/` (опционально)
- UI в `include/UI/` и `src/UI/`

**Шаг 3: После полной миграции на Journal**
- Удалить старые сцены (NodeScene, LocationScene и т.д.)
- Обновить CMakeLists.txt
- Обновить SceneManager.cpp

---

## ПЛАНИРУЕМАЯ ТРАНСФОРМАЦИЯ (60 SECONDS STYLE)

### Вдохновение: 60 Seconds!

**Ключевые механики:**
1. **Текстовое повествование** - история рассказывается через дневник
2. **Тактические паузы** - игрок принимает решения без давления реального времени
3. **Таймер для критических решений** - 60 секунд на подготовку или выбор
4. **Управление ресурсами** - еда, вода, топливо, энергия
5. **Последствия** - каждое решение имеет долгосрочный эффект
6. **Рогалик-элементы** - случайные события, пермасмерть (опционально)

### Новая архитектура

```
┌─────────────────────────────────────────────┐
│          RouteJournalScene                  │
│  (Основной геймплей с таймером и выборами)  │
└──────────────┬──────────────────────────────┘
               │
   ┌───────────┼────────────────────┐
   │           │                    │
┌──▼──────┐ ┌─▼────────┐ ┌─────────▼──────┐
│ Journal │ │Survivor  │ │ Consequences   │
│ Entry   │ │ State    │ │ System         │
└─────────┘ └──────────┘ └────────────────┘
```

### Новые классы (планируемые)

**1. SurvivorState (замена PlayerState)**
```cpp
struct VitalResources {
    float fuel;         // 0-100L
    float energy;       // 0-100%
    float health;       // 0-100%
    float hunger;       // 0-100%
    float thirst;       // 0-100%
    float sanity;       // 0-100%
};

struct TeamStatus {
    int teamSize;
    std::vector<Survivor> team;
    std::map<std::string, bool> injuries;  // травмы
    std::map<std::string, int> skills;     // навыки команды
};

class SurvivorState {
public:
    VitalResources vitals;
    TeamStatus team;
    float money;
    int day;  // день путешествия

    // Ежедневное потребление
    void applyDailyCosts();

    // Проверка состояния
    bool isAlive() const;
    bool canTravel() const;

    // События
    void applyInjury(const std::string& injury);
    void healInjury(const std::string& injury);
};
```

**2. JournalEntry (структура событий)**
```cpp
struct JournalChoice {
    std::string text;
    Requirements requirements;        // что нужно
    Consequences consequences;        // что произойдет
    RiskLevel risk;                   // уровень риска
    std::function<void(SurvivorState*)> action;
};

struct JournalEntry {
    std::string id;
    std::string title;
    std::string text;                 // текст события
    std::vector<JournalChoice> choices;

    // Анимация текста
    float textRevealSpeed;            // скорость показа текста
    float currentRevealProgress;      // текущий прогресс
};
```

**3. Consequences (система последствий)**
```cpp
struct Requirement {
    enum class Type { FUEL, ENERGY, MONEY, ITEM, SKILL };
    Type type;
    float amount;
    std::string itemId;
    std::string description;
};

struct Consequence {
    std::string description;
    std::function<void(SurvivorState*)> apply;
    float probability;  // вероятность (0.0-1.0)
};

enum class RiskLevel {
    SAFE,       // Зеленый - нет риска
    LOW,        // Желтый - небольшой риск
    MEDIUM,     // Оранжевый - средний риск
    HIGH,       // Красный - высокий риск
    DEADLY      // Темно-красный - смертельно опасно
};
```

**4. RouteJournalScene (основная сцена геймплея)**
```cpp
class RouteJournalScene : public Scene {
public:
    RouteJournalScene(SurvivorState* state, const std::string& routeId);

    // Scene interface
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    SurvivorState* m_state;
    JournalEntry m_currentEntry;

    // Таймер для решений
    float m_decisionTimer;
    bool m_timerActive;

    // Анимация текста
    void updateTextReveal(float deltaTime);
    void renderRevealedText(sf::RenderWindow& window);

    // Выборы
    void renderChoices(sf::RenderWindow& window);
    void renderConsequences(sf::RenderWindow& window, const JournalChoice& choice);

    // Таймер
    void renderTimer(sf::RenderWindow& window);
};
```

### Игровой цикл

**Фаза 1: Подготовка (PreprationScene)**
```
┌───────────────────────────────────────┐
│   60 СЕКУНД ДО ОТПРАВЛЕНИЯ            │
│   Соберите вещи в машину!             │
│                                       │
│   Таймер: 00:37                       │
│                                       │
│   [Кликайте на предметы]              │
│   ☐ Аптечка     ☐ Канистра            │
│   ☐ Еда (5шт)   ☐ Вода (10л)          │
│   ☐ Радио       ☐ Карта               │
│   ...                                 │
└───────────────────────────────────────┘
```

**Фаза 2: Выбор маршрута (RouteMapScene)**
```
┌───────────────────────────────────────┐
│   КАРТА РОССИИ                        │
│                                       │
│   Москва ──→ Казань ──→ Екатеринбург │
│         (500км)  (800км)             │
│                                       │
│   Трассы:                             │
│   • М7 (основная) - быстро, опасно   │
│   • Объездная - медленно, безопасно   │
│                                       │
│   [Выберите маршрут]                  │
└───────────────────────────────────────┘
```

**Фаза 3: Путешествие (RouteJournalScene)**
```
┌────────────────────────────────────────────────┐
│ ДЕНЬ 3 - ДОРОГА НА КАЗАНЬ                      │
├────────────────────────────────────────────────┤
│                                                │
│ Еду по трассе М7. Вдруг вижу на обочине       │
│ сломанную машину. Мужчина машет руками,        │
│ просит о помощи.                               │
│                                                │
│ Что делать?                                    │
│                                                │
│ ▸ [1] Остановиться и помочь                    │
│     Требуется: Инструменты, 30 минут           │
│     Риск: СРЕДНИЙ (может быть ловушка)         │
│     Последствия:                               │
│       ✓ +10 репутация                          │
│       ? Может наградить деньгами               │
│       ⚠ Может украсть вещи (30%)               │
│                                                │
│   [2] Проехать мимо                            │
│     Риск: НЕТ                                  │
│     Последствия:                               │
│       - Нет изменений                          │
│                                                │
│   [3] Подбросить до ближайшего города          │
│     Требуется: Место в машине                  │
│     Риск: НИЗКИЙ                               │
│     Последствия:                               │
│       ✓ +5 репутация                           │
│       - -5L топлива                            │
│                                                │
│ [↑/↓ для выбора, Enter для подтверждения]     │
└────────────────────────────────────────────────┘
│ ТОПЛИВО: 45L │ ЭНЕРГИЯ: 60% │ ДЕНЬ: 3         │
└────────────────────────────────────────────────┘
```

**Фаза 4: Город (CityJournalScene)**
```
┌────────────────────────────────────────────────┐
│ КАЗАНЬ - Дневник путешественника               │
├────────────────────────────────────────────────┤
│                                                │
│ Я въехал в Казань. Вечер. Зажигаются огни.    │
│ Город постепенно засыпает.                     │
│                                                │
│ Надо заправиться, бензина почти не осталось.   │
│ Я устал. Нужно отдохнуть.                      │
│                                                │
│ Что мне делать?                                │
│                                                │
│ ВАШ ВЫБОР:                                     │
│ ▸ [1] Заправиться (500₽)                       │
│   [2] Поискать работу                          │
│   [3] Снять номер в мотеле (1000₽)             │
│     ⚠️ Не хватает денег                        │
│   [4] Зайти в магазин                          │
│   [5] Ехать дальше                             │
│     ⚠️ Бензина не хватит                       │
│                                                │
└────────────────────────────────────────────────┘
│ ТОПЛИВО: 15L │ ЭНЕРГИЯ: 30% │ ДЕНЬГИ: 650₽    │
└────────────────────────────────────────────────┘
```

### Система загрузки событий

**JSON/YAML формат событий:**
```json
{
  "id": "event_broken_car",
  "title": "Сломанная машина",
  "text": "Еду по трассе М7. Вдруг вижу на обочине сломанную машину...",
  "triggers": {
    "location": "highway_m7",
    "day_range": [2, 10],
    "probability": 0.3
  },
  "choices": [
    {
      "text": "Остановиться и помочь",
      "requirements": [
        { "type": "item", "id": "toolkit" },
        { "type": "time", "minutes": 30 }
      ],
      "risk": "MEDIUM",
      "consequences": [
        {
          "description": "Помогли с ремонтом",
          "probability": 0.7,
          "effects": {
            "reputation": 10,
            "money_chance": { "amount": 500, "probability": 0.5 }
          }
        },
        {
          "description": "Это была ловушка!",
          "probability": 0.3,
          "effects": {
            "money": -200,
            "item_lost": "random"
          }
        }
      ]
    }
  ]
}
```

---

## ДОРОЖНАЯ КАРТА МИГРАЦИИ

### Фаза 0: Подготовка ✅ (ЗАВЕРШЕНО)

**Задачи:**
- ✅ Создать базовую журнальную систему (Note, JournalScene)
- ✅ Реализовать CityNote с первым лицом
- ✅ Интегрировать DejaVu Sans шрифт с кириллицей
- ✅ Добавить минимальный HUD
- ✅ Протестировать в MainMenu

**Результат:** Работающая CityNote в JournalScene с дневниковым стилем.

### Фаза 1: Очистка кода (В ПРОЦЕССЕ)

**Задачи:**
- 🔄 Создать техническую документацию (этот файл)
- ⏳ Удалить неиспользуемые файлы (MotelScene.cpp и т.д.)
- ⏳ Закоммитить удаленные файлы (ExampleResponsiveScene.h и т.д.)
- ⏳ Организовать структуру папок
- ⏳ Обновить CMakeLists.txt с комментариями

**Оценка:** 1-2 дня

### Фаза 2: Расширение журнальной системы (СЛЕДУЮЩИЙ ШАГ)

**Задачи:**
- Создать TravelNote (путешествие между городами)
- Создать EventNote (случайные события)
- Создать LocationNote (заправка, мотель, магазин)
- Добавить стек записок (возврат назад)
- Реализовать переходы между записками

**Файлы для создания:**
- `include/Journal/TravelNote.h`
- `src/Journal/TravelNote.cpp`
- `include/Journal/EventNote.h`
- `src/Journal/EventNote.cpp`
- `include/Journal/LocationNote.h`
- `src/Journal/LocationNote.cpp`

**Оценка:** 3-4 дня

### Фаза 3: Новая система состояния (SurvivorState)

**Задачи:**
- Создать SurvivorState (замена PlayerState)
- Добавить VitalResources (голод, жажда, здоровье)
- Добавить TeamStatus (команда, травмы, навыки)
- Реализовать ежедневное потребление ресурсов
- Миграция с PlayerState на SurvivorState

**Файлы для создания:**
- `include/SurvivorState.h`
- `src/SurvivorState.cpp`

**Оценка:** 3-4 дня

### Фаза 4: Система последствий

**Задачи:**
- Создать Requirements (требования для выборов)
- Создать Consequences (последствия выборов)
- Добавить уровни риска (RiskLevel)
- Реализовать проверку требований
- Реализовать применение последствий
- Добавить вероятностные события

**Файлы для создания:**
- `include/Consequences.h`
- `src/Consequences.cpp`

**Оценка:** 2-3 дня

### Фаза 5: RouteJournalScene (основной геймплей)

**Задачи:**
- Создать JournalEntry (структура событий)
- Создать RouteJournalScene
- Добавить таймер для критических решений
- Реализовать анимацию постепенного показа текста
- Реализовать рендеринг последствий и рисков
- Добавить визуализацию таймера

**Файлы для создания:**
- `include/Journal/JournalEntry.h`
- `include/Journal/RouteJournalScene.h`
- `src/Journal/RouteJournalScene.cpp`

**Оценка:** 5-6 дней

### Фаза 6: Система загрузки событий

**Задачи:**
- Выбрать формат (JSON или YAML)
- Создать EventLoader
- Реализовать парсинг событий
- Создать библиотеку событий (20-30 событий)
- Добавить систему триггеров (локация, день, вероятность)
- Реализовать случайный выбор событий

**Файлы для создания:**
- `include/EventLoader.h`
- `src/EventLoader.cpp`
- `assets/events/*.json` (библиотека событий)

**Оценка:** 4-5 дней

### Фаза 7: Сцена подготовки (60 секунд)

**Задачи:**
- Создать PreparationScene
- Реализовать таймер обратного отсчета
- Создать интерфейс выбора предметов
- Добавить ограничение по весу/слотам
- Визуализация машины и предметов
- Анимации сбора предметов

**Файлы для создания:**
- `include/PreparationScene.h`
- `src/PreparationScene.cpp`

**Оценка:** 3-4 дня

### Фаза 8: Карта маршрутов

**Задачи:**
- Создать RouteMapScene
- Визуализация карты России
- Отображение городов и дорог
- Выбор маршрута (основная/объездная)
- Показ характеристик дорог (расстояние, риск)
- Интеграция с RouteJournalScene

**Файлы для создания:**
- `include/RouteMapScene.h`
- `src/RouteMapScene.cpp`
- `assets/maps/russia.json` (данные карты)

**Оценка:** 4-5 дней

### Фаза 9: Удаление старых сцен

**Задачи:**
- Удалить NodeScene
- Удалить LocationScene
- Удалить TravelCinematicScene
- Удалить TravelSelectionScene
- Удалить NotesScene
- Обновить SceneManager
- Обновить CMakeLists.txt
- Тестирование полного игрового цикла

**Оценка:** 2-3 дня

### Фаза 10: Полировка и баланс

**Задачи:**
- Балансировка ресурсов (сколько топлива/еды потребляется)
- Балансировка стоимости услуг (заправка, мотель)
- Настройка вероятностей событий
- Добавление звуковых эффектов (опционально)
- Добавление музыки (опционально)
- UI-полировка (анимации переходов)
- Тестирование и исправление багов

**Оценка:** 5-7 дней

### ИТОГО: ~35-45 дней работы

---

## СТАНДАРТЫ РАЗРАБОТКИ

### Стиль кода

**Именование:**
- Классы: `PascalCase` (например, `JournalScene`, `CityNote`)
- Методы: `camelCase` (например, `getTitle()`, `executeChoice()`)
- Переменные-члены: `m_camelCase` (например, `m_playerState`, `m_isFinished`)
- Константы: `UPPER_CASE` (например, `GAS_COST`, `MOTEL_COST`)
- Enum классы: `PascalCase` для типа, `UPPER_CASE` для значений

**Комментарии:**
- Doxygen-стиль для публичных API
- `/** @brief */` для методов и классов
- `// [TODO]`, `// [MVP]`, `// [NEW]` для пометок состояния

**Организация файлов:**
```
include/
  ├── Journal/          # Журнальная система
  │   ├── Note.h
  │   ├── CityNote.h
  │   └── ...
  ├── UI/               # UI компоненты
  │   ├── UIComponent.h
  │   └── ...
  └── ...               # Остальные заголовки

src/
  ├── Journal/          # Реализации журнальной системы
  │   ├── Note.cpp
  │   ├── CityNote.cpp
  │   └── ...
  ├── UI/               # Реализации UI
  │   └── ...
  └── ...               # Остальные реализации
```

### Git workflow

**Commit сообщения (на русском):**
```
Добавлена журнальная система (JournalScene, Note, CityNote)

- Создан базовый класс Note с виртуальными методами
- Реализован CityNote с выборами заправки, мотеля, работы
- Добавлена интеграция в MainMenuScene
- Дневниковый стиль повествования от первого лица

🤖 Generated with Claude Code
Co-Authored-By: Claude <noreply@anthropic.com>
```

**Ветки:**
- `main` - стабильная версия
- `feature/journal-system` - фичи
- `fix/sfml3-compatibility` - багфиксы

### Тестирование

**Модульные тесты (Google Test):**
- Тестировать логику, не графику
- Каждая система должна иметь тесты
- Минимум 80% покрытие для критичных систем

**Примеры тестов:**
```cpp
TEST(SurvivorStateTest, DailyCostsReduceResources) {
    SurvivorState state;
    state.vitals.fuel = 100.0f;
    state.applyDailyCosts();
    EXPECT_LT(state.vitals.fuel, 100.0f);
}

TEST(CityNoteTest, GasStationRequiresMoney) {
    PlayerState ps;
    ps.setMoney(100.0f);  // Недостаточно для заправки (500₽)
    CityNote note("moscow", &ps);
    auto choices = note.getChoices();
    // Проверить, что выбор заправки недоступен
    EXPECT_TRUE(choices[0].isDisabled);
}
```

---

## СБОРКА И ЗАВИСИМОСТИ

### Требования

**macOS:**
- macOS 10.15+ (Catalina или новее)
- Xcode Command Line Tools
- Homebrew (для установки зависимостей)

**Зависимости:**
- CMake 3.10+
- C++17-совместимый компилятор (Clang 10+, GCC 7+)
- SFML 3.x
- Google Test (автоматически загружается CMake)

### Установка SFML 3.x (macOS)

```bash
# Через Homebrew
brew install sfml

# Проверить версию
brew info sfml
# Должно быть 3.x.x
```

### Сборка проекта

```bash
# Клонировать репозиторий
git clone <repo-url>
cd kurs

# Создать директорию сборки
mkdir build
cd build

# Настроить CMake с указанием пути к SFML
cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml/lib/cmake/SFML

# Или для Cellar:
cmake .. -DSFML_DIR=/opt/homebrew/Cellar/sfml/3.0.0/lib/cmake/SFML

# Собрать проект
make

# Запустить
./kkurs
```

### Запуск тестов

```bash
cd build

# Собрать тесты
make

# Запустить все тесты
ctest

# Или напрямую
./tests/kkurs_tests
```

### Структура сборки

```
build/
  ├── kkurs                    # Исполняемый файл игры
  ├── tests/
  │   └── kkurs_tests          # Исполняемый файл тестов
  ├── assets/                  # Скопированные ресурсы
  │   └── fonts/
  │       └── font.ttf         # DejaVu Sans
  ├── CMakeCache.txt
  └── ...
```

### Ресурсы (Assets)

**Шрифты:**
- `assets/fonts/font.ttf` - DejaVu Sans (739KB)
  - Кириллица: полная поддержка
  - Латиница: полная поддержка
  - Размер: 18-28px (основной текст, заголовки)

**Изображения:**
- `images/Press_Start_2P/` - Пиксельный шрифт (опционально)
- `images/Jersey_10/` - Альтернативный шрифт (опционально)

**Карты (планируется):**
- `assets/maps/russia.json` - данные карты России

**События (планируется):**
- `assets/events/*.json` - библиотека игровых событий

### UTF-8 кодировка

**Важно:** Все исходники должны быть в UTF-8!

```bash
# Проверить кодировку файла
file -I src/Journal/CityNote.cpp
# Должно быть: charset=utf-8

# Конвертировать в UTF-8 (если нужно)
iconv -f ISO-8859-1 -t UTF-8 file.cpp > file_utf8.cpp
```

**В коде:**
```cpp
// Helper для конвертации строк
static sf::String utf8(const std::string& str) {
    return sf::String::fromUtf8(str.begin(), str.end());
}

// Использование
sf::Text text(font);
text.setString(utf8("Привет, мир!"));
```

---

## ЗАКЛЮЧЕНИЕ

Этот документ служит полной технической справкой для проекта. Он будет обновляться по мере развития проекта и внедрения новых систем.

**Следующие шаги:**
1. ✅ Завершить создание технической документации (этот файл)
2. Очистка кодовой базы (удаление неиспользуемых файлов)
3. Начать Фазу 2: расширение журнальной системы (TravelNote, EventNote)

**Контакты и вклад:**
- Репозиторий: [TODO: добавить URL]
- Автор: [TODO: добавить автора]
- Документация создана с помощью Claude Code

---

**Версия документа:** 1.0
**Последнее обновление:** 2025-12-07
**Статус проекта:** Фаза 1 - Очистка кода
