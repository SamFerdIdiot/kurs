# CAFE Structure & Bulletin Board Scene Specification
## Кафе и доска объявлений / Cafe and Bulletin Board

**Version:** 2.2  
**Date:** 2025-11-17  
**Feature:** CAFE structure with bulletin board scene for profit-oriented quests

---

## 🎯 Overview / Обзор

The CAFE structure introduces a new gameplay mechanic: bulletin board quests focused on earning profit. Unlike WORK_AGENCY (which provides employment), CAFE offers one-time profit opportunities through various delivery and task-based quests.

Структура CAFE вводит новую игровую механику: доску объявлений с заданиями для получения прибыли. В отличие от WORK_AGENCY (которая предлагает работу), CAFE предлагает разовые возможности заработка через различные задания по доставке и выполнению задач.

---

## 📊 Distribution / Распределение

**Frequency on Map:**
- **CAFE:** 13 nodes (46.4% of all nodes)
- **GARAGE:** 12 nodes (42.9%) - similar rarity
- **WORK_AGENCY:** 2 nodes (7.1%) - CAFE is more common

**✅ Requirements Met:**
- CAFE appears more frequently than WORK_AGENCY ✓
- CAFE has similar frequency to GARAGE ✓
- CAFE is available in most major locations ✓

**Location Types:**
- All CITY nodes (Moscow, Nizhny Novgorod, Kazan)
- All TOWN nodes (Aleksandrov, Vladimir, Gorokhovets, Cheboksary)
- Selected VILLAGE nodes (strategic locations with multiple services)
- HOME_BASE (starting location)

---

## 🏗️ Structure Definition

### JSON Format

```json
{
  "type": "CAFE",
  "name": "Придорожное кафе",
  "name_en": "Roadside Cafe",
  "available": true
}
```

### Naming Conventions

Different types of nodes have different CAFE names:

| Node Type | Russian Name | English Name | Example |
|-----------|--------------|--------------|---------|
| CITY | Кафе '[Город]' | Cafe '[City]' | Кафе 'Казань' |
| TOWN | Придорожное кафе | Roadside Cafe | Придорожное кафе |
| VILLAGE | Столовая | Canteen | Столовая |
| HOME_BASE | Кафе 'У дороги' | Cafe 'By the Road' | Кафе 'У дороги' |

---

## 🎮 Bulletin Board Scene / Сцена доски объявлений

### Scene Flow

```
Player enters CAFE
    ↓
Main Cafe Menu
  ├─ Buy Food/Drinks
  ├─ Rest
  └─ [NEW] View Bulletin Board ⭐
       ↓
Bulletin Board Scene
  ├─ Available Quests (3-5 displayed)
  ├─ Active Quests (player's current tasks)
  ├─ Completed Quests (history)
  └─ Accept/Decline Quest
       ↓
Quest Details Dialog
  ├─ Task Description
  ├─ Rewards (money, reputation)
  ├─ Requirements (car color, cargo space, etc.)
  ├─ Deadline/Distance
  └─ Accept Button
```

### UI Layout

```
┌─────────────────────────────────────────────────────┐
│ ДОСКА ОБЪЯВЛЕНИЙ / BULLETIN BOARD                   │
├─────────────────────────────────────────────────────┤
│                                                     │
│ ┌────────────────────┐  ┌────────────────────┐    │
│ │ ДОСТАВКА           │  │ ПОКРАСКА АВТО      │    │
│ │ Delivery           │  │ Car Repainting     │    │
│ │                    │  │                    │    │
│ │ Доставить посылку  │  │ Покрасить в синий  │    │
│ │ в Владимир         │  │ цвет и приехать    │    │
│ │                    │  │ в Казань           │    │
│ │ Награда: 500₽     │  │ Награда: 800₽     │    │
│ │ [ПРИНЯТЬ]          │  │ [ПРИНЯТЬ]          │    │
│ └────────────────────┘  └────────────────────┘    │
│                                                     │
│ ┌────────────────────┐  ┌────────────────────┐    │
│ │ НАЙТИ КОТА         │  │ УТЕРЯННАЯ ВЕЩЬ     │    │
│ │ Find Cat           │  │ Lost Item          │    │
│ │                    │  │                    │    │
│ │ Найти кота Мурзика │  │ Найти чемодан      │    │
│ │ и доставить в      │  │ на дороге M7       │    │
│ │ Н.Новгород        │  │                    │    │
│ │ Награда: 400₽     │  │ Награда: 300₽     │    │
│ │ [ПРИНЯТЬ]          │  │ [ПРИНЯТЬ]          │    │
│ └────────────────────┘  └────────────────────┘    │
│                                                     │
│              [НАЗАД / BACK]                         │
└─────────────────────────────────────────────────────┘
```

---

## 📋 Quest Types / Типы заданий

### 1. Delivery Quests / Задания на доставку

**Description:** Deliver item/package to specific city  
**Описание:** Доставить предмет/посылку в указанный город

