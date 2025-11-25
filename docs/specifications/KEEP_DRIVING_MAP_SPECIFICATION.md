# Keep Driving Map Specification - Technical Assignment
## Техническое задание для карты в стиле Keep Driving

**Дата создания:** 2025-11-16  
**Статус:** Требует утверждения  
**Версия:** 1.0

---

## 🎯 Цель / Goal

Создать систему карты мира для игры Road Trip Adventure RPG, которая работает по принципу "Keep Driving":
- Непрерывное движение по дорогам
- Управление ресурсами (топливо, энергия, деньги)
- Разные типы дорог с различным расходом топлива
- Остановки только в специальных местах (развязки, города, заправки)
- Случайные события на дорогах

Create a world map system for Road Trip Adventure RPG that follows "Keep Driving" principles:
- Continuous road driving
- Resource management (fuel, energy, money)
- Different road types with varying fuel consumption
- Stops only at special locations (junctions, cities, gas stations)
- Random events on roads

---

## 📋 Принципы Keep Driving / Keep Driving Principles

### 1. Непрерывное движение / Continuous Movement
- Игрок не может остановиться в произвольном месте
- Остановки только на специальных узлах (Node)
- Путешествие между узлами происходит в формате "дорожного кино"
- Player cannot stop at arbitrary locations
- Stops only at special nodes
- Travel between nodes happens in "road cinema" format

### 2. Управление ресурсами / Resource Management
- **Топливо (Fuel):** Расходуется во время движения
- **Энергия (Energy):** Снижается со временем, восстанавливается в мотелях
- **Деньги (Money):** Тратятся на топливо, еду, ремонт
- **Fuel:** Consumed during movement
- **Energy:** Decreases over time, restored at motels
- **Money:** Spent on fuel, food, repairs

### 3. Разные типы дорог / Different Road Types
- **Шоссе (Highway):** -20% расход топлива, быстрее
- **Обычная дорога (Regular Road):** Базовый расход
- **Грунтовка (Dirt Road):** +40% расход, медленнее
- **Highway:** -20% fuel consumption, faster
- **Regular Road:** Base consumption
- **Dirt Road:** +40% consumption, slower

### 4. Случайные события / Random Events
- 3-4 события на каждый отрезок дороги
- Выборы с последствиями (ресурсы, предметы, мораль)
- Встречи с NPC (1 раз на 3 дороги)
- 3-4 events per road segment
- Choices with consequences (resources, items, morale)
- NPC encounters (once per 3 roads)

---

## 🗺️ Структура карты / Map Structure

### Узлы (Nodes) / Nodes

Карта состоит из узлов, соединённых дорогами. Каждый узел - это место остановки.
Map consists of nodes connected by roads. Each node is a stopping point.

#### Типы узлов / Node Types:

1. **HOME_BASE** - Начальная точка / Starting location
   - Процедурно генерируется с 3-4 структурами
   - Всегда доступен для возврата
   - Procedurally generated with 3-4 structures
   - Always available for return

2. **CITY** - Город / City
   - 5+ структур (магазин, мастерская, мотель, агентство работы, доска квестов)
   - Полный набор услуг
   - 5+ structures (store, workshop, motel, work agency, quest board)
   - Full service set

3. **TOWN** - Городок / Town
   - 3-4 структуры (магазин, мастерская, мотель)
   - Базовые услуги
   - 3-4 structures (store, workshop, motel)
   - Basic services

4. **VILLAGE** - Деревня / Village (NEW v2.0)
   - 1-2 структуры (иногда ТОЛЬКО магазин, иногда заправка + магазин)
   - **ДЕФИЦИТ РЕСУРСОВ** - не все деревни имеют заправки!
   - 1-2 structures (sometimes ONLY store, sometimes gas + store)
   - **RESOURCE SCARCITY** - not all villages have gas stations!
   - Создаёт стратегическое планирование / Creates strategic planning

5. **JUNCTION** - Развязка / Junction
   - 2-3 структуры (заправка, магазин)
   - Минимальные услуги
   - 2-3 structures (gas station, store)
   - Minimal services

