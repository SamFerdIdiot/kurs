# Development Guide

**Project**: Road Trip Adventure RPG  
**Architecture**: Scene-based C++ game with SFML  
**Last Updated**: 2025-11-09

---

## Table of Contents

1. [Getting Started](#getting-started)
2. [Project Structure](#project-structure)
3. [Development Workflow](#development-workflow)
4. [Coding Standards](#coding-standards)
5. [Scene Development](#scene-development)
6. [System Integration](#system-integration)
7. [Testing](#testing)
8. [Troubleshooting](#troubleshooting)

---

## Getting Started

### Prerequisites

- **CMake** 3.10 or higher
- **C++17** compatible compiler (GCC, Clang, MSVC)
- **SFML 2.6** libraries (Graphics, Window, System)
- **Git** for version control

### Installation (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install libsfml-dev cmake build-essential git

# Clone repository
git clone <repository-url>
cd kkurs

# Build project
./scripts/build.sh

# Run game
./scripts/run.sh
```

### Quick Commands

```bash
# Build only
./scripts/build.sh

# Clean build artifacts
./scripts/clean.sh

# Full rebuild (clean + build)
./scripts/rebuild.sh

# Run tests
./scripts/test.sh

# Build and run
./scripts/run.sh
```

---

## Project Structure

### Directory Layout

```
kkurs/
├── src/                    # Source files (.cpp)
│   ├── main.cpp           # Entry point
│   ├── SceneManager.cpp   # Scene dispatcher
│   ├── *Scene.cpp         # Scene implementations
│   └── *System.cpp        # System implementations
├── include/               # Header files (.h)
│   ├── Scene.h           # Base scene interface
│   ├── SceneManager.h    # Scene manager header
│   ├── *Scene.h          # Scene headers
│   └── *System.h         # System headers
├── scripts/              # Build and utility scripts
│   ├── build.sh         # Build script
│   ├── run.sh           # Run script
│   ├── clean.sh         # Clean script
│   ├── rebuild.sh       # Full rebuild
│   └── test.sh          # Test script
├── build/                # Build output (gitignored)
│   └── kkurs            # Executable
├── images/               # Game assets
├── CMakeLists.txt        # CMake configuration
├── README.md             # Project overview
├── AGENT_SESSION.md      # Agent session log
├── SESSION_STATE.md      # Current state documentation
├── TODO.md               # Task list
├── DEVELOPMENT_LOG.md    # Change log
└── DEVELOPMENT_GUIDE.md  # This file
```

### Key Files

**Core Architecture**:
- `Scene.h` - Base scene interface, defines scene contract
- `SceneManager.h/cpp` - Scene dispatcher and main loop coordinator
- `main.cpp` - Application entry point

**Game Scenes** (all follow same pattern):
- `MainMenuScene` - Game entry point
- `CharacterCreationScene` - Character setup
- `RouteSelectionScene` - Route choice
- `DrivingScene` - Main gameplay
- `TownScene` - Town interactions
- `MotelScene` - Rest and save
- `DialogueScene` - NPC conversations
- `InventoryScene` - Item management
- `PauseMenuScene` - In-game pause
- `GameOverScene` - End screen

**Support Systems**:
- `ResourceManager` - Manages money, fuel, durability, food, water
- `InventorySystem` - Item management with stacking and weight
- `PlayerState` - Global player state singleton

---

## Development Workflow

### 1. Create a New Feature Branch

```bash
git checkout -b feature/your-feature-name
```

### 2. Make Changes

Follow the [Coding Standards](#coding-standards) section.

### 3. Build and Test

```bash
# Build
./scripts/build.sh

# Test
./scripts/test.sh

# Run to verify
./scripts/run.sh
```

### 4. Commit Changes

```bash
git add .
git commit -m "Brief description of changes"
```

### 5. Push and Create PR

```bash
git push origin feature/your-feature-name
```

---

## Coding Standards

### General Principles

1. **Minimal Changes**: Only modify what's necessary
2. **Consistency**: Follow existing code style
3. **Documentation**: Comment complex logic
4. **Testing**: Test before committing
5. **Clean Code**: Keep functions focused and small

### Naming Conventions

**Classes**: PascalCase
```cpp
class MainMenuScene { };
class ResourceManager { };
```

**Member Variables**: m_ prefix + camelCase
```cpp
float m_speed;
bool m_isFinished;
sf::RectangleShape m_playerCarBox;
```

**Functions**: camelCase
```cpp
void handleInput(const sf::Event& event);
float getResource(ResourceType type);
```

**Constants**: UPPER_SNAKE_CASE
```cpp
const float MAX_SPEED = 100.0f;
```

**Enums**: PascalCase for enum, UPPER_CASE for values
```cpp
enum class SceneType {
    MAIN_MENU,
    DRIVING,
    EXIT
};
```

### File Organization

**Header Files (.h)**:
```cpp
#ifndef SCENE_NAME_H
#define SCENE_NAME_H

#include <SFML/Graphics.hpp>
#include "Scene.h"

// Class documentation comment
class SceneName : public Scene {
public:
    // Public interface
    
private:
    // Private members
};

#endif // SCENE_NAME_H
```

**Source Files (.cpp)**:
```cpp
#include "SceneName.h"

SceneName::SceneName() {
    // Constructor implementation
}

void SceneName::handleInput(const sf::Event& event) {
    // Implementation
}
```

### Comments

**Use TODO comments for future work**:
```cpp
// TODO: Add particle effects
// TODO: Implement save/load functionality
```

**Document complex logic**:
```cpp
// Calculate interpolated position between waypoints
// using cubic Bezier curve for smooth movement
float interpolatedX = calculateBezier(t, p0, p1, p2, p3);
```

**Don't comment obvious code**:
```cpp
// BAD: Counter is incremented by one
counter++;

// GOOD: (no comment needed)
counter++;
```

---

## Scene Development

### Creating a New Scene

#### 1. Create Header File (`include/NewScene.h`)

```cpp
#ifndef NEW_SCENE_H
#define NEW_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>

class NewScene : public Scene {
public:
    NewScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    bool m_isFinished;
    SceneType m_nextScene;
    
    // Visual elements
    sf::RectangleShape m_background;
    // Add more as needed
};

#endif // NEW_SCENE_H
```

#### 2. Create Source File (`src/NewScene.cpp`)

```cpp
#include "NewScene.h"

NewScene::NewScene() 
    : m_isFinished(false), m_nextScene(SceneType::NEW_SCENE) {
    // Initialize visual elements
    m_background.setSize(sf::Vector2f(1366.0f, 768.0f));
    m_background.setFillColor(sf::Color(20, 20, 30));
}

void NewScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            m_nextScene = SceneType::MAIN_MENU;
            m_isFinished = true;
        }
    }
}

void NewScene::update(float deltaTime) {
    // Update game logic
}

void NewScene::render(sf::RenderWindow& window) {
    window.draw(m_background);
}

SceneType NewScene::getNextScene() const {
    return m_nextScene;
}

bool NewScene::isFinished() const {
    return m_isFinished;
}
```

#### 3. Update Scene.h Enum

```cpp
enum class SceneType {
    MAIN_MENU,
    // ... existing scenes ...
    NEW_SCENE,  // Add your scene
    EXIT
};
```

#### 4. Update SceneManager.cpp

```cpp
#include "NewScene.h"  // Add include

std::unique_ptr<Scene> SceneManager::createScene(SceneType type) {
    switch (type) {
        // ... existing cases ...
        case SceneType::NEW_SCENE:
            return std::make_unique<NewScene>();
        // ... rest of cases ...
    }
}
```

#### 5. Update CMakeLists.txt

```cmake
add_executable(kkurs 
    # ... existing files ...
    src/NewScene.cpp
)
```

#### 6. Build and Test

```bash
./scripts/rebuild.sh
./scripts/test.sh
```

---

## System Integration

### Using ResourceManager

```cpp
#include "ResourceManager.h"

// In your scene constructor or initialization
ResourceManager resources;

// Get resource value
float currentFuel = resources.getResource(ResourceType::FUEL);

// Set resource
resources.setResource(ResourceType::MONEY, 500.0f);

// Modify resource (add/subtract)
resources.modifyResource(ResourceType::FUEL, -10.0f);  // Consume 10 fuel

// Check if enough resource
if (resources.hasEnoughResource(ResourceType::MONEY, 50.0f)) {
    // Can afford something
}

// Get maximum
float maxFuel = resources.getMaxResource(ResourceType::FUEL);

// Reset to defaults
resources.resetResources();
```

### Using InventorySystem

```cpp
#include "InventorySystem.h"
#include "Item.h"

// Create inventory (24 slots, 100.0 max weight)
InventorySystem inventory(24, 100.0f);

// Create and add item
Item waterBottle;
waterBottle.name = "Water Bottle";
waterBottle.category = ItemCategory::WATER;
waterBottle.weight = 0.5f;
waterBottle.stackable = true;
waterBottle.maxStack = 10;

inventory.addItem(waterBottle, 3);  // Add 3 water bottles

// Check if has item
if (inventory.hasItem("Water Bottle", 1)) {
    // Player has at least 1 water bottle
}

// Use item
inventory.useItem(0);  // Use item in slot 0

// Get item info
const InventorySlot& slot = inventory.getSlot(0);
if (!slot.isEmpty) {
    std::cout << "Item: " << slot.item.name << " x" << slot.count << std::endl;
}

// Check capacity
float currentWeight = inventory.getCurrentWeight();
float maxWeight = inventory.getMaxWeight();
```

---

## Testing

### Manual Testing Checklist

After making changes, test the following:

1. **Build**: Project compiles without errors/warnings
2. **Launch**: Game window opens
3. **Navigation**: Scene transitions work
4. **Input**: Keyboard controls respond
5. **Visual**: All elements render correctly
6. **Logic**: Game state updates properly
7. **Exit**: Game closes cleanly

### Automated Testing

Run the test script:

```bash
./scripts/test.sh
```

This checks:
- Executable exists and has correct permissions
- Game launches and initializes
- Required files are present

### Scene Transition Testing

Recommended test path:
1. Main Menu → Press Down/Up (navigation)
2. Main Menu → Press Enter on "New Game"
3. Character Creation → Select background
4. Character Creation → Select car
5. Route Selection → Select route
6. Driving Scene → Test controls (arrows/WASD)
7. Driving → Press T (Town)
8. Town → Press Escape (back to Driving)
9. Driving → Press M (Motel)
10. Motel → Press Escape (back to Driving)
11. Driving → Press I (Inventory)
12. Inventory → Press Escape (back to Driving)
13. Driving → Press Escape (Pause Menu)
14. Pause Menu → Test navigation
15. Pause Menu → Resume

---

## Troubleshooting

### Build Errors

**Problem**: `find_package(SFML)` fails
```
Solution: Install SFML development libraries
sudo apt-get install libsfml-dev
```

**Problem**: CMake version too old
```
Solution: Update CMake
sudo apt-get install cmake
```

**Problem**: Compiler not found
```
Solution: Install build tools
sudo apt-get install build-essential
```

### Runtime Errors

**Problem**: Window doesn't open
```
Solution: Check display settings (DISPLAY environment variable)
export DISPLAY=:0
```

**Problem**: Scene transition fails
```
Solution: Check SceneManager logs for error messages
- Verify scene is added to SceneManager::createScene()
- Verify scene type is in Scene.h enum
```

**Problem**: Game crashes on scene change
```
Solution: Check scene constructors
- Ensure all member variables are initialized
- Check for null pointer dereferences
```

### Performance Issues

**Problem**: Low frame rate
```
Solution: 
- Check render loop (should be 60 FPS)
- Profile with performance tools
- Reduce draw calls
```

---

## Best Practices

### DO

✅ Test after every change  
✅ Follow existing code patterns  
✅ Document complex logic  
✅ Keep functions focused  
✅ Use meaningful variable names  
✅ Handle errors gracefully  
✅ Commit frequently with clear messages  

### DON'T

❌ Break existing functionality  
❌ Add unnecessary dependencies  
❌ Commit broken code  
❌ Remove working code without reason  
❌ Ignore compiler warnings  
❌ Skip testing  
❌ Make large unfocused commits  

---

## Additional Resources

### SFML Documentation
- Official Docs: https://www.sfml-dev.org/documentation/
- Tutorials: https://www.sfml-dev.org/tutorials/

### C++ Reference
- https://en.cppreference.com/
- https://cplusplus.com/

### CMake Documentation
- https://cmake.org/documentation/

### Project Documentation
- `README.md` - Project overview and features
- `SESSION_STATE.md` - Current feature status
- `TODO.md` - Pending tasks
- `DEVELOPMENT_LOG.md` - Change history

---

## Contact & Support

For questions or issues:
1. Check documentation in repository
2. Review existing code for examples
3. Search SFML documentation
4. Create an issue on GitHub

---

**Last Updated**: 2025-11-09  
**Version**: 1.0
