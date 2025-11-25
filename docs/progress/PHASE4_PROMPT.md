# Phase 4: Inventory System & UI Polish

**Дата создания / Created**: 2025-11-19  
**Статус / Status**: 🔧 Планирование / Planning  
**Приоритет / Priority**: High  
**Зависимости / Dependencies**: Phase 3 Complete ✅

---

## 📋 ОБЗОР / OVERVIEW

Phase 4 focuses on implementing the missing InventoryScene and polishing the UI across all newly created scenes. This phase will complete the core navigation and menu system, ensuring all NodeScene menu options are functional.

### Цели фазы / Phase Goals
1. Implement InventoryScene with Tarkov-style grid system
2. Polish UI elements in CharacterScene, CompanionsScene, QuestsScene
3. Add smooth scene transitions (fade effects)
4. Integrate with existing systems (ExperienceSystem, NPCManager)
5. Improve visual consistency across all scenes

---

## 🎯 ПРИОРИТЕТЫ / PRIORITIES

### Priority 1: Must Have (Критично)
- [ ] **InventoryScene Implementation** - Complete Tarkov-style grid inventory
- [ ] **ExperienceSystem Integration** - Connect CharacterScene to real level/XP data
- [ ] **NPCManager Integration** - Connect CompanionsScene to real companion data
- [ ] **Scene Transition Effects** - Fade in/out animations
- [ ] **UI Consistency** - Unified color scheme and typography

### Priority 2: Should Have (Желательно)
- [ ] **Keyboard Shortcuts** - Quick access keys (C for Character, I for Inventory, etc.)
- [ ] **HUD Integration** - Context-aware resource display
- [ ] **Visual Feedback** - Hover effects, selection animations
- [ ] **Error Messages** - User-friendly feedback for invalid actions

### Priority 3: Nice to Have (Опционально)
- [ ] **Sound Effects** - UI interaction sounds
- [ ] **Tutorial Hints** - First-time user guidance
- [ ] **Accessibility** - Colorblind mode, larger fonts option

---

## 🔧 DETAILED IMPLEMENTATION PLAN

### 1. InventoryScene Implementation

**Status**: Header exists (`include/InventoryScene.h`), needs implementation

#### Features Required:
- **Grid-based inventory** (6x4 = 24 cells, Tarkov-style)
- **Item size support**: Small (1x1), Medium (2x1 or 1x2), Large (2x2 or 3x2)
- **Mouse-only interaction** within inventory bounds
- **Drag and drop** placement
- **Item rotation** (90 degrees with right-click)
- **Collision detection** (no overlapping items)
- **Weight system** display (current/max weight)
- **Visual cell states** (empty, occupied, invalid placement)

#### UI Layout:
```
+------------------------------------------+
|          INVENTORY (GRID)                |
+------------------------------------------+
| Weight: 20.5 / 64.0 kg      [Close] |
|                                          |
| +----------------------------------+     |
| | [Grid: 6x4 cells]                |     |
| |                                  |     |
| | □□□□□□  ← 1x1 Small items       |     |
| | □□□□□□  ← 2x1 Medium items      |     |
| | □□□□□□  ← 2x2 Large items       |     |
| | □□□□□□                           |     |
| +----------------------------------+     |
|                                          |
| Selected Item:                           |
| +----------------------------------+     |
| | [Item Preview]                   |     |
| | Name: Canned Food                |     |
| | Weight: 0.5 kg                   |     |
| | Description: Emergency rations   |     |
| +----------------------------------+     |
|                                          |
| [R-Click: Rotate] [ESC: Close]           |
+------------------------------------------+
```

#### Files to Create:
- `src/InventoryScene.cpp` - Scene implementation
- Update `include/InventoryScene.h` if needed

#### Integration Points:
- Connect to `GridInventorySystem` (if it exists) or `InventorySystem`
- Link to `PlayerState` for item data
- Add to `SceneManager` creation switch
- Connect from NodeScene menu option "3. Inventory"