6. **GAS_STATION** - Заправка / Gas Station
   - 1-2 структуры (заправка, магазин/кафе)
   - Только топливо и еда
   - 1-2 structures (gas pump, store/diner)
   - Only fuel and food

7. **UNIQUE** - Уникальная локация / Unique Location
   - Особые места (маяк, заброшенная фабрика, военная база)
   - Квесты, уникальные NPC
   - Special places (lighthouse, abandoned factory, military base)
   - Quests, unique NPCs

#### Статусы узлов / Node Statuses:

- **LOCKED** - Недоступен, не виден на карте / Unavailable, not visible on map
- **VISIBLE** - Виден, но недоступен для поездки / Visible but not accessible
- **AVAILABLE** - Доступен для поездки / Available for travel
- **CURRENT** - Текущее положение игрока / Current player position
- **VISITED** - Уже посещённый / Already visited

### Дороги (Roads) / Roads

Дороги соединяют узлы и имеют различные характеристики.
Roads connect nodes and have different characteristics.

#### Типы дорог / Road Types:

| Тип / Type | Модификатор топлива / Fuel Modifier | Скорость / Speed | Описание / Description |
|------------|-------------------------------------|------------------|------------------------|
| **HIGHWAY** | -20% | +20% | Федеральная трасса в хорошем состоянии / Federal highway in good condition |
| **ROAD** | 0% (базовый) | 0% (базовый) | Обычная асфальтированная дорога / Regular paved road |
| **DIRT_ROAD** | +40% | -20% | Грунтовая дорога в плохом состоянии / Dirt road in poor condition |

#### Параметры дороги / Road Parameters:

```cpp
struct Road {
    int fromNodeId;                // ID начального узла / Start node ID
    int toNodeId;                  // ID конечного узла / End node ID
    RoadType type;                 // Тип дороги / Road type
    float distance;                // Расстояние в км / Distance in km
    float baseFuelCost;            // Базовый расход топлива / Base fuel cost
    std::string name;              // Название дороги / Road name (optional)
    int condition;                 // Состояние (0-100%) / Condition (0-100%)
    std::vector<Event> events;     // Возможные события / Possible events
};
```

---

## 🎮 Игровой цикл / Game Loop

### Цикл путешествия / Travel Cycle:

```
┌────────────────────────────────────────────────────────────┐
│ 1. УЗЕЛ (LocationScene)                                    │
│    - Просмотр доступных структур                           │
│    - Покупка ресурсов                                      │
│    - Отдых, работа, квесты                                 │
│    - Выбор направления                                     │
│    └───────────────────┐                                   │
│                        ▼                                    │
│ 2. ПОДТВЕРЖДЕНИЕ (Confirmation Dialog)                     │
│    - Показать маршрут                                      │
│    - Расчёт расхода топлива                                │
│    - Проверка достаточности ресурсов                       │
│    - Предупреждения                                        │
│    └───────────────────┐                                   │
│                        ▼                                    │
│ 3. ПУТЕШЕСТВИЕ (TravelCinemaScene)                         │
│    - Параллакс фон (5 слоёв)                               │
│    - Прогресс-бар                                          │
│    - Случайные события (3-4)                               │
│    - Диалоги/воспоминания (2-3)                            │
│    - Встречи NPC (1 на 3 дороги)                           │
│    - Непрерывное списание топлива                          │
│    └───────────────────┐                                   │
│                        ▼                                    │
│ 4. ПРИБЫТИЕ (Arrival)                                      │
│    - Переход к новому узлу                                 │
│    - Разблокировка соседних узлов                          │
│    - Обновление статусов                                   │
│    └───────────────────┐                                   │
│                        ▼                                    │
│ (ВОЗВРАТ К ШАГУ 1)                                         │
└────────────────────────────────────────────────────────────┘
```

### Расчёт расхода топлива / Fuel Consumption Calculation:

```cpp
float calculateFuelCost(const Road& road, const Car& car) {
    // Базовый расход на км для машины / Base consumption per km for car
    float baseConsumption = car.getFuelConsumptionPerKm(); // например, 0.5 L/km
    
    // Модификатор типа дороги / Road type modifier
    float roadModifier = getRoadModifier(road.type);
    // HIGHWAY: 0.8 (-20%)
    // ROAD: 1.0 (базовый)
    // DIRT_ROAD: 1.4 (+40%)
    
    // Модификатор состояния дороги / Road condition modifier
    float conditionModifier = 1.0f + ((100 - road.condition) / 200.0f);
    // condition 100%: модификатор 1.0
    // condition 50%: модификатор 1.25
    // condition 0%: модификатор 1.5
    
    // Итоговый расход / Final consumption
    float totalFuel = road.distance * baseConsumption * roadModifier * conditionModifier;
    
    return totalFuel;
}
```

---

## 📐 Формат данных карты / Map Data Format

### JSON структура / JSON Structure:

```json
{
  "map_name": "Россия 90-х: Путешествие на восток",
  "version": "1.0",
  "description": "Путешествие через Россию в стиле Keep Driving",
  
  "nodes": [
    {
      "id": 1,
      "type": "HOME_BASE",
      "name": "Москва - Ваш дом",
      "name_en": "Moscow - Your Home",
      "position": {"x": 200, "y": 400},
      "biome": "urban",
      "structures": [
        {"type": "GAS_PUMP", "name": "Заправка на углу"},
        {"type": "STORE", "name": "Продуктовый"},
        {"type": "GARAGE", "name": "Гараж 'Авто-Мастер'"}
      ],
      "unlockNodes": [2, 3]
    },
    {
      "id": 2,
      "type": "JUNCTION",
      "name": "Развязка М7",
      "name_en": "M7 Junction",
      "position": {"x": 400, "y": 350},
      "biome": "suburban",
      "structures": [
        {"type": "GAS_PUMP", "name": "АЗС Газпром"},
        {"type": "STORE", "name": "Магазин 'У дороги'"}
      ],
      "unlockNodes": [4, 5]
    },
    {
      "id": 3,
      "type": "TOWN",
      "name": "Владимир",
      "name_en": "Vladimir",
      "position": {"x": 450, "y": 450},
      "biome": "forest",
      "structures": [
        {"type": "GAS_PUMP", "name": "Заправка"},
        {"type": "STORE", "name": "Магазин"},
        {"type": "GARAGE", "name": "Автосервис"},
        {"type": "MOTEL", "name": "Мотель 'Дорожный'"}
      ],
      "unlockNodes": [4]
    },
    {
      "id": 4,
      "type": "CITY",
      "name": "Нижний Новгород",
      "name_en": "Nizhny Novgorod",
      "position": {"x": 650, "y": 400},
      "biome": "urban",
      "structures": [
        {"type": "GAS_PUMP", "name": "Центральная АЗС"},
        {"type": "STORE", "name": "Супермаркет"},
        {"type": "GARAGE", "name": "Автосервис 'Профи'"},
        {"type": "MOTEL", "name": "Гостиница 'Волга'"},
        {"type": "WORK_AGENCY", "name": "Биржа труда"},
        {"type": "QUEST_BOARD", "name": "Доска объявлений"}
      ],
      "unlockNodes": [6, 7]
    }
  ],
  
  "roads": [
    {
      "from": 1,
      "to": 2,
      "type": "HIGHWAY",
      "distance": 80,
      "name": "М7 'Волга'",
      "condition": 85,
      "description": "Хорошая федеральная трасса"
    },
    {
      "from": 1,
      "to": 3,
      "type": "ROAD",
      "distance": 180,
      "name": "Дорога через Покров",
      "condition": 70,
      "description": "Обычная региональная дорога"
    },
    {
      "from": 2,
      "to": 4,
      "type": "HIGHWAY",
      "distance": 120,
      "name": "М7",
      "condition": 90,
      "description": "Отличное состояние трассы"
    },
    {
      "from": 3,
      "to": 4,
      "type": "ROAD",
      "distance": 110,
      "name": "Дорога через Гороховец",
      "condition": 65,
      "description": "Местами разбитая дорога"
    }
  ],
  
  "events": {
    "road_1_2": [
      "event_tree_on_road",
      "event_hitchhiker",
      "event_police_checkpoint"
    ],
    "road_2_4": [
      "event_broken_bridge",
      "event_roadside_market",
      "event_mechanic_offer"
    ]
  }
}
```

