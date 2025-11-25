# 🚗 KKURS - 69 Crossroads (Россия 90-х)

**A 2D Road Trip Adventure RPG** - Navigate highways, manage resources, interact with NPCs, and experience procedurally generated journey in Russia 90s setting.

2D автоприключенческая RPG в антураже России 90-х годов. Путешествуйте по дорогам, управляйте ресурсами и командой, взаимодействуйте с NPC, выполняйте квесты.

---

## 🚀 Quick Start

### Prerequisites
- C++17 compiler (GCC 9+ / Clang 10+ / MSVC 2019+)
- SFML 2.6
- CMake 3.10+

### Build & Run
```bash
mkdir build && cd build
cmake ..
make
./kkurs
```

### macOS Build
See [docs/guides/MACOS_BUILD_GUIDE.md](docs/guides/MACOS_BUILD_GUIDE.md)

---

## 📊 Project Status

**Current Sprint:** CRUNCH MODE  
**Delivery:** <7 days  
**Focus:** Working game over perfect code

### Completed ✅
- Scene-based architecture
- PlayerState system
- Basic inventory
- NPC framework
- Dialogue system
- Event system
- Save/Load functionality
- UI components (HUD, menus)

### In Progress 🔄
- NodeScene menu interface (separate branch)
- English UI localization
- Content creation (NPCs, quests, locations)

### Planned 📋
- CharacterScene
- CompanionsScene
- QuestsScene
- Grid inventory (Tarkov-style)
- Travel cinematic scene

**See [PROJECT_STATUS.md](PROJECT_STATUS.md) for detailed status**

---

## 🏗️ Architecture

**Pattern:** Scene-Based State Machine  
**Language:** C++17  
**Framework:** SFML 2.6  
**Build System:** CMake

### Core Systems
1. **PlayerState** - Resources, character stats
2. **InventorySystem** - 24-slot inventory with weight management
3. **NPC System** - Companions with levels and abilities
4. **DialogueManager** - Branching dialogue trees
5. **EventManager** - Random events with conditions
6. **ExperienceSystem** - Level progression and ability tree
7. **QuestManager** - Quest objectives and rewards
8. **SaveSystem** - JSON-based save/load
9. **UI System** - HUD, menus, resource bars

### Scene Types (18+)
- MainMenuScene, CharacterCreationScene, MapScene
- NodeScene, TravelCinematicScene, LocationScene
- InventoryScene, AbilityTreeScene, GameOverScene
- And more...

---

## 📚 Documentation

### Essential
- **[PROJECT_STATUS.md](PROJECT_STATUS.md)** - Current project status
- **[PROJECT_ANALYSIS_COMPLETE.md](PROJECT_ANALYSIS_COMPLETE.md)** - Complete analysis & cleanup plan
- **[docs/README.md](docs/README.md)** - Documentation index

### Guides
- **[docs/guides/](docs/guides/)** - User and developer guides
  - NPC Recruitment Guide
  - Save System Guide
  - macOS Build Guide
  - UI/HUD Guide
  - Events List

### Specifications
- **[docs/specifications/](docs/specifications/)** - Technical specifications
  - Car Specifications
  - World Map Format
  - Driving Scene Specification
  - Location Specifications

### Technical Documentation
- **[docs/tech/](docs/tech/)** - Architecture and technical docs
- **[docs/8_CORE_PRINCIPLES.md](docs/8_CORE_PRINCIPLES.md)** - Project principles

---

## 🎮 Gameplay Features

### Core Mechanics
- **Resource Management** - Fuel, money, energy
- **Inventory System** - 24 slots, weight-based
- **NPC Companions** - Recruit and manage team
- **Dialogue System** - Branching conversations
- **Quest System** - Objectives and rewards
- **Random Events** - Dynamic road encounters
- **Character Progression** - Experience and ability tree
- **Save/Load** - Save your journey

### Game Flow
```
Main Menu → Character Creation → World Map →
  → Node (Menu) → Select Action:
     - Map (travel)
     - Character (stats)
     - Inventory
     - Companions
     - Quests
     - Ability Tree
     - Structures (gas station, store, motel)
```

---

## 🛠️ Development

### Repository Structure
```
kkurs/
├── src/              # Source code (.cpp)
├── include/          # Headers (.h/.hpp)
├── assets/           # Game assets
├── data/             # Game data (maps, configs)
├── scripts/          # Build/utility scripts
├── tests/            # Test code
├── docs/             # Documentation
│   ├── guides/       # User guides
│   ├── specifications/  # Technical specs
│   ├── tech/         # Technical docs
│   ├── tasks/        # Task planning
│   └── progress/     # Progress tracking
└── archive/          # Historical files
```

### Contributing
This project follows **Scene-Based Architecture** with **8 Core Principles**:
1. Scene-based architecture
2. Wireframe documentation
3. Mouse-first UI
4. Resource management
5. Grid inventory system
6. Event system
7. Character archetypes
8. Maximum extensibility

See [docs/8_CORE_PRINCIPLES.md](docs/8_CORE_PRINCIPLES.md) for details.

###
 Building from Source
```bash
# Clone repository
git clone https://github.com/barzikol/kkurs.git
cd kkurs

# Install SFML (Ubuntu/Debian)
sudo apt-get install libsfml-dev

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./kkurs
```

---

## 📝 Current Priorities (Crunch Mode)

### This Week
1. ✅ Archive outdated documentation
2. ✅ Consolidate README files
3. 🔄 Merge NodeScene from other branch
4. 🔄 Implement CharacterScene (basic)
5. 🔄 Implement CompanionsScene (basic)
6. 🔄 Implement QuestsScene (basic)
7. 🔄 English UI localization
8. 🔄 Add content (5 NPCs, 3 quests, 3 locations)
9. 🔄 Testing & bug fixes

### Acceptable Workarounds
- Hardcoded data (vs data-driven)
- Simple UI (vs polished)
- Basic error handling
- Limited testing
- Placeholder graphics

**Goal:** Working game that runs and doesn't crash

---

## 📦 Dependencies

- **SFML 2.6** - Graphics, audio, window management
- **C++17 STL** - Standard library features
- **CMake 3.10+** - Build system
- **nlohmann/json** (optional) - JSON parsing for save system

---

## 📄 License

[License information to be added]

---

## 🔗 Links

- **GitHub:** https://github.com/barzikol/kkurs
- **Issues:** https://github.com/barzikol/kkurs/issues
- **Documentation:** [docs/](docs/)

---

## 👥 Team

Development Team - CRUNCH MODE 🔥

---

**Last Updated:** 2025-11-21  
**Version:** Alpha (Pre-release)  
**Status:** Active Development - Crunch Mode
