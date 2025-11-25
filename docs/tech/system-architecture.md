# System Architecture Documentation
# Road Trip Adventure RPG

**Version:** 1.0  
**Last Updated:** 2025-11-11

---

## Table of Contents

1. [System Overview](#system-overview)
2. [Component Architecture](#component-architecture)
3. [Data Flow Diagrams](#data-flow-diagrams)
4. [Class Hierarchy](#class-hierarchy)
5. [Integration Points](#integration-points)
6. [Design Patterns](#design-patterns)

---

## System Overview

Road Trip Adventure RPG follows a scene-based architecture where all game states are encapsulated as independent scenes managed by a central SceneManager.

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                       Application Layer                      │
│                         (main.cpp)                           │
└──────────────────────────┬──────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────┐
│                      Scene Manager                           │
│              (Orchestrates all game states)                  │
└───┬────────────┬────────────┬────────────┬──────────────┬───┘
    │            │            │            │              │
┌───▼───┐  ┌────▼────┐  ┌────▼────┐  ┌───▼────┐   ┌────▼────┐
│ Main  │  │Character│  │  Route  │  │ World  │   │Cinematic│
│ Menu  │  │Creation │  │Selection│  │  Map   │   │ Travel  │
└───┬───┘  └────┬────┘  └────┬────┘  └───┬────┘   └────┬────┘
    │           │            │            │              │
    └───────────┴────────────┴────────────┴──────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────┐
│                     Core Game Systems                        │
├──────────────┬───────────────┬──────────────┬───────────────┤
│  Player      │  Resource     │   Inventory  │    Event      │
│  State       │  Manager      │   System     │   Manager     │
└──────────────┴───────────────┴──────────────┴───────────────┘
```

### Architecture Principles

1. **Separation of Concerns:** Each scene handles its own logic, rendering, and input
2. **Single Responsibility:** Components have one clear purpose
3. **Dependency Inversion:** High-level modules don't depend on low-level modules
4. **Open/Closed Principle:** Open for extension, closed for modification
5. **Scene Independence:** Scenes communicate only through SceneManager and shared state

---

## Component Architecture

### Scene System

All game states inherit from the abstract `Scene` interface:

```cpp
class Scene {
public:
    virtual ~Scene() = default;
    
    // Core scene lifecycle
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    
    // Scene management
    virtual SceneType getNextScene() const = 0;
    virtual bool isFinished() const = 0;
    
    // Optional lifecycle hooks
    virtual void onEnter() {}  // Called when scene becomes active
    virtual void onExit() {}   // Called before transition
    virtual void onPause() {}  // Called when overlayed
    virtual void onResume() {} // Called when overlay removed
};
```

**Scene Lifecycle:**
```
[Creation] → [onEnter()] → [Active] → [onExit()] → [Destruction]
                              ↓
                [handleInput() / update() / render()]
                              ↓
                [isFinished() = true] → Transition
```

### Scene Manager

Central orchestrator for all game scenes:

```cpp
class SceneManager {
private:
    std::unique_ptr<Scene> m_currentScene;
    SceneType m_currentSceneType;
    sf::RenderWindow& m_window;
    sf::Font m_font;
    
public:
    SceneManager(sf::RenderWindow& window);
    
    // Scene lifecycle
    void createScene(SceneType type);
    void transitionTo(SceneType type);
    
    // Main loop
    void run();
    
    // Scene queries
    SceneType getCurrentSceneType() const;
    bool isRunning() const;
    
private:
    void handleSceneTransition();
    std::unique_ptr<Scene> createSceneInstance(SceneType type);
};
```

**Scene Transition Flow:**
```
Current Scene → isFinished() = true
       ↓
Get next scene type
       ↓
Call onExit() on current scene
       ↓
Create new scene instance
       ↓
Call onEnter() on new scene
       ↓
Set as current scene
```

---

## Data Flow Diagrams

### Game Initialization Flow

```
main()
  │
  ├─→ Create RenderWindow (1366x768)
  │
  ├─→ Load global resources (fonts, textures)
  │
  ├─→ Initialize PlayerState singleton
  │     ├─→ Set default character (Nomad)
  │     ├─→ Set default vehicle (Old Sedan)
  │     └─→ Initialize resources (fuel=100, money=200, energy=100)
  │
  ├─→ Create SceneManager
  │     └─→ Create MainMenuScene
  │
  └─→ Start game loop
        ├─→ Poll events
        ├─→ Update current scene
        ├─→ Render current scene
        └─→ Check for scene transition
```

### Journey Flow

```
World Map Scene
  │
  ├─→ Player selects destination
  │     ├─→ Validate route connectivity
  │     ├─→ Calculate fuel cost
  │     └─→ Show confirmation dialog
  │
  ├─→ Player confirms journey
  │     ├─→ Deduct estimated fuel
  │     ├─→ Record route in traveled path
  │     └─→ Transition to Cinematic Travel Scene
  │
Cinematic Travel Scene
  │
  ├─→ Initialize journey
  │     ├─→ Load route data
  │     ├─→ Set travel distance
  │     └─→ Start progress tracking
  │
  ├─→ Travel loop
  │     ├─→ Update progress (distance, time)
  │     ├─→ Check for events
  │     │     ├─→ Evaluate event conditions
  │     │     ├─→ Roll probability
  │     │     └─→ Trigger event if conditions met
  │     ├─→ Process event if active
  │     │     ├─→ Display event UI
  │     │     ├─→ Present choices
  │     │     ├─→ Apply outcome
  │     │     └─→ Resume travel
  │     └─→ Continue until destination reached
  │
  └─→ Journey complete
        ├─→ Update player position on map
        ├─→ Mark location as visited
        └─→ Transition to Location Scene
```

### Inventory Management Flow

```
Player presses 'I'
  │
  └─→ Open Inventory Scene
        │
        ├─→ Load current vehicle inventory
        │     ├─→ Get vehicle type
        │     ├─→ Get grid dimensions
        │     └─→ Load all items
        │
        ├─→ Render grid and items
        │     ├─→ Draw grid cells
        │     ├─→ Draw item sprites
        │     ├─→ Show weight/capacity
        │     └─→ Update item details panel
        │
        ├─→ Handle input
        │     ├─→ Left-click: Pick up / Place
        │     │     ├─→ Check collision
        │     │     ├─→ Validate placement
        │     │     └─→ Update grid state
        │     │
        │     ├─→ Right-click: Rotate
        │     │     ├─→ Swap dimensions
        │     │     ├─→ Check if fits
        │     │     └─→ Update orientation
        │     │
        │     └─→ Drag: Move item
        │           ├─→ Show ghost at cursor
        │           ├─→ Highlight valid cells
        │           └─→ Place on release
        │
        └─→ Press ESC
              └─→ Close Inventory Scene
                    └─→ Return to previous scene
```

### Event Processing Flow

```
Event Triggered
  │
  ├─→ Evaluate Condition
  │     ├─→ Check fuel level
  │     ├─→ Check money
  │     ├─→ Check inventory for required items
  │     ├─→ Check party composition
  │     ├─→ Check time of day
  │     ├─→ Check weather
  │     └─→ Check quest states
  │
  ├─→ If condition met:
  │     ├─→ Pause travel
  │     ├─→ Display event UI
  │     └─→ Present choices
  │
  ├─→ Player selects choice
  │     ├─→ Validate choice availability
  │     │     ├─→ Check required items
  │     │     ├─→ Check required NPCs
  │     │     └─→ Check skill requirements
  │     │
  │     └─→ Process outcome
  │           ├─→ Modify resources
  │           │     ├─→ Fuel
  │           │     ├─→ Money
  │           │     ├─→ Energy
  │           │     └─→ Morale
  │           │
  │           ├─→ Modify inventory
  │           │     ├─→ Add items
  │           │     └─→ Remove items
  │           │
  │           ├─→ Modify party
  │           │     ├─→ Add NPCs
  │           │     ├─→ Remove NPCs
  │           │     └─→ Update morale
  │           │
  │           ├─→ Trigger quests
  │           │     ├─→ Start new quest
  │           │     ├─→ Update objective
  │           │     └─→ Complete quest
  │           │
  │           └─→ Set story flags
  │
  └─→ Display outcome
        ├─→ Show result text
        ├─→ Show resource changes
        └─→ Resume travel
```

---

## Class Hierarchy

### Scene Hierarchy

```
Scene (Abstract Interface)
├── MainMenuScene
├── CharacterCreationScene
├── RouteSelectionScene
├── DrivingScene
├── SideViewScene
├── WorldMapScene (New - Phase 2)
├── CinematicEventScene (New - Phase 3)
├── LocationScene (Enhanced - Phase 4)
│   ├── TownScene (Specialized)
│   └── MotelScene (Specialized)
├── DialogueScene (Enhanced - Phase 6)
├── InventoryScene
├── QuestLogScene (New - Phase 4)
├── PauseMenuScene
└── GameOverScene
```

### Core Systems

```
PlayerState (Singleton)
├── Character Data
│   ├── Name
│   ├── Archetype
│   └── Level/Experience
│
├── Resources
│   ├── Fuel
│   ├── Money
│   ├── Energy
│   └── Morale
│
├── Vehicle Data
│   ├── Type
│   ├── Condition
│   └── Upgrades
│
├── Party
│   ├── NPCs
│   └── Relationships
│
└── Progress
    ├── Current Location
    ├── Visited Locations
    └── Story Flags

ResourceManager
├── Fuel Management
│   ├── getFuel()
│   ├── setFuel()
│   ├── modifyFuel()
│   └── hasEnoughFuel()
│
├── Money Management
│   ├── getMoney()
│   ├── setMoney()
│   ├── modifyMoney()
│   └── canAfford()
│
└── Energy Management
    ├── getEnergy()
    ├── setEnergy()
    ├── modifyEnergy()
    └── needsRest()

InventorySystem
├── GridInventorySystem (Tarkov-style)
│   ├── Grid Management
│   │   ├── canPlaceItem()
│   │   ├── placeItem()
│   │   ├── removeItem()
│   │   └── rotateItem()
│   │
│   ├── Collision Detection
│   │   ├── checkCellOccupancy()
│   │   └── validatePlacement()
│   │
│   ├── Item Search
│   │   ├── getItemAtCell()
│   │   ├── findItem()
│   │   └── getItemsInCategory()
│   │
│   └── Vehicle Constraints
│       ├── setVehicleType()
│       ├── getMaxWeight()
│       └── getGridDimensions()
│
└── Legacy InventorySystem (Deprecated)
    └── Slot-based system

EventManager (New - Phase 3, 7)
├── Event Pool
│   ├── Random Events
│   ├── Scripted Events
│   └── Conditional Events
│
├── Event Processing
│   ├── evaluateCondition()
│   ├── selectRandomEvent()
│   ├── triggerEvent()
│   └── processOutcome()
│
└── Event State
    ├── Triggered Events
    ├── Active Events
    └── Completed Events

NPCManager (New - Phase 6)
├── NPC Database
│   ├── All NPCs
│   ├── Party Members
│   └── Available NPCs
│
├── NPC Interaction
│   ├── startDialogue()
│   ├── updateRelationship()
│   ├── updateMorale()
│   └── checkAbility()
│
└── Party Management
    ├── addToParty()
    ├── removeFromParty()
    ├── getPartySize()
    └── hasSlotAvailable()

QuestSystem (New - Phase 4, 6)
├── Quest Database
│   ├── All Quests
│   ├── Active Quests
│   └── Completed Quests
│
├── Quest Management
│   ├── acceptQuest()
│   ├── updateObjective()
│   ├── completeQuest()
│   └── failQuest()
│
└── Quest Tracking
    ├── getCurrentObjective()
    ├── getQuestProgress()
    └── checkRequirements()
```

---

## Integration Points

### Scene ↔ PlayerState

All scenes can access PlayerState singleton:

```cpp
// Read player data
std::string name = PlayerState::getInstance().getCharacterName();
CharacterArchetype archetype = PlayerState::getInstance().getArchetype();
VehicleType vehicle = PlayerState::getInstance().getVehicleType();

// Modify player data
PlayerState::getInstance().setMoney(newAmount);
PlayerState::getInstance().setCurrentLocation(nodeId);
```

### Scene ↔ ResourceManager

Scenes interact with resources:

```cpp
ResourceManager& rm = ResourceManager::getInstance();

// Check resources
if (rm.hasEnoughFuel(requiredFuel)) {
    // Proceed with action
}

// Modify resources
rm.modifyFuel(-consumedFuel);
rm.modifyMoney(earnedMoney);
rm.modifyEnergy(-drainedEnergy);

// Get current values
float currentFuel = rm.getFuel();
int currentMoney = rm.getMoney();
float currentEnergy = rm.getEnergy();
```

### Scene ↔ InventorySystem

Inventory access:

```cpp
GridInventorySystem& inv = GridInventorySystem::getInstance();

// Add item
Item newItem = createItem("Wrench", 2, 1, 2.5f);
if (inv.addItem(newItem)) {
    // Success
}

// Check for item
if (inv.hasItem("Spare Tire")) {
    // Use item
    inv.removeItem("Spare Tire");
}

// Get inventory status
float weight = inv.getTotalWeight();
int slots = inv.getUsedSlots();
```

### Scene ↔ EventManager

Event system integration:

```cpp
EventManager& em = EventManager::getInstance();

// Register event
Event newEvent = createEvent(...);
em.registerEvent(newEvent);

// Check for event during travel
if (Event* event = em.checkForEvent(currentGameState)) {
    // Process event
    displayEvent(event);
}

// Process outcome
em.processOutcome(chosenOutcome);
```

### Scene ↔ NPCManager

NPC interactions:

```cpp
NPCManager& nm = NPCManager::getInstance();

// Get NPC data
NPC* joe = nm.getNPC("joe_mechanic");

// Start dialogue
nm.startDialogue("joe_mechanic", dialogueTreeId);

// Modify relationship
nm.modifyRelationship("joe_mechanic", +10);

// Party management
if (nm.hasPartySlot()) {
    nm.addToParty("joe_mechanic");
}
```

---

## Design Patterns

### Singleton Pattern

Used for global state and managers:

```cpp
class PlayerState {
private:
    static PlayerState* s_instance;
    PlayerState() = default;
    
public:
    static PlayerState& getInstance() {
        if (!s_instance) {
            s_instance = new PlayerState();
        }
        return *s_instance;
    }
    
    // Delete copy/move
    PlayerState(const PlayerState&) = delete;
    PlayerState& operator=(const PlayerState&) = delete;
};
```

**Applied to:**
- PlayerState
- ResourceManager
- GridInventorySystem
- EventManager (Phase 3)
- NPCManager (Phase 6)
- QuestSystem (Phase 4)

### State Pattern

Scene system implements state pattern:

```cpp
// Each scene is a state
class Scene { /* ... */ };

// SceneManager is the context
class SceneManager {
    std::unique_ptr<Scene> m_currentScene;
    
    void transitionTo(SceneType type) {
        // Change state
        m_currentScene = createSceneInstance(type);
    }
};
```

### Factory Pattern

Scene creation uses factory pattern:

```cpp
std::unique_ptr<Scene> SceneManager::createSceneInstance(SceneType type) {
    switch(type) {
        case SceneType::MAIN_MENU:
            return std::make_unique<MainMenuScene>(m_window);
        case SceneType::WORLD_MAP:
            return std::make_unique<WorldMapScene>(m_window);
        case SceneType::CINEMATIC_TRAVEL:
            return std::make_unique<CinematicEventScene>(m_window, fromNode, toNode);
        // ... more cases
    }
}
```

### Observer Pattern

Event system uses observer pattern:

```cpp
class EventObserver {
public:
    virtual void onEventTriggered(const Event& event) = 0;
    virtual void onEventCompleted(const EventOutcome& outcome) = 0;
};

class EventManager {
    std::vector<EventObserver*> m_observers;
    
    void notifyEventTriggered(const Event& event) {
        for (auto observer : m_observers) {
            observer->onEventTriggered(event);
        }
    }
};
```

### Strategy Pattern

Different NPC abilities use strategy pattern:

```cpp
class Ability {
public:
    virtual void apply(GameState& state) = 0;
};

class MechanicRepairDiscount : public Ability {
    void apply(GameState& state) override {
        state.repairCost *= 0.8f;  // 20% discount
    }
};

class NPC {
    std::vector<std::unique_ptr<Ability>> m_abilities;
    
    void applyAbilities(GameState& state) {
        for (auto& ability : m_abilities) {
            ability->apply(state);
        }
    }
};
```

---

## Performance Considerations

### Memory Management

**Object Pooling:**
- Event pool for frequent allocations
- Particle system pool for effects
- UI element pool for repeated use

**Resource Loading:**
- Lazy loading for large assets
- Texture atlases for sprites
- Font caching for text rendering

### Rendering Optimization

**Batching:**
- Batch similar draw calls
- Use vertex arrays for static geometry
- Minimize state changes

**Culling:**
- Off-screen culling for large maps
- Distance-based LOD (Level of Detail)
- Frustum culling for 3D elements

### Update Optimization

**Spatial Partitioning:**
- Grid-based spatial hashing for location queries
- Quadtree for large world maps
- Broad-phase collision detection

**Delta Time:**
- Frame-independent updates
- Fixed timestep for physics
- Variable timestep for rendering

---

## Security Considerations

**Save File Integrity:**
- Checksums for save data validation
- Encryption for sensitive data
- Version compatibility checks

**Input Validation:**
- Sanitize all user input
- Validate file paths
- Bounds checking for array access

**Resource Limits:**
- Max inventory size enforcement
- Max NPC party size
- Max quest count limits

---

## Extensibility

### Adding New Scenes

```cpp
// 1. Define scene type
enum class SceneType {
    // ... existing types
    NEW_SCENE
};

// 2. Create scene class
class NewScene : public Scene {
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
};

// 3. Add to factory
std::unique_ptr<Scene> SceneManager::createSceneInstance(SceneType type) {
    // ... existing cases
    case SceneType::NEW_SCENE:
        return std::make_unique<NewScene>(m_window);
}
```

### Adding New Events

```cpp
// 1. Define event data
Event newEvent;
newEvent.id = "unique_event_id";
newEvent.category = EventCategory::ENCOUNTER;
newEvent.description = "Event description...";
newEvent.condition = /* ... */;

// 2. Define choices
EventChoice choice1;
choice1.text = "Choice text...";
choice1.outcome = /* ... */;
newEvent.choices.push_back(choice1);

// 3. Register event
EventManager::getInstance().registerEvent(newEvent);
```

### Adding New NPCs

```cpp
// 1. Create NPC data
NPC newNPC;
newNPC.id = "npc_identifier";
newNPC.name = "NPC Name";
newNPC.type = NPCType::MECHANIC;

// 2. Add abilities
newNPC.abilities.push_back(std::make_unique<RepairDiscount>(20));

// 3. Register NPC
NPCManager::getInstance().registerNPC(newNPC);
```

---

**Document Status:** Complete  
**Last Review:** 2025-11-11  
**Next Review:** After Phase 2 implementation
