# Phase 5: Advanced Scene Integration & Gameplay Polish

**Дата создания / Created**: 2025-11-20  
**Статус / Status**: 📋 Планирование / Planning  
**Приоритет / Priority**: High  
**Зависимости / Dependencies**: Phase 4 Complete ✅

---

## 📋 ОБЗОР / OVERVIEW

Phase 5 focuses on applying the responsive layout system to all existing scenes, implementing missing advanced features, and polishing the overall gameplay experience. This phase will leverage the test helpers and responsive layout infrastructure created in Phase 4 to rapidly develop and test new features.

### Цели фазы / Phase Goals
1. Apply responsive layout system (MacBook Air M1) to all existing scenes
2. Implement save/load system integration across all scenes
3. Add visual polish and animations to existing scenes
4. Implement advanced inventory features (stacking, quick-use, sorting)
5. Add HUD enhancements and real-time feedback
6. Improve LocationScene interactions and variety

---

## 🎯 ПРИОРИТЕТЫ / PRIORITIES

### Priority 1: Must Have (Критично)
- [ ] **Apply Responsive Layout** - Update all scenes to use UILayout system (MacBook Air M1 1440x900)
- [ ] **Save/Load Integration** - Auto-save on scene transitions, load last saved state
- [ ] **HUD Enhancements** - Real-time resource updates, context-aware displays
- [ ] **LocationScene Polish** - Enhanced interactions, better visual feedback
- [ ] **Scene Transitions** - Smooth fade in/out effects (deferred from Phase 4)

### Priority 2: Should Have (Желательно)
- [ ] **Advanced Inventory** - Item stacking display, sorting, quick-use hotkeys
- [ ] **Visual Animations** - Hover effects, selection animations, smooth transitions
- [ ] **Sound Effects** - UI interaction sounds, ambient audio
- [ ] **Tutorial System** - First-time hints, contextual help
- [ ] **Performance Optimization** - 60 FPS target, memory optimization

### Priority 3: Nice to Have (Опционально)
- [ ] **Accessibility Features** - Colorblind mode, font size options, key rebinding
- [ ] **Advanced HUD** - Mini-map, compass, quest tracker
- [ ] **Screenshot Mode** - Hide UI for screenshots
- [ ] **Debug Console** - Developer tools for testing

---

## 🔧 DETAILED IMPLEMENTATION PLAN

### 1. Apply Responsive Layout System to All Scenes

**Status**: UILayout system created in Phase 4, needs application

#### Scenes to Update:
1. **MainMenuScene** - Apply MacBook Air M1 layout
2. **CharacterCreationScene** - Responsive backstory/car selection
3. **NodeScene** - Adaptive menu positioning
4. **MapScene** - Scaled node positions and connections
5. **TravelCinematicScene** - Responsive parallax layers
6. **CinematicEventScene** - Adaptive event dialogs
7. **LocationScene** - Responsive location interactions
8. **AbilityTreeScene** - Adaptive ability tree layout

#### Implementation Pattern:
```cpp
void SceneName::setupUI() {
    UILayout& layout = GET_LAYOUT("SceneName");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Replace hardcoded coordinates
    m_titleText.setPosition(SCALE_POS(layout, 100.0f, 25.0f));
    m_titleText.setCharacterSize(SCALE_FONT(layout, 32));
    
    // Use alignment for key elements
    sf::Vector2f buttonPos = layout.getAlignedPosition(
        200.0f, 50.0f,
        Alignment::BOTTOM_RIGHT,
        Padding(20.0f)
    );
}
```

#### Testing Approach:
- Use test helpers to verify layout at different resolutions
- Create visual regression tests
- Test on actual MacBook Air M1 if available

---

### 2. Save/Load System Integration

**Status**: SaveSystem exists, needs scene integration

#### Features Required:
- **Auto-save on scene transitions** - Save before changing scenes
- **Load game state** - Restore PlayerState, inventory, companions, quests
- **Save game metadata** - Timestamp, playtime, location, level
- **Multiple save slots** - Support 3-5 save files
- **Quick save/load** - F5 to quick save, F9 to quick load

