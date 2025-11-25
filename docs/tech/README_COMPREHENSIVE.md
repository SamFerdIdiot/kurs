# Road Trip Adventure RPG - Comprehensive Documentation
# Дорожное приключение RPG - Полная документация

**Version:** 1.0  
**Resolution:** 1366x768  
**Frame Rate:** 60 FPS  
**Engine:** C++17 + SFML 2.6

---

## Содержание / Table of Contents

### 20 этапов документации / 20 Documentation Stages

1. [Введение и мотивация](#1-введение-и-мотивация--introduction-and-motivation)
2. [Архитектура и overview](#2-архитектура-и-overview--architecture-and-overview)
3. [Карта мира и wireframe](#3-карта-мира-и-wireframe--world-map-and-wireframe)
4. [Кино-режим](#4-кино-режим--cinematic-mode)
5. [Система дорожных событий](#5-система-дорожных-событий--road-events-system)
6. [Локации/структуры/сценарии](#6-локацииструктурысценарии--locationsstructuresscenarios)
7. [Тетра-инвентарь](#7-тетра-инвентарь--tetra-inventory)
8. [Автомобили и ограничения](#8-автомобили-и-ограничения--vehicles-and-constraints)
9. [NPC/диалоги/user stories](#9-npcдиалогиuser-stories--npcsdialoguesuser-stories)
10. [Аркадный режим](#10-аркадный-режим--arcade-mode)
11. [Архетипы персонажа](#11-архетипы-персонажа--character-archetypes)
12. [Механика ресурсов](#12-механика-ресурсов--resource-mechanics)
13. [Управление и ограничения](#13-управление-и-ограничения--controls-and-constraints)
14. [User stories](#14-user-stories-игрок-сценарист-тестер)
15. [Архитектура классов/C++](#15-архитектура-классовc--class-architecturec)
16. [UI: wireframe, стандарты, цвета](#16-ui-wireframe-стандарты-цвета--ui-wireframes-standards-colors)
17. [Тесты и чек-листы](#17-тесты-и-чек-листы--tests-and-checklists)
18. [Roadmap](#18-roadmap)
19. [FAQ](#19-faq)
20. [Приложение: макеты и edge-case](#20-приложение-макеты-и-edge-case--appendix-mockups-and-edge-cases)

---

# 1. Введение и мотивация / Introduction and Motivation

## English Version

### Project Vision

**Road Trip Adventure RPG** is a 2D pixel-art road trip adventure game that combines survival mechanics, resource management, and narrative-driven gameplay with a distinctive retro aesthetic. The game places players in control of a cross-country journey where every decision matters, from route selection to resource allocation, from companion management to inventory optimization.

### Core Philosophy

The game is built on three pillars:

1. **Strategic Resource Management**: Every mile traveled, every gallon of fuel consumed, and every dollar spent is a tactical decision that shapes the journey's outcome.

2. **Emergent Narrative**: Through a sophisticated event system and NPC interactions, each playthrough creates unique stories driven by player choices and random encounters.

3. **Spatial Puzzle-Solving**: The Tarkov-style grid inventory system transforms item management into an engaging spatial puzzle, requiring players to think carefully about what to carry and how to organize it.

### Target Audience

- **RPG Enthusiasts**: Players who enjoy deep character progression and meaningful choices
- **Survival Game Fans**: Those who appreciate resource scarcity and strategic planning
- **Roguelite Lovers**: Players seeking procedurally generated content with high replayability
- **Streamers**: Content creators looking for games that generate unique, shareable moments
- **Mod Community**: Developers and scenario writers interested in extensible game systems

### Design Goals

1. **Accessibility**: Simple controls (A/D movement, E interaction, mouse-driven menus)
2. **Depth**: Complex underlying systems that reward strategic thinking
3. **Replayability**: Procedural generation ensures no two journeys are identical
4. **Extensibility**: Modular architecture allows easy content addition
5. **Atmosphere**: Retro pixel-art aesthetic creates nostalgic, immersive experience

## Русская версия

### Видение проекта

**Road Trip Adventure RPG** — это 2D пиксель-арт игра о дорожном путешествии, объединяющая механики выживания, управления ресурсами и сюжетный геймплей с характерной ретро-эстетикой. Игра ставит игроков за руль путешествия через всю страну, где каждое решение имеет значение: от выбора маршрута до распределения ресурсов, от управления командой до оптимизации инвентаря.

### Основная философия

Игра построена на трёх столпах:

1. **Стратегическое управление ресурсами**: Каждая пройденная миля, каждый потраченный галлон топлива и каждый израсходованный доллар — это тактическое решение, формирующее исход путешествия.

2. **Эмерджентный нарратив**: Через сложную систему событий и взаимодействий с NPC каждое прохождение создаёт уникальные истории, движимые выбором игрока и случайными встречами.

3. **Пространственные головоломки**: Система инвентаря в стиле Tarkov превращает управление предметами в увлекательную пространственную головоломку, требующую тщательно продумывать, что брать и как это организовать.

### Целевая аудитория

- **Энтузиасты RPG**: Игроки, ценящие глубокое развитие персонажа и значимые выборы
- **Фанаты игр на выживание**: Те, кто ценит дефицит ресурсов и стратегическое планирование
- **Любители roguelite**: Игроки, ищущие процедурно генерируемый контент с высокой реиграбельностью
- **Стримеры**: Создатели контента, ищущие игры, генерирующие уникальные моменты
- **Сообщество модификаций**: Разработчики и сценаристы, заинтересованные в расширяемых игровых системах

### Цели дизайна

1. **Доступность**: Простое управление (A/D перемещение, E взаимодействие, меню через мышь)
2. **Глубина**: Сложные базовые системы, вознаграждающие стратегическое мышление
3. **Реиграбельность**: Процедурная генерация гарантирует, что нет двух одинаковых путешествий
4. **Расширяемость**: Модульная архитектура позволяет легко добавлять контент
5. **Атмосфера**: Ретро пиксель-арт эстетика создаёт ностальгический, погружающий опыт

### Мотивация разработки

Проект создан для демонстрации:
- Современных подходов к 2D геймдеву на C++
- Эффективной архитектуры на основе сцен (scene-based architecture)
- Интеграции сложных игровых систем (инвентарь, события, ресурсы, NPC)
- Возможностей SFML 2.6 для создания полноценных игр
- Лучших практик документирования и командной разработки

---

# 2. Архитектура и overview / Architecture and Overview

## System Architecture

### High-Level Architecture Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         Application Layer                        │
│                           (main.cpp)                             │
└────────────────────────────┬────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                        SceneManager                              │
│                  (Scene lifecycle coordinator)                   │
│  • Creates scenes                                                │
│  • Manages transitions                                           │
│  • Handles main game loop                                        │
└────────┬───────────────────┬────────────────────┬────────────────┘
         │                   │                    │
         ▼                   ▼                    ▼
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ Menu Scenes  │    │Game Scenes   │    │System Scenes │
├──────────────┤    ├──────────────┤    ├──────────────┤
│• MainMenu    │    │• Driving     │    │• Pause       │
│• CharCreate  │    │• SideView    │    │• GameOver    │
│• RouteSelect │    │• Town        │    │• Inventory   │
│              │    │• Motel       │    │• Dialogue    │
│              │    │• Event       │    │              │
└──────────────┘    └──────────────┘    └──────────────┘
         │                   │                    │
         └───────────────────┼────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                      Core Game Systems                           │
├──────────────────┬──────────────────┬───────────────────────────┤
│ PlayerState      │ ResourceManager  │ GridInventorySystem       │
│ • Character data │ • Fuel           │ • 2D spatial grid         │
│ • Progress       │ • Money          │ • Item placement          │
│ • Archetype      │ • Energy         │ • Collision detection     │
│                  │ • Morale         │ • Drag & drop             │
├──────────────────┼──────────────────┼───────────────────────────┤
│ EventManager     │ NPCManager       │ POIManager                │
│ • Road events    │ • Companions     │ • Locations               │
│ • Scripted       │ • Morale system  │ • Structures              │
│ • Random         │ • Abilities      │ • Services                │
├──────────────────┼──────────────────┼───────────────────────────┤
│ HUDPanel         │ InteractionMenu  │ StartZone                 │
│ • Real-time UI   │ • Context menus  │ • Initial area            │
│ • Retro styling  │ • Services       │ • Structure placement     │
└──────────────────┴──────────────────┴───────────────────────────┘
         │                   │                    │
         └───────────────────┼────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                      SFML Framework                              │
│          • Graphics  • Window  • System                          │
└─────────────────────────────────────────────────────────────────┘
```

### Scene-Based Architecture

**Core Concept**: The entire game is built around the `Scene` interface. Every game state (menu, gameplay, dialogue) is a separate scene that implements a common interface.

**Scene Interface**:
```cpp
class Scene {
public:
    virtual ~Scene() = default;
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual SceneType getNextScene() const = 0;
    virtual bool isFinished() const = 0;
};
```

**Benefits**:
- Clear separation of concerns
- Easy to add new game states
- Simplified testing (test each scene independently)
- Clean transitions and state management
- Memory efficient (only active scene in memory)

### Game Loop

```
┌─────────────────────────────────────────────────────────────────┐
│                      Main Game Loop                              │
│                                                                  │
│  while (running) {                                               │
│      1. Poll Events                                              │
│         │                                                        │
│         ├─> Dispatch to Current Scene                           │
│         └─> Handle Window Events                                │
│                                                                  │
│      2. Update (deltaTime)                                       │
│         │                                                        │
│         ├─> Update Current Scene                                │
│         ├─> Update Game Systems                                 │
│         └─> Check Scene Transitions                             │
│                                                                  │
│      3. Render                                                   │
│         │                                                        │
│         ├─> Clear Window                                        │
│         ├─> Render Current Scene                                │
│         └─> Display Frame                                       │
│                                                                  │
│      4. Check Scene Completion                                   │
│         │                                                        │
│         ├─> If finished: Get next scene type                    │
│         ├─> Create new scene                                    │
│         └─> Transition to new scene                             │
│  }                                                               │
└─────────────────────────────────────────────────────────────────┘
```

### Data Flow

```
Player Input
     │
     ▼
┌──────────┐
│  Scene   │ ◄──── Receives events
└────┬─────┘
     │
     ▼
┌──────────┐
│ Systems  │ ◄──── Processes logic (ResourceManager, EventManager, etc.)
└────┬─────┘
     │
     ▼
┌──────────┐
│  State   │ ◄──── Updates game state (PlayerState, Inventory, etc.)
└────┬─────┘
     │
     ▼
┌──────────┐
│ Render   │ ◄──── Visualizes current state
└──────────┘
```

## Русская версия

### Архитектура на основе сцен

Вся игра построена вокруг интерфейса `Scene`. Каждое состояние игры (меню, геймплей, диалог) является отдельной сценой, реализующей общий интерфейс.

**Преимущества**:
- Чёткое разделение ответственности
- Легко добавлять новые игровые состояния
- Упрощённое тестирование (каждая сцена тестируется независимо)
- Чистые переходы и управление состоянием
- Эффективное использование памяти (только активная сцена в памяти)

### Типы сцен

**Меню-сцены**:
- MainMenuScene - Главное меню
- CharacterCreationScene - Создание персонажа
- RouteSelectionScene - Выбор маршрута
- PauseMenuScene - Пауза
- GameOverScene - Конец игры

**Игровые сцены**:
- DrivingScene - Вид сверху, вождение
- SideViewScene - Вид сбоку, аркадный режим
- TownScene - Городская локация
- MotelScene - Мотель/отдых
- CinematicEventScene - Кино-события

**Системные сцены**:
- InventoryScene - Управление инвентарём
- DialogueScene - Диалоги с NPC
- RestScene - Отдых и восстановление

### Ключевые системы

**PlayerState** - Глобальное состояние игрока:
- Архетип персонажа
- Прогресс путешествия
- Статистика

**ResourceManager** - Управление ресурсами:
- Топливо (0-100%)
- Деньги ($)
- Энергия (0-100%)
- Мораль команды

**GridInventorySystem** - Пространственный инвентарь:
- Сетка 6x4 (по умолчанию)
- Предметы с размерами
- Коллизии и размещение
- Ротация на 90°

**EventManager** - Система событий:
- Случайные дорожные события
- Скриптованные сюжетные события
- Условия и последствия

---

# 3. Карта мира и wireframe / World Map and Wireframe

## World Map System

### Concept

The world map is a **node-based road network** where players navigate between interconnected locations. Each node represents a stopping point with various structures and services.

### Visual Representation

```
Wireframe: World Map View
┌────────────────────────────────────────────────────────────────────────┐
│ ROAD TRIP MAP                                          [FUEL: ████░░] │
│                                                        [CASH: $185]    │
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│        ◉━━━━━◉━━━━━━◉━━━━━━◉           Legend:                       │
│       HOME   GAS  DINER  TOWN           ◉ = Location Node            │
│         ┃                  ┃            ━ = Road Connection          │
│         ┃                  ┃            📍= Current Position          │
│         ◉━━━━━◉━━━━━◉━━━━━◉            🏠 = Home Base               │
│       MOTEL  SHOP  MECH  CITY           ⛽ = Gas Station             │
│                     ┃                   🏪 = Store                   │
│                     ┃                   🔧 = Mechanic                │
│                    📍◉                  🏨 = Motel                   │
│                  PLAYER                 🍔 = Diner                   │
│                     ┃                                                 │
│         ◉━━━━━◉━━━━━◉━━━━━◉                                         │
│       REST  BILL   GAS  GARAGE                                        │
│                                                                        │
│ [Mouse: Select Route] [Click: Confirm] [ESC: Return]                  │
└────────────────────────────────────────────────────────────────────────┘
```

### Node Structure

Each node contains:

```cpp
struct MapNode {
    int id;                              // Unique identifier
    sf::Vector2f position;               // Screen coordinates
    std::string name;                    // Display name
    NodeType type;                       // HOME, TOWN, JUNCTION
    std::vector<Structure> structures;   // Available services (1-7)
    std::vector<int> connectedNodes;     // Linked node IDs
    bool visited;                        // Has player been here?
    float distanceFromHome;              // Total km traveled
};
```

### Structure Types

| Structure | Icon | Services Available | Cost Range |
|-----------|------|-------------------|------------|
| Gas Station | ⛽ | Fuel refill | $50 |
| General Store | 🏪 | Items, Food, Tools | $5-$100 |
| Mechanic | 🔧 | Repairs, Upgrades | $25-$200 |
| Motel | �� | Rest, Save, Storage | $20-$50 |
| Diner | 🍔 | Food, Coffee, Energy | $5-$25 |
| Rest Stop | 🅿️ | Brief rest, Quick save | Free-$10 |
| Billboard | 📋 | Info, Quests, Ads | Free |
| Point of Interest | 🎯 | Unique scenarios | Varies |

### Route Selection Algorithm

```
Player selects destination node:
1. Check if node is connected to current position
   ├─> If YES: Proceed to step 2
   └─> If NO: Show error "Route not available"

2. Calculate route cost
   ├─> Distance = euclidean distance between nodes
   ├─> Fuel cost = distance * fuel consumption rate
   ├─> Estimated time = distance / average speed
   └─> Display confirmation dialog

3. Show confirmation dialog:
   ┌──────────────────────────────────────┐
   │   Confirm Route                      │
   ├──────────────────────────────────────┤
   │   Destination: [Node Name]           │
   │   Distance: XX.X km                  │
   │   Est. Fuel: YY liters               │
   │   Est. Time: ZZ minutes              │
   │                                      │
   │   Current Fuel: [████████░░] 85%    │
   │                                      │
   │   [Confirm]  [Cancel]                │
   └──────────────────────────────────────┘

4. On confirmation:
   ├─> Check if player has enough fuel
   │   ├─> If YES: Start travel (enter CinematicMode)
   │   └─> If NO: Show error "Not enough fuel"
   └─> Update player progress marker

5. Travel begins (see Stage 4: Cinematic Mode)
```

### Map Generation

**Initial Map Creation**:
```
1. Create home node at center (0, 0)
2. Generate 15-25 nodes using:
   ├─> Radial distribution (concentric rings)
   ├─> Random angle variations (±30°)
   └─> Distance scaling (200-800 units apart)

3. Connect nodes using:
   ├─> Minimum spanning tree (ensures all reachable)
   ├─> Additional random connections (15-25% extra)
   └─> Avoid crossing roads where possible

4. Assign structure types to each node:
   ├─> Each node has 1-7 structures
   ├─> Gas stations: 80% probability per node
   ├─> Random distribution of other types
   └─> POI: 10% chance per node
```

### Wireframe: Node Detail View

When hovering over a node:

```
┌──────────────────────────────────────────┐
│ 🏪 JUNCTION ALPHA                        │
├──────────────────────────────────────────┤
│ Distance from current: 45.2 km           │
│ Estimated fuel: 12 liters                │
│                                          │
│ Available Services:                      │
│  ⛽ Gas Station                          │
│  🏪 General Store                        │
│  🔧 Mechanic Shop                        │
│  🏨 Roadside Motel                       │
│                                          │
│ Status: Unvisited                        │
│                                          │
│ [Travel Here] [Cancel]                   │
└──────────────────────────────────────────┘
```

## Русская версия

### Система карты мира

Карта мира представляет собой **сеть дорог на основе узлов**, где игроки перемещаются между связанными локациями. Каждый узел представляет точку остановки с различными структурами и услугами.

### Структура узла

Каждый узел содержит:
- Уникальный идентификатор
- Позиция на экране
- Отображаемое имя
- Тип узла (ДОМ, ГОРОД, РАЗВЯЗКА)
- Массив доступных структур (1-7)
- Связанные узлы (ID соседних узлов)
- Флаг посещения
- Расстояние от дома

### Алгоритм выбора маршрута

1. Игрок выбирает узел назначения (клик мышью)
2. Проверка связности узлов
3. Расчёт стоимости маршрута (топливо, время)
4. Диалог подтверждения с подробной информацией
5. При подтверждении: начало путешествия (кино-режим)

### Генерация карты

**Процедурная генерация**:
- Создание домашнего узла в центре
- Генерация 15-25 узлов радиальным распределением
- Соединение узлов минимальным остовным деревом
- Случайное распределение структур по узлам
- Гарантия наличия заправок (80% вероятность)

---

# 4. Кино-режим / Cinematic Mode

## Overview

The **Cinematic Mode** (Кино-режим) is the core travel experience between locations. It's a dedicated scene that visualizes the journey and triggers road events.

### Key Characteristics

- **Non-skippable**: Players must experience the journey (no "fast travel")
- **Event-driven**: Random and scripted events occur during travel
- **Resource consumption**: Fuel and energy decrease in real-time
- **No free movement**: A/D controls disabled (cinematic experience)
- **Progress visualization**: Progress bar shows journey completion

### Visual Layout

```
Wireframe: Cinematic Travel Mode
┌────────────────────────────────────────────────────────────────────────┐
│ TRAVELING TO: Junction Alpha                                           │
│ Progress: [████████████░░░░░░░░░░░░] 60%                              │
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│    ☁️      ☁️             ☁️                    ☁️         ☁️         │ Sky
│                                                                        │
│  🌲   🌲        ⛽                  🌲    🌲                          │ Background
│                                                                        │
│ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─   │ Road
│                                                                        │
│           🚗→    Road scrolling →  →  →                               │ Player Car
│                                                                        │
│══════════════════════════════════════════════════════════════════════│ Ground
│                                                                        │
│ [FUEL: ████░░░░] [ENERGY: ██████░░] [SPEED: 65 mph]                  │
└────────────────────────────────────────────────────────────────────────┘
```

### Cinematic Mode Phases

**Phase 1: Journey Start**
```
1. Display destination information
2. Calculate total journey distance
3. Initialize progress (0%)
4. Start scrolling background animation
5. Begin fuel/energy consumption
```

**Phase 2: Active Travel**
```
Loop while progress < 100%:
    1. Update progress based on time/distance
    2. Scroll background layers (parallax)
    3. Consume resources (fuel: -0.1%/sec, energy: -0.05%/sec)
    4. Check for random event triggers
       ├─> Roll dice (event probability)
       ├─> If triggered: Pause travel, show event
       └─> Resume after event resolution
    5. Update HUD (speed, fuel, energy)
    6. Render cinematic view
```

**Phase 3: Arrival**
```
1. Progress reaches 100%
2. Fade out cinematic view
3. Display "Arrived at [Location]"
4. Transition to location scene
5. Update player position on map
```

### Parallax Scrolling System

Multiple background layers scroll at different speeds to create depth:

```
Layer System (near to far):
┌──────────────────────────────────────────────────────────────┐
│ Layer 1: Foreground (Roadside objects)   Speed: 1.0x        │
│ Layer 2: Middle ground (Trees, signs)    Speed: 0.6x        │
│ Layer 3: Background (Hills, buildings)   Speed: 0.3x        │
│ Layer 4: Far background (Mountains)      Speed: 0.15x       │
│ Layer 5: Sky (Clouds)                    Speed: 0.05x       │
└──────────────────────────────────────────────────────────────┘

Scrolling formula:
layerPosition.x -= scrollSpeed * layerSpeedMultiplier * deltaTime

When layerPosition.x < -textureWidth:
    layerPosition.x += textureWidth  // Loop seamlessly
```

### Event Trigger System

```cpp
// Event probability check (every 5 seconds)
float eventCheckInterval = 5.0f;
float timeSinceLastCheck = 0.0f;

void update(float deltaTime) {
    timeSinceLastCheck += deltaTime;
    
    if (timeSinceLastCheck >= eventCheckInterval) {
        // Roll for event
        float eventChance = 0.15f;  // 15% chance per check
        float roll = randomFloat(0.0f, 1.0f);
        
        if (roll < eventChance) {
            // Trigger random event
            triggerRandomEvent();
            pauseTravel();
        }
        
        timeSinceLastCheck = 0.0f;
    }
}
```

### Resource Consumption

```cpp
// Resource depletion during travel
void updateResources(float deltaTime) {
    // Fuel consumption (distance-based)
    float fuelConsumptionRate = 0.1f;  // % per second at 60 mph
    float currentSpeed = 65.0f;  // mph
    float fuelDelta = (currentSpeed / 60.0f) * fuelConsumptionRate * deltaTime;
    ResourceManager::modifyFuel(-fuelDelta);
    
    // Energy depletion (time-based)
    float energyDepletionRate = 0.05f;  // % per second
    ResourceManager::modifyEnergy(-energyDepletionRate * deltaTime);
    
    // Check for critical levels
    if (ResourceManager::getFuel() < 10.0f) {
        showWarning("Low fuel!");
    }
    if (ResourceManager::getEnergy() < 20.0f) {
        showWarning("Driver tired!");
    }
}
```

## Русская версия

### Обзор

**Кино-режим** — это основной опыт путешествия между локациями. Это отдельная сцена, визуализирующая поездку и запускающая дорожные события.

### Ключевые характеристики

- **Непропускаемый**: Игроки должны пережить путешествие (нет "быстрого перемещения")
- **Событийно-ориентированный**: Случайные и скриптованные события происходят во время поездки
- **Расход ресурсов**: Топливо и энергия уменьшаются в реальном времени
- **Нет свободного передвижения**: Управление A/D отключено (кинематографический опыт)
- **Визуализация прогресса**: Полоса прогресса показывает завершение путешествия

### Фазы кино-режима

**Фаза 1: Начало путешествия**
- Отображение информации о пункте назначения
- Расчёт общего расстояния путешествия
- Инициализация прогресса (0%)
- Запуск анимации прокрутки фона
- Начало потребления топлива/энергии

**Фаза 2: Активное путешествие**
- Обновление прогресса на основе времени/расстояния
- Прокрутка слоёв фона (параллакс)
- Потребление ресурсов (топливо: -0.1%/сек, энергия: -0.05%/сек)
- Проверка триггеров случайных событий
- Обновление HUD

**Фаза 3: Прибытие**
- Прогресс достигает 100%
- Затухание кинематографического вида
- Отображение "Прибыли в [Локация]"
- Переход к сцене локации
- Обновление позиции игрока на карте

### Система параллакс-прокрутки

Несколько слоёв фона прокручиваются с разными скоростями для создания глубины:

**Слои (от ближнего к дальнему)**:
- Слой 1: Передний план (объекты у дороги) - Скорость: 1.0x
- Слой 2: Средний план (деревья, знаки) - Скорость: 0.6x
- Слой 3: Задний план (холмы, здания) - Скорость: 0.3x
- Слой 4: Дальний задний план (горы) - Скорость: 0.15x
- Слой 5: Небо (облака) - Скорость: 0.05x

---


# 5. Система дорожных событий / Road Events System

## Event System Architecture

### Event Types

**1. Random Events (Случайные события)**
- Occur probabilistically during travel
- Context-sensitive (time, location, weather)
- Modifi by player archetype and companions

**2. Scripted Events (Скриптованные события)**
- Triggered by story progress
- Location-specific
- Quest-related

**3. Environmental Events (Условия окружения)**
- Weather changes
- Time of day effects
- Seasonal variations

### Event Structure

```cpp
struct RoadEvent {
    std::string id;                      // Unique identifier
    std::string title;                   // Event title
    std::string description;             // Event text
    EventType type;                      // RANDOM, SCRIPTED, ENVIRONMENTAL
    float probability;                   // Trigger chance (0.0-1.0)
    
    // Conditions
    std::vector<EventCondition> conditions;  // Requirements to trigger
    
    // Choices
    std::vector<EventChoice> choices;    // Player options
    
    // Visual
    std::string imagePath;               // Event illustration
    sf::Color backgroundColor;           // UI theme color
};

struct EventChoice {
    std::string text;                    // Choice description
    std::vector<ItemRequirement> itemReqs;  // Required items
    std::vector<SkillCheck> skillChecks;    // Ability checks
    EventOutcome outcome;                // Result of choice
};

struct EventOutcome {
    // Resource changes
    float fuelDelta;
    float energyDelta;
    int moneyDelta;
    float moraleDelta;
    
    // Inventory changes
    std::vector<ItemReward> itemsAdded;
    std::vector<ItemRequirement> itemsRemoved;
    
    // Story changes
    std::vector<std::string> flagsSet;
    std::vector<std::string> questsStarted;
    
    // NPC changes
    std::vector<NPCInteraction> npcActions;  // Add/remove companions
    
    // Next event
    std::string chainEventId;            // Optional follow-up event
};
```

### Event Examples

#### Event 1: Police Checkpoint

```
┌──────────────────────────────────────────────────────────────┐
│ 🚔 ДОРОЖНЫЙ ПАТРУЛЬ / POLICE CHECKPOINT                      │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│ "На обочине стоит патрульная машина. Офицер показывает     │
│  вам остановиться. Вас проверяют..."                        │
│                                                              │
│ "A patrol car is parked on the shoulder. An officer        │
│  signals you to pull over. They want to check your          │
│  documents..."                                               │
│                                                              │
├──────────────────────────────────────────────────────────────┤
│ Выберите действие / Choose action:                          │
│                                                              │
│ [1] Остановиться и предъявить документы                     │
│     Stop and show documents                                  │
│     Cost: 5 min, Energy -5%                                  │
│     Risk: Low                                                │
│                                                              │
│ [2] Попытаться убедить, что спешите                         │
│     Try to convince them you're in a hurry                  │
│     Requires: Sociable archetype OR high Morale              │
│     Success: Save time | Failure: Fine $25                   │
│                                                              │
│ [3] Дать взятку (ускорить процесс)                          │
│     Offer bribe (speed up process)                          │
│     Cost: $20                                                │
│     Result: Quick pass, no questions                         │
│                                                              │
│ [ESC] Cannot avoid                                           │
└──────────────────────────────────────────────────────────────┘

Current Resources:
[Fuel: ██████░░ 75%]  [Energy: ████████ 85%]  [Money: $145]
```

**Event Logic**:
```cpp
void handlePoliceCheckpoint() {
    if (choice == 1) {
        // Standard procedure
        decreaseEnergy(5.0f);
        addDelay(5 * 60);  // 5 minutes
        showMessage("Проверка завершена. Можете ехать.");
    }
    else if (choice == 2) {
        // Persuasion attempt
        bool success = checkPersuasion();
        if (success) {
            addDelay(2 * 60);  // Only 2 minutes
            increaseMorale(5.0f);
            showMessage("Офицер отпускает вас. Удачи!");
        } else {
            decreaseMoney(25);
            addDelay(10 * 60);  // 10 minutes penalty
            decreaseMorale(10.0f);
            showMessage("Вам выписали штраф. -$25");
        }
    }
    else if (choice == 3) {
        // Bribe
        if (getMoney() >= 20) {
            decreaseMoney(20);
            addDelay(1 * 60);  // Very quick
            showMessage("Офицер принимает деньги. Свободны.");
        } else {
            showMessage("Недостаточно денег!");
            handlePoliceCheckpoint();  // Show choices again
        }
    }
}
```

#### Event 2: Breakdown

```
┌──────────────────────────────────────────────────────────────┐
│ 🔧 ПОЛОМКА / BREAKDOWN                                       │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│ "Машина начинает дымить. Вы слышите странные звуки из      │
│  под капота. Кажется, проблема с двигателем..."             │
│                                                              │
│ "The car starts smoking. You hear strange sounds from       │
│  the engine. Looks like engine trouble..."                   │
│                                                              │
├──────────────────────────────────────────────────────────────┤
│ Выберите действие / Choose action:                          │
│                                                              │
│ [1] 🔧 Попытаться починить самостоятельно                   │
│     Try to fix it yourself                                   │
│     Requires: Toolbox (have: ✓) OR Mechanic companion      │
│     Success: Free repair | Failure: Need professional       │
│                                                              │
│ [2] 📞 Вызвать эвакуатор                                    │
│     Call for tow truck                                       │
│     Cost: $75 + 2 hours delay                                │
│     Result: Guaranteed fix, expensive                        │
│                                                              │
│ [3] 🚶 Оставить машину, искать помощь пешком               │
│     Abandon car, walk for help                               │
│     Cost: 3 hours, Energy -30%                               │
│     Risk: May get lost, items may be stolen                  │
│                                                              │
│ [4] ⏰ Подождать случайного проезжего                       │
│     Wait for a passing driver                                │
│     Cost: 1-4 hours (random)                                 │
│     Result: May help, may ignore, may rob                    │
│                                                              │
│ [ESC] Cannot skip this event                                 │
└──────────────────────────────────────────────────────────────┘

Inventory Check:
✓ Toolbox [2x1]  |  ✗ Spare Parts
Companions: None with Mechanic skill
```

**Decision Tree**:
```
breakdown()
├─> [1] Self-repair
│   ├─> Has Toolbox?
│   │   ├─> YES: Roll skill check (60% success)
│   │   │   ├─> Success: Free fix, +morale
│   │   │   └─> Failure: -1 hour, call tow anyway
│   │   └─> NO: -2 hours wasted, call tow
│   └─> Has Mechanic companion?
│       └─> YES: 90% success rate
│
├─> [2] Tow truck
│   ├─> Pay $75
│   ├─> Wait 2 hours
│   └─> Guaranteed fix
│
├─> [3] Walk for help
│   ├─> Roll random outcome
│   ├─> 50%: Find help (2 hours)
│   ├─> 30%: Get lost (4 hours, -energy)
│   └─> 20%: Someone steals items (-random item)
│
└─> [4] Wait
    ├─> Roll random outcome
    ├─> 40%: Helper arrives (1 hour, free/small cost)
    ├─> 40%: No one stops (4 hours wasted)
    └─> 20%: Suspicious person (potential robbery)
```

### Event Manager Implementation

```cpp
class EventManager {
private:
    std::map<std::string, RoadEvent> events;
    std::vector<std::string> activeEventPool;
    std::vector<std::string> completedEvents;
    
    float timeSinceLastEvent;
    float minEventInterval = 30.0f;  // Seconds
    
public:
    void registerEvent(const RoadEvent& event) {
        events[event.id] = event;
        if (event.type == EventType::RANDOM) {
            activeEventPool.push_back(event.id);
        }
    }
    
    void update(float deltaTime) {
        timeSinceLastEvent += deltaTime;
        
        if (timeSinceLastEvent >= minEventInterval) {
            if (shouldTriggerEvent()) {
                std::string eventId = selectRandomEvent();
                triggerEvent(eventId);
            }
            timeSinceLastEvent = 0.0f;
        }
    }
    
    bool shouldTriggerEvent() {
        float baseChance = 0.15f;  // 15% per check
        
        // Modify by archetype
        if (PlayerState::getArchetype() == "Ex-Racer") {
            baseChance *= 0.7f;  // Fewer dangerous events
        }
        
        // Modify by morale
        float morale = ResourceManager::getMorale();
        if (morale < 30.0f) {
            baseChance *= 1.5f;  // More bad events when morale low
        }
        
        return randomFloat(0.0f, 1.0f) < baseChance;
    }
    
    std::string selectRandomEvent() {
        // Filter events by conditions
        std::vector<std::string> validEvents;
        
        for (const auto& eventId : activeEventPool) {
            if (checkEventConditions(events[eventId])) {
                validEvents.push_back(eventId);
            }
        }
        
        if (validEvents.empty()) {
            return "";  // No valid events
        }
        
        // Weighted random selection
        return validEvents[randomInt(0, validEvents.size() - 1)];
    }
    
    void triggerEvent(const std::string& eventId) {
        if (events.find(eventId) == events.end()) return;
        
        // Pause cinematic mode
        pauseTravel();
        
        // Show event scene
        showEventScene(events[eventId]);
        
        // Wait for player choice
        // ... (handled by EventScene)
    }
};
```

### Event Categories

**Категория 1: Дорожные инциденты**
- Поломка машины (Breakdown)
- Прокол колеса (Flat tire)
- Авария впереди (Accident ahead)
- Дорожные работы (Road construction)

**Категория 2: Встречи**
- Полицейский патруль (Police checkpoint)
- Попутчик (Hitchhiker)
- Бродячие животные (Stray animals)
- Другие путешественники (Other travelers)

**Категория 3: Находки**
- Забытые вещи (Abandoned items)
- Придорожная распродажа (Roadside sale)
- Таинственная коробка (Mystery box)
- Сломанное авто с ценностями (Wrecked car with loot)

**Категория 4: Погода и природа**
- Гроза (Thunderstorm)
- Туман (Fog)
- Дикие животные на дороге (Wildlife on road)
- Красивый закат (Beautiful sunset - morale boost)

**Категория 5: Сюжетные**
- Квестовые триггеры (Quest triggers)
- Сюжетные развилки (Story branches)
- Встречи с ключевыми NPC (Key NPC encounters)

## Русская версия

### Архитектура системы событий

Система событий является ядром динамического геймплея. Она управляет случайными и скриптованными событиями, которые происходят во время путешествия.

### Типы событий

**Случайные события**:
- Происходят вероятностно во время поездки
- Зависят от контекста (время, локация, погода)
- Модифицируются архетипом игрока и спутниками

**Скриптованные события**:
- Запускаются прогрессом сюжета
- Специфичны для локаций
- Связаны с квестами

### Условия и последствия

Каждое событие имеет:
- **Условия активации**: требования для появления
- **Варианты выбора**: действия игрока
- **Последствия**: изменения ресурсов, предметов, сюжета

### Система проверок

**Проверки навыков**:
- Убеждение (зависит от морали и архетипа)
- Ремонт (требует инструменты или спутника-механика)
- Торговля (зависит от архетипа и спутников)

**Случайные исходы**:
- Используются для непредсказуемости
- Процентные шансы успеха/неудачи
- Разные последствия для разных исходов

---

# 6. Локации/структуры/сценарии / Locations/Structures/Scenarios

## Location System

### Location Types

**1. Junction (Развязка)**
- Basic stopping point
- 1-3 structures
- Quick services

**2. Town (Город)**
- Major hub
- 5-7 structures
- Full services
- NPC interactions
- Quest hub

**3. Rest Stop (Остановка)**
- Minimal services
- Quick rest
- Vending machines
- Restrooms

**4. Point of Interest (Уникальная локация)**
- Story-driven
- Unique scenarios
- Special rewards
- One-time events

### Structure Details

#### Gas Station (Заправка) ⛽

```
Wireframe: Gas Station Interior
┌──────────────────────────────────────────────────────────────┐
│ ⛽ JOE'S GAS STATION                          [ESC: Exit]    │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│     [🚗]  🏪  [💵]                                          │
│    PUMP  SHOP CASHIER                                        │
│                                                              │
│  > [E] Fill Tank - $50 (Full)                               │
│    Current: [████░░░░] 50%                                   │
│                                                              │
│  > [E] Shop - Buy snacks, drinks, maps                      │
│                                                              │
│  > [E] Talk to Attendant                                    │
│    "Hey there! Long trip?"                                   │
│                                                              │
│  > [ESC] Leave                                               │
│                                                              │
├──────────────────────────────────────────────────────────────┤
│ [Fuel: ████░░░░ 50%]  [Money: $185]                         │
└──────────────────────────────────────────────────────────────┘
```

**Services**:
- Fuel refill: $50 (full tank)
- Snacks: $5-15 (energy restore)
- Maps: $10 (reveal nearby nodes)
- Info: Free (hints, rumors)

**Implementation**:
```cpp
class GasStation : public Structure {
public:
    void interact() {
        showMenu({
            {"Fill Tank", [](){ refuelVehicle(); }, "$50"},
            {"Shop", [](){ openShop(); }, ""},
            {"Talk", [](){ startDialogue("attendant"); }, ""},
            {"Leave", [](){ closeMenu(); }, ""}
        });
    }
    
    void refuelVehicle() {
        if (ResourceManager::getMoney() >= 50) {
            ResourceManager::modifyMoney(-50);
            ResourceManager::setFuel(100.0f);
            showMessage("Tank filled. Ready to go!");
        } else {
            showMessage("Not enough money! ($50 required)");
        }
    }
};
```

#### General Store (Магазин) 🏪

```
Wireframe: Store Interior
┌──────────────────────────────────────────────────────────────┐
│ 🏪 GENERAL STORE                             [ESC: Exit]     │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Categories:                      Cart:                      │
│  > Food & Drinks                  [Empty]                    │
│    Tools & Equipment                                         │
│    Vehicle Parts                  Total: $0                  │
│    Medical Supplies                                          │
│    Misc Items                                                │
│                                                              │
│  ┌──────────────────────────┐                               │
│  │ FOOD & DRINKS            │                               │
│  ├──────────────────────────┤                               │
│  │ 🍔 Burger         $8     │ [Buy]                         │
│  │ 🍕 Pizza          $12    │ [Buy]                         │
│  │ ☕ Coffee         $3     │ [Buy]                         │
│  │ 🥤 Soda           $2     │ [Buy]                         │
│  │ 🥫 Canned Food    $5     │ [Buy]                         │
│  │ 🍞 Bread          $4     │ [Buy]                         │
│  │ 💧 Water Bottle   $2     │ [Buy]                         │
│  └──────────────────────────┘                               │
│                                                              │
│  [Inventory: 18/24 slots]  [Money: $185]  [Checkout]        │
└──────────────────────────────────────────────────────────────┘
```

**Inventory**:
- Food items (restore energy)
- Tools (required for events)
- Parts (for vehicle maintenance)
- Medical supplies (heal companions)
- Quest items (special missions)

**Pricing System**:
```cpp
struct StoreItem {
    std::string name;
    ItemCategory category;
    int basePrice;
    int gridWidth, gridHeight;
    bool stackable;
    int maxStack;
    float weight;
    
    int getCurrentPrice() const {
        float multiplier = 1.0f;
        
        // Trader companion bonus
        if (PlayerState::hasCompanionWithSkill("Trader")) {
            multiplier *= 0.85f;  // 15% discount
        }
        
        // Random price variation
        multiplier *= randomFloat(0.9f, 1.1f);
        
        return static_cast<int>(basePrice * multiplier);
    }
};
```

#### Mechanic Shop (Мастерская) 🔧

```
Wireframe: Mechanic Shop
┌──────────────────────────────────────────────────────────────┐
│ 🔧 MIKE'S GARAGE                             [ESC: Exit]     │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Vehicle Status:                                             │
│  ┌────────────────────────────────────────┐                 │
│  │ 🚗 Old Sedan                           │                 │
│  │ Condition: [██████░░░░] 60%            │                 │
│  │                                        │                 │
│  │ Issues:                                │                 │
│  │  ⚠️  Engine wear (minor)              │                 │
│  │  ⚠️  Brake pads (moderate)             │                 │
│  │  ✓  Tires (good)                       │                 │
│  └────────────────────────────────────────┘                 │
│                                                              │
│  Services Available:                                         │
│                                                              │
│  > [E] Quick Repair - $50                                   │
│    Fixes minor issues, +20% condition                        │
│                                                              │
│  > [E] Full Service - $150                                  │
│    Complete overhaul, +50% condition                         │
│                                                              │
│  > [E] Upgrade Vehicle - $500+                              │
│    Improve capacity, speed, efficiency                       │
│                                                              │
│  > [E] Talk to Mechanic                                     │
│    Get advice, learn about nearby issues                     │
│                                                              │
├──────────────────────────────────────────────────────────────┤
│ [Money: $185]  [Has Mechanic Companion: ✗]                  │
└──────────────────────────────────────────────────────────────┘
```

**Services**:
- Quick repair: $50 (minor fixes)
- Full service: $150 (major repairs)
- Upgrades: $500+ (permanent improvements)
- Consultation: Free (advice, info)

**Upgrade System**:
```cpp
enum class UpgradeType {
    ENGINE,          // +10% speed, +15% fuel consumption
    FUEL_TANK,       // +25% fuel capacity
    CARGO_SPACE,     // +50% inventory slots
    SUSPENSION,      // -10% breakdown chance
    TIRES,           // +5% speed on rough terrain
};

struct VehicleUpgrade {
    UpgradeType type;
    std::string name;
    std::string description;
    int cost;
    bool installed;
    
    void install() {
        switch (type) {
            case UpgradeType::ENGINE:
                PlayerState::modifySpeed(1.1f);
                PlayerState::modifyFuelConsumption(1.15f);
                break;
            case UpgradeType::CARGO_SPACE:
                GridInventorySystem::expandGrid(2, 2);
                break;
            // ... other upgrades
        }
        installed = true;
    }
};
```

#### Motel (Мотель) 🏨

```
Wireframe: Motel Room
┌──────────────────────────────────────────────────────────────┐
│ 🏨 ROADSIDE MOTEL - Room 12                  [ESC: Leave]   │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│         🛏️                                                   │
│       [BED]                                                  │
│                                                              │
│  Services:                                                   │
│                                                              │
│  > [R] Sleep (8 hours) - $20                                │
│    • Restore 100% Energy                                     │
│    • Save game                                               │
│    • Advance time                                            │
│    • Restore companion morale                                │
│                                                              │
│  > [R] Quick Nap (2 hours) - $5                             │
│    • Restore 25% Energy                                      │
│    • Shorter time penalty                                    │
│                                                              │
│  > [I] Open Inventory                                        │
│    • Organize items                                          │
│    • Drop unwanted items                                     │
│                                                              │
│  > [T] Talk to Companions                                   │
│    • Check morale                                            │
│    • Character development                                   │
│                                                              │
│  > [ESC] Leave Motel                                        │
│                                                              │
├──────────────────────────────────────────────────────────────┤
│ [Energy: ████░░░░ 45%]  [Money: $185]  [Time: 14:30]        │
└──────────────────────────────────────────────────────────────┘
```

**Rest System**:
```cpp
void sleep(RestDuration duration) {
    int cost = (duration == RestDuration::FULL) ? 20 : 5;
    
    if (ResourceManager::getMoney() < cost) {
        showMessage("Not enough money!");
        return;
    }
    
    // Pay for room
    ResourceManager::modifyMoney(-cost);
    
    // Restore energy
    if (duration == RestDuration::FULL) {
        ResourceManager::setEnergy(100.0f);
        advanceTime(8 * 60);  // 8 hours
        
        // Restore companion morale
        NPCManager::restoreAllMorale(50.0f);
        
        // Auto-save
        SaveSystem::save("auto_save_motel");
        
        showMessage("You had a good night's sleep. Feeling refreshed!");
    }
    else {  // Quick nap
        ResourceManager::modifyEnergy(25.0f);
        advanceTime(2 * 60);  // 2 hours
        showMessage("Quick power nap. Somewhat refreshed.");
    }
}
```

#### Diner (Столовая) 🍔

```
Wireframe: Diner
┌──────────────────────────────────────────────────────────────┐
│ 🍔 ROADSIDE DINER                            [ESC: Exit]     │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  "Welcome to Mama's Diner! What can I get ya?"              │
│                                                              │
│  Menu:                                                       │
│                                                              │
│  > [E] Full Meal - $15                                      │
│    Burger, fries, drink                                      │
│    +30% Energy, +10 Morale                                   │
│                                                              │
│  > [E] Coffee - $3                                          │
│    +10% Energy, Quick                                        │
│                                                              │
│  > [E] Pie - $5                                             │
│    +15% Energy, +5 Morale                                    │
│                                                              │
│  > [E] Breakfast Special - $10                              │
│    +25% Energy, Available 6AM-11AM only                      │
│                                                              │
│  > [T] Talk to Waitress                                     │
│    Get local rumors, tips                                    │
│                                                              │
│  > [T] Talk to Other Customers                              │
│    Random encounters, quests                                 │
│                                                              │
├──────────────────────────────────────────────────────────────┤
│ [Energy: ████░░░░ 45%]  [Money: $185]  [Morale: ██████ 60%] │
└──────────────────────────────────────────────────────────────┘
```

### Location Scenarios

#### Scenario 1: Town Hub

**Setup**:
- Major crossroads
- 5-7 structures available
- Multiple quests
- NPC interactions

**Flow**:
```
1. Arrive in town
2. Overview screen shows available structures:
   ⛽ Gas Station
   🏪 General Store
   🔧 Mechanic
   🏨 Motel
   🍔 Diner
   🏦 Bank (optional)
   👤 Town Square (NPCs)

3. Player navigates with A/D
4. Press E near structure to enter
5. Complete services/interactions
6. Exit to town view
7. Select next location on map or structure
```

#### Scenario 2: Random Encounter at Junction

**Setup**:
- Small junction, 1-2 structures
- Random event triggers
- Quick stop location

**Example Event**:
```
Arrive at junction
├─> Gas station available
├─> Random event: "Stranded Traveler"
│   ├─> Option 1: Offer ride (+companion, -cargo space)
│   ├─> Option 2: Give money ($20, +karma)
│   └─> Option 3: Ignore (-karma)
└─> Continue journey
```

## Русская версия

### Система локаций

Локации являются основными точками взаимодействия в игре. Каждая локация предлагает различные услуги и возможности.

### Типы локаций

**Развязка (Junction)**:
- Базовая точка остановки
- 1-3 структуры
- Быстрые услуги

**Город (Town)**:
- Крупный узел
- 5-7 структур
- Полный сервис
- Взаимодействия с NPC
- Хаб квестов

**Остановка (Rest Stop)**:
- Минимальные услуги
- Быстрый отдых
- Торговые автоматы

**Уникальная локация (POI)**:
- Сюжетно-ориентированная
- Уникальные сценарии
- Специальные награды
- Одноразовые события

### Структуры и их сервисы

Каждая структура предлагает специфические услуги:
- **Заправка**: топливо, закуски, информация
- **Магазин**: предметы, еда, инструменты
- **Мастерская**: ремонт, апгрейды
- **Мотель**: отдых, сохранение, управление командой
- **Столовая**: еда, энергия, слухи

---

# 7. Тетра-инвентарь / Tetra-Inventory

## Tarkov-Style Grid Inventory System

### Overview

The inventory system uses a **spatial 2D grid** where each item occupies specific cells based on its dimensions. This creates a puzzle-like challenge where players must optimize space usage.

### Grid Structure

**Default Configuration**:
- Grid size: 6 columns × 4 rows = 24 cells
- Cell size: 80×80 pixels
- Total capacity: Weight-based (100kg default) + spatial constraints
- Vehicle-dependent sizing (Sedan: 64 cells, Van: 124 cells, Coupe: 32 cells)

### Visual Layout

```
Wireframe: Inventory Screen (Grid System)
┌────────────────────────────────────────────────────────────────────────┐
│ INVENTORY [I to close]                                    [X] Close    │
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  Grid (6x4)                  ┌─────────────────┐    Item Details      │
│                              │[██][██][  ][  ] │                       │
│  Weight: 45.5/100 kg         │[██][██][🔧][  ] │    🔧 Wrench         │
│  Slots Used: 18/24           │[🍔][  ][🔧][💊] │    Size: 2×1 cells   │
│                              │[🍔][⛽][⛽][💊] │    Weight: 2.5 kg    │
│  Controls:                   │                 │    Value: $15        │
│  • Left-click: Pick/Place    │ (MOUSE ONLY)    │    Category: Tool    │
│  • Right-click: Rotate       │                 │                       │
│  • Drag: Move item           └─────────────────┘    "A reliable       │
│  • Hover: Show info                                   tool for car     │
│  • ESC: Close                                         repairs"         │
│                                                                        │
│  Color Legend:                                                         │
│  Green cells = Valid placement      Gray = Common                     │
│  Red cells = Invalid placement      Green = Uncommon                  │
│  Blue cells = Item selected         Blue = Rare                       │
│                                     Purple = Epic                      │
│                                     Orange = Legendary                 │
│                                                                        │
│ [Weight: 45.5/100]  [Items: 12]  [Value: $285]        [ESC: Close]   │
└────────────────────────────────────────────────────────────────────────┘

Item Legend:
[██] = 2×2 large item (spare tire, toolbox)
[🔧] = 2×1 tool (wrench, crowbar)
[🍔] = 1×1 food (burger, can)
[💊] = 1×1 medicine
[⛽] = 1×1 fuel canister
```

### Core Algorithms

#### 1. Placement Validation Algorithm

```cpp
bool GridInventorySystem::canPlaceItem(int x, int y, int width, int height, 
                                       int ignoreItemId) {
    // Step 1: Bounds check
    if (x < 0 || y < 0 || 
        x + width > m_gridWidth || 
        y + height > m_gridHeight) {
        return false;  // Outside grid boundaries
    }
    
    // Step 2: Check each cell in item's footprint
    for (int dy = 0; dy < height; ++dy) {
        for (int dx = 0; dx < width; ++dx) {
            const GridCell& cell = m_grid[y + dy][x + dx];
            
            // Cell must be empty OR belong to item being moved
            if (cell.occupied && cell.itemId != ignoreItemId) {
                return false;  // Collision detected
            }
        }
    }
    
    // Step 3: Weight check
    GridItem* item = getItemById(ignoreItemId);
    float currentWeight = getTotalWeight();
    if (item) currentWeight -= item->item.weight * item->stackCount;
    
    if (currentWeight > m_maxWeight) {
        return false;  // Exceeds weight capacity
    }
    
    return true;  // All checks passed
}
```

#### 2. Auto-Placement Algorithm

```cpp
bool GridInventorySystem::findAvailablePosition(int width, int height, 
                                                 int& outX, int& outY) {
    // First-fit algorithm: scan left-to-right, top-to-bottom
    for (int y = 0; y <= m_gridHeight - height; ++y) {
        for (int x = 0; x <= m_gridWidth - width; ++x) {
            if (canPlaceItem(x, y, width, height, -1)) {
                outX = x;
                outY = y;
                return true;  // Found first available spot
            }
        }
    }
    
    return false;  // No space available
}

// Alternative: Best-fit algorithm (more compact packing)
bool findBestPosition(int width, int height, int& outX, int& outY) {
    int bestX = -1, bestY = -1;
    int minWastedSpace = INT_MAX;
    
    for (int y = 0; y <= m_gridHeight - height; ++y) {
        for (int x = 0; x <= m_gridWidth - width; ++x) {
            if (canPlaceItem(x, y, width, height, -1)) {
                // Calculate wasted space (empty cells nearby)
                int wastedSpace = calculateWastedSpace(x, y, width, height);
                
                if (wastedSpace < minWastedSpace) {
                    minWastedSpace = wastedSpace;
                    bestX = x;
                    bestY = y;
                }
            }
        }
    }
    
    if (bestX != -1) {
        outX = bestX;
        outY = bestY;
        return true;
    }
    
    return false;
}
```

#### 3. Item Rotation Algorithm

```cpp
bool GridInventorySystem::rotateItem(int itemId) {
    GridItem* item = getItemById(itemId);
    if (!item) return false;
    
    // Step 1: Clear current cells
    clearCells(itemId);
    
    // Step 2: Swap dimensions (90° rotation)
    std::swap(item->gridWidth, item->gridHeight);
    item->isRotated = !item->isRotated;
    
    // Step 3: Check if rotated item fits
    if (canPlaceItem(item->gridX, item->gridY, 
                     item->gridWidth, item->gridHeight, itemId)) {
        // Step 4: Success - occupy new cells
        occupyCells(itemId, item->gridX, item->gridY, 
                    item->gridWidth, item->gridHeight);
        return true;
    }
    else {
        // Step 5: Failure - revert rotation
        std::swap(item->gridWidth, item->gridHeight);
        item->isRotated = !item->isRotated;
        occupyCells(itemId, item->gridX, item->gridY, 
                    item->gridWidth, item->gridHeight);
        return false;
    }
}
```

#### 4. Drag and Drop State Machine

```cpp
class GridInventorySystem {
private:
    enum class DragState {
        NONE,           // No item being dragged
        PICKING_UP,     // Mouse down, preparing to drag
        DRAGGING,       // Actively dragging item
        PLACING         // Attempting to place item
    };
    
    DragState m_dragState = DragState::NONE;
    int m_draggedItemId = -1;
    sf::Vector2i m_dragOffset;
    
public:
    void handleMousePress(int mouseX, int mouseY) {
        if (m_dragState == DragState::NONE) {
            // Try to pick up item at cursor position
            int gridX, gridY;
            screenToGrid(mouseX, mouseY, gridX, gridY);
            
            GridItem* item = getItemAtCell(gridX, gridY);
            if (item) {
                m_dragState = DragState::PICKING_UP;
                m_draggedItemId = item->id;
                m_dragOffset = sf::Vector2i(
                    mouseX - (item->gridX * m_cellSize + m_gridStartX),
                    mouseY - (item->gridY * m_cellSize + m_gridStartY)
                );
                clearCells(item->id);  // Temporarily remove from grid
            }
        }
        else if (m_dragState == DragState::DRAGGING) {
            // Try to place item
            int gridX, gridY;
            screenToGrid(mouseX, mouseY, gridX, gridY);
            
            GridItem* item = getItemById(m_draggedItemId);
            if (canPlaceItem(gridX, gridY, item->gridWidth, 
                            item->gridHeight, m_draggedItemId)) {
                // Place item at new position
                item->gridX = gridX;
                item->gridY = gridY;
                occupyCells(m_draggedItemId, gridX, gridY, 
                           item->gridWidth, item->gridHeight);
                m_dragState = DragState::NONE;
                m_draggedItemId = -1;
            }
            else {
                // Invalid placement - return to original position
                occupyCells(m_draggedItemId, item->gridX, item->gridY,
                           item->gridWidth, item->gridHeight);
                m_dragState = DragState::NONE;
                m_draggedItemId = -1;
                showMessage("Cannot place item here!");
            }
        }
    }
    
    void handleMouseMove(int mouseX, int mouseY) {
        if (m_dragState == DragState::PICKING_UP) {
            m_dragState = DragState::DRAGGING;
        }
        
        if (m_dragState == DragState::DRAGGING) {
            // Update visual feedback
            updateDragPreview(mouseX, mouseY);
        }
    }
};
```

### Item Database

```cpp
struct ItemDefinition {
    std::string id;
    std::string name;
    std::string description;
    ItemCategory category;
    int gridWidth, gridHeight;
    float weight;
    int value;
    bool stackable;
    int maxStack;
    Rarity rarity;
};

// Item catalog
std::map<std::string, ItemDefinition> itemDatabase = {
    {"burger", {
        "burger", "Burger", "Tasty fast food",
        ItemCategory::FOOD, 1, 1, 0.3f, 8,
        true, 10, Rarity::COMMON
    }},
    {"wrench", {
        "wrench", "Wrench", "Basic repair tool",
        ItemCategory::TOOL, 2, 1, 2.5f, 15,
        false, 1, Rarity::UNCOMMON
    }},
    {"spare_tire", {
        "spare_tire", "Spare Tire", "Emergency tire",
        ItemCategory::SPARE_PART, 2, 2, 8.0f, 50,
        false, 1, Rarity::UNCOMMON
    }},
    {"toolbox", {
        "toolbox", "Toolbox", "Complete tool set",
        ItemCategory::TOOL, 3, 2, 6.0f, 75,
        false, 1, Rarity::RARE
    }},
    // ... more items
};
```

### Implementation Details

**Cell Management**:
```cpp
void occupyCells(int itemId, int x, int y, int width, int height) {
    for (int dy = 0; dy < height; ++dy) {
        for (int dx = 0; dx < width; ++dx) {
            m_grid[y + dy][x + dx].occupied = true;
            m_grid[y + dy][x + dx].itemId = itemId;
        }
    }
}

void clearCells(int itemId) {
    for (int y = 0; y < m_gridHeight; ++y) {
        for (int x = 0; x < m_gridWidth; ++x) {
            if (m_grid[y][x].itemId == itemId) {
                m_grid[y][x].occupied = false;
                m_grid[y][x].itemId = -1;
            }
        }
    }
}
```

**Visual Rendering**:
```cpp
void render(sf::RenderWindow& window) {
    // Draw grid cells
    for (int y = 0; y < m_gridHeight; ++y) {
        for (int x = 0; x < m_gridWidth; ++x) {
            sf::RectangleShape cell(sf::Vector2f(m_cellSize - 2, m_cellSize - 2));
            cell.setPosition(
                m_gridStartX + x * m_cellSize,
                m_gridStartY + y * m_cellSize
            );
            
            // Color based on state
            if (m_grid[y][x].occupied) {
                cell.setFillColor(sf::Color(100, 100, 100));  // Gray
            } else {
                cell.setFillColor(sf::Color(50, 50, 50));     // Dark gray
            }
            
            cell.setOutlineColor(sf::Color(150, 150, 150));
            cell.setOutlineThickness(1);
            window.draw(cell);
        }
    }
    
    // Draw items
    for (const auto& item : m_items) {
        if (item.id == m_draggedItemId) continue;  // Skip dragged item
        
        sf::RectangleShape itemRect(
            sf::Vector2f(item.gridWidth * m_cellSize - 4,
                        item.gridHeight * m_cellSize - 4)
        );
        itemRect.setPosition(
            m_gridStartX + item.gridX * m_cellSize + 2,
            m_gridStartY + item.gridY * m_cellSize + 2
        );
        
        // Color by rarity
        itemRect.setFillColor(getRarityColor(item.item.rarity));
        itemRect.setOutlineColor(sf::Color::White);
        itemRect.setOutlineThickness(2);
        window.draw(itemRect);
        
        // Draw item name/icon
        // ...
    }
    
    // Draw dragged item (if any)
    if (m_draggedItemId != -1) {
        drawDraggedItem(window);
    }
}
```

## Русская версия

### Система тетра-инвентаря

Инвентарь использует пространственную 2D сетку, где каждый предмет занимает определённые ячейки на основе своих размеров. Это создаёт головоломку, где игроки должны оптимизировать использование пространства.

### Ключевые особенности

**Сетка по умолчанию**:
- Размер: 6 колонок × 4 ряда = 24 ячейки
- Размер ячейки: 80×80 пикселей
- Ограничение по весу: 100 кг (по умолчанию)
- Зависит от автомобиля

**Управление**:
- Левая кнопка мыши: взять/положить предмет
- Правая кнопка мыши: повернуть предмет на 90°
- Перетаскивание: переместить предмет
- Наведение: показать информацию

### Алгоритмы

**Проверка размещения**: Проверяет границы сетки, коллизии с другими предметами и ограничение по весу.

**Авто-размещение**: Алгоритм first-fit сканирует сетку слева направо, сверху вниз для поиска первого доступного места.

**Ротация предметов**: Меняет ширину и высоту предмета, проверяет коллизии в новой ориентации.

**Drag & Drop**: Управляет состояниями перетаскивания с визуальной обратной связью в реальном времени.

---

# 8. Автомобили и ограничения / Vehicles and Constraints

## Vehicle System

### Vehicle Types

| Vehicle | Seats | Inventory | Speed | Fuel Efficiency | Cost |
|---------|-------|-----------|-------|-----------------|------|
| **Old Sedan** | 3 | 64 cells (8×8) | Medium (60 mph) | Good (1.0x) | $1000 |
| **Offroad Van** | 2 | 124 cells (12×10) | Slow (50 mph) | Poor (1.3x) | $1500 |
| **Sports Coupe** | 3 | 32 cells (8×4) | Fast (75 mph) | Very Poor (1.5x) | $2000 |

### Vehicle Characteristics

```cpp
struct Vehicle {
    std::string name;
    VehicleType type;
    
    // Capacity
    int maxCompanions;        // NPC seats
    int inventoryWidth;       // Grid width
    int inventoryHeight;      // Grid height
    float maxWeight;          // kg
    
    // Performance
    float maxSpeed;           // mph
    float acceleration;       // 0-60 time
    float fuelEfficiency;     // Multiplier (1.0 = base)
    float handling;           // Cornering (unused in 2D)
    
    // Condition
    float condition;          // 0-100%
    float breakdownChance;    // Base probability
    
    // Costs
    int purchasePrice;
    int repairCostPerPercent; // Cost per 1% condition
    
    // Upgrades
    std::vector<VehicleUpgrade> installedUpgrades;
};
```

### Vehicle Selection (Character Creation)

```
Wireframe: Vehicle Selection
┌────────────────────────────────────────────────────────────────┐
│ Choose Your Vehicle                                             │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│ ► OLD SEDAN                                                    │
│   ┌──────────────────────────────────────────────────────────┐ │
│   │ [🚗 Pixel Art: Sedan]                                    │ │
│   │                                                          │ │
│   │ Stats:                                                   │ │
│   │  Speed:      ███████░░░  (Medium)                       │ │
│   │  Cargo:      ██████░░░░  (64 cells)                     │ │
│   │  Efficiency: █████████░  (Good)                         │ │
│   │  Seats:      3 companions                               │ │
│   │                                                          │ │
│   │ "Reliable and balanced. A classic choice."              │ │
│   └──────────────────────────────────────────────────────────┘ │
│                                                                │
│   OFFROAD VAN                                                  │
│   ┌──────────────────────────────────────────────────────────┐ │
│   │ [🚐 Pixel Art: Van]                                      │ │
│   │                                                          │ │
│   │ Stats:                                                   │ │
│   │  Speed:      █████░░░░░  (Slow)                         │ │
│   │  Cargo:      ██████████  (124 cells!)                   │ │
│   │  Efficiency: ███████░░░  (Poor)                         │ │
│   │  Seats:      2 companions                               │ │
│   │                                                          │ │
│   │ "Massive storage but sluggish. For hoarders."           │ │
│   └──────────────────────────────────────────────────────────┘ │
│                                                                │
│   SPORTS COUPE                                                 │
│   ┌──────────────────────────────────────────────────────────┐ │
│   │ [🏎️ Pixel Art: Coupe]                                    │ │
│   │                                                          │ │
│   │ Stats:                                                   │ │
│   │  Speed:      ██████████  (Fast!)                        │ │
│   │  Cargo:      ████░░░░░░  (32 cells)                     │ │
│   │  Efficiency: ██░░░░░░░░  (Very Poor)                    │ │
│   │  Seats:      3 companions                               │ │
│   │                                                          │ │
│   │ "Lightning fast but tiny trunk. For minimalists."       │ │
│   └──────────────────────────────────────────────────────────┘ │
│                                                                │
│ [Up/Down: Navigate] [Enter: Select] [ESC: Back]               │
└────────────────────────────────────────────────────────────────┘
```

### Constraint System

#### 1. Companion Limit

```cpp
bool canAddCompanion(const NPC& npc) {
    int currentCompanions = NPCManager::getCompanionCount();
    int maxCompanions = PlayerState::getVehicle().maxCompanions;
    
    if (currentCompanions >= maxCompanions) {
        showMessage("No room in vehicle! (Max: " + 
                    std::to_string(maxCompanions) + ")");
        return false;
    }
    
    return true;
}
```

#### 2. Cargo Space Limit

```cpp
bool canAddItem(const Item& item) {
    // Check weight
    float currentWeight = GridInventorySystem::getTotalWeight();
    float maxWeight = PlayerState::getVehicle().maxWeight;
    
    if (currentWeight + item.weight > maxWeight) {
        showMessage("Too heavy! (" + 
                    std::to_string(currentWeight) + "/" + 
                    std::to_string(maxWeight) + " kg)");
        return false;
    }
    
    // Check spatial availability
    int x, y;
    if (!GridInventorySystem::findAvailablePosition(
            item.gridWidth, item.gridHeight, x, y)) {
        showMessage("No space in trunk! Try rotating or removing items.");
        return false;
    }
    
    return true;
}
```

#### 3. Fuel Consumption

```cpp
void updateFuelConsumption(float deltaTime, float distance) {
    Vehicle& vehicle = PlayerState::getVehicle();
    
    // Base consumption: 0.1% per mile at 60 mph
    float baseConsumption = 0.1f;
    
    // Modified by vehicle efficiency
    float consumption = baseConsumption * vehicle.fuelEfficiency;
    
    // Modified by speed (higher speed = more consumption)
    float speedMultiplier = vehicle.maxSpeed / 60.0f;
    consumption *= speedMultiplier;
    
    // Modified by cargo weight (heavier = more consumption)
    float weightRatio = GridInventorySystem::getTotalWeight() / vehicle.maxWeight;
    consumption *= (1.0f + weightRatio * 0.2f);  // Up to +20%
    
    // Modified by vehicle condition (worse = more consumption)
    float conditionMultiplier = 1.0f + (1.0f - vehicle.condition / 100.0f) * 0.3f;
    consumption *= conditionMultiplier;
    
    // Apply consumption
    float fuelDelta = consumption * (distance / 1.6f);  // Convert km to miles
    ResourceManager::modifyFuel(-fuelDelta);
}
```

### Vehicle Switching System

```cpp
void switchVehicle(const Vehicle& newVehicle) {
    Vehicle& oldVehicle = PlayerState::getVehicle();
    
    // Step 1: Check if new vehicle has less cargo space
    int oldCells = oldVehicle.inventoryWidth * oldVehicle.inventoryHeight;
    int newCells = newVehicle.inventoryWidth * newVehicle.inventoryHeight;
    
    if (newCells < oldCells) {
        // Step 2: Try to repack items into new space
        if (!repackInventory(newVehicle.inventoryWidth, 
                             newVehicle.inventoryHeight)) {
            // Step 3: Items don't fit - player must drop some
            showInventoryOverflowDialog(newVehicle);
            return;  // Cancel switch until resolved
        }
    }
    
    // Step 4: Check if new vehicle has fewer companion seats
    if (newVehicle.maxCompanions < NPCManager::getCompanionCount()) {
        // Must dismiss companions
        showCompanionRemovalDialog(newVehicle);
        return;  // Cancel switch until resolved
    }
    
    // Step 5: All constraints satisfied - perform switch
    PlayerState::setVehicle(newVehicle);
    GridInventorySystem::resizeGrid(newVehicle.inventoryWidth,
                                     newVehicle.inventoryHeight);
    showMessage("Switched to " + newVehicle.name + "!");
}
```

### Condition & Breakdown System

```cpp
void updateVehicleCondition(float deltaTime) {
    Vehicle& vehicle = PlayerState::getVehicle();
    
    // Gradual wear (0.01% per hour of driving)
    float wear = 0.01f * (deltaTime / 3600.0f);
    vehicle.condition = std::max(0.0f, vehicle.condition - wear);
    
    // Check for breakdown
    float breakdownRoll = randomFloat(0.0f, 1.0f);
    float breakdownThreshold = vehicle.breakdownChance * 
                               (1.0f - vehicle.condition / 100.0f);
    
    if (breakdownRoll < breakdownThreshold) {
        triggerBreakdownEvent();
    }
    
    // Warnings
    if (vehicle.condition < 30.0f) {
        showWarning("Vehicle condition critical! Visit a mechanic.");
    }
}
```

## Русская версия

### Система автомобилей

Три типа автомобилей предлагают различные компромиссы между скоростью, грузоподъёмностью и топливной эффективностью.

### Характеристики

**Старый седан**:
- Баланс между всеми характеристиками
- 64 ячейки инвентаря (8×8)
- 3 места для попутчиков
- Хорошая топливная эффективность

**Внедорожный фургон**:
- Огромный багажник (124 ячейки, 12×10)
- Медленный и прожорливый
- Только 2 места для попутчиков
- Идеален для накопителей

**Спортивное купе**:
- Очень быстрое
- Маленький багажник (32 ячейки, 8×4)
- Очень большой расход топлива
- 3 места для попутчиков

### Система ограничений

**Лимит попутчиков**: Зависит от автомобиля. Невозможно взять больше попутчиков, чем мест.

**Лимит груза**: Ограничен весом (кг) и пространством (ячейки). Оба ограничения должны быть соблюдены.

**Расход топлива**: Зависит от типа автомобиля, скорости, веса груза и состояния машины.

**Состояние автомобиля**: Постепенно ухудшается. При критическом состоянии повышается шанс поломки.

---

# 9. NPC/диалоги/user stories / NPCs/Dialogues/User Stories

## NPC System

### NPC Structure

```cpp
struct NPC {
    std::string id;
    std::string name;
    std::string description;
    NPCRole role;
    
    // Stats
    float morale;             // 0-100%, affects performance
    int loyalty;              // 0-100, affects leaving chance
    
    // Skills
    std::vector<NPCSkill> skills;
    
    // Personality
    PersonalityType personality;
    std::vector<std::string> likes;
    std::vector<std::string> dislikes;
    
    // Dialogue
    std::map<std::string, DialogueTree> dialogues;
    
    // State
    bool isCompanion;         // Currently in party?
    sf::Vector2f position;    // For location scenes
};
```

### NPC Roles & Skills

**Mechanic (Механик)**:
```cpp
struct MechanicSkill {
    // Abilities
    float repairBonus = 25.0f;      // +25% repair success
    float costReduction = 0.15f;    // -15% repair costs
    bool canFieldRepair = true;     // Can repair during events
    
    void apply() {
        // Modify repair outcomes
        onRepairEvent([this](RepairEvent& event) {
            event.successChance += repairBonus;
            event.cost *= (1.0f - costReduction);
        });
    }
};
```

**Trader (Торговец)**:
```cpp
struct TraderSkill {
    float buyDiscount = 0.15f;      // -15% purchase prices
    float sellBonus = 0.20f;        // +20% sell prices
    bool accessRareItems = true;    // Unlocks rare shop items
    
    void apply() {
        onShopTransaction([this](Transaction& tx) {
            if (tx.type == TransactionType::BUY) {
                tx.cost *= (1.0f - buyDiscount);
            } else {
                tx.revenue *= (1.0f + sellBonus);
            }
        });
    }
};
```

**Entertainer (Душа компании)**:
```cpp
struct EntertainerSkill {
    float moraleBonus = 10.0f;       // +10% morale per hour
    float eventSuccessBonus = 0.20f; // +20% persuasion success
    bool boostsAllCompanions = true; // Affects entire party
    
    void apply() {
        onUpdate([this](float deltaTime) {
            if (boostsAllCompanions) {
                NPCManager::modifyAllMorale(moraleBonus * deltaTime / 3600.0f);
            }
        });
        
        onDialogueEvent([this](DialogueEvent& event) {
            if (event.type == DialogueType::PERSUASION) {
                event.successChance += eventSuccessBonus;
            }
        });
    }
};
```

### Morale System

```cpp
class MoraleSystem {
public:
    void updateMorale(NPC& npc, float deltaTime) {
        // Base decay: -1% per 2 hours if idle
        float decay = -0.5f * (deltaTime / 3600.0f);
        
        // Modified by player archetype
        if (PlayerState::getArchetype() == "Sociable") {
            decay *= 0.5f;  // Half decay for Sociable archetype
        }
        
        // Modified by Entertainer companion
        if (NPCManager::hasCompanionWithSkill("Entertainer")) {
            decay *= 0.0f;  // No decay with Entertainer
            npc.morale += 10.0f * (deltaTime / 3600.0f);  // Gain instead
        }
        
        // Apply morale change
        npc.morale = std::clamp(npc.morale + decay, 0.0f, 100.0f);
        
        // Check for leaving
        if (npc.morale < 20.0f && npc.loyalty < 50) {
            if (randomFloat(0.0f, 1.0f) < 0.05f) {  // 5% chance per update
                triggerCompanionLeavingEvent(npc);
            }
        }
    }
    
    void modifyMorale(NPC& npc, float delta, const std::string& reason) {
        float oldMorale = npc.morale;
        npc.morale = std::clamp(npc.morale + delta, 0.0f, 100.0f);
        
        // Show notification
        if (delta > 0) {
            showMessage(npc.name + " morale increased! (" + reason + ")");
        } else {
            showMessage(npc.name + " morale decreased. (" + reason + ")");
        }
        
        // Adjust loyalty based on morale changes
        if (npc.morale > 80.0f && npc.loyalty < 100) {
            npc.loyalty += 1;  // High morale increases loyalty
        }
    }
};
```

### Dialogue System

```cpp
struct DialogueNode {
    std::string id;
    std::string speakerName;
    std::string text;
    std::vector<DialogueChoice> choices;
    bool isEndNode;
    
    // Conditions
    std::vector<DialogueCondition> requirements;
    
    // Effects
    std::vector<DialogueEffect> effects;
};

struct DialogueChoice {
    std::string text;
    std::string nextNodeId;
    
    // Requirements
    bool requiresItem;
    std::string requiredItemId;
    bool requiresSkill;
    std::string requiredSkillName;
    bool requiresArchetype;
    std::string requiredArchetype;
    
    // Outcomes
    int moraleDelta;
    int moneyDelta;
    float fuelDelta;
    std::vector<std::string> flagsSet;
    std::vector<Item> itemsGiven;
};
```

**Dialogue Example**:
```
Wireframe: Dialogue Scene
┌────────────────────────────────────────────────────────────────┐
│ 👤 Gas Station Attendant                     [ESC: End Talk]  │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│ [Portrait: Old man with cap]                                   │
│                                                                │
│ "Welcome to Joe's Gas! Been driving long?"                     │
│                                                                │
├────────────────────────────────────────────────────────────────┤
│ Your response:                                                 │
│                                                                │
│ ► [1] "Yeah, pretty tired. Any good places to rest nearby?"   │
│                                                                │
│   [2] "Just passing through. Fill the tank, please."          │
│                                                                │
│   [3] [Sociable] "Long enough! Got any interesting stories?"  │
│       Requires: Sociable archetype                             │
│       Effect: +10 morale, unlock quest hint                    │
│                                                                │
│   [4] "Seen anything unusual on the road lately?"              │
│                                                                │
│   [ESC] "Nothing, thanks."                                     │
│                                                                │
├────────────────────────────────────────────────────────────────┤
│ [1-4: Select] [ESC: End conversation]                          │
└────────────────────────────────────────────────────────────────┘
```

### User Stories for NPCs

**As a player, I want to...**:

1. **Recruit companions with useful skills**
   - So I can get bonuses during events and services
   - So I can access special dialogue options
   - So I have tactical advantages in challenges

2. **Manage companion morale**
   - So they don't leave my party
   - So they perform better in their roles
   - So I build loyalty over time

3. **Have meaningful conversations with NPCs**
   - So I learn about the world and quests
   - So I can make choices that matter
   - So I experience different outcomes based on my archetype

4. **Build relationships with recurring NPCs**
   - So I get better prices and services
   - So I unlock unique content
   - So the world feels alive and reactive

### Companion Acquisition

```cpp
void offerCompanionship(NPC& npc) {
    // Check vehicle capacity
    if (!canAddCompanion(npc)) {
        showMessage("No room in vehicle!");
        return;
    }
    
    // Show recruitment dialog
    showDialog({
        .title = "Recruit " + npc.name + "?",
        .message = npc.description + "\n\nSkills: " + 
                   joinSkills(npc.skills) + "\n\nTake them along?",
        .choices = {
            {"Yes, join us!", [&](){ recruitCompanion(npc); }},
            {"Not right now", [](){ /* dismiss */ }}
        }
    });
}

void recruitCompanion(NPC& npc) {
    npc.isCompanion = true;
    npc.morale = 100.0f;  // Start with high morale
    npc.loyalty = 50;     // Neutral loyalty
    NPCManager::addCompanion(npc);
    
    showMessage(npc.name + " has joined your journey!");
    
    // Apply passive skills immediately
    for (const auto& skill : npc.skills) {
        skill.activate();
    }
}
```

## Русская версия

### Система NPC

NPC (не-играемые персонажи) могут стать попутчиками, предлагая уникальные способности и добавляя глубину путешествию.

### Роли NPC

**Механик**: Снижает стоимость ремонта, повышает шансы успешного ремонта, может чинить в дороге.

**Торговец**: Снижает цены покупки, повышает цены продажи, открывает доступ к редким предметам.

**Душа компании**: Постоянно повышает мораль всей команды, даёт бонусы к убеждению.

### Система морали

Мораль попутчиков постепенно снижается со временем. Низкая мораль может привести к тому, что попутчик покинет команду. Архетип "Общительный" и попутчик-Весельчак помогают поддерживать высокую мораль.

### Диалоги

Система диалогов основана на деревьях с ветвлениями. Выборы игрока влияют на:
- Мораль и лояльность NPC
- Доступ к квестам
- Получение предметов или информации
- Сюжетные флаги и развитие историй

---

# 10. Аркадный режим / Arcade Mode

## Side-View Highway Scene

### Overview

The **Side-View Scene** (also called Arcade Mode) is a side-scrolling driving experience that offers an alternative to the top-down driving view. It features full parallax scrolling, traffic system, and procedurally generated structures.

### Visual Layout

```
Wireframe: Side-View Highway Mode
┌────────────────────────────────────────────────────────────────────────┐
│ [E:███████░ 75%] [$185]     [SPD: 65]  [ODO: 125.6mi]   [F:████░░ 50%]│
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│ ☁️     ☁️           ☁️                    ☁️              ☁️          │ Sky Layer (0.05x)
│                                                                        │
│  🏔️        🏔️                  🏔️                🏔️                │ Mountains (0.15x)
│                                                                        │
│    🌲  🌲      ⛽         🌲    🌲                                     │ Trees/Structures (0.6x)
│                GAS                                                     │
│ ═══════════════════════════════════════════════════════════════════════│ Road line
│                                                                        │
│        🚗→              ←🚙                    ←🚐                    │ Traffic layer
│                                                                        │
│ ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓│ Ground
│                                                                        │
│ [A/D: Move]  [E: Interact with structures]  [I: Inventory]  [ESC]    │
└────────────────────────────────────────────────────────────────────────┘
```

### Layer System

**7-Layer Rendering Architecture**:

```cpp
enum class RenderLayer {
    LAYER_0_SKY = 0,          // Clouds, sky (slowest scroll)
    LAYER_1_FAR_BG,           // Mountains, distant objects
    LAYER_2_MID_BG,           // Hills, buildings
    LAYER_3_STRUCTURES,       // Gas stations, signs
    LAYER_4_ROAD,             // Road surface, lines
    LAYER_5_TRAFFIC,          // Vehicles (player + AI)
    LAYER_6_HUD               // UI overlay (no scroll)
};

void renderSideView(sf::RenderWindow& window) {
    // Layer 0: Sky
    renderLayer(window, LAYER_0_SKY, 0.05f);
    
    // Layer 1: Far background
    renderLayer(window, LAYER_1_FAR_BG, 0.15f);
    
    // Layer 2: Mid background
    renderLayer(window, LAYER_2_MID_BG, 0.3f);
    
    // Layer 3: Structures
    renderLayer(window, LAYER_3_STRUCTURES, 0.6f);
    
    // Layer 4: Road
    renderLayer(window, LAYER_4_ROAD, 1.0f);
    
    // Layer 5: Traffic (player + AI vehicles)
    renderTraffic(window);
    
    // Layer 6: HUD (no parallax)
    renderHUD(window);
}
```

### Parallax Scrolling

```cpp
class ParallaxLayer {
private:
    sf::Sprite m_sprite;
    float m_scrollSpeed;
    float m_position;
    
public:
    void update(float cameraSpeed, float deltaTime) {
        // Scroll at fraction of camera speed
        m_position -= cameraSpeed * m_scrollSpeed * deltaTime;
        
        // Seamless loop
        float spriteWidth = m_sprite.getTextureRect().width;
        if (m_position <= -spriteWidth) {
            m_position += spriteWidth;
        }
    }
    
    void render(sf::RenderWindow& window, float cameraX) {
        // Draw current instance
        m_sprite.setPosition(m_position + cameraX, m_sprite.getPosition().y);
        window.draw(m_sprite);
        
        // Draw next instance for seamless loop
        m_sprite.setPosition(m_position + cameraX + spriteWidth, 
                             m_sprite.getPosition().y);
        window.draw(m_sprite);
    }
};
```

### Traffic System

```cpp
class TrafficVehicle {
private:
    sf::Sprite m_sprite;
    float m_worldX;
    float m_speed;
    TrafficDirection m_direction;
    
public:
    void update(float deltaTime, float playerSpeed) {
        if (m_direction == TrafficDirection::ONCOMING) {
            // Oncoming traffic moves opposite direction
            m_worldX -= (m_speed + playerSpeed) * deltaTime;
        } else {
            // Passing traffic moves same direction but faster
            m_worldX += (m_speed - playerSpeed) * deltaTime;
        }
        
        // Remove if off-screen
        if (m_worldX < cameraX - 200 || m_worldX > cameraX + 1600) {
            markForRemoval();
        }
    }
};

class TrafficManager {
private:
    std::vector<TrafficVehicle> m_vehicles;
    float m_spawnTimer = 0.0f;
    float m_spawnInterval = 3.0f;  // Seconds between spawns
    
public:
    void update(float deltaTime, float cameraX, float playerSpeed) {
        // Update existing vehicles
        for (auto& vehicle : m_vehicles) {
            vehicle.update(deltaTime, playerSpeed);
        }
        
        // Remove off-screen vehicles
        m_vehicles.erase(
            std::remove_if(m_vehicles.begin(), m_vehicles.end(),
                [](const TrafficVehicle& v) { return v.shouldRemove(); }),
            m_vehicles.end()
        );
        
        // Spawn new vehicles
        m_spawnTimer += deltaTime;
        if (m_spawnTimer >= m_spawnInterval) {
            spawnRandomVehicle(cameraX);
            m_spawnTimer = 0.0f;
        }
    }
    
    void spawnRandomVehicle(float cameraX) {
        TrafficVehicle vehicle;
        
        // Random direction
        if (randomFloat(0.0f, 1.0f) < 0.6f) {
            // Oncoming traffic (60% chance)
            vehicle.m_direction = TrafficDirection::ONCOMING;
            vehicle.m_worldX = cameraX + 1400;  // Spawn ahead
            vehicle.m_speed = randomFloat(55.0f, 75.0f);
        } else {
            // Passing traffic (40% chance)
            vehicle.m_direction = TrafficDirection::PASSING;
            vehicle.m_worldX = cameraX - 200;  // Spawn behind
            vehicle.m_speed = randomFloat(70.0f, 85.0f);
        }
        
        m_vehicles.push_back(vehicle);
    }
};
```

### Structure Placement (Procedural)

```cpp
class StructurePlacement {
private:
    std::vector<Structure> m_structures;
    float m_lastStructureX = 0.0f;
    float m_minSpacing = 800.0f;
    float m_maxSpacing = 2000.0f;
    
public:
    void update(float cameraX) {
        // Spawn new structures ahead of camera
        while (m_lastStructureX < cameraX + 2000.0f) {
            spawnRandomStructure();
        }
        
        // Remove structures behind camera
        m_structures.erase(
            std::remove_if(m_structures.begin(), m_structures.end(),
                [cameraX](const Structure& s) { 
                    return s.worldX < cameraX - 500.0f; 
                }),
            m_structures.end()
        );
    }
    
    void spawnRandomStructure() {
        float spacing = randomFloat(m_minSpacing, m_maxSpacing);
        m_lastStructureX += spacing;
        
        StructureType type = selectRandomStructureType();
        Structure structure;
        structure.worldX = m_lastStructureX;
        structure.type = type;
        structure.isActive = false;
        
        m_structures.push_back(structure);
    }
    
    StructureType selectRandomStructureType() {
        float roll = randomFloat(0.0f, 1.0f);
        
        if (roll < 0.30f) return StructureType::GAS_STATION;
        if (roll < 0.50f) return StructureType::DINER;
        if (roll < 0.65f) return StructureType::MOTEL;
        if (roll < 0.80f) return StructureType::STORE;
        if (roll < 0.90f) return StructureType::MECHANIC;
        return StructureType::BILLBOARD;
    }
};
```

### Interaction System

```cpp
void checkStructureInteraction() {
    float playerX = m_playerWorldX;
    const float interactionRange = 150.0f;
    
    for (auto& structure : m_structures) {
        float distance = std::abs(structure.worldX - playerX);
        
        if (distance < interactionRange) {
            // Show interaction prompt
            structure.highlighted = true;
            showPrompt("Press E to enter " + structure.getName());
            
            // Handle E key press
            if (isKeyPressed(sf::Keyboard::E)) {
                enterStructure(structure);
            }
        } else {
            structure.highlighted = false;
        }
    }
}

void enterStructure(const Structure& structure) {
    // Pause side-view mode
    m_paused = true;
    
    // Transition to structure scene
    switch (structure.type) {
        case StructureType::GAS_STATION:
            SceneManager::transitionTo(SceneType::GAS_STATION);
            break;
        case StructureType::DINER:
            SceneManager::transitionTo(SceneType::DINER);
            break;
        // ... other structure types
    }
}
```

## Русская версия

### Аркадный режим (Вид сбоку)

Альтернативный игровой режим с видом сбоку, предлагающий аркадный опыт вождения с параллакс-прокруткой и трафиком.

### Особенности

**7-слойная система рендеринга**:
- Слой 0: Небо и облака (медленная прокрутка)
- Слой 1: Далёкий фон (горы)
- Слой 2: Средний фон (холмы)
- Слой 3: Структуры (заправки, здания)
- Слой 4: Дорога
- Слой 5: Транспорт (игрок и AI)
- Слой 6: HUD (без прокрутки)

**Система трафика**:
- Встречный транспорт (движется навстречу)
- Обгоняющий транспорт (движется быстрее игрока)
- Процедурная генерация автомобилей
- Автоматическая очистка за кадром

**Размещение структур**:
- Процедурная генерация через 800-2000 единиц
- 8 типов структур (заправка, столовая, мотель и др.)
- Интерактивные (нажать E для входа)
- Автоматическая очистка позади камеры

---

# 11. Архетипы персонажа / Character Archetypes

## Archetype System

### Overview

At the start of the game, players choose a **character archetype** that permanently influences gameplay. This choice cannot be changed and affects events, resource management, and NPC interactions throughout the journey.

### Archetype Definitions

#### 1. Ex-Racer (Бывший гонщик)

```cpp
struct ExRacerArchetype {
    // Passive bonuses
    float dangerousEventReduction = 0.30f;   // -30% dangerous events
    float racingSuccessBonus = 0.25f;        // +25% racing event success
    int startingMoney = 250;                 // $50 more than base
    
    // Starting items
    std::vector<std::string> startingItems = {
        "racing_gloves",      // +10% vehicle handling
        "turbo_mod",          // Speed upgrade item
        "sponsor_card"        // Discount at mechanics
    };
    
    // Event modifiers
    void modifyEvent(RoadEvent& event) {
        if (event.category == EventCategory::DANGEROUS) {
            event.probability *= (1.0f - dangerousEventReduction);
        }
        if (event.category == EventCategory::RACING) {
            for (auto& choice : event.choices) {
                choice.successChance += racingSuccessBonus;
            }
        }
    }
};
```

**Benefits**:
- Fewer breakdowns and accidents
- Better outcomes in speed-related events
- Unique starting equipment
- Special dialogue options with mechanics

**Drawbacks**:
- Normal energy consumption
- No social bonuses

#### 2. Nomad (Кочевник)

```cpp
struct NomadArchetype {
    // Passive bonuses
    float energyConsumptionReduction = 0.25f;  // -25% energy drain
    float sleepEfficiencyBonus = 0.30f;        // +30% energy from rest
    int startingFuel = 100;                    // Start with full tank
    
    // Starting items
    std::vector<std::string> startingItems = {
        "sleeping_bag",       // Can rest in car (free)
        "water_filter",       // Purify water (+10 uses)
        "road_map"            // Reveals nearby nodes
    };
    
    // Resource modifiers
    void modifyEnergyConsumption(float& energyDelta) {
        energyDelta *= (1.0f - energyConsumptionReduction);
    }
    
    void modifyRestOutcome(float& energyRestored) {
        energyRestored *= (1.0f + sleepEfficiencyBonus);
    }
};
```

**Benefits**:
- Energy lasts longer
- Better rest outcomes
- Can sleep in vehicle (free rest option)
- Experienced traveler perks

**Drawbacks**:
- No combat/racing bonuses
- No social bonuses

#### 3. Sociable/Hitchhiker (Общительный)

```cpp
struct SociableArchetype {
    // Passive bonuses
    float moraleBonus = 20.0f;                 // +20 starting morale
    float moraleDecayReduction = 0.50f;        // -50% morale decay
    float persuasionBonus = 0.35f;             // +35% dialogue success
    
    // Starting items
    std::vector<std::string> startingItems = {
        "guitar",             // Morale boost item (+15/use)
        "joke_book",          // Random morale events
        "friendship_bracelet" // NPC loyalty bonus
    };
    
    // NPC modifiers
    void modifyCompanionMorale(NPC& npc, float& moraleDelta) {
        if (moraleDelta < 0) {
            moraleDelta *= (1.0f - moraleDecayReduction);
        }
    }
    
    void modifyDialogueOutcome(DialogueChoice& choice) {
        if (choice.requiresPersuasion) {
            choice.successChance += persuasionBonus;
        }
    }
    
    void modifyPrices(Transaction& tx) {
        if (tx.type == TransactionType::BUY) {
            tx.cost *= 0.90f;  // 10% discount
        }
    }
};
```

**Benefits**:
- High companion morale (always happy)
- Better dialogue outcomes
- Trading bonuses
- Easy companion recruitment
- Unique social events

**Drawbacks**:
- No mechanical bonuses
- Normal energy consumption

### Archetype Selection Screen

```
Wireframe: Archetype Selection
┌────────────────────────────────────────────────────────────────┐
│ CHOOSE YOUR BACKGROUND                                          │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│ ► EX-RACER                                                     │
│   ┌──────────────────────────────────────────────────────────┐ │
│   │ 🏎️ "You live for the thrill of speed..."                │ │
│   │                                                          │ │
│   │ Strengths:                                               │ │
│   │  • Fewer dangerous road events (-30%)                    │ │
│   │  • Better racing outcomes (+25%)                         │ │
│   │  • Unique racing equipment                               │ │
│   │  • Mechanic shop discounts                               │ │
│   │                                                          │ │
│   │ Starting Bonus: $250, Racing Gloves, Turbo Mod          │ │
│   └──────────────────────────────────────────────────────────┘ │
│                                                                │
│   NOMAD                                                        │
│   ┌──────────────────────────────────────────────────────────┐ │
│   │ 🎒 "The road is your home..."                            │ │
│   │                                                          │ │
│   │ Strengths:                                               │ │
│   │  • Energy lasts longer (-25% drain)                      │ │
│   │  • Better rest outcomes (+30%)                           │ │
│   │  • Can sleep in car (free rest)                          │ │
│   │  • Full fuel tank at start                               │ │
│   │                                                          │ │
│   │ Starting Bonus: Full Fuel, Sleeping Bag, Road Map       │ │
│   └──────────────────────────────────────────────────────────┘ │
│                                                                │
│   SOCIABLE                                                     │
│   ┌──────────────────────────────────────────────────────────┐ │
│   │ 🤝 "You make friends wherever you go..."                 │ │
│   │                                                          │ │
│   │ Strengths:                                               │ │
│   │  • High companion morale (always)                        │ │
│   │  • Better dialogue success (+35%)                        │ │
│   │  • Trading discounts (10%)                               │ │
│   │  • Easy companion recruitment                            │ │
│   │                                                          │ │
│   │ Starting Bonus: $200, Guitar, Friendship Bracelet       │ │
│   └──────────────────────────────────────────────────────────┘ │
│                                                                │
│ [Up/Down: Select] [Enter: Confirm] [ESC: Back]                │
│                                                                │
│ ⚠️ This choice is PERMANENT and cannot be changed!            │
└────────────────────────────────────────────────────────────────┘
```

## Implementation Notes

**Archetype Persistence**:
```cpp
class PlayerState {
private:
    static std::string s_archetype;  // Set once, never changes
    
public:
    static void setArchetype(const std::string& archetype) {
        if (!s_archetype.empty()) {
            throw std::runtime_error("Archetype already set!");
        }
        s_archetype = archetype;
    }
    
    static const std::string& getArchetype() {
        return s_archetype;
    }
    
    static bool isArchetype(const std::string& archetype) {
        return s_archetype == archetype;
    }
};
```

**Event System Integration**:
```cpp
void EventManager::triggerEvent(const std::string& eventId) {
    RoadEvent event = getEvent(eventId);
    
    // Apply archetype modifiers
    std::string archetype = PlayerState::getArchetype();
    if (archetype == "Ex-Racer") {
        ExRacerArchetype::modifyEvent(event);
    } else if (archetype == "Nomad") {
        NomadArchetype::modifyEvent(event);
    } else if (archetype == "Sociable") {
        SociableArchetype::modifyEvent(event);
    }
    
    // Display event
    showEventScene(event);
}
```

## Русская версия

### Система архетипов

В начале игры игрок выбирает архетип персонажа, который постоянно влияет на геймплей. Этот выбор нельзя изменить.

### Три архетипа

**Бывший гонщик**: Меньше опасных событий, лучше в гонках, уникальное снаряжение.

**Кочевник**: Энергия расходуется медленнее, лучше отдых, может спать в машине.

**Общительный**: Высокая мораль команды, лучше диалоги, скидки в торговле.

---

# 12. Механика ресурсов / Resource Mechanics

## Resource Management System

### Core Resources

#### 1. Fuel (Топливо)

```cpp
class FuelSystem {
private:
    float m_currentFuel = 100.0f;   // Percentage (0-100%)
    float m_maxFuel = 100.0f;        // Liters
    float m_basConsumption = 0.1f;  // % per mile
    
public:
    void consumeFuel(float distance, const Vehicle& vehicle) {
        // Calculate consumption
        float consumption = m_baseConsumption;
        
        // Modified by vehicle efficiency
        consumption *= vehicle.fuelEfficiency;
        
        // Modified by speed
        float speedFactor = vehicle.currentSpeed / 60.0f;
        consumption *= speedFactor;
        
        // Modified by cargo weight
        float weightRatio = getTotalCargoWeight() / vehicle.maxWeight;
        consumption *= (1.0f + weightRatio * 0.2f);
        
        // Modified by condition
        consumption *= (1.0f + (1.0f - vehicle.condition/100.0f) * 0.3f);
        
        // Apply consumption
        m_currentFuel -= consumption * distance;
        m_currentFuel = std::max(0.0f, m_currentFuel);
        
        // Warnings
        if (m_currentFuel < 10.0f && m_currentFuel > 0.0f) {
            showWarning("Low fuel!");
        } else if (m_currentFuel == 0.0f) {
            triggerOutOfFuelEvent();
        }
    }
    
    void refuel(float amount) {
        m_currentFuel = std::min(m_maxFuel, m_currentFuel + amount);
    }
    
    float getFuelPercentage() const {
        return (m_currentFuel / m_maxFuel) * 100.0f;
    }
};
```

#### 2. Energy (Энергия)

```cpp
class EnergySystem {
private:
    float m_currentEnergy = 100.0f;   // Percentage
    float m_drainRate = 1.0f;         // % per hour
    
public:
    void update(float deltaTime) {
        // Base drain
        float drain = m_drainRate * (deltaTime / 3600.0f);
        
        // Modified by archetype
        if (PlayerState::isArchetype("Nomad")) {
            drain *= 0.75f;  // 25% reduction
        }
        
        // Apply drain
        m_currentEnergy -= drain;
        m_currentEnergy = std::max(0.0f, m_currentEnergy);
        
        // Warnings and effects
        if (m_currentEnergy < 20.0f) {
            showWarning("Driver exhausted!");
            increaseBreakdownChance(0.1f);
        } else if (m_currentEnergy == 0.0f) {
            forceSleep();
        }
    }
    
    void restore(float amount, RestType type) {
        float multiplier = 1.0f;
        
        // Modified by archetype
        if (PlayerState::isArchetype("Nomad")) {
            multiplier = 1.30f;  // 30% bonus
        }
        
        // Modified by rest type
        if (type == RestType::MOTEL) {
            multiplier *= 1.0f;  // Normal
        } else if (type == RestType::CAR) {
            multiplier *= 0.7f;  // Less effective
        } else if (type == RestType::DINER_COFFEE) {
            multiplier *= 0.3f;  // Quick boost only
        }
        
        m_currentEnergy += amount * multiplier;
        m_currentEnergy = std::min(100.0f, m_currentEnergy);
    }
};
```

#### 3. Money (Деньги)

```cpp
class MoneySystem {
private:
    int m_currentMoney = 200;  // Starting amount (varies by archetype)
    
public:
    bool canAfford(int cost) const {
        return m_currentMoney >= cost;
    }
    
    bool spend(int amount, const std::string& reason) {
        if (!canAfford(amount)) {
            showMessage("Not enough money! Need $" + std::to_string(amount));
            return false;
        }
        
        m_currentMoney -= amount;
        logTransaction("SPENT", amount, reason);
        return true;
    }
    
    void earn(int amount, const std::string& source) {
        m_currentMoney += amount;
        logTransaction("EARNED", amount, source);
        showMessage("+$" + std::to_string(amount) + " (" + source + ")");
    }
    
    int getMoney() const {
        return m_currentMoney;
    }
};
```

#### 4. Morale (Мораль)

```cpp
class MoraleSystem {
private:
    float m_teamMorale = 70.0f;  // Average of all companions
    
public:
    void update(float deltaTime) {
        // Calculate average morale
        float totalMorale = 0.0f;
        int companionCount = 0;
        
        for (auto& companion : NPCManager::getCompanions()) {
            totalMorale += companion.morale;
            companionCount++;
        }
        
        if (companionCount > 0) {
            m_teamMorale = totalMorale / companionCount;
        } else {
            m_teamMorale = 100.0f;  // Solo = max morale
        }
    }
    
    float getMoraleMultiplier() const {
        // High morale = bonuses, low morale = penalties
        if (m_teamMorale >= 80.0f) {
            return 1.2f;  // +20% to success rates
        } else if (m_teamMorale >= 50.0f) {
            return 1.0f;  // Normal
        } else if (m_teamMorale >= 30.0f) {
            return 0.8f;  // -20% penalty
        } else {
            return 0.6f;  // -40% penalty
        }
    }
};
```

### Resource Display (HUD)

```
┌───────────────────────────────────────────────────────────────┐
│ ╔═══════════════════════════════════════════════════════════╗ │
│ ║ [E] ████████░░ 80%     [SPD: 65]      [F] ████████░░ 85% ║ │
│ ║                                                           ║ │
│ ║ [$] $185               [ODO: 125.6]   [T] NORMAL         ║ │
│ ║                                                           ║ │
│ ║ [M] ██████░░░░ 60%     [MESSAGE]      [⚠] ---            ║ │
│ ╚═══════════════════════════════════════════════════════════╝ │
└───────────────────────────────────────────────────────────────┘

Legend:
[E] = Energy (0-100%)
[$] = Money (dollars)
[F] = Fuel (0-100%)
[T] = Temperature (status)
[M] = Morale (team average, 0-100%)
[SPD] = Speed (mph)
[ODO] = Odometer (miles traveled)
```

### Resource Interactions

**Fuel + Energy Trade-off**:
- Low energy increases breakdown chance
- Breakdown consumes fuel (idling)
- Must balance driving vs resting

**Money Management**:
- Fuel: $50 (full tank)
- Food: $3-15 (energy restore)
- Motel: $20 (full rest + save)
- Repairs: $50-200 (vehicle maintenance)
- Upgrades: $500+ (permanent improvements)

**Morale Impact**:
- High morale: +20% success in events
- Low morale: -40% success, companions may leave
- Affected by: rest, food, events, archetype

## Русская версия

### Система управления ресурсами

Четыре основных ресурса: топливо, энергия, деньги и мораль. Все они взаимосвязаны и требуют постоянного баланса.

**Топливо**: Расходуется при движении. Зависит от автомобиля, скорости, веса груза и состояния.

**Энергия**: Медленно снижается со временем. Низкая энергия повышает шанс поломки.

**Деньги**: Используются для покупки топлива, еды, ремонта и апгрейдов.

**Мораль**: Средняя мораль команды. Влияет на успех в событиях.

---

# 13. Управление и ограничения / Controls and Constraints

## Control Scheme

### Gameplay Controls (STRICT)

```
┌──────────────────────────────────────────────────────────────┐
│ MOVEMENT (Gameplay Only)                                      │
├──────────────────────────────────────────────────────────────┤
│ A        - Move LEFT (ONLY)                                   │
│ D        - Move RIGHT (ONLY)                                  │
│                                                              │
│ ⚠️ FORBIDDEN:                                                 │
│ Arrow keys, W, S - NOT allowed during gameplay              │
├──────────────────────────────────────────────────────────────┤
│ INTERACTION (Gameplay Only)                                   │
├──────────────────────────────────────────────────────────────┤
│ E        - Interact with structures/objects (ONLY)           │
│                                                              │
│ ⚠️ FORBIDDEN:                                                 │
│ Space key - NOT allowed for interaction                     │
├──────────────────────────────────────────────────────────────┤
│ SYSTEM KEYS                                                   │
├──────────────────────────────────────────────────────────────┤
│ I        - Open/Close Inventory                              │
│ T        - Town menu (quick access)                          │
│ M        - Motel menu (quick access)                         │
│ O        - Dialogue (testing)                                │
│ ESC      - Pause / Back / Close                              │
├──────────────────────────────────────────────────────────────┤
│ INVENTORY (Mouse ONLY within bounds)                         │
├──────────────────────────────────────────────────────────────┤
│ Left-click    - Pick up / Place item                         │
│ Right-click   - Rotate item 90°                              │
│ Drag          - Move item (hold left-click)                  │
│ Hover         - Show item details                            │
│ ESC           - Close inventory                              │
│                                                              │
│ ⚠️ Mouse restricted to inventory bounds ONLY                 │
├──────────────────────────────────────────────────────────────┤
│ MENU NAVIGATION                                               │
├──────────────────────────────────────────────────────────────┤
│ Up/Down  - Navigate menu options                             │
│ Enter    - Select option                                     │
│ ESC      - Back / Cancel                                     │
└──────────────────────────────────────────────────────────────┘
```

### Implementation (Enforced in Code)

```cpp
// DrivingController.cpp - ONLY A/D allowed
void DrivingController::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        // ALLOWED keys
        if (event.key.code == sf::Keyboard::A) {
            m_movingLeft = true;
        }
        else if (event.key.code == sf::Keyboard::D) {
            m_movingRight = true;
        }
        
        // Arrow keys, W, S are IGNORED (not processed)
        // Space is IGNORED for gameplay
    }
}

// DrivingScene.cpp - ONLY E for interaction
void DrivingScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::E) {
            checkStructureInteraction();
        }
        
        // Space key is IGNORED
    }
}

// InventoryScene.cpp - Mouse ONLY within bounds
void InventoryScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        int mouseX = event.mouseMove.x;
        int mouseY = event.mouseMove.y;
        
        // Enforce boundary restriction
        if (!isMouseInInventoryBounds(mouseX, mouseY)) {
            // Ignore mouse events outside inventory
            return;
        }
        
        // Process mouse movement
        updateHover(mouseX, mouseY);
    }
}
```

### Constraints Summary

**Movement Constraints**:
- Gameplay: ONLY A/D keys
- Menus: Up/Down arrow keys
- Inventory: Mouse ONLY

**Interaction Constraints**:
- Gameplay: ONLY E key
- Inventory: Mouse clicks ONLY
- Dialogues: Number keys OR Up/Down + Enter

**Mouse Constraints**:
- Inventory: Restricted to grid bounds
- Gameplay: Not used (except quick menus)
- Menus: Optional (keyboard preferred)

## Русская версия

### Схема управления

**Движение (геймплей)**: Только A (влево) и D (вправо). Стрелки и W/S запрещены.

**Взаимодействие (геймплей)**: Только клавиша E. Пробел запрещён.

**Инвентарь**: Только мышь внутри границ сетки. Левая кнопка — взять/положить. Правая — повернуть.

**Системные клавиши**: I (инвентарь), ESC (пауза/назад), T (город), M (мотель).

**Меню**: Стрелки вверх/вниз, Enter (выбор), ESC (назад).

---

# 14. User Stories (игрок, сценарист, тестер)

## Player User Stories

### As a player, I want to...

**Navigation & Exploration**:
1. **Select routes by clicking on the map**
   - So I can plan my journey visually
   - So I see fuel costs before confirming
   - Acceptance: Mouse-driven route selection with confirmation dialog

2. **Experience non-skippable travel events**
   - So every journey feels meaningful
   - So I must engage with the story
   - Acceptance: Cinematic mode cannot be skipped, events must be resolved

3. **Stop only at designated locations**
   - So the game has strategic depth
   - So I must plan fuel and energy carefully
   - Acceptance: No free stopping between nodes

**Inventory Management**:
4. **Organize items in a spatial grid**
   - So inventory management is a puzzle
   - So I must think about what to carry
   - Acceptance: Tarkov-style grid with rotation and collision

5. **See immediate visual feedback**
   - So I know if an item fits before placing
   - So I understand constraints
   - Acceptance: Green/red cell highlighting, weight display

**Companions & NPCs**:
6. **Recruit companions with useful skills**
   - So I get tactical advantages
   - So I can access special content
   - Acceptance: Mechanic, Trader, Entertainer skills working

7. **Maintain companion morale**
   - So they don't abandon me
   - So they perform better
   - Acceptance: Morale system affecting performance and loyalty

**Progression**:
8. **Choose a permanent archetype**
   - So my playstyle is defined
   - So replays feel different
   - Acceptance: Three archetypes with meaningful differences

9. **Make meaningful choices in events**
   - So my decisions matter
   - So outcomes vary based on my resources/skills
   - Acceptance: Events with 2-4 choices, varied outcomes

## Scenario Writer User Stories

### As a scenario writer, I want to...

**Content Creation**:
1. **Add new events easily**
   - So I can expand content without coding
   - So I can test scenarios quickly
   - Acceptance: JSON/data-driven event system

2. **Create branching dialogues**
   - So NPCs feel alive
   - So player choices lead to different outcomes
   - Acceptance: Dialogue tree system with conditions

3. **Define event conditions clearly**
   - So events trigger at the right time
   - So prerequisites are explicit
   - Acceptance: Condition system (items, flags, archetypes)

**Documentation**:
4. **Use wireframes as reference**
   - So I understand UI layout
   - So I know where text appears
   - Acceptance: All scenes have ASCII wireframes

5. **Follow standardized templates**
   - So events are consistent
   - So integration is smooth
   - Acceptance: Event/dialogue/location templates

**Testing**:
6. **Test events in isolation**
   - So I can debug without full playthrough
   - So iteration is fast
   - Acceptance: Event testing mode/debug commands

## QA Tester User Stories

### As a QA tester, I want to...

**Functional Testing**:
1. **Test all event branches**
   - So no path is broken
   - So outcomes are correct
   - Acceptance: All event choices lead to valid outcomes

2. **Verify inventory constraints**
   - So exploits are prevented
   - So weight/space limits work
   - Acceptance: Cannot exceed limits, clear error messages

3. **Check resource calculations**
   - So fuel/energy/money are accurate
   - So no infinite resources possible
   - Acceptance: All calculations match specifications

**Edge Cases**:
4. **Test edge scenarios**
   - So game handles unusual situations
   - So no crashes occur
   - Test cases:
     - Out of fuel mid-journey
     - Zero money at gas station
     - Full inventory when finding items
     - No companion seats when recruiting
     - Zero energy while driving

5. **Verify control restrictions**
   - So forbidden keys are truly ignored
   - So only allowed inputs work
   - Acceptance: Arrow keys don't move in gameplay, Space doesn't interact

**UI/UX**:
6. **Check readability at 1366x768**
   - So all text is legible
   - So UI elements don't overlap
   - Acceptance: Manual check of all scenes

7. **Verify color coding**
   - So status indicators are clear
   - So warnings are visible
   - Acceptance: Green=good, Yellow=warning, Red=critical

**Automation**:
8. **Run automated test suites**
   - So regression is caught early
   - So common bugs are prevented
   - Acceptance: Unit tests for core systems

### Test Checklist

```
┌──────────────────────────────────────────────────────────────┐
│ QA TEST CHECKLIST                                             │
├──────────────────────────────────────────────────────────────┤
│ CORE GAMEPLAY                                                 │
│ [ ] Route selection works                                    │
│ [ ] Travel consumes fuel correctly                           │
│ [ ] Events trigger during travel                             │
│ [ ] E key interaction works at all structures                │
│ [ ] A/D movement works in gameplay scenes                    │
│                                                              │
│ INVENTORY                                                     │
│ [ ] Items can be placed in grid                              │
│ [ ] Items can be rotated                                     │
│ [ ] Collision detection prevents overlap                     │
│ [ ] Weight limit is enforced                                 │
│ [ ] Mouse restricted to grid bounds                          │
│                                                              │
│ RESOURCES                                                     │
│ [ ] Fuel depletes with travel                                │
│ [ ] Energy depletes over time                                │
│ [ ] Money transactions work                                  │
│ [ ] Out-of-fuel triggers event                               │
│ [ ] Out-of-energy forces rest                                │
│                                                              │
│ NPCS & COMPANIONS                                             │
│ [ ] Companions can be recruited                              │
│ [ ] Vehicle capacity limit enforced                          │
│ [ ] Morale system works                                      │
│ [ ] Skills provide correct bonuses                           │
│ [ ] Low morale triggers leaving event                        │
│                                                              │
│ EVENTS                                                        │
│ [ ] Events trigger randomly                                  │
│ [ ] Choices lead to correct outcomes                         │
│ [ ] Item requirements checked                                │
│ [ ] Archetype modifiers applied                              │
│ [ ] Cannot skip events                                       │
│                                                              │
│ UI & CONTROLS                                                 │
│ [ ] All text readable at 1366x768                            │
│ [ ] Color coding correct (green/yellow/red)                  │
│ [ ] Arrow keys ignored in gameplay                           │
│ [ ] Space key ignored in gameplay                            │
│ [ ] ESC closes menus/scenes properly                         │
│                                                              │
│ EDGE CASES                                                    │
│ [ ] Out of fuel handled correctly                            │
│ [ ] Zero money prevents purchases                            │
│ [ ] Full inventory prevents pickup                           │
│ [ ] Max companions prevents recruitment                      │
│ [ ] Vehicle switch with overflow handled                     │
└──────────────────────────────────────────────────────────────┘
```

## Русская версия

### User Stories

**Игрок**: Хочу выбирать маршруты на карте, управлять пространственным инвентарём, нанимать попутчиков со способностями, делать значимые выборы в событиях.

**Сценарист**: Хочу легко добавлять события, создавать ветвящиеся диалоги, использовать wireframes как reference, следовать стандартизированным шаблонам.

**Тестер**: Хочу тестировать все ветви событий, проверять ограничения инвентаря, верифицировать расчёты ресурсов, тестировать edge-cases, запускать автоматизированные тесты.

---

# 15. Архитектура классов/C++ / Class Architecture/C++

## Core Class Hierarchy

```cpp
// Scene.h - Base interface
class Scene {
public:
    virtual ~Scene() = default;
    virtual void handleInput(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual SceneType getNextScene() const = 0;
    virtual bool isFinished() const = 0;
};

// SceneManager.h - Scene coordinator
class SceneManager {
private:
    std::unique_ptr<Scene> m_currentScene;
    sf::RenderWindow& m_window;
public:
    void run();
    void transitionTo(SceneType type);
};

// PlayerState.h - Global state singleton
class PlayerState {
private:
    static std::string s_archetype;
    static Vehicle s_vehicle;
    static float s_distanceTraveled;
public:
    static void setArchetype(const std::string& archetype);
    static const std::string& getArchetype();
    // ... other methods
};

// ResourceManager.h - Resource tracking
class ResourceManager {
private:
    static float s_fuel;
    static float s_energy;
    static int s_money;
    static float s_morale;
public:
    static float getFuel();
    static void modifyFuel(float delta);
    // ... other methods
};

// GridInventorySystem.h - Spatial inventory
class GridInventorySystem {
private:
    std::vector<std::vector<GridCell>> m_grid;
    std::vector<GridItem> m_items;
    int m_gridWidth, m_gridHeight;
    float m_maxWeight;
public:
    bool canPlaceItem(int x, int y, int w, int h, int ignoreId);
    bool placeItem(int itemId, int x, int y);
    bool rotateItem(int itemId);
    void render(sf::RenderWindow& window);
};

// EventManager.h - Event system
class EventManager {
private:
    std::map<std::string, RoadEvent> m_events;
    std::vector<std::string> m_activePool;
public:
    void registerEvent(const RoadEvent& event);
    void triggerEvent(const std::string& eventId);
    std::string selectRandomEvent();
};

// NPCManager.h - Companion management
class NPCManager {
private:
    static std::vector<NPC> s_companions;
public:
    static void addCompanion(const NPC& npc);
    static void removeCompanion(const std::string& npcId);
    static void updateAllMorale(float deltaTime);
    static bool hasCompanionWithSkill(const std::string& skill);
};
```

## Class Relationships

```
SceneManager
    ├─► Scene (interface)
    │   ├─► MainMenuScene
    │   ├─► CharacterCreationScene
    │   ├─► RouteSelectionScene
    │   ├─► DrivingScene
    │   │   └─► uses: PlayerState, ResourceManager, StartZone
    │   ├─► SideViewScene
    │   │   └─► uses: StructurePlacement, TrafficManager, HUDPanel
    │   ├─► TownScene
    │   ├─► MotelScene
    │   ├─► DialogueScene
    │   ├─► InventoryScene
    │   │   └─► uses: GridInventorySystem
    │   ├─► CinematicEventScene
    │   │   └─► uses: EventManager
    │   └─► PauseMenuScene, GameOverScene
    │
    └─► uses: PlayerState (singleton)

PlayerState (singleton)
    ├─► Vehicle
    └─► Archetype data

ResourceManager (singleton)
    └─► Tracks: fuel, energy, money, morale

GridInventorySystem
    ├─► GridCell[][]
    └─► GridItem[]
        └─► Item (base data)

EventManager
    ├─► RoadEvent[]
    │   ├─► EventChoice[]
    │   └─► EventOutcome
    └─► EventCondition[]

NPCManager (singleton)
    └─► NPC[]
        ├─► NPCSkill[]
        └─► DialogueTree
```

---

# 16. UI: wireframe, стандарты, цвета / UI: Wireframes, Standards, Colors

## UI Design Standards

### Color Palette

```cpp
// colors.h
namespace Colors {
    // Status colors
    const sf::Color GOOD = sf::Color(50, 205, 50);      // LimeGreen
    const sf::Color WARNING = sf::Color(255, 215, 0);   // Gold
    const sf::Color CRITICAL = sf::Color(220, 20, 60);  // Crimson
    
    // UI elements
    const sf::Color BORDER = sf::Color(200, 200, 200);  // LightGray
    const sf::Color BG_DARK = sf::Color(30, 30, 30);    // DarkGray
    const sf::Color BG_PANEL = sf::Color(50, 50, 50);   // Gray
    const sf::Color TEXT = sf::Color(255, 255, 255);    // White
    const sf::Color TEXT_DIM = sf::Color(150, 150, 150);// DimGray
    
    // Inventory rarity
    const sf::Color COMMON = sf::Color(128, 128, 128);  // Gray
    const sf::Color UNCOMMON = sf::Color(0, 255, 0);    // Lime
    const sf::Color RARE = sf::Color(0, 112, 221);      // DodgerBlue
    const sf::Color EPIC = sf::Color(163, 53, 238);     // DarkOrchid
    const sf::Color LEGENDARY = sf::Color(255, 128, 0); // Orange
    
    // Interactive
    const sf::Color HOVER = sf::Color(100, 149, 237);   // CornflowerBlue
    const sf::Color SELECTED = sf::Color(255, 215, 0);  // Gold
}
```

### Typography

```cpp
// Fonts
- Primary: "Press Start 2P" (pixel font)
- Monospace: "Jersey 10" (HUD displays)
- Size: 12-16px for body, 20-24px for headers
```

### Layout Grid

```
Window: 1366×768
├─ Margins: 20px
├─ Panel padding: 10px
└─ Element spacing: 8px

HUD Layout:
┌────────┬────────────────┬────────┐
│  LEFT  │     CENTER     │ RIGHT  │
│ 300px  │    766px       │ 300px  │
└────────┴────────────────┴────────┘
```

### Wireframe Templates

All UI scenes follow these wireframe standards (see stages 3-10 for specific examples).

---

# 17. Тесты и чек-листы / Tests and Checklists

## Test Categories

### Unit Tests (C++)

```cpp
// test_inventory.cpp
TEST(GridInventorySystem, PlacementValidation) {
    GridInventorySystem inventory(6, 4);
    
    // Valid placement
    ASSERT_TRUE(inventory.canPlaceItem(0, 0, 2, 1, -1));
    
    // Out of bounds
    ASSERT_FALSE(inventory.canPlaceItem(5, 0, 2, 1, -1));
    
    // Collision
    inventory.placeItem(createItem(2, 1), 0, 0);
    ASSERT_FALSE(inventory.canPlaceItem(1, 0, 2, 1, -1));
}

TEST(ResourceManager, FuelConsumption) {
    ResourceManager::setFuel(100.0f);
    ResourceManager::modifyFuel(-25.0f);
    ASSERT_FLOAT_EQ(ResourceManager::getFuel(), 75.0f);
}

TEST(EventManager, ArchetypeModifiers) {
    PlayerState::setArchetype("Ex-Racer");
    RoadEvent event = EventManager::getEvent("breakdown");
    ASSERT_LT(event.probability, 0.15f);  // Reduced for Ex-Racer
}
```

### Integration Tests

```
- Scene transitions work correctly
- Resource persistence across scenes
- Save/load maintains all state
- Event outcomes modify resources
- Companion skills apply bonuses
```

### Manual Test Checklist

See Stage 14 for complete QA checklist (60+ test cases).

---

# 18. Roadmap

## Phase 1: Core Foundation ✅ COMPLETE

- [x] Scene-based architecture
- [x] Main menu and character creation
- [x] Route selection
- [x] Driving scene (top-down)
- [x] Side-view scene (arcade mode)
- [x] Basic resource system
- [x] HUD implementation
- [x] Start zone generation

## Phase 2: Game Systems ✅ COMPLETE

- [x] Grid inventory system (Tarkov-style)
- [x] Event system (road events)
- [x] NPC/companion system
- [x] Morale system
- [x] Dialogue system
- [x] Structure interactions
- [x] Vehicle system
- [x] Archetype system

## Phase 3: Content Expansion 🚧 IN PROGRESS

- [ ] 50+ unique events
- [ ] 20+ NPC companions
- [ ] 15+ location types
- [ ] Quest system
- [ ] Achievement system
- [ ] Multiple endings
- [ ] Extended dialogue trees

## Phase 4: Polish & Balance

- [ ] Performance optimization
- [ ] Balance tuning (resource costs, event probabilities)
- [ ] Additional vehicle types
- [ ] More archetypes
- [ ] Enhanced graphics/sprites
- [ ] Sound effects and music
- [ ] Localization improvements

## Phase 5: Post-Launch

- [ ] Modding support
- [ ] Community events
- [ ] DLC content
- [ ] Steam Workshop integration
- [ ] Speedrun mode
- [ ] Hardcore difficulty

---

# 19. FAQ

## Gameplay Questions

**Q: Can I skip travel between locations?**
A: No, the cinematic travel mode is non-skippable by design. Events may occur during travel.

**Q: How do I change my character archetype?**
A: You cannot. The archetype is chosen at game start and is permanent.

**Q: Can I carry unlimited items?**
A: No. You're limited by grid space (varies by vehicle) and weight (100kg default).

**Q: What happens if I run out of fuel?**
A: A breakdown event triggers. You may need to call for help or walk to find fuel.

**Q: Can companions leave my party?**
A: Yes, if their morale drops too low and loyalty is insufficient.

## Technical Questions

**Q: Why mouse-only for inventory?**
A: It's part of the Tarkov-style design. Spatial inventory requires precise positioning.

**Q: Why can't I use arrow keys for movement?**
A: Design choice. A/D keys enforce simple, consistent controls.

**Q: How do I add new events?**
A: Use the EventManager::registerEvent() API with event data structures.

**Q: Can I mod the game?**
A: Not officially yet, but the architecture supports it (planned for Phase 5).

## Development Questions

**Q: What's the project structure?**
A: See Stage 2 (Architecture) for detailed breakdown.

**Q: How do I build the project?**
A: `mkdir build && cd build && cmake .. && make`

**Q: Where's the documentation?**
A: You're reading it! Also see `def/documentation/` for additional docs.

---

# 20. Приложение: макеты и edge-case / Appendix: Mockups and Edge Cases

## Critical Edge Cases

### 1. Out of Fuel Mid-Journey

**Scenario**: Player confirms travel but fuel reaches 0% before arrival.

**Handling**:
```cpp
if (ResourceManager::getFuel() <= 0.0f && !arrivedAtDestination) {
    pauseTravel();
    triggerEvent("out_of_fuel_breakdown");
    // Options: Call tow truck ($75), walk for help, wait for passerby
}
```

### 2. Vehicle Downgrade with Inventory Overflow

**Scenario**: Switching to smaller vehicle when inventory exceeds new capacity.

**Handling**:
```cpp
void switchVehicle(Vehicle& newVehicle) {
    if (newVehicle.inventorySlots < getCurrentItemCount()) {
        showInventoryManagementDialog();
        // Player must drop items until they fit
        while (getCurrentItemCount() > newVehicle.inventorySlots) {
            waitForPlayerToDropItem();
        }
    }
    applyVehicleSwitch(newVehicle);
}
```

### 3. Maximum Companions with Recruitment Opportunity

**Scenario**: Event offers companion but vehicle is full.

**Handling**:
```cpp
if (NPCManager::getCompanionCount() >= vehicle.maxCompanions) {
    showDialog("No room in vehicle. Dismiss a companion first?");
    // Options: Dismiss someone, decline recruitment
}
```

### 4. Zero Money at Critical Service

**Scenario**: Out of fuel, no money for gas station.

**Handling**:
```cpp
if (ResourceManager::getMoney() < 50 && needsFuel) {
    // Alternative options unlock:
    // - Sell items from inventory
    // - Accept odd jobs at location
    // - Take a loan (with interest)
    // - Game over (last resort)
}
```

### 5. All Companions Leave (Low Morale)

**Scenario**: All companions abandon party due to low morale.

**Handling**:
```cpp
if (NPCManager::getCompanionCount() == 0) {
    // Solo journey mode
    // - No companion bonuses
    // - Morale always 100% (self-sufficient)
    // - Certain events unavailable
}
```

## UI Mockups

All wireframes are documented in stages 3-10 and stage 16.

## Performance Notes

- Target: 60 FPS at 1366×768
- Grid inventory: O(1) lookups, O(W×H) placement checks
- Event system: O(N) filtering, where N = active event count
- Parallax rendering: 7 layers, hardware-accelerated

## Build Instructions

```bash
# Ubuntu/Debian
sudo apt-get install libsfml-dev cmake build-essential
mkdir build && cd build
cmake ..
make
cd ..
./build/kkurs

# Alternative: Use provided scripts
./scripts/build.sh
./scripts/run.sh
```

## Project Statistics

- Lines of code: ~8,000 (C++)
- Source files: ~50
- Header files: ~50
- Scene types: 12
- Event types: 30+ (extensible)
- NPC companions: 10+ (extensible)
- Structure types: 8
- Vehicle types: 3
- Archetype types: 3
- Documentation: 4,000+ lines

---

# Заключение / Conclusion

## Summary

**Road Trip Adventure RPG** is a comprehensive 2D road trip adventure game featuring:

✅ **Complete scene-based architecture**
✅ **Tarkov-style spatial inventory**
✅ **Dynamic event system**
✅ **NPC companion management**
✅ **Resource management mechanics**
✅ **Character archetypes**
✅ **Arcade mode (side-scrolling)**
✅ **Procedural generation**
✅ **Comprehensive documentation**

## Getting Started

1. Clone repository
2. Install dependencies (SFML 2.6)
3. Build with CMake
4. Run `./build/kkurs`
5. Choose archetype
6. Begin journey!

## Contributing

This project follows strict coding standards:
- Scene-based architecture
- Mouse-first UI (inventory)
- A/D movement, E interaction
- Comprehensive wireframes
- Extensible systems

## License

[Add license information here]

## Credits

Development Team: [Add credits]
Graphics: Pixel art style
Fonts: Press Start 2P, Jersey 10
Framework: SFML 2.6

---

**Version**: 1.0  
**Last Updated**: November 2024  
**Status**: Core systems complete, content expansion in progress

**Документация готова для командного ревью, расширения, автоматизации и тестирования.**
**Documentation ready for team review, expansion, automation, and testing.**

