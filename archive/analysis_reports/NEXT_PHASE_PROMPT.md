# Phase 2: NodeScene UI Implementation - English Localization & Full UI

## Overview

Implement complete English-only UI for NodeScene menu interface, removing all Russian text and creating a polished user experience.

## Requirements

### 1. Repository Compliance
- Follow existing code style and patterns
- Use C++17 standards
- Maintain SFML 2.6 compatibility
- Keep scene-based architecture
- Follow existing naming conventions (camelCase for variables, PascalCase for classes)

### 2. English Localization (PRIORITY)

**Current Russian UI elements to translate:**

In `NodeScene.cpp`:
- "1. Карта" → "1. Map"
- "2. Персонаж" → "2. Character"
- "3. Инвентарь" → "3. Inventory"
- "4. Попутчики" → "4. Companions"
- "5. Задания" → "5. Quests"
- "6. Древо способностей" → "6. Ability Tree"
- "Заправка 'Петрович'" → "Pete's Gas Station"
- "Магазин 'У дороги'" → "Roadside Store"
- "Мотель '69'" → "Motel 69"
- "A/D - навигация | Enter - выбрать | Esc - карта" → "A/D - Navigate | Enter - Select | Esc - Map"

**Search for ALL Russian text in:**
- All .cpp files in src/
- All .h files in include/
- UI components in src/UI/ and include/UI/
- Structure.cpp and related files
- LocationScene.cpp
- MainMenuScene.cpp
- CharacterCreationScene.cpp
- Any other scene files

**Translation Guidelines:**
- Use clear, concise English
- Maintain game atmosphere (Russia 90s road trip vibe)
- Keep UI text short for display
- Use proper capitalization

### 3. Complete NodeScene UI Implementation

**Menu Rendering:**
- ✅ Menu item list with selection highlighting (DONE)
- ⚠️ Add visual indicators (arrows, icons)
- ⚠️ Improve text layout and spacing
- ⚠️ Add background panels for menu items
- ⚠️ Better color scheme (not just blue/gray)
- ⚠️ Add transition animations (fade in/out)

**Menu Item Details:**
Each menu item should show:
- Number/icon
- Name
- Brief description or status
- Visual separator

**Interactive Elements:**
- Hover effect preview (optional, if time permits)
- Selection sound effects (TODO: add sound system)
- Visual feedback on selection

**HUD Panel Enhancement:**
Current HUD shows:
- Money
- Fuel
- Energy
- Cargo

Should also show:
- Current node name (prominent)
- Current time/day (if time system exists)
- Active quest indicator
- Companion count
- Vehicle condition

**Structure Menu Items:**
When structures are present:
- Show structure type icon/indicator
- Show structure name
- Show availability status (open/closed)
- Show price/cost indicator if applicable

### 4. Code Implementation Tasks

**Task 1: English Localization**
```cpp
// Replace in NodeScene.cpp
m_menuItems.push_back("1. Map");
m_menuItems.push_back("2. Character");
m_menuItems.push_back("3. Inventory");
m_menuItems.push_back("4. Companions");
m_menuItems.push_back("5. Quests");
m_menuItems.push_back("6. Ability Tree");

// Update instructions
m_instructionText.setString("A/D - Navigate | Enter - Select | Esc - Map");

// Update structure names
m_structures.push_back(std::make_unique<Structure>(
    LocationType::GAS_STATION,
    "Pete's Gas Station",
    sf::Vector2f(0.0f, 0.0f)
));
```

**Task 2: Enhanced Menu Rendering**
```cpp
void NodeScene::renderMenu(sf::RenderWindow& window) {
    // Draw title background
    // Draw menu items with improved styling
    // Add selection indicator (arrow or highlight)
    // Show item descriptions
    // Add visual separators
}
```

**Task 3: Menu Item Description System**
```cpp
struct MenuItemInfo {
    std::string name;
    std::string description;
    bool isAvailable;
    std::string statusText;
};

std::vector<MenuItemInfo> m_menuItemInfo;
```

**Task 4: Improved HUD Integration**
```cpp
// In update():
m_hudPanel.setNodeName(m_nodeName);
m_hudPanel.setCompanionCount(NPCManager::getInstance().getTeamSize());
m_hudPanel.setActiveQuestCount(questCount); // TODO: get from QuestManager
```

**Task 5: Structure Menu Enhancement**
```cpp
void NodeScene::renderStructureMenuItem(sf::RenderWindow& window, int index) {
    // Show structure icon
    // Show structure status (open/closed)
    // Show brief info (prices, services)
}
```

### 5. Visual Design Specifications