#### Integration Points:
```cpp
// In SceneManager::changeScene()
void SceneManager::changeScene(SceneType newType) {
    // Auto-save before transition
    SaveSystem::getInstance().autoSave();
    
    // Fade out current scene
    performFadeOut();
    
    // Switch scene
    m_currentScene = createScene(newType);
    
    // Fade in new scene
    performFadeIn();
}

// In MainMenuScene - "Continue" option
if (SaveSystem::getInstance().hasSaveFiles()) {
    // Load most recent save
    SaveSystem::getInstance().loadGame(0);
    m_sceneManager->changeScene(SceneType::NODE);
}
```

#### Save File Structure:
```json
{
    "version": "1.0",
    "timestamp": "2025-11-20T10:00:00Z",
    "playtime_seconds": 3600,
    "player": {
        "level": 5,
        "experience": 450,
        "money": 5000.0,
        "fuel": 45.5,
        "energy": 75.0
    },
    "inventory": {
        "items": [...],
        "weight": 12.5
    },
    "companions": [...],
    "quests": [...],
    "location": {
        "current_node": "moscow",
        "position": {"x": 100, "y": 200}
    }
}
```

---

### 3. Scene Transition Effects

**Status**: Deferred from Phase 4 Priority 2

#### Implementation:
```cpp
class SceneTransition {
public:
    enum class Type {
        FADE,
        SLIDE_LEFT,
        SLIDE_RIGHT,
        DISSOLVE
    };
    
    static void fadeOut(sf::RenderWindow& window, float duration = 0.3f);
    static void fadeIn(sf::RenderWindow& window, float duration = 0.3f);
    
private:
    static sf::RectangleShape m_overlay;
    static sf::Clock m_transitionClock;
};
```

#### Transition Timing:
- Fade out: 0.3 seconds
- Scene switch
- Fade in: 0.3 seconds
- Total: 0.6 seconds

#### Test with Helpers:
```cpp
TEST_F(SceneTransitionTest, FadeOutCompletes) {
    SceneTransition::fadeOut(window, 0.3f);
    TestHelpers::simulateFrames(scene, 20);  // ~0.33s at 60 FPS
    // Verify overlay alpha is 255
}
```

---

### 4. HUD Enhancements

**Current State**: Basic HUD exists, needs enhancement

#### Features to Add:
1. **Real-time Updates** - Resource changes animate smoothly
2. **Context-Aware Display** - Show relevant info per scene
3. **Notifications** - Quest updates, level up, low resources
4. **Resource Change Indicators** - +/- animations for money, fuel, energy
5. **Mini Status** - Compact mode for less clutter

#### HUD Modes:
```cpp
enum class HUDMode {
    FULL,        // Show all resources (NodeScene, MapScene)
    MINIMAL,     // Show only critical info (CinematicScene)
    HIDDEN,      // No HUD (MenuScenes, CharacterCreation)
    CONTEXT      // Scene-specific display
};
```

#### Notification System:
```cpp
class NotificationManager {
public:
    void showNotification(const std::string& message, 
                         NotificationType type,
                         float duration = 3.0f);
    
    void showResourceChange(const std::string& resource,
                           float amount,
                           bool isPositive);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
};

// Usage
NotificationManager::getInstance().showNotification(
    "Level Up! You are now level 5",
    NotificationType::SUCCESS,
    5.0f
);

NotificationManager::getInstance().showResourceChange(
    "Money", -500.0f, false  // Spent money
);
```

---

### 5. Advanced Inventory Features

**Current State**: Basic grid inventory works

#### Features to Add:
1. **Item Stacking** - Display "x5" for stackable items
2. **Sorting** - Sort by name, weight, value, type
3. **Quick-Use Hotkeys** - 1-9 for quick access items
4. **Item Categories** - Filter by type (food, medical, tools, etc.)
5. **Drag to Use/Drop** - Drag items outside inventory to use/drop
6. **Context Menu** - Right-click for item actions

