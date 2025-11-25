# 🚀 KKURS — Быстрая справка

## 📋 Что это за проект?

**69 Crossroads** — 2D автоприключенческая RPG игра в стиле Russia 90s.

**Кратко**: Путешествуйте по карте, управляйте ресурсами, собирайте команду NPC, выполняйте квесты, принимайте решения.

**Технологии**: C++17, SFML 2.6, CMake

---

## 🏗️ Архитектура

**Scene-Based State Machine** — вся игра состоит из сцен.

### Базовый интерфейс Scene:
- `handleInput()` — обработка ввода
- `update(deltaTime)` — обновление логики
- `render()` — отрисовка
- `getNextScene()` — следующая сцена
- `isFinished()` — завершена ли сцена

### Типы сцен (18):
- MAIN_MENU, CHARACTER_CREATION, WORLD_MAP, NODE, TRAVEL_CINEMATIC
- LOCATION, CINEMATIC_EVENT, REST, ABILITY_TREE, INVENTORY
- DIALOGUE, PAUSE_MENU, GAME_OVER, EXIT

### SceneManager:
Управляет сценами и игровым циклом:
```
Input → Update → Check Finish → Transition → Render → Repeat
```

---

## 📦 Основные системы

### 1. PlayerState (Состояние игрока)
**Файлы**: `PlayerState.h/.cpp`

**Хранит**:
- Ресурсы: energy, money, fuel, vehicleCondition
- Персонаж: origin, carType
- Игровое состояние: currentNodeId, inventory

**Ключевые методы**:
- `getEnergy()`, `setEnergy()`, `addEnergy()`
- `getMoney()`, `canAfford(cost)`
- `getFuel()`, `addFuel()`
- `initializeResources()`

---

### 2. InventorySystem (Инвентарь)
**Файлы**: `InventorySystem.h/.cpp`, `Item.h`

**Характеристики**:
- 24 слота (по умолчанию)
- Ограничение веса (100.0)
- Автоматическое складывание в стопки

**Item**:
- name, description, category, weight, value
- isStackable, maxStackSize, rarity

**Методы**:
- `addItem(item)` — добавить предмет
- `removeItem(slotIndex)` — удалить
- `hasItem(name)` — проверить наличие
- `getTotalWeight()` — общий вес

---

### 3. NPC System (Попутчики)
**Файлы**: `NPC.h/.cpp`

**NPC класс**:
- id, name, bio, level, experience
- passiveAbility, activeAbility
- attachedQuests

**NPCManager** (синглтон):
- `addNPC()` — добавить в команду
- `removeNPC()` — убрать из команды
- `hasSpace()` — есть место?
- `addExperienceToTeam()` — опыт команде

**NPCFactory**:
- `createMechanic()` — механик
- `createTrader()` — торговец
- `createCompanion()` — компаньон
- `createMedic()` — медик
- `createNavigator()` — навигатор

---

### 4. DialogueManager (Диалоги)
**Файлы**: `DialogueManager.h/.cpp`

**Структура**:
- DialogueTree → nodes (DialogueNode)
- DialogueNode → choices (DialogueChoice)
- DialogueChoice → conditions, consequences

**Методы** (синглтон):
- `registerDialogue(id, tree)` — зарегистрировать
- `startDialogue(id)` — начать диалог
- `selectChoice(index)` — выбрать вариант
- `getCurrentNode()` — текущий узел

---

### 5. EventManager (События)
**Файлы**: `EventManager.h/.cpp`

**GameEvent**:
- id, title, description
- condition (условия срабатывания)
- choices (варианты действий)

**Методы**:
- `addEvent(event)` — добавить событие
- `getRandomEvent(fuel, energy, money)` — получить событие
- `applyChoice(choice, &fuel, &energy, &money)` — применить выбор

---

### 6. ExperienceSystem + AbilityTree
**Файлы**: `ExperienceSystem.h/.cpp`, `AbilityTreeSystem.h/.cpp`

**ExperienceSystem**:
- level, experience, skillPoints
- `addExperience()`, `levelUp()`

**AbilityTreeSystem** (синглтон):
- `unlockAbility(id)` — разблокировать
- `canUnlock(id)` — можно?
- `getTotalBonus(type)` — суммарный бонус

---

### 7. QuestManager (Квесты)
**Файлы**: `QuestManager.h/.cpp`

**Quest**:
- id, title, description, status
- objectives (список целей)
- rewards (money, experience, item)

**Методы**:
- `addQuest()`, `startQuest()`
- `updateObjective()`, `completeQuest()`

---

