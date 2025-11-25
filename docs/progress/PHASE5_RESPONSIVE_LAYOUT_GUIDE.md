# Phase 5: Responsive Layout Application Guide

**Дата создания / Created**: 2025-11-20  
**Статус / Status**: 🚧 В разработке / In Progress  
**Цель / Purpose**: Guide for applying UILayout system to all game scenes

---

## 📋 ОБЗОР / OVERVIEW

This guide provides step-by-step instructions for applying the responsive layout system (UILayout) to existing game scenes. The goal is to make all UI elements scale properly for MacBook Air M1 resolution (1440x900).

### Target Resolution
- **Base Resolution**: 800x600 (design baseline)
- **Target Resolution**: 1440x900 (MacBook Air M1)
- **Scale Factors**: 
  - Width: 1.8x (1440 / 800)
  - Height: 1.5x (900 / 600)
  - Font: 1.5x (minimum of width/height scale)

---

## 🎯 ПРИМЕНЕНИЕ ПАТТЕРНА / APPLICATION PATTERN

### Step 1: Include UILayout Header

Add to the scene header file:

```cpp
#include "UILayout.h"
```

### Step 2: Initialize Layout in Constructor

At the beginning of the scene constructor, initialize the layout:

```cpp
SceneName::SceneName() {
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("SceneName");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Rest of initialization...
}
```

### Step 3: Replace Hardcoded Coordinates

Replace all hardcoded position values with the SCALE_POS macro:

**Before:**
```cpp
m_titleText.setPosition(400.0f, 150.0f);
m_button.setPosition(500.0f, 300.0f);
```

**After:**
```cpp
m_titleText.setPosition(SCALE_POS(layout, 400.0f, 150.0f));
m_button.setPosition(SCALE_POS(layout, 500.0f, 300.0f));
```

### Step 4: Replace Hardcoded Sizes

Replace all hardcoded size values with the SCALE_SIZE macro:

**Before:**
```cpp
m_box.setSize(sf::Vector2f(400, 60));
m_panel.setSize(sf::Vector2f(200, 150));
```

**After:**
```cpp
m_box.setSize(SCALE_SIZE(layout, 400, 60));
m_panel.setSize(SCALE_SIZE(layout, 200, 150));
```

### Step 5: Replace Hardcoded Font Sizes

Replace all hardcoded font sizes with the SCALE_FONT macro:

**Before:**
```cpp
m_titleText.setCharacterSize(32);
m_bodyText.setCharacterSize(16);
```

**After:**
```cpp
m_titleText.setCharacterSize(SCALE_FONT(layout, 32));
m_bodyText.setCharacterSize(SCALE_FONT(layout, 16));
```

### Step 6: Update Dynamic Positioning

If the scene has dynamic positioning (e.g., in update() method), get the layout and use it:

**Before:**
```cpp
void SceneName::update(float deltaTime) {
    m_selector.setPosition(450.0f, yPositions[m_index]);
}
```

**After:**
```cpp
void SceneName::update(float deltaTime) {
    UILayout& layout = GET_LAYOUT("SceneName");
    sf::Vector2f pos = SCALE_POS(layout, 450.0f, yPositions[m_index]);
    m_selector.setPosition(pos);
}
```

---

## 📝 COMPLETE EXAMPLE: MainMenuScene

### Header File Changes

```cpp
#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "Scene.h"
#include "UILayout.h"  // <-- Add this line
#include <SFML/Graphics.hpp>

// Rest of header remains the same...
```

### Implementation File Changes

```cpp
#include "MainMenuScene.h"
#include "GameStateManager.h"
#include <iostream>

MainMenuScene::MainMenuScene() 
    : m_selectedIndex(0),
      m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU),
      m_fontLoaded(false) {
    
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("MainMenuScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Load font...
    
    // Setup title text with responsive scaling
    m_titleText.setFont(m_font);
    m_titleText.setString("69 CROSSROADS");
    m_titleText.setCharacterSize(SCALE_FONT(layout, 32));
    m_titleText.setFillColor(sf::Color(255, 200, 0));
    m_titleText.setPosition(SCALE_POS(layout, 400.0f, 150.0f));
    
    // Setup menu items with responsive scaling
    m_newGameText.setFont(m_font);
    m_newGameText.setString("NEW GAME");
    m_newGameText.setCharacterSize(SCALE_FONT(layout, 20));
    m_newGameText.setFillColor(sf::Color(0, 255, 0));
    m_newGameText.setPosition(SCALE_POS(layout, 500.0f, 300.0f));
    
    // ... repeat for all other UI elements
}

void MainMenuScene::update(float deltaTime) {
    // Get layout for dynamic positioning
    UILayout& layout = GET_LAYOUT("MainMenuScene");
    
    // Update selector position with responsive scaling
    float yPositions[] = {300.0f, 360.0f, 420.0f};
    sf::Vector2f selectorPos = SCALE_POS(layout, 450.0f, yPositions[m_selectedIndex]);
    m_selectorBox.setPosition(selectorPos);
    
    // Rest of update logic...
}
```

---

## ✅ CHECKLIST FOR EACH SCENE

When applying responsive layout to a scene, verify:

- [ ] UILayout.h is included in the header file
- [ ] Layout is initialized in constructor with MacBook Air M1 resolution
- [ ] All `setPosition()` calls use SCALE_POS macro
- [ ] All `setSize()` calls use SCALE_SIZE macro
- [ ] All `setCharacterSize()` calls use SCALE_FONT macro
- [ ] Dynamic positioning in update() uses layout
- [ ] Scene compiles without errors
- [ ] Scene displays correctly at target resolution
- [ ] No regressions in functionality

---

## 🔧 SCENES STATUS