#### UI Enhancements:
```
+------------------------------------------------+
|  INVENTORY                         [Sort ▼]    |
+------------------------------------------------+
|  Weight: 12.5 / 64.0 kg                        |
|  Items: 8 / 24                                 |
+------------------------------------------------+
|  [All] [Food] [Medical] [Tools] [Misc]        |
+------------------------------------------------+
|  Grid with stacking display:                   |
|  +--------+  +--------+  +--------+            |
|  | Food   |  | Medkit |  | Water  |            |
|  |   x5   |  |   x2   |  |   x3   |            |
|  +--------+  +--------+  +--------+            |
+------------------------------------------------+
|  Quick Slots:                                  |
|  [1: Food] [2: Medkit] [3: Water] [4-9: Empty]|
+------------------------------------------------+
```

#### Implementation:
```cpp
// In InventoryScene
void InventoryScene::renderItemWithStack(const GridItem& item, 
                                         sf::RenderWindow& window) {
    // Draw item sprite/rectangle
    window.draw(item.sprite);
    
    // Draw stack count if > 1
    if (item.stackCount > 1) {
        sf::Text stackText;
        stackText.setString("x" + std::to_string(item.stackCount));
        stackText.setCharacterSize(12);
        stackText.setPosition(item.position.x + 50, item.position.y + 60);
        window.draw(stackText);
    }
}

void InventoryScene::sortInventory(SortType sortType) {
    switch(sortType) {
        case SortType::BY_NAME:
            std::sort(items.begin(), items.end(), 
                     [](const auto& a, const auto& b) { 
                         return a.name < b.name; 
                     });
            break;
        case SortType::BY_WEIGHT:
            // Sort by weight
            break;
        case SortType::BY_VALUE:
            // Sort by value
            break;
    }
    rearrangeItems();  // Re-position in grid
}
```

---

### 6. LocationScene Improvements

**Current State**: Basic LocationScene with transactions

#### Improvements Needed:
1. **More Location Types** - Add Hospital, Police Station, Black Market
2. **Interactive Objects** - Click on pumps, shelves, beds
3. **NPC Interactions** - Talk to location NPCs
4. **Mini-games** - Simple interactions (pump gas, haggle prices)
5. **Location State** - Remember if visited, relationships with NPCs

#### New Location Types:
```cpp
enum class LocationType {
    // Existing
    GAS_STATION,
    STORE,
    GARAGE,
    DINER,
    MOTEL,
    MECHANIC,
    HOME,
    
    // New in Phase 5
    HOSPITAL,        // Health restoration, medical supplies
    POLICE_STATION,  // Quest hub, fines, wanted level
    BLACK_MARKET,    // Illegal items, high prices
    REST_AREA,       // Free rest, meet travelers
    TRUCK_STOP,      // Supplies, rumors, hitchhikers
    JUNKYARD,        // Scavenge for parts
    CAMPSITE         // Cook food, repair, rest
};
```

#### Interactive Objects:
```cpp
class InteractiveObject {
public:
    std::string name;
    sf::FloatRect bounds;
    std::function<void()> onInteract;
    bool isHighlighted = false;
    
    void update(const sf::Vector2i& mousePos) {
        isHighlighted = bounds.contains(
            static_cast<float>(mousePos.x),
            static_cast<float>(mousePos.y)
        );
    }
    
    void render(sf::RenderWindow& window) {
        sf::RectangleShape shape(sf::Vector2f(bounds.width, bounds.height));
        shape.setPosition(bounds.left, bounds.top);
        shape.setFillColor(isHighlighted ? 
                          sf::Color(100, 150, 200, 100) : 
                          sf::Color(50, 50, 50, 50));
        window.draw(shape);
    }
};

// Usage in LocationScene
void LocationScene::initializeObjects() {
    if (m_type == LocationType::GAS_STATION) {
        m_objects.push_back({
            "Gas Pump",
            sf::FloatRect(300, 200, 80, 120),
            [this]() { refuelVehicle(); }
        });
        
        m_objects.push_back({
            "Store Entrance",
            sf::FloatRect(500, 150, 100, 150),
            [this]() { enterStore(); }
        });
    }
}
```

---

### 7. Visual Polish & Animations

