# TECHNICAL - Техническое Описание Кода

**Полная техническая документация проекта KKURS**

---

## 📚 Содержание

1. [Архитектура проекта](#архитектура-проекта)
2. [Scene System](#scene-system)
3. [Core Systems](#core-systems)
4. [UI Components](#ui-components)
5. [Data Structures](#data-structures)
6. [Build System](#build-system)

---

## Архитектура проекта

### Паттерн: Scene-Based State Machine

```
┌─────────────────┐
│  SceneManager   │ ← Singleton, управляет переходами
└────────┬────────┘
         │
         │ создает и переключает
         │
    ┌────▼────────────────────┐
    │   Scene (abstract)      │ ← Базовый класс
    └────┬────────────────────┘
         │
         ├─ MainMenuScene
         ├─ CharacterCreationScene
         ├─ MapScene
         ├─ NodeScene
         ├─ TravelCinematicScene
         ├─ LocationScene
         ├─ InventoryScene
         ├─ CharacterScene
         ├─ CompanionsScene
         ├─ QuestsScene
         └─ AbilityTreeScene
```

### Структура директорий

```
kkurs/
├── include/              # Header files (.h)
│   ├── Scene.h          # Базовый класс сцен
│   ├── *Scene.h         # Сцены
│   ├── UI/              # UI компоненты
│   └── *.h              # Системы и структуры
├── src/                  # Source files (.cpp)
│   ├── main.cpp         # Entry point
│   ├── *Scene.cpp       # Реализация сцен
│   ├── UI/              # UI реализация
│   └── *.cpp            # Системы и структуры
├── assets/               # Ресурсы игры
│   └── fonts/           # Шрифты
├── data/                 # Игровые данные
│   └── maps/            # Карты
├── tests/                # Unit tests
│   └── unit/            # Unit test файлы
├── build/                # CMake build directory (gitignored)
└── CMakeLists.txt        # CMake configuration
```

---

## Scene System

### Scene.h - Базовый класс

```cpp
class Scene {
public:
    virtual ~Scene() = default;

    // Handle input events
    virtual void handleInput(const sf::Event& event) = 0;

    // Update scene logic (deltaTime in seconds)
    virtual void update(float deltaTime) = 0;

    // Render the scene
    virtual void render(sf::RenderWindow& window) = 0;

    // Get next scene type
    virtual SceneType getNextScene() const = 0;

    // Check if scene is finished
    virtual bool isFinished() const = 0;
};
```

### SceneType Enum

```cpp
enum class SceneType {
    MAIN_MENU,           // Главное меню
    CHARACTER_CREATION,  // Создание персонажа
    WORLD_MAP,          // Карта мира
    NODE,               // Город (side-view gameplay)
    INVENTORY,          // Grid-инвентарь
    CHARACTER,          // Статистика персонажа
    COMPANIONS,         // Управление командой
    QUESTS,             // Журнал квестов
    ABILITY_TREE,       // Дерево способностей
    CINEMATIC_EVENT,    // Событие с выбором
    TRAVEL_CINEMATIC,   // Кинематографическое путешествие
    REST,               // Отдых/сон
    LOCATION,           // Взаимодействие с локациями
    EXIT                // Выход из игры
};
```

### SceneManager

**Файлы:** `src/SceneManager.cpp`, `include/SceneManager.h`

**Паттерн:** Singleton

**Методы:**
```cpp
class SceneManager {
public:
    static SceneManager& getInstance();

    void changeScene(SceneType newSceneType);
    void handleInput(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    SceneType getCurrentSceneType() const;

private:
    std::unique_ptr<Scene> createScene(SceneType type);

    std::unique_ptr<Scene> m_currentScene;
    SceneType m_currentSceneType;
};
```

**Использование:**
```cpp
SceneManager& manager = SceneManager::getInstance();

// В game loop
while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        manager.handleInput(event);
    }

    manager.update(deltaTime);

    window.clear();
    manager.render(window);
    window.display();
}
```

---

## Core Systems

### 1. PlayerState

**Файлы:** `src/PlayerState.cpp`, `include/PlayerState.h`

**Паттерн:** Singleton (через GameStateManager)

**Данные:**
```cpp
class PlayerState {
private:
    // Resources
    float m_fuel;           // Топливо (0-100L)
    float m_energy;         // Энергия (0-100)
    int m_money;           // Деньги

    // Character
    std::string m_characterName;
    std::string m_characterBackground;  // Ex-Racer/Nomad/Sociable
    std::string m_carType;              // BMW/Volvo/Mercedes

    // Stats
    int m_maxFuel;         // Зависит от машины
    int m_maxEnergy;       // 100
    int m_cargoCapacity;   // Зависит от машины (40-64kg)
};
```

**Ключевые методы:**
```cpp
// Getters
float getFuel() const;
float getEnergy() const;
int getMoney() const;
std::string getCarType() const;

// Setters
void setFuel(float fuel);
void setEnergy(float energy);
void setMoney(int money);

// Modifiers
void addMoney(int amount);      // money += amount
void modifyMoney(int delta);    // money += delta (может быть отрицательным)
bool canAfford(int cost) const; // money >= cost
```

### 2. InventorySystem

**Файлы:** `src/InventorySystem.cpp`, `include/InventorySystem.h`

**Тип:** Simple Slot-based (24 слота)

**Структура:**
```cpp
struct InventorySlot {
    std::string itemId;
    std::string itemName;
    int quantity;
    float weight;          // Вес одного предмета
    bool isEmpty;
};

class InventorySystem {
public:
    InventorySystem(int slots = 24, float maxWeight = 64.0f);

    bool addItem(const std::string& id, const std::string& name,
                 int qty, float weight);
    bool removeItem(int slotIndex, int quantity = 1);
    bool useItem(int slotIndex);

    float getCurrentWeight() const;
    float getMaxWeight() const;
    int getEmptySlots() const;

private:
    std::vector<InventorySlot> m_slots;
    float m_maxWeight;
    float m_currentWeight;
};
```

### 3. GridInventorySystem

**Файлы:** `src/GridInventorySystem.cpp`, `include/GridInventorySystem.h`

**Тип:** Tarkov-style Grid (6x4 = 24 cells)

**Структура:**
```cpp
struct GridItem {
    std::string id;
    std::string name;
    int width, height;     // Размер в ячейках
    int gridX, gridY;      // Позиция в сетке
    bool rotated;          // Повернут ли предмет
    float weight;
    int value;             // Цена
};

class GridInventorySystem {
public:
    GridInventorySystem(int gridWidth = 6, int gridHeight = 4);

    bool canPlaceItem(const GridItem& item, int x, int y) const;
    bool placeItem(const GridItem& item);
    bool removeItem(const std::string& itemId);
    GridItem* getItemAt(int x, int y);

    float getTotalWeight() const;
    int getFreeSlots() const;

private:
    int m_gridWidth, m_gridHeight;
    std::vector<std::vector<bool>> m_grid;  // Занятость ячеек
    std::vector<GridItem> m_items;

    bool checkCollision(const GridItem& item, int x, int y) const;
};
```

**Алгоритм размещения:**
1. Проверить границы сетки
2. Проверить коллизии с другими предметами
3. Отметить ячейки как занятые
4. Добавить предмет в список

### 4. ExperienceSystem

**Файлы:** `src/ExperienceSystem.cpp`, `include/ExperienceSystem.h`

**Паттерн:** Singleton

**Формула опыта:**
```cpp
int getExperienceForNextLevel(int level) {
    return static_cast<int>(100 * std::pow(level, 1.5));
}
```

**Пример:**
- Level 1 → 2: 100 XP
- Level 2 → 3: 282 XP
- Level 3 → 4: 519 XP
- Level 4 → 5: 800 XP

**Методы:**
```cpp
class ExperienceSystem {
public:
    void addExperience(int amount);
    bool canLevelUp() const;
    void levelUp();

    int getLevel() const;
    int getExperience() const;
    int getSkillPoints() const;

private:
    int m_level;
    int m_experience;
    int m_skillPoints;
};
```

### 5. AbilityTreeSystem

**Файлы:** `src/AbilityTreeSystem.cpp`, `include/AbilityTreeSystem.h`

**Паттерн:** Singleton

**Структура:**
```cpp
class AbilityNode; // Узел способности

class AbilityTreeSystem {
public:
    bool canUnlock(const std::string& abilityId) const;
    bool unlockAbility(const std::string& abilityId);
    bool isUnlocked(const std::string& abilityId) const;

    int getAvailableSkillPoints() const;
    std::vector<std::string> getUnlockedAbilities() const;

private:
    std::map<std::string, AbilityNode> m_abilities;
    std::set<std::string> m_unlockedAbilities;
    int m_availableSkillPoints;
};
```

**AbilityNode:**
```cpp
class AbilityNode {
public:
    enum class Type { PASSIVE, ACTIVE };

    std::string getId() const;
    std::string getName() const;
    int getCost() const;          // Стоимость в skill points
    std::vector<std::string> getPrerequisites() const;

private:
    std::string m_id;
    std::string m_name;
    std::string m_description;
    Type m_type;
    int m_cost;
    int m_requiredLevel;
    std::vector<std::string> m_prerequisites;
};
```

### 6. DialogueManager

**Файлы:** `src/DialogueManager.cpp`, `include/DialogueManager.h`

**Паттерн:** Singleton

**Структура диалога:**
```cpp
struct DialogueChoice {
    std::string text;              // Текст выбора
    int nextDialogueId;            // ID следующего диалога (-1 = конец)
    bool requiresItem;             // Нужен ли предмет
    std::string requiredItemId;    // ID требуемого предмета
};

struct DialogueNode {
    int id;
    std::string npcName;
    std::string text;              // Текст диалога
    std::vector<DialogueChoice> choices;
};

class DialogueManager {
public:
    void startDialogue(const std::string& dialogueId);
    DialogueNode getCurrentNode() const;
    void selectChoice(int choiceIndex);
    bool isDialogueActive() const;
    void endDialogue();

private:
    std::map<std::string, std::vector<DialogueNode>> m_dialogues;
    DialogueNode m_currentNode;
    bool m_isActive;
};
```

### 7. QuestManager

**Файлы:** `src/QuestManager.cpp`, `include/QuestManager.h`

**Паттерн:** Singleton

**Структура квеста:**
```cpp
enum class ObjectiveType {
    COLLECT_ITEM,    // Собрать N предметов
    DELIVER_ITEM,    // Доставить предмет
    TRAVEL_TO,       // Добраться до локации
    TALK_TO_NPC,     // Поговорить с NPC
    COMPLETE_EVENT,  // Завершить событие
    EARN_MONEY       // Заработать N денег
};

struct QuestObjective {
    ObjectiveType type;
    std::string target;    // Что/кого (item ID, location ID, NPC ID)
    int required;          // Сколько нужно
    int current;           // Сколько сейчас

    bool isComplete() const { return current >= required; }
};

struct QuestReward {
    int money;
    int experience;
    std::vector<std::string> items;
};

struct Quest {
    std::string id;
    std::string title;
    std::string description;
    std::vector<QuestObjective> objectives;
    QuestReward reward;
    bool isActive;
    bool isCompleted;

    bool canComplete() const;
    float getProgress() const;  // 0.0-1.0
};

class QuestManager {
public:
    void addQuest(const Quest& quest);
    void acceptQuest(const std::string& questId);
    void completeQuest(const std::string& questId);

    // Notification helpers
    void notifyItemCollected(const std::string& itemId);
    void notifyLocationVisited(const std::string& locationId);
    void notifyNPCTalked(const std::string& npcId);

    std::vector<Quest> getActiveQuests() const;
    std::vector<Quest> getCompletedQuests() const;

private:
    std::map<std::string, Quest> m_quests;
};
```

### 8. EventManager

**Файлы:** `src/EventManager.cpp`, `include/EventManager.h`

**Паттерн:** Singleton

**Структура события:**
```cpp
struct EventEffect {
    int moneyChange;
    float fuelChange;
    float energyChange;
    std::vector<std::string> itemsGained;
    std::vector<std::string> itemsLost;
};

struct EventChoice {
    std::string text;           // Текст выбора
    EventEffect effect;         // Что произойдет
    bool requiresItem;          // Нужен ли предмет
    std::string requiredItemId;
};

struct RandomEvent {
    std::string id;
    std::string title;
    std::string description;
    std::vector<EventChoice> choices;

    // Условия срабатывания
    float minFuel;              // Минимум топлива
    float maxFuel;              // Максимум топлива
    float minEnergy;            // Минимум энергии
    int minMoney;               // Минимум денег

    float probability;          // Вероятность 0.0-1.0
};

class EventManager {
public:
    void registerEvent(const RandomEvent& event);
    RandomEvent* triggerRandomEvent(float fuel, float energy, int money);

    std::vector<RandomEvent> getAllEvents() const;

private:
    std::vector<RandomEvent> m_events;
    std::mt19937 m_rng;
};
```

**Дефолтные события:**
1. Заправка (fuel < 30%)
2. Попутчик (+200₽, -10 energy)
3. Пост ДПС (-300₽ или проблемы)
4. Поломка (energy < 40%)
5. Место отдыха (+20 energy, -150₽)

### 9. SaveSystem

**Файлы:** `src/SaveSystem.cpp`, `include/SaveSystem.h`

**Паттерн:** Singleton

**Структура сохранения:**
```cpp
struct SaveData {
    // Player state
    float fuel;
    float energy;
    int money;
    std::string characterBackground;
    std::string carType;

    // Progress
    int level;
    int experience;
    std::vector<std::string> unlockedAbilities;

    // Inventory
    std::vector<InventorySlot> inventory;

    // Quests
    std::vector<std::string> activeQuests;
    std::vector<std::string> completedQuests;

    // Location
    std::string currentLocation;
};

class SaveSystem {
public:
    bool save(const std::string& filename);
    bool load(const std::string& filename);

    SaveData createSaveData();
    void applySaveData(const SaveData& data);

private:
    std::string serializeToJSON(const SaveData& data);
    SaveData deserializeFromJSON(const std::string& json);
};
```

---

## UI Components

### 1. UIComponent (базовый)

**Файлы:** `src/UI/UIComponent.cpp`, `include/UI/UIComponent.h`

```cpp
class UIComponent {
public:
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    void setPosition(float x, float y);
    void setVisible(bool visible);
    bool isVisible() const;

protected:
    sf::Vector2f m_position;
    bool m_visible;
};
```

### 2. ResourceBar

**Файлы:** `src/UI/ResourceBar.cpp`, `include/UI/ResourceBar.h`

**Анимированный прогресс-бар с цветовым градиентом:**

```cpp
class ResourceBar : public UIComponent {
public:
    ResourceBar(float x, float y, float width, float height);

    void setValue(float value);         // 0.0-1.0
    void setMaxValue(float maxValue);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    float m_currentValue;
    float m_targetValue;
    float m_maxValue;

    sf::RectangleShape m_background;
    sf::RectangleShape m_fill;
    sf::RectangleShape m_border;

    // Smooth transitions
    const float TRANSITION_SPEED = 0.2f;

    sf::Color getColorForPercentage(float percent);
};
```

**Цветовой градиент:**
- > 70%: Зеленый (#4CAF50)
- 40-70%: Желтый (#FFD700)
- < 40%: Красный (#F44336)

### 3. HUDPanel

**Файлы:** `src/UI/HUDPanel.cpp`, `include/UI/HUDPanel.h`

**Top-right HUD для отображения ресурсов:**

```cpp
class HUDPanel : public UIComponent {
public:
    HUDPanel();

    void updateTopRight(float deltaTime, PlayerState& state);
    void enable();
    void disable();

    void render(sf::RenderWindow& window) override;

private:
    // Resource bars
    ResourceBar m_fuelBar;
    ResourceBar m_energyBar;

    // Text elements
    sf::Text m_moneyText;
    sf::Text m_fuelText;
    sf::Text m_energyText;
    sf::Text m_cargoText;

    sf::Font m_font;
    bool m_enabled;
};
```

**Расположение:** (650, 10) - верхний правый угол

**Иконки:** Unicode символы
- 💰 Money (gold #FFD700)
- ⛽ Fuel (orange #FFA500)
- ⚡ Energy (cyan #00FFFF)
- 📦 Cargo (white #FFFFFF)

---

## Data Structures

### Character.h

```cpp
struct CharacterBackground {
    std::string name;        // "Ex-Racer"
    std::string description; // Описание
    int startingMoney;       // 800-2000₽
    std::string startingCar; // BMW/Volvo/Mercedes

    // Бонусы
    float fuelEfficiency;    // 1.0-1.2 (множитель)
    float diplomacy;         // 1.0-1.3
};
```

### Car.h

```cpp
struct CarType {
    std::string name;         // "BMW E30 Coupe", "Volvo 940 Sedan", "Mercedes-Benz Sprinter"
    int fuelCapacity;         // 75-200L
    int cargoCapacity;        // 160-620kg
    float fuelConsumption;    // 1.5-2.0 L/100km
    int maxSpeed;             // 80-120 km/h
};
```

### Item.h

```cpp
struct Item {
    std::string id;           // Уникальный ID
    std::string name;         // Название
    std::string description;  // Описание
    float weight;             // Вес в кг
    int value;                // Цена в рублях
    int width, height;        // Размер в ячейках (для Grid)

    enum class Type {
        CONSUMABLE,           // Расходуемые (еда, вода)
        TOOL,                 // Инструменты
        QUEST_ITEM,           // Квестовые предметы
        JUNK                  // Хлам
    };
    Type type;
};
```

### NPC.h

```cpp
class NPC {
public:
    std::string getId() const;
    std::string getName() const;
    int getLevel() const;

    std::string getPassiveAbility() const;

    void addExperience(int xp);
    bool canLevelUp() const;
    void levelUp();

private:
    std::string m_id;
    std::string m_name;
    std::string m_bio;
    int m_level;
    int m_experience;
    int m_morale;             // 0-100
    std::string m_passiveAbility;
};
```

### Structure.h

```cpp
enum class LocationType {
    GAS_STATION,
    STORE,
    GARAGE,
    DINER,
    MOTEL,
    MECHANIC,
    HOME
};

class Structure {
public:
    Structure(LocationType type, float x, float y);

    LocationType getType() const;
    std::string getName() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    bool isPlayerNearby(sf::Vector2f playerPos, float radius) const;
    void render(sf::RenderWindow& window);

private:
    LocationType m_type;
    std::string m_name;
    sf::Vector2f m_position;
    sf::RectangleShape m_shape;
    sf::Color m_color;
};
```

---

## Build System

### CMakeLists.txt

**Основные параметры:**
```cmake
cmake_minimum_required(VERSION 3.10)
project(kkurs VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
```

**Source files:**
```cmake
set(SOURCES
    src/main.cpp
    src/MainMenuScene.cpp
    src/CharacterCreationScene.cpp
    src/SceneManager.cpp
    src/PlayerState.cpp
    src/Character.cpp
    src/Car.cpp
    src/InventorySystem.cpp
    src/GridInventorySystem.cpp
    src/NPC.cpp
    src/DialogueManager.cpp
    src/ExperienceSystem.cpp
    src/AbilityNode.cpp
    src/AbilityTreeSystem.cpp
    src/AbilityTreeScene.cpp
    src/NodeScene.cpp
    src/MapScene.cpp
    src/EventManager.cpp
    src/CinematicEventScene.cpp
    src/TravelCinematicScene.cpp
    src/QuestManager.cpp
    src/LocationScene.cpp
    src/Structure.cpp
    src/GameStateManager.cpp
    src/SaveSystem.cpp
    src/CharacterScene.cpp
    src/CompanionsScene.cpp
    src/QuestsScene.cpp
    src/InventoryScene.cpp
    src/UI/UIComponent.cpp
    src/UI/HUDPanel.cpp
    src/UI/ResourceBar.cpp
)
```

**Build commands:**
```bash
mkdir build && cd build
cmake .. -DSFML_DIR=/path/to/sfml
make -j$(nproc)
./kkurs
```

### Testing

**Google Test framework:**
```cmake
enable_testing()
add_subdirectory(tests)
```

**Запуск тестов:**
```bash
cd build
ctest --output-on-failure
```

**Текущий статус:** 88/97 tests passing (91%)

---

## Константы и конфигурация

### Window

```cpp
const int WINDOW_WIDTH = 1440;
const int WINDOW_HEIGHT = 900;
const int FPS_LIMIT = 60;
const std::string WINDOW_TITLE = "KKURS - Road Trip Adventure";
```

### Colors

```cpp
const sf::Color BG_DARK(40, 40, 50);
const sf::Color BG_LIGHT(60, 60, 70);
const sf::Color ACCENT_BLUE(70, 130, 180);
const sf::Color ACCENT_GREEN(60, 180, 75);
const sf::Color ACCENT_RED(220, 50, 50);
const sf::Color ACCENT_YELLOW(255, 200, 50);
const sf::Color BUTTON_NORMAL(80, 80, 90);
const sf::Color BUTTON_HOVER(100, 100, 110);
const sf::Color TEXT_WHITE(255, 255, 255);
const sf::Color BORDER(150, 150, 160);
```

### Gameplay

```cpp
const float FUEL_CONSUMPTION_HIGHWAY = 1.5f;  // L/100km
const float FUEL_CONSUMPTION_ROAD = 2.0f;
const float FUEL_CONSUMPTION_PATH = 2.8f;

const float TRAVEL_SPEED = 50.0f;  // km/sec (игровое время)
const float EVENT_TRIGGER_INTERVAL = 5.0f;  // секунды

const int STARTING_MONEY_MIN = 800;
const int STARTING_MONEY_MAX = 2000;

const int INVENTORY_SLOTS = 24;
const float MAX_CARGO_WEIGHT = 64.0f;  // kg
```

---

## Game Loop (main.cpp)

```cpp
int main() {
    // Window setup
    sf::RenderWindow window(sf::VideoMode(1440, 900), "KKURS");
    window.setFramerateLimit(60);

    // Scene manager
    SceneManager& sceneManager = SceneManager::getInstance();
    sceneManager.changeScene(SceneType::MAIN_MENU);

    // Delta time
    sf::Clock clock;

    // Main loop
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            sceneManager.handleInput(event);
        }

        // Update
        sceneManager.update(deltaTime);

        // Check for scene exit
        if (sceneManager.getCurrentSceneType() == SceneType::EXIT) {
            window.close();
        }

        // Render
        window.clear(sf::Color(40, 40, 50));
        sceneManager.render(window);
        window.display();
    }

    return 0;
}
```

---

## Полезные команды

### Build & Run
```bash
# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# Release build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run
./kkurs
```

### Clean
```bash
rm -rf build/*
```

### Tests
```bash
cd build
ctest --verbose
```

---

**Последнее обновление:** 2025-11-26
**Версия:** 1.0
**Статус:** Active Development
