# Keep Driving Map Files

This directory contains map data files for the Road Trip Adventure RPG game following Keep Driving principles.

## Available Maps

### 1. `russia_90s_map.json` - Main Game Map
**Полная карта путешествия / Full Journey Map**

- **Nodes:** 28 locations (Moscow to Kazan) - много маленьких населённых пунктов
- **Roads:** 90 connections (bidirectional, web-like structure) ✨ **NEW v2.1**
- **Distance:** ~1286 km total (shorter distances between settlements)
- **Theme:** Russia 90s atmosphere with resource scarcity
- **Difficulty:** Medium-Hard (resource management + route planning)
- **Playtime:** 10-14 hours
- **Structure:** **Spider web / Паутина** - свободный выбор маршрута! 🕸️

**Route:** Свободный выбор! Игрок сам выбирает путь через Россию / Free choice! Player chooses their own path through Russia

**Features:**
- 3 road types (Highway, Road, Dirt Road)
- 5 node types (Home Base, City, Town, Village - many small settlements)
- **🕸️ Web Structure (v2.1):** Bidirectional roads + cross-connections
  - **90 roads** (was 33) - multiple routes everywhere
  - **Bidirectional travel** - can go back to previous locations
  - **12 cross-connections** - shortcuts and alternative paths
  - **Free exploration** - choose your own route!
  - **Hub nodes** - major intersections with 4-5 route options
- **Resource Scarcity:** Not all villages have all services!
  - Some villages: only store (no gas station)
  - Some villages: only gas station and store (no service)
  - Some villages: service unavailable or closed
- Player must plan ahead - visible structures before traveling
- Shorter distances (25-50 km between stops)
- Multiple path options through rural Russia
- Event pools for each road
- Biome diversity (urban, rural, forest, industrial)

### 2. `test_map.json` - Development Test Map
**Тестовая карта / Test Map**

- **Nodes:** 3 locations (linear progression)
- **Roads:** 2 connections
- **Distance:** 130 km total
- **Theme:** Testing
- **Difficulty:** Easy
- **Playtime:** 15-20 minutes

**Purpose:** Quick testing and development of map systems

## Map Format Specification

See `KEEP_DRIVING_MAP_SPECIFICATION.md` in the root directory for complete format documentation.

### Quick Reference

#### Node Types
- `HOME_BASE` - Starting location (procedurally generated with 3-4 structures)
- `CITY` - Large city (5+ structures, all services)
- `TOWN` - Town (3-4 structures, basic services)
- `VILLAGE` - Small village (1-2 structures, **scarce resources** - may lack gas station or service!)
- `JUNCTION` - Road junction (2-3 structures, minimal services)
- `GAS_STATION` - Gas station (1-2 structures, fuel and food only)
- `UNIQUE` - Unique location (special places with quests)

#### Road Types & Fuel Modifiers
- `HIGHWAY` - -20% fuel consumption, +20% speed (good federal roads)
- `ROAD` - Base fuel consumption, base speed (regular roads)
- `DIRT_ROAD` - +40% fuel consumption, -20% speed (poor condition)

#### Structure Types
- `GAS_PUMP` - Gas station / Заправка
- `STORE` - General store / Магазин
- `GARAGE` - Auto repair / Гараж/Автосервис
- `MOTEL` - Rest and sleep / Мотель
- `CAFE` - Cafe with bulletin board / Кафе с доской объявлений ⭐ **NEW v2.2**
  - **Scene:** Bulletin board (доска объявлений) for profit-oriented quests
  - **Quest Types:**
    - Delivery/help tasks (доставка/помощь)
    - Car repainting and delivery (перекраска машины и приезд в город)
    - Find and deliver cat (найти кота и привезти в нужный город)
    - Find lost item (поиск пропавшей вещи)
  - **Frequency:** ~46% of nodes (similar to GARAGE, more common than WORK_AGENCY)
- `WORK_AGENCY` - Job opportunities / Биржа труда (~7% of nodes)
- `QUEST_BOARD` - Quest board / Доска квестов (only in major cities)
- `UNIQUE_LOCATION` - Special location / Уникальная локация

## Fuel Consumption Calculation

```
totalFuel = distance × baseConsumption × roadModifier × conditionModifier

where:
  baseConsumption = 0.5 L/km (depends on car)
  roadModifier = 0.8 (HIGHWAY), 1.0 (ROAD), 1.4 (DIRT_ROAD)
  conditionModifier = 1.0 + ((100 - roadCondition) / 200)
```

### Example Calculations

**Highway (85% condition, 80 km):**
- `80 × 0.5 × 0.8 × 1.075 = 34.4 L`

**Regular Road (70% condition, 180 km):**
- `180 × 0.5 × 1.0 × 1.15 = 103.5 L`

**Dirt Road (40% condition, 45 km):**
- `45 × 0.5 × 1.4 × 1.3 = 40.95 L`

## Resource Scarcity Mechanic (v2.0)

**Ключевая особенность / Key Feature:** Not all villages have all services!

### Deficit Strategy / Стратегия дефицита

The new map (v2.0+) implements **resource scarcity** as a core gameplay mechanic:

1. **Villages (VILLAGE type)** - Small settlements with 1-2 structures:
   - Some have: Gas station + Store ✅
   - Some have: Store only (NO gas station!) ⚠️
   - Some have: Gas station + Store + Service (rare) ✅
   - Some have: Service unavailable (`"available": false`) 🔒

2. **Player Must Plan Ahead:**
   - Before traveling, player can see destination structures in UI
   - Must calculate if they can reach next gas station
   - May need to take longer route to find service
   - Creates tension and strategic decisions