#### Animation Types:
1. **Button Hover** - Scale up slightly, change color
2. **Selection** - Pulse effect, border highlight
3. **Resource Changes** - Smooth bar transitions, +/- indicators
4. **Item Pickup** - Fly to inventory animation
5. **Scene Transitions** - Fade, slide, dissolve

#### Implementation with Animations:
```cpp
class UIAnimation {
public:
    enum class Type {
        SCALE,
        FADE,
        SLIDE,
        PULSE,
        SHAKE
    };
    
    void start(Type type, float duration, 
               std::function<void()> onComplete = nullptr);
    void update(float deltaTime);
    bool isComplete() const;
    
    sf::Transform getTransform() const;
    sf::Color getColor() const;
};

// Usage
class AnimatedButton {
    UIAnimation m_hoverAnimation;
    
    void onMouseEnter() {
        m_hoverAnimation.start(UIAnimation::Type::SCALE, 0.2f);
    }
    
    void update(float deltaTime) {
        m_hoverAnimation.update(deltaTime);
    }
    
    void render(sf::RenderWindow& window) {
        sf::RenderStates states;
        states.transform = m_hoverAnimation.getTransform();
        window.draw(m_sprite, states);
    }
};
```

---

### 8. Sound Effects System

**Status**: No audio system yet

#### Basic Sound System:
```cpp
class AudioManager {
public:
    static AudioManager& getInstance();
    
    void playSound(const std::string& soundName);
    void playMusic(const std::string& musicName, bool loop = true);
    void stopMusic();
    
    void setVolume(float volume);  // 0.0 - 1.0
    void setMusicVolume(float volume);
    void setSFXVolume(float volume);
    
private:
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::map<std::string, sf::Sound> m_sounds;
    sf::Music m_music;
};

// Usage
AudioManager::getInstance().playSound("button_click");
AudioManager::getInstance().playSound("item_pickup");
AudioManager::getInstance().playMusic("menu_theme", true);
```

#### Sound Effects Needed:
- UI: button_click, button_hover, menu_open, menu_close
- Inventory: item_pickup, item_drop, item_rotate
- Travel: car_start, car_stop, road_ambient
- Events: event_appear, quest_complete, level_up
- Locations: door_open, cash_register, fuel_pump

---

### 9. Tutorial System

**Status**: No tutorial system

#### Implementation:
```cpp
class TutorialManager {
public:
    static TutorialManager& getInstance();
    
    void showHint(const std::string& hintId, const std::string& message);
    void markAsShown(const std::string& hintId);
    bool hasShown(const std::string& hintId) const;
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
private:
    std::set<std::string> m_shownHints;
    std::queue<TutorialHint> m_activeHints;
};

struct TutorialHint {
    std::string message;
    float displayTime;
    float currentTime;
    sf::Vector2f position;
};

// Usage in scenes
void InventoryScene::onFirstOpen() {
    if (!TutorialManager::getInstance().hasShown("inventory_intro")) {
        TutorialManager::getInstance().showHint(
            "inventory_intro",
            "Left-click to pick up items. Right-click to rotate. ESC to close."
        );
    }
}
```

#### Tutorial Hints:
- First inventory open: "Drag items, right-click to rotate"
- First map open: "Click nodes to travel"
- First companion: "Companions help in events"
- First quest: "Complete quests for rewards"
- First ability unlock: "Unlock abilities to gain bonuses"

---

## 📝 IMPLEMENTATION STEPS

### Week 1: Responsive Layout Application
1. Day 1: Update MainMenuScene, CharacterCreationScene
2. Day 2: Update NodeScene, MapScene
3. Day 3: Update TravelCinematicScene, CinematicEventScene
4. Day 4: Update LocationScene, AbilityTreeScene
5. Day 5: Testing and adjustments

### Week 2: Save/Load Integration
1. Day 1: Design save file format
2. Day 2: Implement auto-save on scene transitions
3. Day 3: Implement load game functionality
4. Day 4: Add multiple save slots support
5. Day 5: Testing and bug fixes

### Week 3: Visual Polish
1. Day 1-2: Implement scene transitions (fade in/out)
2. Day 3: Add UI animations (hover, selection)
3. Day 4: Implement HUD enhancements
4. Day 5: Add notification system