### 8. SaveSystem (Сохранение)
**Файлы**: `SaveSystem.h/.cpp`, `GameStateManager.h/.cpp`

**GameState**:
- PlayerState (ресурсы, персонаж)
- Inventory, Team NPC, Quests, Abilities
- playTime

**Методы**:
- `saveGame()` → savegame.json
- `loadGame()` ← savegame.json

---

## 🎮 Основные сцены

### MainMenuScene
Главное меню: New Game, Continue, Options, Exit

### CharacterCreationScene
Выбор происхождения и машины

### MapScene / WorldMapScene
Карта мира с узлами, выбор следующей точки

### NodeScene
Геймплей в узле (город): движение A/D, взаимодействие E

### TravelCinematicScene
Анимация путешествия между узлами, события

### LocationScene
Универсальная локация: заправка, магазин, мастерская, мотель, закусочная

### CinematicEventScene
Показ события с выборами

### RestScene
Отдых в мотеле, сохранение

### AbilityTreeScene
Выбор и разблокировка способностей

### InventoryScene
Просмотр и управление инвентарём

---

## 🔄 Как данные передаются

### Пример 1: Покупка предмета
```
LocationScene (магазин)
  ↓
PlayerState.canAfford(10)?
  ↓
InventorySystem.addItem(item)
  ↓
PlayerState.addMoney(-10)
  ↓
UI обновляется
```

### Пример 2: Событие
```
TravelCinematicScene
  ↓
EventManager.getRandomEvent()
  ↓
CinematicEventScene (показ)
  ↓
Игрок выбирает
  ↓
EventManager.applyChoice()
  ↓
PlayerState изменяется
```

---

## 🚀 Быстрый старт

### Сборка:
```bash
git clone https://github.com/barzikol/kkurs.git
cd kkurs
mkdir build && cd build
cmake ..
make
./kkurs
```

### Зависимости:
```bash
# Ubuntu/Debian
sudo apt-get install libsfml-dev cmake build-essential

# MacOS
brew install sfml cmake
```

---

## 📂 Структура файлов

```
kkurs/
├── src/              # .cpp файлы
│   ├── main.cpp     # Точка входа
│   ├── *Scene.cpp   # Реализация сцен
│   └── *System.cpp  # Реализация систем
│
├── include/         # .h файлы
│   ├── Scene.h      # Базовый интерфейс
│   ├── *Scene.h     # Заголовки сцен
│   └── *System.h    # Заголовки систем
│
├── assets/          # Ресурсы (шрифты)
├── images/          # Изображения
├── tests/           # Тесты
├── scripts/         # Скрипты сборки
└── CMakeLists.txt   # CMake конфиг
```

---

## 📝 Как добавить...

### Новую сцену:
1. Создать `NewScene.h` и `NewScene.cpp`
2. Наследоваться от `Scene`
3. Реализовать 5 методов
4. Добавить в `SceneType` enum
5. Добавить в `SceneManager::createScene()`

### Нового NPC:
```cpp
auto npc = NPCFactory::createMechanic("id");
NPCManager::getInstance().addNPC(std::move(npc));
```

### Новое событие:
```cpp
GameEvent event;
event.id = "event_id";
event.title = "Название";
event.description = "Описание...";
// choices, conditions...
eventManager.addEvent(event);
```

### Новый диалог:
```cpp
DialogueTree tree;
tree.id = "dialogue_id";
// nodes, choices...
DialogueManager::getInstance().registerDialogue(tree.id, tree);
```

---

## 🎯 Ключевые файлы

**Архитектура**:
- `Scene.h`, `SceneManager.h/.cpp`, `PlayerState.h/.cpp`

**Системы**:
- `InventorySystem.h/.cpp`, `NPC.h/.cpp`
- `DialogueManager.h/.cpp`, `EventManager.h/.cpp`

**Сцены**:
- `MainMenuScene.cpp`, `NodeScene.cpp`, `LocationScene.cpp`

---

## ⚠️ Статус реализации

✅ **Готово**: Scene architecture, PlayerState, InventorySystem, NPC, Dialogue, Events, Experience, Quests, Save/Load

⚠️ **Частично**: GridInventorySystem (только заголовок), контент (диалоги, события, квесты)

❌ **TODO**: Система репутации, динамическая экономика, crafting, погода, день/ночь

---

## 📖 Полная документация

**Детальный анализ**: `CODE_STRUCTURE_ANALYSIS.md` (1411 строк)
**Основная документация**: `README.md`

---

**Быстрая справка для разработчиков проекта KKURS**
**Дата**: 2025-11-18 (обновлено) | **Версия**: 1.1