**Parameters:**
- `destination_city` - Target city ID
- `item_name` - What to deliver
- `reward` - Money reward (100-1000₽)
- `time_limit` - Optional deadline (in-game days)
- `cargo_space_required` - 1-3 units

**Example:**
```json
{
  "quest_type": "DELIVERY",
  "title": "Доставка посылки",
  "title_en": "Package Delivery",
  "description": "Доставить посылку в Владимир",
  "destination_node": 10,
  "destination_name": "Владимир",
  "item": "Посылка с документами",
  "reward_money": 500,
  "time_limit_days": 3,
  "cargo_space": 1
}
```

**Gameplay:**
- Player accepts quest → item added to cargo
- Player drives to destination
- Upon arrival, quest completes automatically
- Reward granted immediately

---

### 2. Car Repainting Quests / Задания на перекраску

**Description:** Repaint car to specific color and drive to city  
**Описание:** Перекрасить машину в нужный цвет и приехать в город

**Parameters:**
- `required_color` - Target car color (RGB or preset)
- `destination_city` - Where to arrive
- `reward` - Money reward (500-1500₽)
- `paint_cost` - Cost of repainting (200-400₽)

**Example:**
```json
{
  "quest_type": "REPAINT",
  "title": "Синяя машина",
  "title_en": "Blue Car",
  "description": "Покрасить авто в синий цвет и приехать в Казань",
  "required_color": "blue",
  "destination_node": 28,
  "destination_name": "Казань",
  "reward_money": 800,
  "paint_cost": 300
}
```

**Gameplay:**
- Player accepts quest
- Player visits GARAGE to repaint car (pays paint_cost)
- Player drives to destination with correct color
- Quest completes upon arrival
- Net profit: reward - paint_cost

---

### 3. Find Cat Quests / Задания на поиск кота

**Description:** Find lost cat and deliver to city  
**Описание:** Найти потерянного кота и доставить в город

**Parameters:**
- `cat_name` - Cat's name
- `search_area` - Road or node where cat is located
- `destination_city` - Owner's city
- `reward` - Money reward (300-600₽)

**Example:**
```json
{
  "quest_type": "FIND_CAT",
  "title": "Потерявшийся кот",
  "title_en": "Lost Cat",
  "description": "Найти кота Мурзика и доставить в Нижний Новгород",
  "cat_name": "Мурзик",
  "search_road_id": 15,
  "search_description": "Видели на дороге между Владимиром и Гороховцем",
  "destination_node": 20,
  "destination_name": "Нижний Новгород",
  "reward_money": 400
}
```

**Gameplay:**
- Player accepts quest
- When driving on specified road → random event triggers
- Event: "You see a cat by the roadside" → [Stop and catch] / [Continue]
- If caught: cat added to cargo (takes 1 space)
- Drive to destination city
- Quest completes, reward granted

---

### 4. Find Lost Item Quests / Задания на поиск вещи

**Description:** Find lost item on road/location  
**Описание:** Найти утерянную вещь на дороге/локации

**Parameters:**
- `item_name` - What to find
- `search_location` - Road or node ID
- `reward` - Money reward (200-500₽)
- `return_to_cafe` - Return to same cafe or specific city

**Example:**
```json
{
  "quest_type": "FIND_ITEM",
  "title": "Потерянный чемодан",
  "title_en": "Lost Suitcase",
  "description": "Найти чемодан на трассе М7",
  "item_name": "Чемодан",
  "search_road_id": 9,
  "search_description": "Потерян на трассе М7 около Красного",
  "return_to_node": 20,
  "return_location": "Нижний Новгород",
  "reward_money": 300
}
```

**Gameplay:**
- Player accepts quest
- When driving on specified road → random event
- Event: "You notice something by the road" → [Stop and check] / [Continue]
- If checked: item found and added to cargo
- Return to specified location
- Quest completes, reward granted

---

## 💰 Reward System

### Money Rewards by Quest Type

| Quest Type | Min Reward | Max Reward | Average | Effort |
|------------|-----------|-----------|---------|--------|
| Delivery | 100₽ | 1000₽ | 500₽ | Low |
| Repaint | 500₽ | 1500₽ | 800₽ | Medium |
| Find Cat | 300₽ | 600₽ | 400₽ | Medium |
| Find Item | 200₽ | 500₽ | 300₽ | Low |

### Difficulty Modifiers

**Distance Multiplier:**
- Short (<100km): 1.0x base reward
- Medium (100-300km): 1.5x base reward
- Long (>300km): 2.0x base reward

**Time Pressure:**
- No deadline: base reward
- Tight deadline (1-2 days): +50% reward
- Very tight (<1 day): +100% reward

---

## 🎲 Quest Generation

### Quest Pool System

Each CAFE has a rotating pool of 3-5 quests:
- Quests refresh every in-game day
- Player can have up to 3 active quests simultaneously
- Failed quests (expired deadline) go to "Failed" list
- Completed quests go to "Completed" list for reputation

### Generation Algorithm