### Week 4: Advanced Features
1. Day 1-2: Advanced inventory features (stacking, sorting)
2. Day 3: LocationScene improvements
3. Day 4: Tutorial system
4. Day 5: Sound effects integration (if time permits)

---

## 🧪 TESTING REQUIREMENTS

### Use Test Helpers from Phase 4
```cpp
// Test responsive layout
TEST_F(SceneLayoutTest, MainMenuScalesCorrectly) {
    UILayout layout(800, 600, 1440, 900);
    MainMenuScene scene;
    
    // Verify positions are scaled
    sf::Vector2f scaled = SCALE_POS(layout, 100, 50);
    EXPECT_NEAR(scaled.x, 180.0f, 1.0f);  // 100 * 1.8
    EXPECT_NEAR(scaled.y, 75.0f, 1.0f);   // 50 * 1.5
}

// Test save/load
TEST_F(SaveLoadTest, AutoSaveWorks) {
    PlayerState state;
    state.setMoney(5000.0f);
    
    SaveSystem::getInstance().autoSave();
    
    state.setMoney(0.0f);  // Clear
    SaveSystem::getInstance().loadGame(0);
    
    EXPECT_FLOAT_EQ(state.getMoney(), 5000.0f);
}

// Test animations
TEST_F(AnimationTest, ScaleAnimationCompletes) {
    UIAnimation anim;
    anim.start(UIAnimation::Type::SCALE, 0.2f);
    
    TestHelpers::simulateFrames(scene, 15);  // ~0.25s
    
    EXPECT_TRUE(anim.isComplete());
}
```

### Manual Testing Checklist:
- [ ] All scenes display correctly at 1440x900
- [ ] Save/load preserves all game state
- [ ] Scene transitions are smooth
- [ ] HUD updates in real-time
- [ ] Inventory sorting/stacking works
- [ ] LocationScene interactions are intuitive
- [ ] Animations are smooth
- [ ] Tutorial hints appear correctly
- [ ] Sound effects play at appropriate times
- [ ] No performance degradation

---

## 📦 DELIVERABLES

### New Files:
1. `src/SceneTransition.cpp` - Scene transition effects
2. `src/NotificationManager.cpp` - Notification system
3. `src/AudioManager.cpp` - Sound effects and music
4. `src/TutorialManager.cpp` - Tutorial hints
5. `src/UIAnimation.cpp` - UI animation system
6. `docs/progress/PHASE5_IMPLEMENTATION_SUMMARY.md` - Summary document

### Modified Files:
1. All scene files - Apply responsive layout
2. `src/SceneManager.cpp` - Add auto-save, transitions
3. `src/SaveSystem.cpp` - Enhanced save/load
4. `src/InventoryScene.cpp` - Advanced features
5. `src/LocationScene.cpp` - Improvements
6. `tests/CMakeLists.txt` - New test files

### Documentation:
1. Phase 5 implementation summary
2. Updated PROJECT_STATUS.md
3. Audio asset list
4. Tutorial script

---

## 🎨 DESIGN SPECIFICATIONS

### Color Scheme (Consistent with Phase 4):
```cpp
namespace UIColors {
    const sf::Color Background(30, 30, 46);      // #1E1E2E
    const sf::Color Panel(62, 62, 78);           // #3E3E4E
    const sf::Color Selected(94, 126, 160);      // #5E7EA0
    const sf::Color TextNormal(204, 204, 204);   // #CCCCCC
    const sf::Color TextHighlight(255, 255, 255); // #FFFFFF
    const sf::Color Accent(255, 165, 0);         // #FFA500 (Orange)
    const sf::Color Success(76, 175, 80);        // #4CAF50 (Green)
    const sf::Color Warning(255, 193, 7);        // #FFC107 (Yellow)
    const sf::Color Error(244, 67, 54);          // #F44336 (Red)
}
```

### Animation Timing:
- Button hover: 0.15s
- Selection pulse: 1.0s loop
- Scene transition: 0.6s total (0.3s out + 0.3s in)
- Notification: 3.0s display
- Resource change indicator: 2.0s