### Completed ✅
1. **MainMenuScene** - ✅ Complete (2025-11-20)
2. **CharacterCreationScene** (+ CharacterCreationMenu) - ✅ Complete (2025-11-20)
3. **NodeScene** - ✅ Complete (2025-11-20)
4. **MapScene** - ✅ Complete (2025-11-20)
5. **TravelCinematicScene** - ✅ Complete (2025-11-20)
6. **CinematicEventScene** - ✅ Complete (2025-11-20)
7. **LocationScene** - ✅ Complete (2025-11-20)
8. **AbilityTreeScene** - ✅ Complete (2025-11-20)
9. **CharacterScene** - ✅ Complete (2025-11-20)
10. **CompanionsScene** - ✅ Complete (2025-11-20)
11. **QuestsScene** - ✅ Complete (2025-11-20)
12. **InventoryScene** - ✅ Complete (2025-11-20)

### In Progress 🚧
- None (All scenes complete!)

### Pending ⏳
- None (All scenes complete!)

---

## 🧪 TESTING GUIDELINES

### Visual Testing

1. **Resolution Check**: Verify scene displays correctly at 1440x900
2. **Proportions Check**: Verify UI elements maintain proper proportions
3. **Text Readability**: Verify text is legible and properly sized
4. **Element Spacing**: Verify spacing between elements is appropriate
5. **Alignment**: Verify elements align correctly

### Functional Testing

1. **User Input**: Verify all user interactions work correctly
2. **Scene Transitions**: Verify transitions to/from scene work
3. **Dynamic Elements**: Verify dynamic positioning updates correctly
4. **State Preservation**: Verify scene state is preserved correctly

### Performance Testing

1. **Frame Rate**: Verify 60 FPS is maintained
2. **Memory Usage**: Verify no memory leaks
3. **Load Time**: Verify scene loads quickly

---

## 📊 IMPLEMENTATION PROGRESS

### Week 1 Progress - COMPLETE ✅
- Day 1: ✅ All 12 scenes completed in single session
  - MainMenuScene
  - CharacterCreationScene (+ CharacterCreationMenu)
  - NodeScene, MapScene
  - TravelCinematicScene, CinematicEventScene
  - LocationScene, AbilityTreeScene
  - CharacterScene, CompanionsScene
  - QuestsScene, InventoryScene

### Metrics
- **Scenes Completed**: 12 / 12 (100%) ✅
- **Files Modified**: 24 files (12 headers + 12 implementations)
- **Time Taken**: 1 session (~3 hours)
- **Current Velocity**: 12 scenes completed

### Achievement 🎉
**All responsive layout work for Phase 5 Week 1 is COMPLETE!**

---

## 🎨 ADVANCED USAGE

### Using Alignment

For elements that need to be aligned to screen edges or center:

```cpp
// Center element horizontally
sf::Vector2f pos = layout.getAlignedPosition(
    elementWidth, 
    elementHeight,
    Alignment::TOP_CENTER,
    Padding(20.0f)  // 20px padding from top
);
element.setPosition(pos);
```

### Using Relative Positioning

For elements that need to be positioned relative to screen dimensions:

```cpp
// Position at 50% width, 75% height
RelativePosition relPos(0.5f, 0.75f);
sf::Vector2f pos = layout.fromRelative(relPos);
element.setPosition(pos);
```

### Creating Grids

For grid-based layouts (e.g., inventory):

```cpp
// Create 6x4 grid with 10px padding and 5px spacing
std::vector<UILayout::GridCell> grid = layout.createGrid(
    4,                  // rows
    6,                  // columns
    Padding(10.0f),     // padding
    5.0f                // spacing
);

// Use grid cells for positioning
for (size_t i = 0; i < grid.size(); ++i) {
    elements[i].setPosition(grid[i].position);
    elements[i].setSize(grid[i].size);
}
```

---

## ⚠️ COMMON PITFALLS

### Pitfall 1: Forgetting to Get Layout in Update

**Wrong:**
```cpp
void Scene::update(float deltaTime) {
    // Layout from constructor is out of scope!
    m_element.setPosition(SCALE_POS(layout, x, y));  // ERROR
}
```

**Correct:**
```cpp
void Scene::update(float deltaTime) {
    UILayout& layout = GET_LAYOUT("SceneName");
    m_element.setPosition(SCALE_POS(layout, x, y));  // OK
}
```

### Pitfall 2: Inconsistent Scene Names

**Wrong:**
```cpp
// Constructor
UILayout& layout = GET_LAYOUT("MainMenu");

// Update
UILayout& layout = GET_LAYOUT("MainMenuScene");  // Different name!
```

**Correct:**
```cpp
// Use consistent name everywhere
UILayout& layout = GET_LAYOUT("MainMenuScene");
```

### Pitfall 3: Scaling Already-Scaled Values

**Wrong:**
```cpp
// First scaling
sf::Vector2f pos1 = SCALE_POS(layout, 100, 200);
m_element.setPosition(pos1);

// Later, scaling again
sf::Vector2f pos2 = SCALE_POS(layout, pos1.x, pos1.y);  // Double scaling!
m_element.setPosition(pos2);
```

**Correct:**
```cpp
// Always scale from base coordinates
sf::Vector2f pos = SCALE_POS(layout, 100, 200);
m_element.setPosition(pos);
```

---

## 📚 REFERENCES

- **UILayout.h** - Full API documentation
- **TEST_HELPERS_AND_LAYOUT_SYSTEM.md** - System architecture
- **PHASE4_IMPLEMENTATION_SUMMARY.md** - Previous phase reference
- **ExampleResponsiveScene.h** - Reference implementation

---

**Status**: Phase 5 - Week 1 (Responsive Layout Application)  
**Last Updated**: 2025-11-20  
**Next Update**: After CharacterCreationScene implementation