#### Technical Requirements:
- Mouse coordinate to grid cell conversion
- Item placement validation (check occupied cells)
- Item rendering with proper positioning
- Drag state management
- Visual feedback for valid/invalid placement

---

### 2. ExperienceSystem Integration

**Current State**: CharacterScene uses placeholder data (level = 1, exp = 0/100)

#### Changes Required:

**In `src/CharacterScene.cpp`:**
```cpp
// Current (placeholder):
m_level = 1;
m_currentExp = 0;
m_maxExp = 100;

// New (real data):
ExperienceSystem& expSystem = ExperienceSystem::getInstance();
m_level = expSystem.getLevel();
m_currentExp = expSystem.getCurrentExperience();
m_maxExp = expSystem.getExperienceForNextLevel();
```

#### Features to Add:
- Display current level from ExperienceSystem
- Show real experience progress
- Display total experience earned
- Show experience needed for next level
- Update in real-time if experience is gained

---

### 3. NPCManager Integration

**Current State**: CompanionsScene shows empty slots only

#### Changes Required:

**In `src/CompanionsScene.cpp`:**
```cpp
// Load real companion data from NPCManager
void CompanionsScene::updateCompanionsDisplay() {
    NPCManager& npcManager = NPCManager::getInstance();
    std::vector<NPC*> teamMembers = npcManager.getTeamMembers();
    
    m_companions.clear();
    for (size_t i = 0; i < m_maxCompanions; ++i) {
        if (i < teamMembers.size() && teamMembers[i]) {
            NPC* npc = teamMembers[i];
            m_companions.push_back(CompanionInfo(
                npc->getName(),
                npc->getRole(),
                npc->getAbilityDescription(),
                npc->getMorale()  // If morale system exists
            ));
        } else {
            m_companions.push_back(CompanionInfo());  // Empty slot
        }
    }
}
```

#### Features to Add:
- Display recruited companions from NPCManager
- Show real companion stats (name, role, abilities)
- Display morale if system exists
- Option to dismiss companion
- Show empty slots for recruitment

---

### 4. Scene Transition Effects

**Current State**: Instant transitions between scenes

#### Implementation:

**Create `SceneTransition` utility class:**

```cpp
// include/SceneTransition.h
class SceneTransition {
public:
    enum class Type {
        NONE,
        FADE,
        SLIDE,
        DISSOLVE
    };
    
    static void fadeOut(sf::RenderWindow& window, float duration);
    static void fadeIn(sf::RenderWindow& window, float duration);
    
private:
    static sf::RectangleShape m_overlay;
};
```

**Update SceneManager:**
- Add transition state (transitioning in/out)
- Render transition overlay
- Delay scene switch until transition completes

#### Transition Timing:
- Fade out: 0.3 seconds
- Scene switch
- Fade in: 0.3 seconds
- Total transition time: 0.6 seconds

---

### 5. UI Polish & Consistency

#### Color Scheme Standardization:
```cpp
// Create shared UI constants
namespace UIColors {
    const sf::Color Background(30, 30, 46);      // #1E1E2E
    const sf::Color Panel(62, 62, 78);           // #3E3E4E
    const sf::Color Selected(94, 126, 160);      // #5E7EA0
    const sf::Color TextNormal(204, 204, 204);   // #CCCCCC
    const sf::Color TextHighlight(255, 255, 255); // #FFFFFF
    const sf::Color Accent(255, 165, 0);         // #FFA500
    const sf::Color Success(76, 175, 80);        // #4CAF50
    const sf::Color Warning(255, 193, 7);        // #FFC107
    const sf::Color Error(244, 67, 54);          // #F44336
}
```

#### Typography Standardization:
- **Title**: 36pt, bold
- **Section Headers**: 28pt, normal
- **Body Text**: 20pt, normal
- **Details**: 16pt, normal
- **Controls Hint**: 14pt, italic