3. **Shorter Distances (25-50 km):**
   - More frequent stops possible
   - But not all stops have what you need!
   - Creates "hop between villages" gameplay

4. **Examples from the map:**
   - Node #5 (Заречный): Only store, no gas! Must plan fuel carefully
   - Node #8 (Сосновый): Only store, no gas or service
   - Node #9 (Покровское): Service closed (`available: false`)
   - Node #17 (Павлово): Only store, isolated location

### Strategic Implications

- **Fuel Management Critical:** Can't always refuel at next stop
- **Route Planning:** Sometimes longer route is safer (has gas stations)
- **Risk vs Reward:** Shorter route might lack services
- **Resource Conservation:** Don't waste fuel on detours
- **Emergency Planning:** Always keep reserve fuel

## Web Structure / Паутина (v2.1) 🕸️

**Новая возможность / New Feature:** Free route choice with bidirectional travel!

### Spider Web Map Design

Version 2.1 introduces **web-like structure** for true exploration gameplay:

1. **Bidirectional Roads (90 total):**
   - All roads can be traveled in both directions
   - Player can return to previous locations
   - No more one-way progression

2. **Cross-Connections (12 shortcuts):**
   - Alternative paths between nearby villages
   - Shortcuts through forests and fields
   - Bypasses for major towns
   - Examples:
     - Павлово ↔ Лысково (32 km shortcut)
     - Сосновый ↔ Покровское (35 km forest path)
     - Владимир ↔ Речной (38 km bypass)
     - Балахна ↔ Кстово (42 km industrial road)

3. **Hub Nodes (Major Intersections):**
   - **Покровское:** 5 connections (central hub)
   - **Нижний Новгород:** 5 connections (major city)
   - **Ольховка, Речной, Лысково, Ковров:** 4 connections each
   - **Чебоксары:** 4 connections (regional hub)

4. **Free Exploration:**
   - Choose your own route through Russia
   - Explore different paths
   - Return to favorite locations
   - Find your optimal route considering resources

### Strategic Route Planning

**Multiple Path Options:**
- From Moscow to Nizhny Novgorod: 10+ different routes possible!
- Each route has different:
  - Distance
  - Gas station availability
  - Road conditions
  - Events

**Example Routes from Moscow to Nizhny Novgorod:**

**Route A (Northern, safer):**
Moscow → Petrovo → Beryozovka → Aleksandrov → Sosnovy → Pokrovskoye → Olkhovka → ... → Nizhny
- More gas stations
- Better road conditions
- Longer distance

**Route B (Direct, risky):**
Moscow → Petrovo → Dorozhny → Zarechny → Krasnoye → Vladimir → ... → Nizhny
- Shorter distance
- Fewer gas stations ⚠️
- Some villages without services

**Route C (Southern, scenic):**
Moscow → Petrovo → Dorozhny → Zarechny → (swamp road) → Krasnoye → Pokrovskoye → ... → Nizhny
- Scenic but challenging
- Dirt road section (high fuel consumption)
- Alternative services

### Web Connectivity Statistics

```
Connection Levels:
  1 connection:  1 node  (endpoints)
  2 connections: 4 nodes (linear segments)
  3 connections: 15 nodes (branching points)
  4 connections: 6 nodes (hubs)
  5 connections: 2 nodes (major hubs)

Average connections per node: 3.2
Web density: High (allows free exploration)
All nodes reachable: ✅ Yes
Bidirectional: ✅ All roads
```

## Creating Custom Maps

1. Copy `test_map.json` as a template
2. Modify nodes, roads, and events
3. Ensure all `unlockNodes` point to existing node IDs
4. Ensure all roads reference existing nodes
5. Add events to event_pool
6. Update metadata
7. **For scarcity:** Mix villages with different service availability
8. Test in game

### Validation Checklist
- [ ] All node IDs are unique integers
- [ ] All `unlockNodes` reference existing nodes
- [ ] All roads have valid `from` and `to` node IDs
- [ ] Starting node (id: 1) exists and is HOME_BASE type
- [ ] Position coordinates are within map bounds (0-1366, 0-768)
- [ ] All event references in roads exist in event_pool
- [ ] JSON is valid (use JSON validator)
- [ ] **Resource scarcity:** Not all villages have gas stations ⚠️
- [ ] **Service availability:** Some services marked as unavailable

## Tips for Map Design

1. **Progressive Unlocking:** Design nodes to unlock gradually as player progresses
2. **Multiple Paths:** Provide choice between different route options
3. **Risk vs Reward:** Dirt roads are risky but may lead to unique locations
4. **Resource Balance:** Place gas stations strategically for fuel management
5. **Scarcity Pattern:** 60-70% of villages should have gas stations, not all
6. **Service Availability:** Some services should be closed/unavailable (`available: false`)
7. **Shorter Distances:** Keep roads 25-50 km for villages, 35-50 km for longer routes
5. **Event Diversity:** Assign different events to different roads for variety
6. **Distance Realism:** Consider realistic distances between locations
7. **Road Quality:** Match road condition to terrain and location importance

## Integration with Game

Maps are loaded by `WorldMap::loadFromJSON(filename)` and used by:
- `WorldMapScene` - Display and navigation
- `LocationScene` - Structure availability
- `TravelCinematicScene` - Events during travel
- `EventManager` - Event triggering

See `KEEP_DRIVING_MAP_SPECIFICATION.md` for technical implementation details.

---

**Last Updated:** 2025-11-16  
**Version:** 1.0  
**Format Version:** 1.0