### MacBook Air M1 Specific:
- Base resolution: 800x600
- Target resolution: 1440x900
- Scale factor: 1.8x (width), 1.5x (height)
- Font scaling: Use smaller of scale factors (1.5x)

---

## 🔗 DEPENDENCIES

### Existing Systems Used:
- **UILayout** (Phase 4) - Responsive layout
- **Test Helpers** (Phase 4) - Rapid test development
- **SaveSystem** - Save/load functionality
- **PlayerState** - Game state management
- **GridInventorySystem** - Inventory management
- **ExperienceSystem** - Level and XP
- **NPCManager** - Companions
- **QuestManager** - Quest tracking

### External Dependencies:
- SFML Audio (sf::Sound, sf::Music) - For audio system
- JSON library (if not already used) - For enhanced save files

---

## ⚠️ KNOWN CHALLENGES

### 1. Performance with Animations
- **Challenge:** Multiple animations may impact framerate
- **Solution:** Limit concurrent animations, use object pooling
- **Mitigation:** Profile performance, optimize rendering

### 2. Save File Size
- **Challenge:** Large inventories may create big save files
- **Solution:** Compress save files, limit inventory size
- **Mitigation:** Use efficient serialization

### 3. Audio Asset Management
- **Challenge:** Loading many sound files may use memory
- **Solution:** Load on-demand, unload unused sounds
- **Mitigation:** Implement audio streaming for music

### 4. Responsive Layout Edge Cases
- **Challenge:** Some UI elements may not scale well
- **Solution:** Use alignment system, test at multiple resolutions
- **Mitigation:** Provide manual override for problematic elements

---

## 📊 SUCCESS CRITERIA

Phase 5 is complete when:
- ✅ All existing scenes use responsive layout (1440x900)
- ✅ Save/load system works across all scenes
- ✅ Scene transitions have fade effects
- ✅ HUD provides real-time feedback
- ✅ Advanced inventory features implemented
- ✅ LocationScene has enhanced interactions
- ✅ Basic animations implemented
- ✅ Tutorial system functional (optional)
- ✅ Sound effects integrated (optional)
- ✅ All tests passing (>90%)
- ✅ No performance regression
- ✅ Documentation complete

---

## 🚀 NEXT PHASE PREVIEW

**Phase 6: Content Expansion & Gameplay Depth**
- More locations and regions
- Complex quest chains
- Dynamic events system
- Weather and time of day
- Vehicle customization
- Advanced NPC interactions
- Multiple story branches
- Endgame content

---

## 📌 NOTES

### Follow 8 Core Principles:
1. ✅ **Immediate Functionality** - Everything works, no placeholders
2. ✅ **Independence** - Use existing systems
3. ✅ **Strict Specification** - Follow this spec exactly
4. ✅ **Single Development Plan** - No experiments
5. ✅ **Separate Report Directory** - All reports in `docs/progress/`
6. ✅ **UI + Assets Together** - Use primitives until assets ready
7. ✅ **Modular Structure** - Clear separation
8. ✅ **Russian + English** - UI English, comments Russian

### Leverage Phase 4 Infrastructure:
- Use TEST_SCENE_BASIC_FUNCTIONALITY macro
- Use test helper functions for rapid testing
- Use UILayout for all UI positioning
- Use SceneTestFixture for integration tests

### Code Standards:
- C++17 standards
- SFML 2.6 compatibility
- camelCase for variables
- PascalCase for classes
- UI text in English
- Code comments in Russian

---

**Refer to:**
- `docs/progress/PHASE4_IMPLEMENTATION_SUMMARY.md` - Previous phase
- `docs/progress/TEST_HELPERS_AND_LAYOUT_SYSTEM.md` - Infrastructure
- `include/UILayout.h` - Responsive layout system
- `tests/test_helpers.h` - Test infrastructure
- `README.md` - Architecture patterns
- `PROJECT_STATUS.md` - Current state

---

**Estimated Timeline**: 3-4 weeks  
**Priority Level**: High  
**Risk Level**: Medium (audio and animation complexity)  
**Impact**: High (polishes entire game, improves UX significantly)