#### Apply to All Scenes:
- NodeScene ✓ (already consistent)
- CharacterScene ✓ (already consistent)
- CompanionsScene ✓ (already consistent)
- QuestsScene ✓ (already consistent)
- InventoryScene (implement with standards)

---

### 6. Keyboard Shortcuts

#### Proposed Shortcuts:
- **M** - Open Map
- **C** - Open Character
- **I** - Open Inventory
- **P** - Open Companions (Party)
- **Q** - Open Quests
- **T** - Open Ability Tree
- **ESC** - Return to NodeScene (already implemented)

#### Implementation:
Add to NodeScene::handleInput():
```cpp
case sf::Keyboard::M:
    m_finished = true;
    m_nextScene = SceneType::WORLD_MAP;
    break;
case sf::Keyboard::C:
    m_finished = true;
    m_nextScene = SceneType::CHARACTER;
    break;
// ... etc
```

---

## 📝 IMPLEMENTATION STEPS

### Step 1: InventoryScene Core (Days 1-2)
1. Create `src/InventoryScene.cpp` basic structure
2. Implement grid rendering (6x4 cells)
3. Add mouse position tracking
4. Implement cell highlighting on hover
5. Test basic rendering

### Step 2: InventoryScene Interactions (Days 2-3)
1. Implement drag and drop
2. Add item placement validation
3. Implement item rotation (R key)
4. Add collision detection
5. Test all interactions

### Step 3: InventoryScene Polish (Day 3)
1. Add item preview panel
2. Implement weight display
3. Add visual feedback (colors for valid/invalid)
4. Connect to actual inventory data
5. Test with real items

### Step 4: System Integrations (Day 4)
1. Integrate ExperienceSystem with CharacterScene
2. Integrate NPCManager with CompanionsScene
3. Test data flow and updates
4. Fix any integration bugs

### Step 5: Transition Effects (Day 5)
1. Create SceneTransition utility class
2. Implement fade in/out
3. Update SceneManager
4. Test all scene transitions
5. Adjust timing if needed

### Step 6: UI Polish (Day 6)
1. Create UIColors constants
2. Apply to all scenes
3. Standardize typography
4. Add keyboard shortcuts
5. Test consistency

### Step 7: Testing & Documentation (Day 7)
1. Test all new features
2. Test all scene transitions
3. Test keyboard shortcuts
4. Update documentation
5. Create Phase 4 summary

---

## 🧪 TESTING REQUIREMENTS

### Manual Testing Checklist:
- [ ] InventoryScene opens from NodeScene menu
- [ ] Grid renders correctly (6x4 cells)
- [ ] Items can be dragged and dropped
- [ ] Item rotation works (R key)
- [ ] Collision detection prevents overlapping
- [ ] Weight display updates correctly
- [ ] ESC returns to NodeScene
- [ ] CharacterScene shows real level/XP
- [ ] CompanionsScene shows real companions
- [ ] Scene transitions are smooth
- [ ] All keyboard shortcuts work
- [ ] UI colors are consistent
- [ ] Typography is consistent
- [ ] No crashes or errors
- [ ] No memory leaks

### Integration Testing:
- [ ] InventoryScene ↔ PlayerState
- [ ] CharacterScene ↔ ExperienceSystem
- [ ] CompanionsScene ↔ NPCManager
- [ ] All scenes ↔ NodeScene
- [ ] SceneManager ↔ All scenes

---

## 📦 DELIVERABLES

### New Files:
1. `src/InventoryScene.cpp` - Full implementation
2. `include/SceneTransition.h` - Transition utility (optional)
3. `src/SceneTransition.cpp` - Transition implementation (optional)
4. `include/UIConstants.h` - Shared UI constants (optional)
5. `docs/progress/PHASE4_IMPLEMENTATION_SUMMARY.md` - Summary document

### Modified Files:
1. `src/CharacterScene.cpp` - ExperienceSystem integration
2. `src/CompanionsScene.cpp` - NPCManager integration
3. `src/NodeScene.cpp` - Keyboard shortcuts
4. `src/SceneManager.cpp` - Transition effects, InventoryScene creation
5. `CMakeLists.txt` - New source files