---

## 🏗️ Техническая реализация / Technical Implementation

### Классы / Classes:

#### 1. WorldMap

```cpp
class WorldMap {
public:
    bool loadFromJSON(const std::string& filename);
    
    MapNode* getNode(int nodeId);
    std::vector<Road> getRoadsFrom(int nodeId);
    std::vector<MapNode*> getAvailableNodes(int fromNodeId);
    
    float calculateFuelCost(int fromId, int toId, const Car& car);
    float calculateTravelTime(int fromId, int toId, const Car& car);
    
    void unlockNode(int nodeId);
    void updateNodeStatus(int nodeId, NodeStatus status);
    
private:
    std::string m_mapName;
    std::map<int, MapNode> m_nodes;
    std::vector<Road> m_roads;
    std::map<std::string, std::vector<std::string>> m_eventPool;
};
```

#### 2. MapNode (Enhanced)

```cpp
struct MapNode {
    int id;
    NodeType type;
    std::string name;
    std::string nameEn;
    sf::Vector2f position;
    NodeStatus status;
    BiomeType biome;
    
    std::vector<Structure> structures;
    std::vector<int> connections;  // IDs соединённых узлов
    std::vector<int> unlockNodes;  // Узлы, которые разблокируются при посещении
    
    // Constructor
    MapNode(int id_, NodeType type_, const std::string& name_, 
            sf::Vector2f pos, NodeStatus status_, BiomeType biome_)
        : id(id_), type(type_), name(name_), position(pos), 
          status(status_), biome(biome_) {}
};
```

#### 3. Road (Enhanced)

```cpp
enum class RoadType {
    HIGHWAY,     // Шоссе
    ROAD,        // Обычная дорога
    DIRT_ROAD    // Грунтовка
};

struct Road {
    int fromNodeId;
    int toNodeId;
    RoadType type;
    float distance;              // км
    std::string name;
    int condition;               // 0-100%
    std::string description;
    
    // Calculated values
    float baseFuelCost;          // Базовый расход топлива
    float baseTravelTime;        // Базовое время в пути (часы)
    
    std::vector<std::string> eventIds;  // События на этой дороге
    
    Road(int from, int to, RoadType t, float dist, int cond)
        : fromNodeId(from), toNodeId(to), type(t), 
          distance(dist), condition(cond) {
        calculateBaseCosts();
    }
    
    void calculateBaseCosts() {
        // Базовый расход: 0.5 L/km для обычной дороги
        float baseConsumption = 0.5f;
        
        // Модификаторы типа дороги
        float typeModifier = 1.0f;
        if (type == RoadType::HIGHWAY) typeModifier = 0.8f;  // -20%
        if (type == RoadType::DIRT_ROAD) typeModifier = 1.4f; // +40%
        
        // Модификатор состояния
        float condModifier = 1.0f + ((100 - condition) / 200.0f);
        
        baseFuelCost = distance * baseConsumption * typeModifier * condModifier;
        
        // Базовое время: ~60 км/ч для обычной дороги
        float baseSpeed = 60.0f;
        if (type == RoadType::HIGHWAY) baseSpeed = 80.0f;    // +33%
        if (type == RoadType::DIRT_ROAD) baseSpeed = 40.0f;  // -33%
        
        baseTravelTime = distance / baseSpeed;
    }
};
```

---

## 🎨 UI/UX Спецификация / UI/UX Specification

### WorldMapScene UI:

```
┌────────────────────────────────────────────────────────────────┐
│ КАРТА ПУТЕШЕСТВИЯ / TRAVEL MAP          [💰 $250] [⚡ 75%]     │
├────────────────────────────────────────────────────────────────┤
│                                                                  │
│      ⬤ Москва                                                   │
│      │ (CURRENT)                                               │
│      │                                                          │
│      ├──── M7 (HIGHWAY, 80км) ────┐                           │
│      │                             │                            │
│      │                             ⬤ Развязка М7               │
│      │                             │ (AVAILABLE)               │
│      │                             │                            │
│      └──── Дорога (ROAD, 180км) ──┤                           │
│                                     │                            │
│                                     ⬤ Владимир                  │
│                                     │ (AVAILABLE)               │
│                                     │                            │
│                                   ⬤ Нижний Новгород            │
│                                     (VISIBLE)                   │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐ │
│  │ ⓘ ИНФОРМАЦИЯ                                             │ │
│  │                                                           │ │
│  │ Наведите курсор на узел для информации                   │ │
│  │ Кликните по доступному узлу для выбора маршрута          │ │
│  │                                                           │ │
│  └──────────────────────────────────────────────────────────┘ │
│                                                                  │
│  [ESC] Выход  [I] Инвентарь                                    │
└────────────────────────────────────────────────────────────────┘
```

### Подсказка при наведении / Hover Tooltip:

```
┌────────────────────────────────────┐
│ 🏘️ Развязка М7                     │
├────────────────────────────────────┤
│ Тип: Развязка / Junction           │
│ Статус: Доступен / Available       │
│                                     │
│ Структуры:                         │
│   • Заправка                       │
│   • Магазин                        │
│                                     │
│ Маршрут из Москвы:                 │
│   Дорога: M7 (Шоссе)               │
│   Расстояние: 80 км                │
│   ⛽ Расход: ~32 L                 │
│   ⏱️ Время: ~1.0 ч                 │
│                                     │
│ [Кликните для выбора]              │
└────────────────────────────────────┘
```

### Диалог подтверждения / Confirmation Dialog:

```
┌────────────────────────────────────────────────────────────┐
│ 🚗 ПОДТВЕРЖДЕНИЕ МАРШРУТА / ROUTE CONFIRMATION              │
├────────────────────────────────────────────────────────────┤
│                                                              │
│ Откуда: Москва (HOME_BASE)                                  │
│ Куда: Развязка М7 (JUNCTION)                                │
│                                                              │
│ 📍 Маршрут: M7 'Волга' (Шоссе)                             │
│    Расстояние: 80 км                                        │
│    Состояние дороги: 85% (Хорошее)                         │
│                                                              │
│ 💰 Стоимость:                                               │
│    ⛽ Расход топлива: 32 L                                  │
│    ⏱️ Время в пути: ~1.0 часа                              │
│                                                              │
│ 📊 Ваши ресурсы:                                            │
│    ⛽ Топливо: 150/200 L  ✅ ДОСТАТОЧНО                     │
│    ⚡ Энергия: 75%        ✅ В НОРМЕ                        │
│    💰 Деньги: $250                                          │
│                                                              │
│ ┌────────────────────────────────────────────────────────┐ │
│ │ ⚠️ ВНИМАНИЕ                                            │ │
│ │ На дороге могут встретиться случайные события.         │ │
│ │ Убедитесь, что у вас есть припасы!                     │ │
│ └────────────────────────────────────────────────────────┘ │
│                                                              │
│ [Enter] ВЫЕХАТЬ     [Esc] ОТМЕНИТЬ                          │
└────────────────────────────────────────────────────────────┘
```

---

## ✅ Критерии приёмки / Acceptance Criteria

### Функциональные требования / Functional Requirements:

- [ ] Карта загружается из JSON файла
- [ ] Отображаются все узлы с правильными статусами
- [ ] Отображаются дороги между узлами
- [ ] Узлы можно выбрать мышью (только доступные)
- [ ] При наведении показывается информация об узле
- [ ] Диалог подтверждения показывает расчёт ресурсов
- [ ] Блокировка выезда при недостатке топлива
- [ ] Разблокировка соседних узлов при посещении
- [ ] Расчёт топлива учитывает тип дороги
- [ ] Расчёт топлива учитывает состояние дороги
- [ ] Расчёт топлива учитывает тип автомобиля

### Интеграция / Integration:

- [ ] WorldMapScene интегрирована с SceneManager
- [ ] PlayerState сохраняет текущий узел
- [ ] Переход в LocationScene с данными узла
- [ ] Переход в TravelCinemaScene с данными дороги
- [ ] События на дороге берутся из пула событий карты

### UI/UX:

- [ ] Читаемые шрифты (pixel-perfect)
- [ ] Цветовое кодирование статусов узлов
- [ ] Плавные переходы между сценами
- [ ] Подсказки при наведении
- [ ] Визуальная обратная связь при выборе
- [ ] Разрешение 1366x768
- [ ] 60 FPS

---

## 📊 Пример карты / Example Map

### Простая линейная карта для тестирования:

```
Москва (HOME_BASE) 
    ↓ M7 (HIGHWAY, 80км)
Развязка М7 (JUNCTION)
    ↓ M7 (HIGHWAY, 120км)
Нижний Новгород (CITY)
    ↓ M7 (HIGHWAY, 400км)
Казань (CITY)
    ↓ M7 (HIGHWAY, 500км)
Екатеринбург (CITY)
```

### Сложная разветвлённая карта:

```
                    Ярославль (TOWN)
                          ↑ ROAD (150км)
                          |
Москва (HOME_BASE) ─────→ Развязка М7 (JUNCTION)
    |                          |
    | ROAD (180км)            | HIGHWAY (120км)
    ↓                          ↓
Владимир (TOWN) ────────→ Нижний Новгород (CITY)
         ROAD (110км)               |
                                    | HIGHWAY (400км)
                                    ↓
                              Казань (CITY)
```

---

## 🚀 План реализации / Implementation Plan

### Этап 1: Структуры данных (1-2 дня)
- [ ] Определить enum'ы (NodeType, RoadType, NodeStatus)
- [ ] Создать структуры MapNode и Road
- [ ] Создать класс WorldMap с методами загрузки

### Этап 2: JSON парсинг (1-2 дня)
- [ ] Реализовать loadFromJSON()
- [ ] Парсинг узлов
- [ ] Парсинг дорог
- [ ] Парсинг событий
- [ ] Создать тестовую карту test_map.json

### Этап 3: Логика карты (2-3 дня)
- [ ] Реализовать calculateFuelCost()
- [ ] Реализовать getAvailableNodes()
- [ ] Реализовать систему разблокировки узлов
- [ ] Реализовать проверки ресурсов

### Этап 4: UI WorldMapScene (3-4 дня)
- [ ] Рендеринг узлов (цветовое кодирование)
- [ ] Рендеринг дорог (линии между узлами)
- [ ] Обработка мыши (выбор, наведение)
- [ ] Подсказки при наведении
- [ ] Диалог подтверждения

### Этап 5: Интеграция (2-3 дня)
- [ ] Интеграция с SceneManager
- [ ] Интеграция с PlayerState
- [ ] Переходы в LocationScene
- [ ] Переходы в TravelCinemaScene
- [ ] Тестирование полного цикла

### Этап 6: Полировка (1-2 дня)
- [ ] Визуальные эффекты
- [ ] Звуковое оформление (опционально)
- [ ] Оптимизация
- [ ] Финальное тестирование

**Общее время: 10-16 дней разработки**

---

## 📝 Примечания / Notes

### Важные особенности:
1. Карта создаётся вручную (не процедурно) в JSON
2. Узлы разблокируются по мере продвижения
3. Расход топлива зависит от типа дороги и автомобиля
4. События привязаны к конкретным дорогам
5. Система должна быть расширяемой (легко добавлять узлы/дороги)

### Отличия от текущей реализации:
- **До:** Простые узлы без типов и структур
- **После:** Полноценные узлы с типами, структурами, биомами
- **До:** Нет расчёта топлива по типу дороги
- **После:** Детальный расчёт с учётом всех факторов
- **До:** Нет системы разблокировки
- **После:** Прогрессивная система открытия карты
- **До:** Нет интеграции с событиями
- **После:** События привязаны к дорогам

---

**Статус:** ✅ Готово к утверждению  
**Следующий шаг:** Утверждение спецификации владельцем  
**После утверждения:** Начать реализацию согласно плану