```python
def generate_cafe_quests(cafe_node_id, player_level, current_date):
    quests = []
    
    # 40% chance for delivery quest
    if random.random() < 0.4:
        quests.append(generate_delivery_quest(cafe_node_id))
    
    # 25% chance for repaint quest
    if random.random() < 0.25:
        quests.append(generate_repaint_quest(cafe_node_id))
    
    # 20% chance for find cat quest
    if random.random() < 0.20:
        quests.append(generate_cat_quest(cafe_node_id))
    
    # 15% chance for find item quest
    if random.random() < 0.15:
        quests.append(generate_item_quest(cafe_node_id))
    
    # Ensure at least 2 quests available
    while len(quests) < 2:
        quest_type = random.choice(['DELIVERY', 'FIND_ITEM'])
        quests.append(generate_quest(quest_type, cafe_node_id))
    
    return quests
```

---

## 🔧 Implementation Plan

### Phase 1: Data Structures (1 day)

```cpp
enum class QuestType {
    DELIVERY,
    REPAINT,
    FIND_CAT,
    FIND_ITEM
};

enum class QuestStatus {
    AVAILABLE,    // On bulletin board
    ACTIVE,       // Player accepted
    COMPLETED,    // Successfully finished
    FAILED,       // Deadline expired or cancelled
    EXPIRED       // No longer available
};

struct BulletinQuest {
    int id;
    QuestType type;
    QuestStatus status;
    std::string title;
    std::string titleEn;
    std::string description;
    
    // Common fields
    int rewardMoney;
    int destinationNodeId;
    std::string destinationName;
    
    // Type-specific fields
    std::string itemName;           // DELIVERY, FIND_ITEM, FIND_CAT
    std::string requiredColor;      // REPAINT
    int paintCost;                  // REPAINT
    int searchRoadId;               // FIND_CAT, FIND_ITEM
    std::string searchDescription;  // FIND_CAT, FIND_ITEM
    
    // Optional
    int timeLimitDays;
    int cargoSpaceRequired;
    sf::Time expirationTime;
};
```

### Phase 2: Bulletin Board Scene (2-3 days)

- Create BulletinBoardScene class
- UI rendering (quest cards grid)
- Quest selection and details dialog
- Accept/decline functionality
- Integration with CafeScene

### Phase 3: Quest Logic (2-3 days)

- Quest generation algorithm
- Quest tracking system
- Completion detection
- Road event triggers for find quests
- Reward distribution

### Phase 4: Integration (1-2 days)

- Connect to CAFE structure interaction
- Save/load quest state
- Player cargo management
- Car color tracking
- Statistics and reputation

---

## 📈 Statistics & Balancing

### Expected Player Behavior

**Quest Acceptance Rate:** 60-80% of displayed quests  
**Completion Rate:** 70-85% of accepted quests  
**Average Active Quests:** 2-3 simultaneously  
**Quest Revenue per Hour:** 200-400₽  

### Balancing vs Other Income

| Income Source | ₽ per Hour | Effort | Availability |
|---------------|-----------|--------|--------------|
| CAFE Quests | 200-400₽ | Medium | 46% of nodes |
| WORK_AGENCY | 300-600₽ | High | 7% of nodes |
| Trading | 100-300₽ | Low | Always |
| Events | 50-200₽ | Variable | Random |

**Balance Goal:** CAFE quests provide steady supplementary income without being overpowered.

---

## ✅ Implementation Checklist

### Data Layer
- [x] Add CAFE structure type to JSON
- [x] Distribute CAFE to 13 nodes (~46%)
- [x] Document structure in README
- [ ] Create quest data templates
- [ ] Add quest pool to game data

### Scene Layer
- [ ] Create BulletinBoardScene class
- [ ] Design UI layout and assets
- [ ] Implement quest card rendering
- [ ] Add quest details dialog
- [ ] Connect to CafeScene menu

### Logic Layer
- [ ] Implement quest generation
- [ ] Add quest tracking to PlayerState
- [ ] Create quest completion detection
- [ ] Implement road event triggers
- [ ] Add reward distribution system

### Integration
- [ ] Update CAFE interaction handler
- [ ] Add cargo space management
- [ ] Implement car color tracking
- [ ] Create save/load for quest state
- [ ] Add quest statistics to UI

### Testing
- [ ] Test quest generation variety
- [ ] Verify completion detection
- [ ] Balance reward amounts
- [ ] Test edge cases (full cargo, wrong color)
- [ ] Playtest for fun factor

---

## 🎯 Success Criteria

✅ CAFE structures appear in ~46% of nodes  
✅ CAFE is more common than WORK_AGENCY  
✅ CAFE is similar frequency to GARAGE  
✅ Bulletin board scene implemented and functional  
✅ All 4 quest types working correctly  
✅ Quest rewards balanced with other income sources  
✅ Player can have 3 active quests simultaneously  
✅ Quests integrate with existing cargo/color systems  
✅ UI is intuitive and visually appealing  
✅ Save/load preserves quest state  

---

**Version:** 2.2  
**Status:** ✅ Data structure complete, implementation pending  
**Next:** Implement BulletinBoardScene and quest logic
