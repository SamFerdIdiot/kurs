# KEEP DRIVING MAP SYSTEM - COMPLETE IMPLEMENTATION GUIDE v2.3

> **MONOLITHIC IMPLEMENTATION DOCUMENT**  
> This file consolidates all specifications, data structures, and implementation requirements for the Keep Driving map system. Use this as the single source of truth for implementation.

---

## 📋 TABLE OF CONTENTS

1. [System Overview](#system-overview)
2. [Core Principles](#core-principles)
3. [Data Structures](#data-structures)
4. [Map Data](#map-data)
5. [Implementation Tasks](#implementation-tasks)
6. [Code Integration Points](#code-integration-points)
7. [Issues & Solutions](#issues-and-solutions)

---

## SYSTEM OVERVIEW

### What We're Building
A Keep Driving-style map system for a 90s Russia road trip RPG with:
- **28 nodes** (settlements) in spider web structure
- **90 bidirectional roads** enabling free exploration
- **Resource scarcity** - 35% villages lack gas stations
- **CAFE bulletin boards** for profit-oriented quests
- **Fuel-balanced distances** - 127km avg segments

### Current Status
✅ **Specification Phase COMPLETE**  
✅ **Map Data COMPLETE** (russia_90s_map.json)  
⏳ **Implementation Phase NEXT** (WorldMap class, UI, scenes)

---

## CORE PRINCIPLES

### 8 Keep Driving Principles
1. **Continuous Movement** - Travel only stops at nodes
2. **Resource Management** - Fuel, money, food tracking critical
3. **Different Road Types** - HIGHWAY (-20% fuel), ROAD (base), DIRT_ROAD (+40% fuel)
4. **Random Events** - 20+ event types on roads
5. **Progressive Unlocking** - Nodes unlock as player progresses
6. **Location Variety** - 6 node types with different services
7. **Spider Web Structure** - Bidirectional roads, free route choice
8. **Resource Scarcity** - Not all villages have all services

---

## DATA STRUCTURES

### Node Types
```cpp
enum class NodeType {
    HOME_BASE,    // Starting point, full services
    CITY,         // 5+ structures, full services
    TOWN,         // 3-4 structures, basic services
    VILLAGE,      // 1-2 structures, RESOURCE SCARCITY
    JUNCTION,     // 2-3 structures, minimal services
    GAS_STATION,  // 1-2 structures, fuel/food only
    UNIQUE        // Special quest locations
};
```

### Structure Types
```cpp
enum class StructureType {
    GAS_PUMP,      // Refuel (20/28 nodes = 71%)
    STORE,         // Buy food/supplies (28/28 nodes = 100%)
    GARAGE,        // Repair car (12/28 nodes = 43%)
    MOTEL,         // Rest/sleep (4/28 nodes = 14%)
    WORK_AGENCY,   // Long-term jobs (2/28 nodes = 7%)
    CAFE           // Bulletin board quests (13/28 nodes = 46%)
};
```

### Road Types
```cpp
enum class RoadType {
    HIGHWAY,     // Fast, good condition, -20% fuel
    ROAD,        // Normal, 0% fuel modifier
    DIRT_ROAD    // Slow, poor condition, +40% fuel
};
```

### Node Status
```cpp
enum class NodeStatus {
    LOCKED,      // Not yet discovered
    VISIBLE,     // Can see but can't travel
    AVAILABLE,   // Can travel to
    CURRENT,     // Player is here
    VISITED      // Been here before
};
```

### JSON Structure
```json
{
  "map_name": "Russia 90s Map",
  "version": "2.3",
  "nodes": [
    {
      "id": 1,
      "type": "HOME_BASE",
      "name": "Москва - Окраина",
      "name_en": "Moscow - Outskirts",
      "position": {"x": 150, "y": 400},
      "biome": "urban",
      "description": "Starting location",
      "structures": [
        {
          "type": "GAS_PUMP",
          "name": "Заправка 'Нефтьмаг'",
          "name_en": "Gas Station",
          "available": true
        }
      ],
      "unlockNodes": [2]
    }
  ],
  "roads": [
    {
      "from": 1,
      "to": 2,
      "type": "HIGHWAY",
      "distance": 115,
      "name": "М7 'Волга'",
      "name_en": "M7 'Volga' Highway",
      "condition": 85,
      "events": ["encounter_police", "hitchhiker"]
    }
  ],
  "event_pool": [...]
}
```

---

## MAP DATA

### Map Statistics v2.3
- **Total Distance:** 5705 km (was 1286 km in v2.2)
- **Average Segment:** 127 km (was 38 km)
- **Distance Range:** 100-200 km per segment
- **Nodes:** 28 (1 HOME_BASE, 2 CITY, 4 TOWN, 21 VILLAGE)
- **Roads:** 90 (all bidirectional)
- **Cross-connections:** 12 shortcuts

### Resource Distribution
- **Gas Stations:** 20/28 nodes (71%)
- **Stores:** 28/28 nodes (100%)
- **Garages:** 12/28 nodes (43%)
- **Motels:** 4/28 nodes (14%)
- **CAFE:** 13/28 nodes (46%)
- **Work Agency:** 2/28 nodes (7%)

### Fuel Consumption Formula
```cpp
float calculateFuelCost(float distance, RoadType roadType, int roadCondition) {
    const float BASE_CONSUMPTION = 2.5f; // L per 100km
    
    // Road type modifiers
    float roadModifier = 1.0f;
    switch(roadType) {
        case RoadType::HIGHWAY:    roadModifier = 0.8f; break;  // -20%
        case RoadType::ROAD:       roadModifier = 1.0f; break;  // base
        case RoadType::DIRT_ROAD:  roadModifier = 1.4f; break;  // +40%
    }
    
    // Condition modifier (worse condition = more fuel)
    float conditionModifier = 1.0f + ((100.0f - roadCondition) / 200.0f);
    
    return (distance / 100.0f) * BASE_CONSUMPTION * roadModifier * conditionModifier;
}
```

### Vehicle Tank Sizes
- **Mercedes Sprinter:** 75L (smallest, target vehicle)
- **Volvo 940:** 100L (balanced)
- **BMW E30:** 200L (largest)

**75L Tank Range Calculation:**
- Fuel per segment (avg): 127km × 2.5L/100km = 3.17L
- Segments on full tank: 75L / 3.17L = ~24 segments
- Gas stations appear every 1.4 segments on average (20 stations / 28 nodes)
- **Result:** Strategic planning required!

---

## IMPLEMENTATION TASKS

### Phase 1: Data Layer (2-3 days)

#### 1.1 WorldMap Class Enhancement
**File:** `include/WorldMap.h` + `src/WorldMap.cpp`

```cpp
class WorldMap {
private:
    std::string mapName;
    std::string version;
    std::vector<MapNode> nodes;
    std::vector<Road> roads;
    std::unordered_map<std::string, Event> eventPool;
    int currentNodeId;
    
public:
    // JSON Loading
    bool loadFromJSON(const std::string& filepath);
    bool validateMapData();
    
    // Node Operations
    MapNode* getNode(int id);
    std::vector<MapNode*> getConnectedNodes(int nodeId);
    std::vector<MapNode*> getAvailableNodes(int nodeId);
    bool unlockNode(int nodeId);
    void setCurrentNode(int nodeId);
    
    // Road Operations
    Road* getRoad(int fromId, int toId);
    std::vector<Road*> getRoadsFromNode(int nodeId);
    float calculateFuelCost(int fromId, int toId);
    
    // Event Operations
    Event* getRandomEvent(const std::string& eventId);
};
```

**Key Issues to Solve:**
- ❌ **No JSON parsing currently exists**
- ❌ **Node unlocking logic not implemented**
- ❌ **Bidirectional road handling needed**

**Solution:**
```cpp
bool WorldMap::loadFromJSON(const std::string& filepath) {
    // Use nlohmann/json library
    std::ifstream file(filepath);
    json j = json::parse(file);
    
    // Parse nodes
    for (auto& nodeJson : j["nodes"]) {
        MapNode node;
        node.id = nodeJson["id"];
        node.type = parseNodeType(nodeJson["type"]);
        node.name = nodeJson["name"];
        node.nameEn = nodeJson["name_en"];
        // ... parse rest
        nodes.push_back(node);
    }
    
    // Parse roads (create bidirectional entries)
    for (auto& roadJson : j["roads"]) {
        Road road;
        road.from = roadJson["from"];
        road.to = roadJson["to"];
        road.type = parseRoadType(roadJson["type"]);
        road.distance = roadJson["distance"];
        roads.push_back(road);
        
        // Add reverse road for bidirectional travel
        Road reverseRoad = road;
        reverseRoad.from = road.to;
        reverseRoad.to = road.from;
        roads.push_back(reverseRoad);
    }
    
    return validateMapData();
}
```

#### 1.2 MapNode Structure
**File:** `include/GameTypes.h`

```cpp
struct MapNode {
    int id;
    NodeType type;
    std::string name;
    std::string nameEn;
    sf::Vector2f position;
    std::string biome;
    std::string description;
    std::vector<Structure> structures;
    std::vector<int> unlockNodes;
    NodeStatus status;
    
    // Helper methods
    bool hasStructure(StructureType type) const;
    Structure* getStructure(StructureType type);
    bool isAvailable() const { return status == NodeStatus::AVAILABLE; }
};

struct Structure {
    StructureType type;
    std::string name;
    std::string nameEn;
    bool available;
};

struct Road {
    int from;
    int to;
    RoadType type;
    float distance;
    std::string name;
    std::string nameEn;
    int condition; // 0-100
    std::vector<std::string> events;
};
```

### Phase 2: WorldMapScene UI (3-4 days)

#### 2.1 Map Visualization
**File:** `src/MapScene.cpp`

**Current Issues:**
- ❌ **No spider web visualization**
- ❌ **No node status indicators**
- ❌ **No fuel cost preview**

**Required UI Elements:**
```cpp
class WorldMapScene : public Scene {
private:
    WorldMap* worldMap;
    sf::Sprite mapBackground;
    std::vector<NodeSprite> nodeSprites;
    std::vector<RoadLine> roadLines;
    UIPanel* infoPanel;
    UIPanel* fuelPanel;
    
    // Selected node
    int selectedNodeId;
    int hoveredNodeId;
    
public:
    void renderMap();
    void renderNodes();
    void renderRoads();
    void renderInfoPanel();
    void handleNodeClick(int nodeId);
    void handleNodeHover(int nodeId);
};
```

**UI Layout:**
```
┌──────────────────────────────────────────────────┐
│  [Map Background - 1366x768]                     │
│                                                   │
│    ●────────●────────●  Nodes (colored by status)│
│   / \      / \      /   LOCKED: Gray             │
│  ●   ●────●   ●────●    AVAILABLE: Green         │
│       \   /         \   CURRENT: Yellow          │
│        ● ●           ●  VISITED: Blue            │
│                                                   │
│  ┌────────────────────────────┐                  │
│  │ Nizhny Novgorod (CITY)     │ Info Panel       │
│  │ Distance: 594 km           │                  │
│  │ Fuel needed: 17.2L         │                  │
│  │ Structures:                │                  │
│  │  ⛽ Gas Station             │                  │
│  │  🏪 Store                   │                  │
│  │  ☕ Cafe                     │                  │
│  │ [Travel] [Cancel]          │                  │
│  └────────────────────────────┘                  │
└──────────────────────────────────────────────────┘
```

**Node Rendering:**
```cpp
void WorldMapScene::renderNodes() {
    for (auto& node : worldMap->getAllNodes()) {
        sf::CircleShape circle(10.f);
        circle.setPosition(node.position);
        
        // Color by status
        switch(node.status) {
            case NodeStatus::LOCKED:     circle.setFillColor(sf::Color(100,100,100)); break;
            case NodeStatus::VISIBLE:    circle.setFillColor(sf::Color(150,150,150)); break;
            case NodeStatus::AVAILABLE:  circle.setFillColor(sf::Color(0,255,0)); break;
            case NodeStatus::CURRENT:    circle.setFillColor(sf::Color(255,255,0)); break;
            case NodeStatus::VISITED:    circle.setFillColor(sf::Color(100,150,255)); break;
        }
        
        window->draw(circle);
        
        // Draw icon based on type
        if (node.type == NodeType::CITY) {
            // Draw city icon
        } else if (node.type == NodeType::VILLAGE) {
            // Draw village icon
        }
        
        // Draw name label
        sf::Text label(node.name, font, 12);
        label.setPosition(node.position.x + 15, node.position.y - 5);
        window->draw(label);
    }
}
```

#### 2.2 Travel Confirmation Dialog
**When player clicks AVAILABLE node:**

```
┌──────────────────────────────────────┐
│  Travel to Vladimir?                  │
│                                       │
│  Distance: 180 km                     │
│  Road Type: Regular Road              │
│  Condition: 70% (Fair)                │
│                                       │
│  Fuel Cost: 10.35 L                   │
│  Current Fuel: 65 L                   │
│  After Travel: 54.65 L ✅             │
│                                       │
│  Time: ~2 hours                       │
│                                       │
│  [Confirm Travel]  [Cancel]           │
└──────────────────────────────────────┘
```

**Implementation:**
```cpp
void WorldMapScene::showTravelConfirmation(int nodeId) {
    MapNode* node = worldMap->getNode(nodeId);
    Road* road = worldMap->getRoad(worldMap->getCurrentNodeId(), nodeId);
    
    float fuelCost = worldMap->calculateFuelCost(worldMap->getCurrentNodeId(), nodeId);
    float currentFuel = PlayerState::getInstance()->getFuel();
    float afterFuel = currentFuel - fuelCost;
    
    // Check if have enough fuel
    if (afterFuel < 0) {
        showMessage("Not enough fuel! Need " + std::to_string(fuelCost) + "L");
        return;
    }
    
    // Show dialog
    TravelDialog* dialog = new TravelDialog(node, road, fuelCost, afterFuel);
    dialog->onConfirm = [this, nodeId]() {
        startTravel(nodeId);
    };
    addChild(dialog);
}
```

### Phase 3: LocationScene Integration (2-3 days)

#### 3.1 CAFE Structure Implementation
**File:** `src/LocationScene.cpp`

**Current Issues:**
- ❌ **CAFE structure type not handled**
- ❌ **Bulletin board scene doesn't exist**

**Solution:**
```cpp
void LocationScene::handleStructureClick(Structure* structure) {
    switch(structure->type) {
        case StructureType::GAS_PUMP:
            showGasStationMenu();
            break;
        case StructureType::STORE:
            showStoreMenu();
            break;
        case StructureType::GARAGE:
            showGarageMenu();
            break;
        case StructureType::CAFE:
            showCafeMenu();  // NEW
            break;
        // ... other structures
    }
}

void LocationScene::showCafeMenu() {
    CafeMenu* menu = new CafeMenu();
    menu->addOption("Поесть (Eat)", [this]() { eatAtCafe(); });
    menu->addOption("Доска объявлений (Bulletin Board)", [this]() {
        SceneManager::getInstance()->pushScene(new BulletinBoardScene());
    });
    menu->addOption("Назад (Back)", [this]() { menu->close(); });
    addChild(menu);
}
```

#### 3.2 BulletinBoardScene
**New File:** `include/BulletinBoardScene.h` + `src/BulletinBoardScene.cpp`

```cpp
class BulletinBoardScene : public Scene {
private:
    std::vector<Quest> availableQuests;
    QuestManager* questManager;
    
public:
    void generateQuests();
    void renderQuestCards();
    void handleQuestAccept(Quest* quest);
};
```

**Quest Types:**
```cpp
enum class QuestType {
    DELIVERY,      // Deliver package to city X, reward 100-1000₽
    CAR_REPAINT,   // Repaint car color, drive to city X, reward 500-1500₽
    FIND_CAT,      // Find cat on road Y, bring to city X, reward 300-600₽
    FIND_ITEM      // Find lost item on road Y, return to X, reward 200-500₽
};

struct Quest {
    QuestType type;
    std::string title;
    std::string description;
    int targetNodeId;
    std::string targetRoad; // for cat/item quests
    int reward;
    int deadline; // in-game hours (TODO)
    
    // Quest-specific data
    std::string carColor;   // for repaint
    std::string itemName;   // for find item
};
```

**Quest Generation:**
```cpp
void BulletinBoardScene::generateQuests() {
    availableQuests.clear();
    
    // Generate 3-5 random quests
    int numQuests = 3 + rand() % 3;
    
    for (int i = 0; i < numQuests; i++) {
        Quest quest;
        
        // 40% delivery, 25% repaint, 20% cat, 15% item
        int roll = rand() % 100;
        if (roll < 40) {
            quest = generateDeliveryQuest();
        } else if (roll < 65) {
            quest = generateRepaintQuest();
        } else if (roll < 85) {
            quest = generateCatQuest();
        } else {
            quest = generateItemQuest();
        }
        
        availableQuests.push_back(quest);
    }
}

Quest BulletinBoardScene::generateDeliveryQuest() {
    Quest quest;
    quest.type = QuestType::DELIVERY;
    quest.title = "Доставка посылки / Deliver Package";
    
    // Random destination (must be different from current)
    int currentNode = worldMap->getCurrentNodeId();
    int targetNode = worldMap->getRandomNodeId(currentNode);
    quest.targetNodeId = targetNode;
    
    // Calculate distance for reward
    float distance = worldMap->getShortestDistance(currentNode, targetNode);
    quest.reward = 100 + (int)(distance * 0.5f); // ~0.5₽ per km
    
    MapNode* target = worldMap->getNode(targetNode);
    quest.description = "Доставьте посылку в " + target->name;
    
    return quest;
}
```

**UI Layout:**
```
┌──────────────────────────────────────────────────┐
│  Доска объявлений / Bulletin Board               │
├──────────────────────────────────────────────────┤
│                                                   │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────┐│
│  │ ДОСТАВКА     │  │ ПОКРАСКА     │  │ ПОИСК   ││
│  │              │  │              │  │         ││
│  │ Доставить    │  │ Перекрасить  │  │ Найти   ││
│  │ посылку в    │  │ машину в     │  │ кота на ││
│  │ Нижний       │  │ синий цвет,  │  │ дороге  ││
│  │ Новгород     │  │ приехать в   │  │ М7      ││
│  │              │  │ Владимир     │  │         ││
│  │ 594 км       │  │ 180 км       │  │ -       ││
│  │ ~17.2L топл. │  │ ~5.2L топл.  │  │         ││
│  │              │  │              │  │         ││
│  │ 💰 450₽      │  │ 💰 800₽      │  │ 💰 350₽ ││
│  │              │  │              │  │         ││
│  │ [Принять]    │  │ [Принять]    │  │[Принять]││
│  └──────────────┘  └──────────────┘  └─────────┘│
│                                                   │
│  Активных заданий: 1/3                            │
│                                                   │
│  [Назад / Back]                                   │
└──────────────────────────────────────────────────┘
```

### Phase 4: TravelCinematicScene Updates (1-2 days)

#### 4.1 Event Triggering
**File:** `src/TravelCinematicScene.cpp`

**Current Issues:**
- ❌ **Event pool from JSON not used**
- ❌ **Quest events not triggered**

**Solution:**
```cpp
void TravelCinematicScene::checkForEvents() {
    Road* road = worldMap->getRoad(fromNodeId, toNodeId);
    
    // Check for quest events (cat/item finding)
    for (Quest* quest : questManager->getActiveQuests()) {
        if (quest->targetRoad == road->name && !quest->completed) {
            if (quest->type == QuestType::FIND_CAT) {
                triggerCatEvent(quest);
                return;
            } else if (quest->type == QuestType::FIND_ITEM) {
                triggerItemEvent(quest);
                return;
            }
        }
    }
    
    // Random events from road's event pool
    if (!road->events.empty() && rand() % 100 < 30) { // 30% chance
        std::string eventId = road->events[rand() % road->events.size()];
        Event* event = worldMap->getEvent(eventId);
        triggerEvent(event);
    }
}
```

### Phase 5: Quest System (2-3 days)

#### 5.1 QuestManager Enhancement
**File:** `src/QuestManager.cpp`

```cpp
class QuestManager {
private:
    std::vector<Quest> activeQuests;
    std::vector<Quest> completedQuests;
    const int MAX_ACTIVE_QUESTS = 3;
    
public:
    bool acceptQuest(Quest quest);
    void checkQuestCompletion();
    void completeQuest(Quest* quest);
    std::vector<Quest*> getActiveQuests();
    
    // Quest-specific checks
    void onArriveAtNode(int nodeId);
    void onCarRepainted(std::string color);
    void onItemFound(std::string itemName);
};
```

**Quest Completion Logic:**
```cpp
void QuestManager::onArriveAtNode(int nodeId) {
    for (auto& quest : activeQuests) {
        if (quest.targetNodeId == nodeId && !quest.completed) {
            switch(quest.type) {
                case QuestType::DELIVERY:
                    completeQuest(&quest);
                    break;
                case QuestType::CAR_REPAINT:
                    if (PlayerState::getInstance()->getCarColor() == quest.carColor) {
                        completeQuest(&quest);
                    }
                    break;
                case QuestType::FIND_CAT:
                case QuestType::FIND_ITEM:
                    if (quest.itemCollected) {
                        completeQuest(&quest);
                    }
                    break;
            }
        }
    }
}

void QuestManager::completeQuest(Quest* quest) {
    quest->completed = true;
    PlayerState::getInstance()->addMoney(quest->reward);
    
    // Show notification
    NotificationManager::show("Quest Complete! +" + std::to_string(quest->reward) + "₽");
    
    // Move to completed
    completedQuests.push_back(*quest);
    activeQuests.erase(std::remove_if(activeQuests.begin(), activeQuests.end(),
        [quest](Quest& q) { return q.title == quest->title; }), activeQuests.end());
}
```

---

## CODE INTEGRATION POINTS

### Existing Files to Modify

#### 1. SceneManager.cpp
**Add new scene type:**
```cpp
enum class SceneType {
    MAIN_MENU,
    WORLD_MAP,      // MapScene - NEEDS UPDATE
    LOCATION,       // LocationScene - NEEDS CAFE HANDLING
    TRAVEL,         // TravelCinematicScene - NEEDS EVENT UPDATES
    BULLETIN_BOARD, // NEW - Create this
    // ... existing scenes
};
```

#### 2. PlayerState.h
**Add quest tracking:**
```cpp
class PlayerState {
private:
    // ... existing fields
    std::vector<int> activeQuestIds;
    std::string carColor;
    
public:
    // ... existing methods
    void setCarColor(std::string color) { carColor = color; }
    std::string getCarColor() const { return carColor; }
};
```

#### 3. GameTypes.h
**Add all new enums and structs** (see Data Structures section above)

#### 4. CMakeLists.txt
**Add new source files:**
```cmake
add_executable(game
    # ... existing files
    src/BulletinBoardScene.cpp
    src/WorldMap.cpp
    # ... etc
)
```

---

## ISSUES AND SOLUTIONS

### Issue 1: No JSON Parsing Library
**Problem:** C++ doesn't have built-in JSON parsing  
**Solution:** Add nlohmann/json single-header library

```bash
# Download to project
curl -o include/json.hpp https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
```

```cpp
// In WorldMap.cpp
#include "json.hpp"
using json = nlohmann::json;
```

### Issue 2: Bidirectional Roads Not Explicit
**Problem:** JSON has 44 roads but need 90 for bidirectional  
**Solution:** Create reverse roads when loading

```cpp
// When loading roads
for (auto& roadJson : j["roads"]) {
    Road road = parseRoad(roadJson);
    roads.push_back(road);
    
    // Create reverse
    Road reverse = road;
    reverse.from = road.to;
    reverse.to = road.from;
    roads.push_back(reverse);
}
```

### Issue 3: Node Unlocking Logic Missing
**Problem:** No code to handle progressive unlocking  
**Solution:** Implement in WorldMap

```cpp
void WorldMap::unlockConnectedNodes(int nodeId) {
    MapNode* node = getNode(nodeId);
    for (int unlockId : node->unlockNodes) {
        MapNode* unlockNode = getNode(unlockId);
        if (unlockNode->status == NodeStatus::LOCKED) {
            unlockNode->status = NodeStatus::VISIBLE;
        }
    }
    
    // Set connected nodes to AVAILABLE
    for (Road* road : getRoadsFromNode(nodeId)) {
        MapNode* connectedNode = getNode(road->to);
        if (connectedNode->status == NodeStatus::VISIBLE) {
            connectedNode->status = NodeStatus::AVAILABLE;
        }
    }
}
```

### Issue 4: No CAFE/Bulletin Board Scenes
**Problem:** Completely new scenes need creation  
**Solution:** Follow existing scene pattern (see Phase 3)

### Issue 5: Quest Deadline System Not Implemented
**Problem:** Spec mentions deadlines but not implemented  
**Status:** Marked as TODO - implement time system first

### Issue 6: Fuel Warning Not Present
**Problem:** Player might run out of fuel  
**Solution:** Add warnings in UI

```cpp
void WorldMapScene::renderFuelWarning() {
    float currentFuel = PlayerState::getInstance()->getFuel();
    float nearestGasDistance = worldMap->getNearestGasStationDistance();
    float fuelNeeded = calculateFuelCost(nearestGasDistance);
    
    if (currentFuel < fuelNeeded * 1.2f) { // 20% buffer
        // Show warning
        sf::Text warning("⚠️ Fuel Low!", font, 24);
        warning.setFillColor(sf::Color::Red);
        warning.setPosition(10, 10);
        window->draw(warning);
    }
}
```

### Issue 7: Event Pool Not Loaded
**Problem:** Event pool in JSON but not used  
**Solution:**

```cpp
void WorldMap::loadEvents(json& j) {
    for (auto& eventJson : j["event_pool"]) {
        Event event;
        event.id = eventJson["id"];
        event.title = eventJson["title"];
        event.description = eventJson["description"];
        event.type = parseEventType(eventJson["type"]);
        
        for (auto& choiceJson : eventJson["choices"]) {
            EventChoice choice;
            choice.text = choiceJson["text"];
            choice.fuelCost = choiceJson.value("fuel_cost", 0);
            choice.moneyCost = choiceJson.value("money_cost", 0);
            choice.moneyReward = choiceJson.value("money_reward", 0);
            event.choices.push_back(choice);
        }
        
        eventPool[event.id] = event;
    }
}
```

### Issue 8: Resource Scarcity Not Enforced
**Problem:** Can access any structure  
**Solution:** Check availability flag

```cpp
void LocationScene::handleStructureClick(Structure* structure) {
    if (!structure->available) {
        showMessage("Закрыто / Closed");
        return;
    }
    // ... normal handling
}
```

### Issue 9: Performance with 90 Roads
**Problem:** Many roads to render  
**Solution:** Spatial culling

```cpp
void WorldMapScene::renderRoads() {
    sf::FloatRect viewport = getViewportBounds();
    
    for (Road& road : worldMap->getRoads()) {
        MapNode* from = worldMap->getNode(road.from);
        MapNode* to = worldMap->getNode(road.to);
        
        // Skip if both nodes outside viewport
        if (!viewport.contains(from->position) && !viewport.contains(to->position)) {
            continue;
        }
        
        // Draw road line
        sf::Vertex line[] = {
            sf::Vertex(from->position),
            sf::Vertex(to->position)
        };
        window->draw(line, 2, sf::Lines);
    }
}
```

### Issue 10: Missing Localization System
**Problem:** Russian + English names everywhere  
**Solution:** Simple language toggle

```cpp
class Localization {
private:
    static std::string currentLanguage; // "ru" or "en"
    
public:
    static std::string get(std::string ruText, std::string enText) {
        return (currentLanguage == "ru") ? ruText : enText;
    }
    
    static void setLanguage(std::string lang) { currentLanguage = lang; }
};

// Usage
std::string name = Localization::get(node->name, node->nameEn);
```

---

## IMPLEMENTATION CHECKLIST

### Data Layer
- [ ] Add nlohmann/json library
- [ ] Implement WorldMap::loadFromJSON()
- [ ] Add MapNode/Road/Structure structs to GameTypes.h
- [ ] Implement fuel calculation function
- [ ] Implement node unlocking logic
- [ ] Add bidirectional road creation
- [ ] Load event pool from JSON

### WorldMapScene
- [ ] Update map rendering for 28 nodes
- [ ] Add spider web road visualization
- [ ] Implement node status coloring
- [ ] Add hover tooltips for nodes
- [ ] Add fuel cost preview in tooltip
- [ ] Create travel confirmation dialog
- [ ] Add fuel warning system

### LocationScene
- [ ] Add CAFE structure handling
- [ ] Create cafe menu UI
- [ ] Add "Bulletin Board" menu option

### BulletinBoardScene (NEW)
- [ ] Create BulletinBoardScene.h/.cpp
- [ ] Implement quest generation algorithms
- [ ] Create quest card UI
- [ ] Add quest accept/decline functionality
- [ ] Integrate with QuestManager

### QuestManager
- [ ] Add quest tracking (max 3 active)
- [ ] Implement quest completion detection
- [ ] Add quest reward distribution
- [ ] Implement quest-specific events (cat/item finding)
- [ ] Add quest UI indicators

### TravelCinematicScene
- [ ] Update event triggering to use event pool
- [ ] Add quest event checking (cat/item)
- [ ] Implement event completion callbacks

### PlayerState
- [ ] Add carColor field and methods
- [ ] Add quest ID tracking
- [ ] Add quest item inventory

### Testing
- [ ] Test map loading from JSON
- [ ] Test bidirectional travel
- [ ] Test fuel calculations
- [ ] Test resource scarcity enforcement
- [ ] Test all 4 quest types
- [ ] Test quest completion rewards
- [ ] Test event triggering

---

## ESTIMATED TIMELINE

| Phase | Tasks | Duration |
|-------|-------|----------|
| Phase 1: Data Layer | WorldMap class, JSON parsing, fuel calc | 2-3 days |
| Phase 2: WorldMapScene UI | Map rendering, tooltips, dialogs | 3-4 days |
| Phase 3: LocationScene | CAFE integration, bulletin board | 2-3 days |
| Phase 4: TravelScene | Event updates, quest events | 1-2 days |
| Phase 5: Quest System | Quest generation, tracking, completion | 2-3 days |
| Phase 6: Testing & Polish | Bug fixes, balancing, optimization | 2-3 days |
| **TOTAL** | | **12-18 days** |

---

## PRIORITY ORDER

1. **HIGH PRIORITY** (Core Functionality)
   - WorldMap JSON loading ⚠️
   - Fuel calculation system ⚠️
   - Map visualization ⚠️
   - Travel confirmation ⚠️

2. **MEDIUM PRIORITY** (Key Features)
   - CAFE structure handling
   - Bulletin board scene
   - Quest generation
   - Quest completion

3. **LOW PRIORITY** (Polish)
   - Quest deadlines (requires time system)
   - Advanced event system
   - Achievements
   - Statistics tracking

---

## NOTES

- All 28 nodes and 90 roads are defined in `data/maps/russia_90s_map.json`
- Resource scarcity is already in data (8 villages lack gas)
- Fuel calculations are balanced for 75L Mercedes Sprinter
- Quest system is fully specified but needs implementation
- No breaking changes to existing code required
- All new code is additive

---

**END OF IMPLEMENTATION MASTER DOCUMENT**

*Use this as single source of truth. All other spec documents archived.*
