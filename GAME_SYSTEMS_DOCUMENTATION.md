# 69 Crossroads - Comprehensive Game Systems Documentation

## Table of Contents
1. [Project Overview](#project-overview)
2. [Architecture Overview](#architecture-overview)
3. [Scene Management System](#scene-management-system)
4. [Player State & Resources](#player-state--resources)
5. [Inventory System](#inventory-system)
6. [Travel System](#travel-system)
7. [Location System](#location-system)
8. [Shop System](#shop-system)
9. [UI Systems](#ui-systems)
10. [Save/Load System](#saveload-system)
11. [Quest System](#quest-system)
12. [Event System](#event-system)
13. [Dialogue System](#dialogue-system)
14. [Ability Tree System](#ability-tree-system)
15. [Current Implementation Status](#current-implementation-status)

---

## Project Overview

**Game Title**: 69 Crossroads
**Genre**: Road Trip RPG / Interactive Fiction
**Technical Stack**: C++17, SFML 3.x
**Architecture**: Scene-based state machine
**Estimated Code Size**: ~20,000+ lines across ~130 files
**Resolution**: Fixed 1300x800 (no responsive design)

### Core Concept
A narrative-driven road trip game where the player drives across Russia (5 cities), managing resources (fuel, energy, money), making choices during events, and progressing through a story. Inspired by "Keep Driving" (visual style) and "Kentucky Route Zero" (narrative approach).

---

## Architecture Overview

### Design Pattern: Scene-Based State Machine

The game uses a **scene-based architecture** where each major game state is represented as a scene that inherits from a common `Scene` base class.

**Key Components:**
- **Base Scene Class** (`Scene.h`): Abstract interface defining scene lifecycle
- **Scene Manager** (`SceneManager`): Manages scene transitions and lifecycle
- **Game State Manager** (`GameStateManager`): Singleton providing global access to player state
- **Save System** (`SaveSystem`): Handles auto-save and backup management

### File Structure
```
kurs/
├── include/          # Header files (~80 files)
│   ├── Scene.h
│   ├── PlayerState.h
│   ├── GameStateManager.h
│   ├── [Scene implementations].h
│   └── UI/           # UI components
├── src/              # Source files (~50 files)
│   ├── main.cpp
│   ├── SceneManager.cpp
│   └── [implementations].cpp
├── assets/           # Game assets
│   └── fonts/
├── build/            # CMake build directory
└── saves/            # Save files (auto-generated)
```

---

## Scene Management System

### Scene Base Class (`Scene.h`)

**Purpose**: Abstract interface for all game scenes

**Core Methods:**
```cpp
class Scene {
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual SceneType getNextScene() const = 0;
    virtual bool isFinished() const = 0;
};
```

### Scene Types (16 Total)

Defined in `SceneType` enum (`Scene.h:7-27`):

| Scene Type | Purpose | Status |
|------------|---------|--------|
| MAIN_MENU | Main menu with New Game/Continue/Exit | ✅ Working |
| CHARACTER_CREATION | Origin & car selection | ✅ Working |
| NODE | City interaction hub | ✅ Working |
| MOTEL | Rest location (separate scene) | ⚠️ Legacy |
| DIALOGUE | NPC conversation interface | ⚠️ System exists, needs scenes |
| INVENTORY | Inventory management UI | ✅ Working |
| CHARACTER | Character stats display | ✅ Working |
| COMPANIONS | Party management | ✅ Working |
| QUESTS | Quest journal | ✅ Working |
| ABILITY_TREE | Pip-Boy style skill tree | ✅ Working |
| GAME_OVER | Failure state | ⚠️ Planned |
| END_GAME | Victory scene (MVP) | ✅ Working |
| CINEMATIC_EVENT | Event during travel | ✅ Working |
| TRAVEL_CINEMATIC | Keep Driving style travel | ✅ Working |
| TRAVEL_SELECTION | Choose travel route | ✅ Working |
| LOCATION | Universal location scene | ✅ Working |
| NOTES | Player notepad | ✅ Working |
| MAP | Static map viewer | ✅ Working |
| EXIT | Quit game | ✅ Working |

### SceneManager (`SceneManager.cpp`)

**Purpose**: Manages scene lifecycle and transitions

**Key Features:**
- Creates scenes based on `SceneType` enum
- Handles scene transitions when `isFinished()` returns true
- Manages game loop (input → update → render)
- Integrates with `GameStateManager` for state persistence

**Scene Creation (`SceneManager.cpp:36-136`):**
```cpp
std::unique_ptr<Scene> SceneManager::createScene(SceneType type) {
    switch (type) {
        case SceneType::MAIN_MENU:
            return std::make_unique<MainMenuScene>();
        case SceneType::CHARACTER_CREATION:
            return std::make_unique<CharacterCreationScene>();
        case SceneType::LOCATION:
            LocationType locationType = GameStateManager::getInstance()
                .getCurrentLocationType();
            return std::make_unique<LocationScene>(locationType);
        // ... etc
    }
}
```

**Game Loop (`SceneManager.cpp:139-189`):**
```cpp
void SceneManager::run() {
    while (m_window.isOpen() && m_currentScene) {
        float deltaTime = clock.restart().asSeconds();

        // Update playtime tracking
        playerState.updatePlayTime(deltaTime);

        // Handle events (SFML 3.x uses std::optional)
        while (const std::optional<sf::Event> event = m_window.pollEvent()) {
            if (EventHelper::isClosed(*event)) {
                m_window.close();
                return;
            }
            m_currentScene->handleInput(*event);
        }

        // Update and render
        m_currentScene->update(deltaTime);

        // Check for scene transition
        if (m_currentScene->isFinished()) {
            SceneType nextSceneType = m_currentScene->getNextScene();
            if (nextSceneType == SceneType::EXIT) {
                m_window.close();
                return;
            }
            m_currentScene = createScene(nextSceneType);
        }

        m_window.clear();
        m_currentScene->render(m_window);
        m_window.display();
    }
}
```

**SFML 3.x Compatibility:**
- Uses `std::optional<sf::Event>` for event polling
- `sf::Text` requires font in constructor (uses `std::optional<sf::Text>`)
- `sf::Vector2<T>` used instead of separate vector types

---

## Player State & Resources

### PlayerState (`PlayerState.h`, `PlayerState.cpp`)

**Purpose**: Manages player resources and game state

**Core Resources:**
- **Energy** (0-100): Player fatigue, depletes during travel
- **Money** (float): Currency for purchases (starts at 1000₽)
- **Fuel** (0-100): Vehicle fuel, depletes during travel
- **Vehicle Condition** (0-100): Car health, affects breakdowns

**Starting Resources (`PlayerState.cpp:56-84`):**
```cpp
void PlayerState::initializeResources(OriginType origin, CarType carType) {
    switch (origin) {
        case OriginType::EX_RACER:
            m_money = 150.0f;   // Less money
            m_fuel = 80.0f;     // Medium fuel
            m_energy = 100.0f;  // Full energy
            break;
        case OriginType::NOMAD:
            m_money = 200.0f;   // Standard money
            m_fuel = 100.0f;    // Full fuel
            m_energy = 90.0f;   // Slightly tired
            break;
        case OriginType::HITCHHIKER:
            m_money = 100.0f;   // Less money (hard mode)
            m_fuel = 60.0f;     // Low fuel (hard mode)
            m_energy = 70.0f;   // Low energy
            break;
    }
}
```

**Character Origins (`GameTypes.h:18-25`):**
- **NOMAD**: Balanced stats, used to rough conditions
- **HITCHHIKER**: Hard mode (low money, low fuel)
- **EX_RACER**: Less money but full energy, knows vehicles

**Car Types (`GameTypes.h:28-34`):**
- **OLD_SEDAN**: Reliable but slow, good fuel economy
- **OFFROAD_VAN**: Spacious, handles rough terrain
- **SPORTS_COUPE**: Fast but fuel-hungry, fragile

**Additional State:**
- **Inventory System** (24 slots, 50kg max)
- **Current City Index** (0-4 for 5 cities)
- **Total Play Time** (tracked in seconds)
- **Current Node ID** (for world map navigation)

**Game Over Condition (`PlayerState.cpp:88-90`):**
```cpp
bool PlayerState::isGameOver() const {
    return (m_energy <= 0.0f || m_fuel <= 0.0f);
}
```

---

## Inventory System

### Overview

**Purpose**: Manage player items with weight-based carrying capacity

**Key Features:**
- 24-slot grid system (default)
- 50kg maximum weight capacity (default)
- Stackable items support
- 5 item categories
- 5 rarity levels

### Item Structure (`Item.h:34-89`)

```cpp
struct Item {
    std::string name;
    std::string description;
    ItemCategory category;      // CONSUMABLE, TOOL, QUEST, JUNK, EQUIPMENT
    ItemRarity rarity;         // COMMON, UNCOMMON, RARE, EPIC, LEGENDARY
    int value;                 // Monetary value
    float weight;              // Weight in kg
    bool isStackable;          // Can stack in one slot
    int maxStackSize;          // Max stack size if stackable
};
```

**Item Categories (`Item.h:8-14`):**
- **CONSUMABLE**: Food, water, medicine (used up)
- **TOOL**: Repair kit, map, flashlight (utility items)
- **QUEST**: Quest-related items
- **JUNK**: Sellable items with no direct use
- **EQUIPMENT**: Spare tire, roof rack (can be equipped)

**Item Rarity (`Item.h:17-24`):**
- **COMMON** (White): Frequently found
- **UNCOMMON** (Green): Less common
- **RARE** (Blue): Hard to find
- **EPIC** (Purple): Very rare
- **LEGENDARY** (Orange): Extremely rare, unique

### InventorySystem Class (`InventorySystem.h`)

**Core Methods:**

**Adding Items (`InventorySystem.h:39`):**
```cpp
bool addItem(const Item& item, int count = 1);
```
- Auto-stacks stackable items
- Checks weight capacity
- Returns false if inventory full or too heavy

**Removing Items:**
```cpp
bool removeItem(int slotIndex, int count = 1);
int removeItemByName(const std::string& itemName, int count);
```

**Inventory Queries:**
```cpp
bool hasItem(const std::string& itemName, int minCount = 1) const;
int getItemCount(const std::string& itemName) const;
float getTotalWeight() const;
bool canAddItem(const Item& item, int count = 1) const;
```

**Inventory Management:**
```cpp
void sortInventory();           // Not yet implemented
void compactInventory();        // Fills empty slots
bool swapSlots(int slot1, int slot2);
bool dropItem(int slotIndex, int count = -1);
```

### InventorySlot Structure (`Item.h:98-150`)

```cpp
struct InventorySlot {
    Item item;       // The item in this slot
    int count;       // Number of items (for stackables)
    bool isEmpty;    // Whether slot is empty

    int addItems(int amount);     // Returns overflow
    int removeItems(int amount);  // Returns amount removed
    void clear();
};
```

**Stacking Logic (`Item.h:123-136`):**
- Only stackable items can stack
- Respects `maxStackSize` limit
- Returns overflow if exceeds max stack

---

## Travel System

### TravelCinematicScene (`TravelCinematicScene.h`)

**Purpose**: "Keep Driving" style side-scrolling travel between cities

**Visual Style:**
- 7-layer parallax scrolling
- Side-view car driving
- Oncoming traffic
- Road markings and posts
- Pip-Boy style HUD panel at bottom

### Parallax Layers

**Layer System (7 layers, `TravelCinematicScene.h:97-113`):**

| Layer | Element | Speed Multiplier | Purpose |
|-------|---------|------------------|---------|
| 0 | Sky | 0.0x (static) | Background |
| 1 | Clouds | 0.05x | Very slow |
| 2 | Mountains | 0.1x | Slow |
| 3 | Hills | 0.3x | Medium |
| 4 | Trees | 0.5x | Medium-fast |
| 5 | Fence/Posts | 0.7x | Fast |
| 6 | Road | 1.0x | Fastest |

**Decorative Objects:**
- Each layer has vector of rectangles (`m_layer1Objects`, `m_layer2Objects`, etc.)
- Speed squares on road layer for speed indication
- White posts along road edges
- Road lane markings

### Travel Mechanics

**Road Data Structure (`TravelCinematicScene.h:26-40`):**
```cpp
struct RoadData {
    std::string fromNodeId;      // Starting city
    std::string toNodeId;        // Destination city
    std::string roadType;        // "highway", "road", "path"
    float distance;              // Distance in km (default: 100)
    float baseEventChance;       // Event probability (0.0-1.0)
};
```

**Travel State:**
- `m_progress`: 0.0 to 1.0 (travel completion)
- `m_travelSpeed`: km per second
- `m_distanceTraveled`: Current distance in km

**Resource Consumption (`TravelCinematicScene.h:62`):**
```cpp
void consumeResources(float deltaTime);
float calculateFuelConsumption() const;
```
- Fuel depletes based on distance and car type
- Energy depletes over time
- Tracked via PlayerState reference

### Event System Integration

**Event Triggering (`TravelCinematicScene.h:60`):**
```cpp
void tryTriggerEvent();
```
- Checks for random events during travel
- Uses `EventManager` for event selection
- Creates `CinematicEventScene` when event triggers
- Event check interval: configurable seconds

**Active Event:**
```cpp
std::unique_ptr<CinematicEventScene> m_activeEvent;
float m_timeSinceLastEventCheck;
float m_eventCheckInterval;
```

### Dialogue System (Roadmovie)

**Travel Dialogues (`TravelCinematicScene.h:15-23`):**
```cpp
struct TravelDialogue {
    std::string npcName;       // Speaking NPC
    std::string text;          // Dialogue text
    float displayDuration;     // Display time (default: 7s)
};
```

**Dialogue State:**
```cpp
std::vector<TravelDialogue> m_availableDialogues;
std::optional<TravelDialogue> m_currentDialogue;
float m_dialogueTimer;
float m_timeSinceLastDialogue;
```

### HUD System

**Keep Driving Style Panel (`TravelCinematicScene.h:174-186`):**
- Bottom panel with 3 sections (left, center, right)
- Left: Energy bars + status
- Center: Date/time/weather display
- Right: Car fuel + condition bars
- Character sprite and car top-view

**Warning Indicators (`TravelCinematicScene.h:156-161`):**
- Fuel warning icon + text (flashing when low)
- Energy warning icon + text
- Flash timer with visibility toggle

**Speedometer (`TravelCinematicScene.h:164-166`):**
- Background + speed text
- Needle indicator

**Arrival Notification (`TravelCinematicScene.h:169-172`):**
- Shows when approaching destination
- Auto-dismisses after timer

### Scene Transition

**Arrival Check (`TravelCinematicScene.h:63`):**
```cpp
void checkArrival();
```

When `m_progress >= 1.0`:
1. Player city index advances (`playerState.advanceToNextCity()`)
2. Auto-save triggered
3. Transitions to next scene (usually NODE scene)

---

## Location System

### LocationScene (`LocationScene.h`)

**Purpose**: Universal scene for all location types (gas stations, stores, diners, garages, motels)

**Location Types (`Location.h:8-17`):**
```cpp
enum class LocationType {
    HOME,           // Player's starting home
    GAS_STATION,    // Refuel location
    STORE,          // Buy supplies
    GARAGE,         // Repair car
    MOTEL,          // Rest location
    DINER,          // Eat and recover energy
    MECHANIC,       // Advanced repairs
    UNKNOWN         // Default/uninitialized
};
```

### Visual Elements (`LocationScene.h:59-66`)

```cpp
sf::RectangleShape m_background;        // Location interior
sf::RectangleShape m_counterBox;        // Counter/service area
sf::RectangleShape m_playerBox;         // Player character
sf::RectangleShape m_exitDoorBox;       // Exit door

std::vector<sf::RectangleShape> m_objects;      // Shelves, pumps, tables
std::vector<sf::Text> m_objectLabels;           // Interactive object labels
```

### Interaction System

**Available Interactions (`LocationScene.h:41-47`):**
```cpp
void handleRefuel();    // Gas station: refuel vehicle
void handleShop();      // Store: open shop UI
void handleRepair();    // Garage: repair vehicle
void handleEat();       // Diner: buy food/drinks
void handleRest();      // Any location: quick rest
void handleTalk();      // Talk to NPC if present
void handleLeave();     // Exit location
```

**Interaction Menu:**
- Simple menu system (`m_showInteractionMenu`)
- Options based on location type
- Keyboard navigation

### Shop System Integration

**Shop State (`LocationScene.h:88-94`):**
```cpp
bool m_isInShopMode;                    // In shopping UI
InventorySystem m_storeInventory;       // Store's items for sale
int m_selectedStoreIndex;               // Selected store item
int m_selectedPlayerIndex;              // Selected player item
int m_storeScrollOffset;                // Store list scroll
int m_playerScrollOffset;               // Inventory scroll
bool m_isStoreListActive;               // Which list is focused
```

**Shop Methods (`LocationScene.h:50-56`):**
```cpp
void initializeStoreInventory();        // Setup store items
void enterShopMode();                   // Enter shop UI
void exitShopMode();                    // Exit shop UI
void handleShopInput(const sf::Event& event);
void buySelectedItem();                 // Buy from store
void sellSelectedItem();                // Sell from inventory
void renderShopUI(sf::RenderWindow& window);
```

### Resource Management

**HUD Panel (`LocationScene.h:85`):**
```cpp
HUDPanel m_hudPanel;  // Displays resources (money, fuel, energy)
```

**Transaction Validation:**
- Money check: `playerState.canAfford(cost)`
- Weight check: `inventory.canAddItem(item, count)`
- Fuel limit: max 100
- Energy limit: max 100

---

## Shop System

### Shop Mechanics

**Buy Flow:**
1. Player navigates store list (Tab to switch lists)
2. Select item with Arrow keys
3. Press Enter to buy
4. System checks:
   - Player has enough money
   - Item weight won't exceed capacity
5. If valid:
   - Deduct money
   - Add item to inventory
   - Remove from store (if limited stock)

**Sell Flow:**
1. Player navigates inventory (Tab to switch lists)
2. Select item with Arrow keys
3. Press Enter to sell
4. System calculates sell price (50% of value)
5. Add money to player
6. Remove item from inventory
7. Optionally add to store inventory

**Price Calculation:**
- **Buy Price**: `item.value` (full price)
- **Sell Price**: `item.value * 0.5` (50% of value)

### Shop UI Layout

**Dual-List Interface:**
```
┌─────────────────────────────────────────┐
│           STORE NAME                    │
├─────────────────────────────────────────┤
│  STORE INVENTORY    │  PLAYER INVENTORY │
│  [Active List]      │  [Inactive List]  │
│  > Item 1 - 100₽    │    Item A - 50₽   │
│    Item 2 - 50₽     │    Item B - 30₽   │
│    Item 3 - 200₽    │    Item C - 100₽  │
├─────────────────────────────────────────┤
│  ITEM DETAILS                           │
│  Description: ...                       │
│  Weight: 2.5kg                          │
├─────────────────────────────────────────┤
│  [Tab] Switch | [Enter] Buy/Sell | [Esc]│
└─────────────────────────────────────────┘
```

**Controls:**
- **Arrow Up/Down**: Navigate list
- **Tab**: Switch between store/inventory
- **Enter**: Buy (if store active) or Sell (if inventory active)
- **Escape**: Exit shop mode

### Store Inventory Initialization

**Location-Specific Items:**
- **Gas Station**: Fuel (special handling), snacks, drinks
- **Store**: General supplies, food, tools
- **Diner**: Food items, coffee, meals
- **Garage**: Repair kits, spare parts
- **Motel**: Rest services (not items)

---

## UI Systems

### UIConstants (`UIConstants.h`)

**Screen Dimensions:**
```cpp
constexpr float SCREEN_WIDTH = 1300.0f;
constexpr float SCREEN_HEIGHT = 800.0f;
```
**Note**: Fixed resolution, no responsive design

**Color Palette:**
```cpp
namespace UI::Color {
    const sf::Color BACKGROUND_DARK(40, 40, 50);
    const sf::Color BACKGROUND_LIGHT(60, 60, 70);
    const sf::Color ACCENT_BLUE(70, 130, 180);
    const sf::Color ACCENT_GREEN(60, 180, 75);
    const sf::Color ACCENT_RED(220, 50, 50);
    const sf::Color ACCENT_YELLOW(255, 200, 50);
    const sf::Color TEXT_PRIMARY(255, 255, 255);
    const sf::Color TEXT_SECONDARY(200, 200, 200);
}
```

**Font Sizes:**
```cpp
constexpr unsigned int FONT_SIZE_HUGE = 48;
constexpr unsigned int FONT_SIZE_TITLE = 32;
constexpr unsigned int FONT_SIZE_SUBTITLE = 24;
constexpr unsigned int FONT_SIZE_NORMAL = 18;
constexpr unsigned int FONT_SIZE_SMALL = 14;
constexpr unsigned int FONT_SIZE_TINY = 12;
```

**Font Paths:**
```cpp
namespace UI::Fonts {
    const char* const PRIMARY = "assets/fonts/font.ttf";
    const char* const FALLBACK_1 = "images/Press_Start_2P/PressStart2P-Regular.ttf";
    const char* const FALLBACK_2 = "/System/Library/Fonts/Helvetica.ttc";
}
```

**Spacing:**
```cpp
constexpr float PADDING_HUGE = 40.0f;
constexpr float PADDING_LARGE = 20.0f;
constexpr float PADDING_MEDIUM = 10.0f;
constexpr float PADDING_SMALL = 5.0f;
```

### HUDPanel Component

**Purpose**: Display player resources in compact panel

**Features:**
- Money display (₽)
- Fuel gauge (0-100)
- Energy gauge (0-100)
- Vehicle condition gauge (0-100)

**Usage:**
```cpp
HUDPanel m_hudPanel;
m_hudPanel.update(playerState);
m_hudPanel.render(window);
```

### UIComponent System

**Base Class** (`UI/UIComponent.h`):
- Abstract base for reusable UI elements
- Supports positioning, visibility, interaction
- Used for buttons, panels, dialogs

---

## Save/Load System

### SaveSystem (`SaveSystem.h`)

**Purpose**: Auto-save with rotating backups (no manual save)

**Key Features:**
- 1 main save file
- 4 rotating backups
- JSON format (human-readable)
- Auto-save on city arrival
- Load from main or any backup

### Backup Rotation

**System (`SaveSystem.h:17`):**
```
saves/
├── save.json           (Main save)
├── save_backup_0.json  (Most recent)
├── save_backup_1.json
├── save_backup_2.json
└── save_backup_3.json  (Oldest)
```

**Rotation Logic:**
1. When saving:
   - Copy `save.json` → `save_backup_0.json`
   - Shift backups: 0→1, 1→2, 2→3
   - Oldest backup (3) is overwritten
2. New save written to `save.json`

### Save Data Structure

**Serialized Data:**
- Player resources (energy, money, fuel, condition)
- Character origin and car type
- Current city index (0-4)
- Total play time (seconds)
- Current node ID
- Inventory (items, quantities, weights)

**Format**: JSON-like (details in `SaveSystem::serializePlayerState`)

### Save Operations

**Save Game (`SaveSystem.h:23`):**
```cpp
bool saveGame(const PlayerState& playerState);
```
- Serializes player state
- Rotates backups
- Writes to `save.json`

**Load Game (`SaveSystem.h:26`):**
```cpp
bool loadGame(PlayerState& playerState);
```
- Reads from `save.json`
- Deserializes into PlayerState

**Load Backup (`SaveSystem.h:29`):**
```cpp
bool loadBackup(PlayerState& playerState, int backupIndex);
```
- Loads from specific backup (0-3)
- Useful for recovery

**Clear All Saves (`SaveSystem.h:47`):**
```cpp
void clearAllSaves();
```
- Deletes all save files
- Used when starting new game

### Backup Info

**BackupInfo Structure (`SaveSystem.h:38-44`):**
```cpp
struct BackupInfo {
    int index;              // 0-3
    std::string timestamp;  // Save time
    int nodeId;            // Player location
    bool exists;           // File exists
};
```

**Get Backup List (`SaveSystem.h:44`):**
```cpp
std::vector<BackupInfo> getBackupList() const;
```
- Returns metadata for all backups
- Used for load menu UI

---

## Quest System

### QuestManager (`QuestManager.h`)

**Purpose**: Track and manage player quests

**Status**: System implemented, content not fully populated

### Quest Structure (`QuestManager.h:77-107`)

```cpp
struct Quest {
    std::string id;
    std::string title;
    std::string description;
    std::string questGiverNpcId;        // NPC who gives quest
    std::string completionNpcId;        // NPC to complete quest
    QuestDifficulty difficulty;         // EASY, MEDIUM, HARD, EXTREME
    QuestStatus status;                 // NOT_STARTED, ACTIVE, COMPLETED, FAILED
    std::vector<QuestObjective> objectives;
    QuestReward reward;
    bool repeatable;
};
```

### Quest Objectives

**Objective Types (`QuestManager.h:10-18`):**
```cpp
enum class QuestObjectiveType {
    COLLECT_ITEM,       // Collect specific items
    DELIVER_ITEM,       // Deliver items to location
    TRAVEL_TO,          // Travel to location
    TALK_TO_NPC,        // Talk to specific NPC
    COMPLETE_EVENT,     // Complete event
    EARN_MONEY,         // Earn money amount
    CUSTOM              // Custom check
};
```

**Objective Structure (`QuestManager.h:37-61`):**
```cpp
struct QuestObjective {
    QuestObjectiveType type;
    std::string description;
    std::string targetId;       // Item/NPC/location ID
    int targetAmount;           // Amount needed
    int currentAmount;          // Current progress
    bool isCompleted;

    int getProgress() const;    // Returns 0-100
};
```

### Quest Rewards

**Reward Structure (`QuestManager.h:64-74`):**
```cpp
struct QuestReward {
    int money;
    int experience;
    std::vector<std::string> itemIds;   // Items to give
};
```

### Quest Management

**Core Methods:**
```cpp
// Registration
void registerQuest(const Quest& quest);
void registerQuestsFromFile(const std::string& jsonPath);

// Management
void startQuest(const std::string& questId);
void completeQuest(const std::string& questId);
void failQuest(const std::string& questId);

// Queries
std::vector<Quest*> getActiveQuests();
std::vector<Quest*> getCompletedQuests();
std::vector<Quest*> getAvailableQuests();

// Objective updates
void updateObjective(const std::string& questId, int objectiveIndex, int progress);
void incrementObjective(const std::string& questId, int objectiveIndex);
void completeObjective(const std::string& questId, int objectiveIndex);
```

**Convenience Notifications (`QuestManager.h:137-142`):**
```cpp
void notifyItemCollected(const std::string& itemId, int amount = 1);
void notifyItemDelivered(const std::string& itemId, const std::string& location);
void notifyLocationVisited(const std::string& locationId);
void notifyNPCTalkedTo(const std::string& npcId);
void notifyEventCompleted(const std::string& eventId);
void notifyMoneyEarned(int amount);
```

**Auto-Completion:**
- System automatically checks objective completion
- When all objectives complete, quest auto-completes
- Rewards granted via `grantReward(questId)`

---

## Event System

### EventManager (`EventManager.h`)

**Purpose**: Manage random events during travel

**Status**: System implemented, default events exist

### Event Structure (`EventManager.h:41-52`)

```cpp
struct GameEvent {
    std::string id;
    std::string title;
    std::string description;
    EventCondition condition;           // When can event trigger
    std::vector<EventChoice> choices;   // Player choices
    bool triggered;                     // Has event been triggered
};
```

### Event Conditions

**Condition Structure (`EventManager.h:10-22`):**
```cpp
struct EventCondition {
    float minFuel = 0.0f;
    float maxFuel = 100.0f;
    float minEnergy = 0.0f;
    float maxEnergy = 100.0f;
    int minMoney = 0;
    int maxMoney = 999999;
    float probability = 1.0f;           // 0.0-1.0
    std::string requiredLocation = "";
    std::string requiredRoadType = "";  // "highway", "road", "path"
};
```

**Condition Check (`EventManager.h:75-78`):**
```cpp
bool checkCondition(const EventCondition& condition,
                   float fuel, float energy, int money,
                   const std::string& location = "",
                   const std::string& roadType = "") const;
```

### Event Choices

**Choice Structure (`EventManager.h:25-38`):**
```cpp
struct EventChoice {
    std::string text;               // Choice text
    std::string outcomeText;        // Result description
    float fuelChange = 0.0f;
    float energyChange = 0.0f;
    int moneyChange = 0;
    float vehicleConditionChange = 0.0f;
    bool endsEvent = true;
    std::string perkId = "";        // Ability perk ID
    bool isPerkChoice = false;      // Uses ability perk
};
```

**Outcome Application (`EventManager.h:92-93`):**
```cpp
void applyChoice(const EventChoice& choice,
                float& fuel, float& energy, int& money);
```

### Event Selection

**Random Event (`EventManager.h:81-83`):**
```cpp
GameEvent* getRandomEvent(float fuel, float energy, int money,
                         const std::string& location = "",
                         const std::string& roadType = "");
```

**Process:**
1. Filter events by condition
2. Weight by probability
3. Select random event
4. Check if already triggered (unless repeatable)
5. Return event or nullptr

### Perk Integration

**Perk Choices (`EventManager.h:106`):**
```cpp
void addPerkChoicesToEvent(GameEvent* event,
                          float playerFuel,
                          float playerEnergy,
                          int playerMoney);
```

**Purpose:**
- Injects ability-based choices into events
- If player has unlocked perks, adds special options
- Example: "Mechanic" perk adds repair option to breakdown event

### Default Events

**Initialization (`EventManager.h:102`):**
```cpp
void initializeDefaultEvents();
```

**Example Events:**
- Hitchhiker encounter
- Breakdown (requires repair)
- Police checkpoint
- Gas station closed
- Roadside vendor
- Wildlife crossing
- Weather conditions

---

## Dialogue System

### DialogueManager (`DialogueManager.h`)

**Purpose**: Tree-based dialogue system for NPC conversations

**Status**: System implemented, dialogue content partially populated

### Dialogue Tree Structure

**DialogueTree (`DialogueManager.h:63-70`):**
```cpp
struct DialogueTree {
    std::string id;                         // Dialogue ID
    std::string title;                      // Dialogue name
    std::map<std::string, DialogueNode> nodes;  // Nodes by ID
    std::string startNodeId;                // Entry point ("start")
};
```

**DialogueNode (`DialogueManager.h:50-60`):**
```cpp
struct DialogueNode {
    std::string id;                         // Node ID
    std::string speakerName;                // NPC or player name
    std::string text;                       // Dialogue text
    std::vector<DialogueChoice> choices;    // Response options
    bool isPlayerSpeaking;                  // Is player speaking
    bool isEndNode;                         // Dialogue ends here
};
```

### Dialogue Choices

**DialogueChoice (`DialogueManager.h:34-47`):**
```cpp
struct DialogueChoice {
    std::string text;                       // Choice text
    std::string nextNodeId;                 // Next node ID
    DialogueCondition condition;            // Availability check
    DialogueConsequence consequence;        // What happens
    bool requiresMoney;                     // Costs money
    float moneyCost;                        // Amount
    bool requiresItem;                      // Requires item
    std::string requiredItemId;             // Item ID
};
```

**Conditions (`DialogueManager.h:14-21`):**
```cpp
struct DialogueCondition {
    std::function<bool()> check;            // Condition function
    std::string description;                // Description
};
```

**Consequences (`DialogueManager.h:24-31`):**
```cpp
struct DialogueConsequence {
    std::function<void()> execute;          // Action function
    std::string description;                // Description
};
```

### Dialogue Management

**Core Methods:**
```cpp
// Singleton
static DialogueManager& getInstance();

// Registration
void registerDialogue(const std::string& id, const DialogueTree& tree);
void removeDialogue(const std::string& id);

// Flow control
bool startDialogue(const std::string& dialogueId);
void selectChoice(int choiceIndex);
bool goToNode(const std::string& nodeId);
void endDialogue();

// State queries
bool isActive() const;
const DialogueNode* getCurrentNode() const;
const DialogueTree* getCurrentTree() const;

// Choice validation
bool isChoiceAvailable(int choiceIndex) const;
std::string getChoiceUnavailableReason(int choiceIndex) const;
```

### History Tracking

**History System (`DialogueManager.h:103-105`):**
```cpp
void addToHistory(const std::string& dialogueId, const std::string& choiceId);
bool wasChoiceMade(const std::string& dialogueId, const std::string& choiceId) const;
void clearHistory();
```

**Purpose:**
- Track player's dialogue choices
- Enable branching based on past decisions
- Used for narrative consequences

### Dialogue Factory

**Predefined Dialogues (`DialogueManager.h:130-168`):**
```cpp
namespace DialogueFactory {
    DialogueTree createMechanicDialogue();
    DialogueTree createTraderDialogue();
    DialogueTree createCompanionDialogue();
    DialogueTree createRecruitDialogue(const std::string& npcId,
                                       const std::string& npcName,
                                       float hireCost);
    DialogueTree createConflictDialogue();
    DialogueTree createInfoDialogue(const std::string& id,
                                   const std::string& speakerName,
                                   const std::string& message);

    // NPC-specific recruitment dialogues
    DialogueTree createRecruitMechanicDialogue();
    DialogueTree createRecruitTraderDialogue();
    DialogueTree createRecruitNavigatorDialogue();
    DialogueTree createRecruitMedicDialogue();
    DialogueTree createRecruitCompanionAlexDialogue();
}
```

---

## Ability Tree System

### AbilityTreeScene (`AbilityTreeScene.h`)

**Purpose**: Pip-Boy style terminal interface for skill progression

**Visual Style:**
- Monochrome green terminal (#00FF00 on #000000)
- ASCII art tree structure
- Terminal border with scanlines
- Category-based navigation

### Color Scheme (`AbilityTreeScene.h:71-76`)

```cpp
sf::Color m_colorBackground;    // #000000 Black
sf::Color m_colorText;          // #00FF00 Bright green
sf::Color m_colorTextDim;       // #007700 Dim green
sf::Color m_colorHighlight;     // #00FF00 Bright green
sf::Color m_colorLocked;        // #004400 Very dim green
sf::Color m_colorBorder;        // #00AA00 Medium green
```

### Ability Categories

**Categories (from `AbilityTypes.h`):**
- **DRIVING**: Vehicle-related skills
- **SOCIAL**: Dialogue and persuasion
- **TECHNICAL**: Repair and crafting
- **SURVIVAL**: Resource management
- **COMBAT**: Confrontation skills (if needed)

### Ability Node System

**AbilityNode Structure:**
```cpp
struct AbilityNode {
    std::string id;
    std::string name;
    std::string description;
    AbilityCategory category;
    int tier;                   // Tree depth (0-3)
    int cost;                   // Skill points required
    std::vector<std::string> prerequisites;  // Required ability IDs
    bool isUnlocked;
    bool isAvailable;           // Prerequisites met
};
```

### Navigation

**Controls:**
- **1-5 Keys**: Switch category
- **Arrow Up/Down**: Navigate abilities
- **Enter**: Unlock selected ability (if available)
- **Escape**: Exit to previous scene

**State Symbols:**
- `[✓]`: Unlocked
- `[ ]`: Available (prerequisites met)
- `[✗]`: Locked (prerequisites not met)

### Rendering Layout (`AbilityTreeScene.h:46-51`)

```
┌─────────────────────────────────────────────┐
│ [VAULT-TEC ABILITY TERMINAL v1.0]          │
├─────────────────────────────────────────────┤
│ Categories: [1] DRIVING [2] SOCIAL ...     │
├─────────────────────────────────────────────┤
│   [✓] Basic Driving                         │
│   │                                         │
│   ├── [ ] Fuel Efficiency                   │
│   │                                         │
│   └── [✗] Advanced Maneuvers                │
├─────────────────────────────────────────────┤
│ SELECTED: Fuel Efficiency                  │
│ Cost: 2 SP | Current SP: 5                 │
│ Description: Reduces fuel consumption...   │
├─────────────────────────────────────────────┤
│ [1-5] Category | [↑↓] Navigate | [Enter]   │
└─────────────────────────────────────────────┘
```

### Integration

**ExperienceSystem:**
- Player gains XP from events/quests
- Level up grants skill points
- Skill points spent to unlock abilities

**Event System:**
- Unlocked abilities add choices to events
- Example: "Mechanic" perk adds repair option

---

## Current Implementation Status

### MVP (Minimum Viable Product) Status

**✅ FULLY WORKING:**

1. **Scene Management**
   - Base Scene architecture
   - SceneManager with 18+ scene types
   - Scene transitions

2. **Character Creation**
   - Origin selection (Nomad, Hitchhiker, Ex-Racer)
   - Car selection (Old Sedan, Offroad Van, Sports Coupe)
   - Resource initialization

3. **Player State**
   - Energy, Money, Fuel, Vehicle Condition
   - Resource getters/setters with validation
   - Game over check

4. **Inventory System**
   - 24-slot grid (50kg max)
   - Stacking support
   - Weight management
   - Add/remove/query operations

5. **Travel System**
   - 7-layer parallax scrolling
   - Keep Driving style visuals
   - Resource consumption (fuel/energy)
   - Progress tracking
   - Arrival detection

6. **Location System**
   - Universal LocationScene
   - 7 location types (gas station, store, etc.)
   - Interior visualization
   - Interaction menu

7. **Shop System**
   - Dual-list UI (store/inventory)
   - Buy/sell mechanics
   - Weight checking
   - Money validation

8. **Save System**
   - Auto-save on city arrival
   - 1 main save + 4 backups
   - Backup rotation
   - Load from any backup

9. **City Progression**
   - 5-city linear progression (indices 0-4)
   - City advancement on travel completion
   - Playtime tracking

10. **UI Components**
    - HUDPanel for resources
    - UIConstants for consistent styling
    - Text rendering (SFML 3.x compatible)

11. **End Game Scene**
    - Victory screen with stats
    - Total playtime display
    - Cities visited count

**⚠️ PARTIALLY IMPLEMENTED:**

1. **Event System**
   - EventManager: ✅ Working
   - Event conditions: ✅ Working
   - Default events: ✅ Populated
   - CinematicEventScene: ✅ Working
   - **Missing**: Full event content population

2. **Quest System**
   - QuestManager: ✅ Working
   - Quest structure: ✅ Defined
   - Objective tracking: ✅ Working
   - **Missing**: Quest content, NPC integration

3. **Dialogue System**
   - DialogueManager: ✅ Working
   - Tree structure: ✅ Defined
   - Conditions/consequences: ✅ Working
   - **Missing**: Dialogue content, scene integration

4. **Ability Tree**
   - AbilityTreeScene: ✅ Working
   - Terminal UI: ✅ Working
   - Navigation: ✅ Working
   - **Missing**: Ability effects, game integration

5. **Companion System**
   - CompanionsScene: ✅ UI exists
   - **Missing**: Companion mechanics, effects

**❌ NOT IMPLEMENTED:**

1. **Dynamic World Map**
   - Static MapScene exists
   - Node-based world map: ❌ Not implemented
   - Dynamic route selection: ❌ Hardcoded

2. **Advanced Combat**
   - No combat system currently

3. **Crafting System**
   - No crafting mechanics

4. **Advanced Events**
   - Complex multi-stage events: ❌
   - Persistent world changes: ❌

5. **Character Portraits**
   - Currently using colored rectangles

6. **Sound System**
   - No audio implemented

7. **Settings/Options**
   - No settings menu

### Code Statistics

**Estimated Metrics:**
- **Total Files**: ~130 (80 headers, 50 source)
- **Total Lines**: ~20,000+ (including comments)
- **Classes**: ~40+
- **Structs**: ~30+
- **Enums**: ~15+

**Key Files by Size (Estimated):**
- `LocationScene.cpp`: 1000+ lines
- `TravelCinematicScene.cpp`: 800+ lines
- `InventorySystem.cpp`: 500+ lines
- `SceneManager.cpp`: 200+ lines
- `PlayerState.cpp`: 100+ lines

### Technical Debt

**Known Issues:**
1. **SFML 3.x Migration**: Partial migration, some systems may use old patterns
2. **Hardcoded Values**: Many magic numbers should be constants
3. **Asset Management**: No centralized asset loader
4. **Error Handling**: Minimal error handling in some systems
5. **Testing**: No unit tests
6. **Documentation**: Sparse inline documentation

**Performance Considerations:**
- No optimization done yet
- Rendering happens every frame (no dirty checking)
- No object pooling
- Minimal caching

---

## Technical Architecture Summary

### Design Patterns Used

1. **Singleton Pattern**
   - `GameStateManager`
   - `QuestManager`
   - `DialogueManager`
   - `AbilityTreeSystem` (implied)

2. **State Machine Pattern**
   - Scene system (each scene is a state)
   - Quest status transitions
   - Event flow

3. **Observer Pattern**
   - Event callbacks
   - Dialogue consequences
   - Quest notifications

4. **Factory Pattern**
   - `DialogueFactory` for dialogue creation
   - Scene creation in SceneManager

5. **Strategy Pattern**
   - Different location behaviors
   - Event condition checking

### Data Flow

```
main.cpp
  └─> SceneManager::run()
       ├─> Scene::handleInput()
       ├─> Scene::update()
       │    └─> GameStateManager::getInstance()
       │         └─> PlayerState (modify resources)
       ├─> Scene::render()
       └─> Check isFinished() → createScene()
```

### Dependencies

**External:**
- SFML 3.x (Graphics, Window, System)
- C++17 Standard Library

**Internal Coupling:**
- Low: Scenes → GameStateManager → PlayerState
- Medium: LocationScene ↔ InventorySystem
- High: TravelCinematicScene ↔ EventManager ↔ CinematicEventScene

### File Organization

**Headers** (`include/`):
- Scene definitions
- System managers
- Data structures

**Sources** (`src/`):
- Implementation files
- Main entry point

**Assets** (`assets/`):
- Fonts
- (Future: textures, sounds)

**Build** (`build/`):
- CMake generated files
- Compiled executable

**Saves** (`saves/`):
- Auto-generated save files

---

## Future Development Roadmap

### Phase 1: Content Population (MVP Completion)
- Populate event content (20+ events)
- Create quest content (10+ quests)
- Write dialogue trees (15+ NPCs)
- Define all ability nodes (50+ abilities)

### Phase 2: Core Systems Enhancement
- Implement ability effects in gameplay
- Add companion mechanics and benefits
- Integrate dialogue with quest system
- Add more location types

### Phase 3: Polish & Assets
- Add character portraits
- Implement sound system
- Add music tracks
- Improve visual assets

### Phase 4: Advanced Features
- Dynamic world map
- Complex event chains
- Persistent world changes
- New game+ mode

### Phase 5: Optimization & Release
- Performance optimization
- Bug fixing
- Playtesting
- Final polish

---

## Appendices

### A. Key File Locations

**Core Systems:**
- Scene Management: `include/Scene.h`, `src/SceneManager.cpp`
- Player State: `include/PlayerState.h`, `src/PlayerState.cpp`
- Game State: `include/GameStateManager.h`, `src/GameStateManager.cpp`

**Gameplay Systems:**
- Inventory: `include/InventorySystem.h`, `src/InventorySystem.cpp`
- Items: `include/Item.h`
- Locations: `include/Location.h`, `include/LocationScene.h`
- Travel: `include/TravelCinematicScene.h`, `src/TravelCinematicScene.cpp`

**Content Systems:**
- Quests: `include/QuestManager.h`, `src/QuestManager.cpp`
- Events: `include/EventManager.h`, `src/EventManager.cpp`
- Dialogue: `include/DialogueManager.h`, `src/DialogueManager.cpp`
- Abilities: `include/AbilityTreeScene.h`, `src/AbilityTreeScene.cpp`

**UI:**
- Constants: `include/UIConstants.h`
- HUD: `include/UI/HUDPanel.hpp`, `src/UI/HUDPanel.cpp`

**Persistence:**
- Save System: `include/SaveSystem.h`, `src/SaveSystem.cpp`

### B. Naming Conventions

**Classes:** PascalCase (`PlayerState`, `SceneManager`)
**Methods:** camelCase (`getEnergy()`, `addItem()`)
**Members:** m_ prefix (`m_energy`, `m_currentScene`)
**Constants:** UPPER_SNAKE_CASE (`SCREEN_WIDTH`, `MAX_BACKUPS`)
**Enums:** PascalCase for type, UPPER_SNAKE_CASE for values

### C. Build Instructions

**Requirements:**
- CMake 3.10+
- C++17 compiler
- SFML 3.x

**Build Steps:**
```bash
cd build
cmake ..
make
./kkurs
```

**Run Game:**
```bash
cd build
./kkurs
```

### D. Contributing Guidelines

**Code Style:**
- 4 spaces for indentation
- Opening braces on same line
- Descriptive variable names
- Comments for complex logic

**Git Workflow:**
- Feature branches
- Descriptive commit messages
- Pull requests for major changes

---

## Document Metadata

**Version**: 1.0
**Last Updated**: 2025-12-04
**Author**: Generated from codebase analysis
**Lines**: 1600+
**Completeness**: Comprehensive coverage of all major systems

**Coverage:**
- ✅ All 18 scene types documented
- ✅ All major systems analyzed
- ✅ Data structures documented
- ✅ Current status assessed
- ✅ Code examples provided
- ✅ File locations specified

---

**End of Documentation**