### Documentation:
1. Phase 4 implementation summary
2. Updated PROJECT_STATUS.md
3. Updated DEVELOPMENT_ROADMAP.md

---

## 🎨 DESIGN SPECIFICATIONS

### InventoryScene Visual Design:

**Grid Cells:**
- Empty cell: Dark gray (#282828), 1px border
- Occupied cell: Medium gray (#404040), 1px border
- Hovered cell: Light blue (#5E7EA0), 2px border
- Invalid placement: Red (#F44336), 2px border
- Valid placement: Green (#4CAF50), 2px border

**Cell Size:**
- 80x80 pixels per cell
- 5px spacing between cells
- Total grid: 485x325 pixels (6x4 cells + spacing)

**Item Display:**
- Items rendered with colored rectangles (until sprites available)
- Small (1x1): 80x80px, Green
- Medium (2x1): 165x80px, Yellow
- Medium (1x2): 80x165px, Yellow
- Large (2x2): 165x165px, Orange
- Large (3x2): 250x165px, Red

---

## 🔗 DEPENDENCIES

### Existing Systems Used:
- **InventorySystem** - Item storage and management
- **GridInventorySystem** - If exists, grid-based inventory logic
- **ExperienceSystem** - Level and XP data
- **NPCManager** - Companion data
- **PlayerState** - Resource data
- **SceneManager** - Scene management

### No New External Dependencies Required

---

## ⚠️ KNOWN ISSUES TO ADDRESS

1. **Font Loading** - Already fixed in Phase 3
2. **Scene Navigation** - Already working
3. **Cell Indicators** - Already implemented

### New Potential Issues:
- Mouse coordinate conversion accuracy
- Item drag performance with many items
- Transition effect performance
- Integration with mock/real data

---

## 📊 SUCCESS CRITERIA

Phase 4 is complete when:
- ✅ InventoryScene fully functional with grid system
- ✅ Items can be dragged, dropped, and rotated
- ✅ CharacterScene shows real ExperienceSystem data
- ✅ CompanionsScene shows real NPCManager data
- ✅ Scene transitions have fade effects
- ✅ Keyboard shortcuts work for all scenes
- ✅ UI is visually consistent across all scenes
- ✅ No compilation errors or warnings
- ✅ All manual testing checklist items pass
- ✅ Documentation updated

---

## 🚀 NEXT PHASE PREVIEW

**Phase 5: Location Scenes & Advanced Interactions**
- Implement LocationScenes for gas stations, stores, motels
- Add interactive elements (refueling, shopping, resting)
- Implement dialogue system integration
- Add transaction validation
- Create location-specific UI

---

## 📌 NOTES

### Follow 8 Core Principles:
1. ✅ **Immediate Functionality** - Everything works, no placeholders
2. ✅ **Independence** - Use existing systems, no new dependencies
3. ✅ **Strict Specification** - Follow this spec exactly
4. ✅ **Single Development Plan** - No experiments
5. ✅ **Separate Report Directory** - All reports in `docs/progress/`
6. ✅ **UI + Assets Together** - Use primitives until assets ready
7. ✅ **Modular Structure** - Clear separation
8. ✅ **Russian + English** - UI English, comments Russian

### Code Standards:
- C++17 standards
- SFML 2.6 compatibility
- camelCase for variables
- PascalCase for classes
- UI text in English
- Code comments in Russian

---

**Refer to:**
- `docs/PROMPT_TEMPLATE.md` - Project principles
- `README.md` - Architecture patterns
- `PROJECT_STATUS.md` - Current state
- `UI_COMPONENTS_SPEC.md` - UI guidelines (if exists)
- `docs/progress/PHASE3_IMPLEMENTATION_SUMMARY.md` - Previous phase

---

**Estimated Timeline**: 7 days  
**Priority Level**: High  
**Risk Level**: Medium (InventoryScene complexity)  
**Impact**: High (completes core navigation system)
