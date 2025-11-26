# 🚗 KKURS - Road Trip Adventure

**2D RPG о путешествии по дорогам России 90-х годов**

Navigate highways, manage resources, build your team, complete quests.

---

## 🚀 Quick Start

### Prerequisites
- C++17 compiler (GCC 9+ / Clang 10+ / MSVC 2019+)
- SFML 2.6
- CMake 3.10+

### Build & Run

```bash
# Clone repository
git clone https://github.com/barzikol/kkurs.git
cd kkurs

# Install SFML
# Ubuntu/Debian:
sudo apt-get install libsfml-dev

# macOS (Homebrew):
brew install sfml@2

# Build
mkdir build && cd build

# macOS:
cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml@2/lib/cmake/SFML

# Linux:
cmake ..

# Compile
make -j$(nproc)

# Run
./kkurs
```

---

## 🎮 Gameplay

### Game Flow

```
Main Menu → Character Creation → Node Menu:
  ├─ Map (travel to next node)
  ├─ Character (stats & progression)
  ├─ Inventory (Tarkov-style grid)
  ├─ Companions (team management)
  ├─ Quests (quest journal)
  ├─ Ability Tree (skills)
  └─ Structures:
      ├─ Gas Station (refuel, shop)
      ├─ Store (buy supplies)
      ├─ Garage (repairs)
      ├─ Diner (food, energy)
      ├─ Motel (rest, save)
      └─ Mechanic (advanced repairs)
```

### Character Creation

**3 Archetypes:**

| Archetype | Money | Car | Bonus |
|-----------|-------|-----|-------|
| Ex-Racer (Гонщик) | 2000₽ | BMW | +20% fuel efficiency |
| Nomad (Безработный) | 800₽ | Volvo | Event resistance |
| Sociable (Студент) | 1500₽ | Mercedes | +30% diplomacy |

**3 Car Types:**

| Car | Fuel Tank | Cargo | Consumption |
|-----|-----------|-------|-------------|
| BMW | 70L | 64kg | 2.2 L/100km |
| Volvo | 60L | 55kg | 1.8 L/100km |
| Mercedes | 65L | 50kg | 2.0 L/100km |

### Core Mechanics

- **Resource Management** - Fuel, money, energy
- **Grid Inventory** - Tarkov-style (6x4 grid, drag & drop, rotation)
- **NPC Team** - Recruit companions with unique abilities
- **Quest System** - Multiple objectives, rewards
- **Random Events** - Dynamic road encounters
- **Ability Tree** - 5 categories of skills
- **Save/Load** - JSON-based saves

---

## 🎯 Features

### ✅ Implemented

- **Scene System** - 13+ game scenes
- **Resource Management** - Fuel/Money/Energy tracking
- **Grid Inventory** - Tarkov-style with drag & drop
- **NPC System** - Companions with levels and abilities
- **Dialogue System** - Branching conversations
- **Quest System** - 6 objective types
- **Event System** - 5+ random events
- **Experience & Levels** - Progressive leveling (100 * level^1.5)
- **Ability Tree** - Unlock skills with points
- **Save/Load** - JSON saves
- **HUD** - Animated resource bars
- **UI Components** - Modular UI system

### 🔄 In Progress

- Упрощенное меню Node (все структуры в одном меню)
- DialogueScene (базовая реализация)
- MotelScene (отдых и сохранение)
- GameOverScene (экран окончания игры)

### 📋 Planned

- More content (20+ events, 15+ quests, 15+ NPCs)
- Graphics & sprites (cars, locations, UI)
- Audio system (music, sound effects)
- Advanced features (weather, day/night cycle)

---

## 🕹️ Controls

### Main Menu
- **↑↓ / W/S** - Navigate
- **Enter** - Select
- **Escape** - Exit

### Node Menu (Simplified)
- **↑↓** - Navigate menu
- **Enter** - Select option
- **M** - Quick access to Map
- **C** - Quick access to Character
- **I** - Quick access to Inventory
- **P** - Quick access to Party (Companions)
- **Q** - Quick access to Quests
- **T** - Quick access to Ability Tree
- **Escape** - Close/Back

### Map Scene
- **Mouse Click** - Select node
- **Escape** - Back to Node Menu

### Inventory (Grid)
- **Mouse** - Drag & drop items
- **Right Click** - Rotate item
- **Escape** - Close

### Other Scenes
- **↑↓** - Navigate
- **Enter** - Confirm
- **Escape** - Back

---

## 📚 Project Structure

```
kkurs/
├── src/              # C++ source files
├── include/          # Header files
├── assets/           # Game assets
│   └── fonts/       # Fonts
├── data/             # Game data
│   └── maps/        # Map files
├── tests/            # Unit tests (Google Test)
├── build/            # Build directory (gitignored)
├── CMakeLists.txt    # CMake configuration
├── README.md         # This file
├── DOCUMENTATION.md  # Full user documentation
├── ROADMAP.md        # Development roadmap
└── TECHNICAL.md      # Technical documentation
```

---

## 🛠️ Development

### Architecture

**Pattern:** Scene-Based State Machine
**Language:** C++17
**Framework:** SFML 2.6
**Build System:** CMake
**Testing:** Google Test (88/97 passing, 91%)

### Core Systems

1. **SceneManager** - Scene transitions & lifecycle
2. **PlayerState** - Resources & character data
3. **InventorySystem** - Simple + Grid inventory
4. **ExperienceSystem** - Leveling & skill points
5. **AbilityTreeSystem** - Skill unlocking
6. **QuestManager** - Quest tracking & completion
7. **EventManager** - Random road events
8. **DialogueManager** - Branching dialogues
9. **SaveSystem** - JSON-based saves

### 8 Core Principles

This project follows **strict development principles**:

1. **Immediate Functionality** - No TODO comments, everything works
2. **Independence** - No undocumented dependencies
3. **Strict Adherence** - Implement exactly as specified
4. **Unified Plan** - Follow ROADMAP.md
5. **Clean Structure** - Organized documentation
6. **UI + Assets** - UI implemented immediately
7. **Modular Structure** - Clear separation of concerns
8. **Documentation** - Keep docs updated

See `DOCUMENTATION.md` for details.

---

## 📖 Documentation

- **README.md** (this file) - Quick start & overview
- **DOCUMENTATION.md** - Full game documentation
- **ROADMAP.md** - Development plan & TODO
- **TECHNICAL.md** - Technical code documentation

---

## 🧪 Testing

```bash
cd build
ctest --output-on-failure
```

**Current Status:** 88/97 tests passing (91%)

---

## 📊 Project Status

**Version:** Alpha
**Completion:** ~75%
**Phase:** 5 - Advanced Integration
**Next Milestone:** Content expansion & UI polish

### Timeline

- ✅ Phase 0-4: Core systems (Complete)
- 🔄 Phase 5: Integration (Current)
- ⏳ Phase 6: Content expansion
- ⏳ Phase 7: Graphics & UI
- ⏳ Phase 8: Audio & polish
- ⏳ Phase 9: Testing & release

See `ROADMAP.md` for detailed plan.

---

## 🤝 Contributing

This is a learning project following strict architectural principles. See `DOCUMENTATION.md` (section "8 Core Principles") before contributing.

---

## 📝 License

[To be determined]

---

## 🔗 Links

- **Repository:** https://github.com/barzikol/kkurs
- **Issues:** https://github.com/barzikol/kkurs/issues

---

**Last Updated:** 2025-11-26
**Status:** Active Development