**Color Scheme:**
- Background: Dark blue-gray (#1E1E2E)
- Menu items (normal): Medium gray (#3E3E4E)
- Menu items (selected): Light blue (#5E7EA0)
- Text (normal): Light gray (#CCCCCC)
- Text (selected): White (#FFFFFF)
- Accent: Orange/amber (#FFA500) for important info
- HUD: Semi-transparent dark (#2E2E3EAA)

**Typography:**
- Title: 36pt, bold
- Menu items: 24pt, normal
- Descriptions: 18pt, italic
- HUD: 20pt, bold
- Instructions: 18pt, normal

**Layout:**
```
+----------------------------------+
|       [Node Name - Title]        |
+----------------------------------+
|                                  |
|    1. Map                        |
|       View world map             |
|                                  |
|    2. Character              <-- Selected
|       View stats & level         |
|                                  |
|    3. Inventory                  |
|       Manage items               |
|                                  |
|    [... more items ...]          |
|                                  |
+----------------------------------+
|  A/D - Navigate | Enter - Select |
+----------------------------------+
|  [HUD: Money | Fuel | Energy]    |
+----------------------------------+
```

### 6. Further Development Roadmap

**Phase 3: Scene Integration**
- Implement CharacterScene
- Implement CompanionsScene  
- Implement QuestsScene
- Create structure interaction menus (in NodeScene)

**Phase 4: Travel System**
- Create TravelCinematicScene (side-view)
- Car movement animation
- Background parallax scrolling
- Random events during travel

**Phase 5: World Map Integration**
- Connect nodes on map
- Show travel paths
- Display node information
- Route planning

**Phase 6: Polish & Features**
- Sound system integration
- Save/load from NodeScene
- Tutorial system
- Achievements display

**Phase 7: Content**
- More structure types
- More node varieties
- Quest content
- NPC dialogues
- Random events

### 7. Testing Requirements

**Manual Testing Checklist:**
- [ ] All menu items display in English
- [ ] Navigation with A/D works smoothly
- [ ] Selection with Enter triggers correct action
- [ ] HUD displays all information correctly
- [ ] Structure menu items appear when structures exist
- [ ] Escape returns to map
- [ ] No Russian text anywhere in UI
- [ ] Text is readable and well-formatted
- [ ] Colors are consistent and pleasant
- [ ] No visual glitches or overlapping text

**Code Review Checklist:**
- [ ] No Russian strings in code
- [ ] Comments in English
- [ ] Proper error handling
- [ ] No memory leaks
- [ ] Consistent code style
- [ ] Proper const correctness
- [ ] SFML resources properly managed

### 8. Files to Modify

**Primary:**
- `src/NodeScene.cpp` - Main implementation
- `include/NodeScene.h` - Interface updates if needed

**Secondary (Russian text removal):**
- `src/MainMenuScene.cpp`
- `src/CharacterCreationScene.cpp`
- `src/LocationScene.cpp`
- `src/RestScene.cpp`
- `src/CinematicEventScene.cpp`
- `src/TravelCinematicScene.cpp`
- `src/MapScene.cpp`
- `src/Structure.cpp`
- Any other files with Russian strings

**UI Components:**
- `src/UI/HUDPanel.cpp`
- `include/UI/HUDPanel.hpp`

### 9. Implementation Priority

**Priority 1 (Must Have):**
1. Replace ALL Russian text with English
2. Improve menu item rendering
3. Add menu item descriptions
4. Enhance HUD display

**Priority 2 (Should Have):**
1. Visual indicators (arrows)
2. Better color scheme
3. Improved layout
4. Structure status indicators

**Priority 3 (Nice to Have):**
1. Transition animations
2. Sound effects hooks
3. Advanced visual polish
4. Performance optimizations

### 10. Success Criteria

The implementation is successful when:
- ✅ Zero Russian text in entire UI
- ✅ NodeScene menu is fully functional
- ✅ All 6 base menu items work correctly
- ✅ Structure menu items display and work
- ✅ HUD shows all relevant information
- ✅ Visual design is clean and consistent
- ✅ Navigation is smooth and intuitive
- ✅ Code follows repository standards
- ✅ No compilation warnings or errors
- ✅ Documentation is updated

### 11. Documentation Updates

After implementation, update:
- `NODESCENE_REFACTOR.md` - Add English UI section
- `CODE_STRUCTURE_ANALYSIS.md` - Update NodeScene section
- `QUICK_REFERENCE.md` - Update controls and features
- Create `UI_STYLE_GUIDE.md` - Document UI standards

### 12. Example Code Patterns

**Menu Item with Description:**
```cpp
struct MenuItem {
    std::string name;
    std::string description;
    SceneType targetScene;
    bool isImplemented;
};

std::vector<MenuItem> m_menuItems = {
    {"Map", "View world map and travel", SceneType::WORLD_MAP, true},
    {"Character", "View stats and progression", SceneType::CHARACTER, false},
    {"Inventory", "Manage items and equipment", SceneType::INVENTORY, true},
    // ...
};
```

**Enhanced Rendering:**
```cpp
void NodeScene::renderMenuItemDetailed(sf::RenderWindow& window, 
                                       const MenuItem& item, 
                                       int index, 
                                       bool isSelected) {
    // Background
    sf::RectangleShape bg(sf::Vector2f(800, 80));
    bg.setFillColor(isSelected ? sf::Color(94, 126, 160) : sf::Color(62, 62, 78));
    
    // Selection indicator
    if (isSelected) {
        sf::Text arrow;
        arrow.setString(">");
        // ... position and draw
    }
    
    // Item name
    // Item description  
    // Status indicator (TODO, READY, etc.)
}
```

## Summary

This prompt defines a comprehensive plan to:
1. Remove all Russian text (English only)
2. Implement complete NodeScene UI
3. Enhance visual design
4. Set up for future development phases

The focus is on creating a polished, English-language UI that serves as the foundation for further game development.
