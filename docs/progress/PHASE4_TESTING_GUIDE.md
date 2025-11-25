# Phase 4: Testing Guide

**Date:** 2025-11-19  
**Branch:** copilot/add-inventory-scene-ui-polish  
**Status:** Ready for Testing

---

## 🎯 QUICK START

### Building the Project

```bash
# Navigate to project directory
cd /home/runner/work/kkurs/kkurs

# Create build directory
mkdir -p build && cd build

# Configure with SFML (Mac example - adjust path as needed)
cmake .. -DSFML_DIR=/opt/homebrew/opt/sfml@2/lib/cmake/SFML

# OR for Ubuntu/Linux
cmake .. -DSFML_DIR=/usr/lib/x86_64-linux-gnu/cmake/SFML

# Build
make -j4

# Run
./kkurs
```

---

## 🧪 TESTING CHECKLIST

### 1. InventoryScene Tests

**Access:**
- [ ] From NodeScene: Press "3" or "I"
- [ ] Menu shows "3. Inventory"
- [ ] Scene loads without errors

**Visual Elements:**
- [ ] Grid displays (6 columns × 4 rows = 24 cells)
- [ ] Title shows "INVENTORY"
- [ ] Weight display shows "Weight: X.X / 64.0 kg"
- [ ] Detail panel on right side
- [ ] Close button (red, top-right)
- [ ] Controls hint at bottom

**Test Items (Pre-loaded):**
- [ ] Canned Food (1x1, green) - Position (0,0)
- [ ] Water Bottle (1x1, green) - Position (1,0)
- [ ] First Aid Kit (2x1, yellow) - Position (0,1)
- [ ] Fuel Can (2x2, orange) - Position (3,0)

**Mouse Interaction:**
- [ ] Hover over cells → blue highlight appears
- [ ] Hover over item → detail panel updates
- [ ] Left-click item → item follows mouse (drag state)
- [ ] Drag to empty space → green highlight (valid)
- [ ] Drag to occupied space → red highlight (invalid)
- [ ] Release on valid space → item placed
- [ ] Release on invalid space → item returns to original position

**Item Rotation:**
- [ ] Right-click item → item rotates 90 degrees
- [ ] First Aid Kit (2x1) → becomes (1x2)
- [ ] Rotated item placement validation works
- [ ] Visual representation updates

**Item Details Panel:**
- [ ] Shows "Select an item" when nothing hovered
- [ ] Hover item shows:
  - "Item Details:"
  - Name (e.g., "Fuel Can")
  - Weight (e.g., "5.0 kg")
  - Value (e.g., "30 RUB")
  - Size (e.g., "2x2 cells")
  - Rotation status if rotated

**Weight System:**
- [ ] Total weight displays correctly
- [ ] Weight updates when items moved
- [ ] Format: "Weight: 8.5 / 64.0 kg"

**Navigation:**
- [ ] ESC closes inventory
- [ ] Returns to NodeScene

---

### 2. CharacterScene Tests (ExperienceSystem Integration)

**Access:**
- [ ] From NodeScene: Press "2" or "C"
- [ ] Scene loads

**Display:**
- [ ] Shows "CHARACTER INFORMATION" title
- [ ] Player name displays
- [ ] Backstory displays (Ex-Racer, Hitchhiker, or Nomad)
- [ ] **Level displays** (should show real level, not always 1)
- [ ] **XP displays** (should show real experience, not always 0/100)
- [ ] Experience bar fills according to real data
- [ ] Skills display with cell indicators
- [ ] Active effects display if any

**Testing Experience System:**
To test the integration, you can add experience in the game:
```cpp
// In any scene or event, add:
ExperienceSystem::getInstance().addExperience(50);
```

Then check CharacterScene to see:
- [ ] Level increases when enough XP
- [ ] XP bar updates
- [ ] Experience carries over correctly

**Navigation:**
- [ ] ESC returns to NodeScene

---

### 3. CompanionsScene Tests (NPCManager Integration)

**Access:**
- [ ] From NodeScene: Press "4" or "P"
- [ ] Scene loads

**Display:**
- [ ] Shows "PARTY & COMPANIONS" title
- [ ] Vehicle type displays correctly
- [ ] Capacity shows (e.g., "Capacity: 0/2 companions")
- [ ] Companion slots display based on vehicle:
  - Sports Coupe: 2 slots
  - Old Sedan: 3 slots
  - Offroad Van: 4 slots

**Without Companions (Initial State):**
- [ ] All slots show "Empty"
- [ ] Shows "> Recruit Companion" text
- [ ] Capacity: 0/X

**With Companions (To Test):**
To add test companions for testing:
```cpp
// In game initialization or test scene:
NPCManager& npcManager = NPCManager::getInstance();
npcManager.addNPC(NPCFactory::createMechanic());
npcManager.addNPC(NPCFactory::createTrader());
```

Then check CompanionsScene:
- [ ] Shows NPC names
- [ ] Shows NPC bios (as roles)
- [ ] Shows passive abilities
- [ ] Morale displays (cell indicator)
- [ ] Capacity count updates (e.g., "2/2")

**Navigation:**
- [ ] Up/Down keys navigate slots
- [ ] Selected slot highlights
- [ ] ESC returns to NodeScene

---

### 4. Keyboard Shortcuts Tests

**From NodeScene, press each key:**
- [ ] **M** → Opens World Map
- [ ] **C** → Opens Character scene
- [ ] **I** → Opens Inventory scene
- [ ] **P** → Opens Companions (Party) scene
- [ ] **Q** → Opens Quests scene
- [ ] **T** → Opens Ability Tree scene

**Verify:**
- [ ] All shortcuts work instantly
- [ ] No lag or errors
- [ ] Scene transitions smooth
- [ ] ESC returns from all scenes

**Controls Hint:**
- [ ] Bottom of NodeScene shows: "[Up/Down: Select] [Enter: Confirm] [ESC: Exit] [M/C/I/P/Q/T: Quick Access]"

---

## 🐛 KNOWN ISSUES

### Expected Behavior

1. **Empty Companions Initially**
   - CompanionsScene shows empty slots
   - This is correct - no companions recruited yet
   - Will show real data once NPCs are recruited in gameplay

2. **Level 1 / 0 XP Initially**
   - New game starts at level 1 with 0 experience
   - This is correct
   - Will update as player gains experience

3. **Test Items Only**
   - Inventory shows 4 hardcoded test items
   - This is for demonstration
   - Will connect to real item system in future

### Not Bugs

- AbilityTreeSystem test failures (9/97 tests) - pre-existing, unrelated to Phase 4
- Inventory items don't persist - test items only, no save/load yet
- Can't remove items from inventory - needs future implementation

---

## 📸 SCREENSHOT GUIDE

When testing, take screenshots of:

1. **InventoryScene:**
   - Grid with test items
   - Dragging an item (item following mouse)
   - Valid placement (green highlight)
   - Invalid placement (red highlight)
   - Rotated item
   - Detail panel showing item info

2. **CharacterScene:**
   - Full scene showing level/XP
   - Skill indicators (cells)
   - Experience bar

3. **CompanionsScene:**
   - Empty slots (initial state)
   - With companions (if you add test NPCs)
   - Morale indicator

4. **Keyboard Shortcuts:**
   - NodeScene menu with controls hint
   - Quick access to each scene

---

## 🔍 WHAT TO LOOK FOR

### Visual Quality
- [ ] Text is readable (font loaded correctly)
- [ ] Colors match specification:
  - Background: Dark (#1E1E2E)
  - Panels: Medium gray (#3E3E4E)
  - Highlights: Light blue (#5E7EA0)
  - Success: Green (#4CAF50)
  - Error: Red (#F44336)
- [ ] No visual glitches or artifacts
- [ ] Smooth rendering (no flickering)

### Interaction Quality
- [ ] Mouse tracking accurate
- [ ] Drag feels responsive
- [ ] Highlights appear/disappear correctly
- [ ] No lag when moving items
- [ ] Keyboard shortcuts instant

### Integration Quality
- [ ] CharacterScene shows real data (not placeholders)
- [ ] CompanionsScene shows real data (when NPCs exist)
- [ ] Weight calculations accurate
- [ ] Scene transitions smooth
- [ ] No crashes or freezes

---

## 🚀 PERFORMANCE

Expected performance:
- **Startup:** < 1 second
- **Scene transitions:** Instant (no fade yet)
- **Mouse interaction:** < 16ms latency
- **Rendering:** 60 FPS
- **Memory:** < 100 MB

---

## 💡 TESTING TIPS

### To Test ExperienceSystem Integration:
Add this in `main.cpp` after character creation:
```cpp
ExperienceSystem::getInstance().addExperience(150);
```
Then check CharacterScene - should show level 2 with updated XP.

### To Test NPCManager Integration:
Add this in `main.cpp` after character creation:
```cpp
NPCManager& npcManager = NPCManager::getInstance();
npcManager.addNPC(NPCFactory::createMechanic());
npcManager.addNPC(NPCFactory::createTrader());
```
Then check CompanionsScene - should show 2 companions with details.

### To Test Inventory with More Items:
In `InventoryScene::addTestItems()`, add more items:
```cpp
Item item5;
item5.name = "Toolbox";
item5.weight = 3.0f;
item5.value = 75.0f;
m_gridInventory->addItem(item5, 3, 2, 0, 2, 1);
```

---

## 📋 REPORTED BUGS FORMAT

If you find issues, report with:

```
**Bug:** [Brief description]
**Scene:** [Which scene - Inventory/Character/Companions/NodeScene]
**Steps to Reproduce:**
1. Step 1
2. Step 2
3. Step 3

**Expected:** [What should happen]
**Actual:** [What happened]
**Screenshot:** [If applicable]
**Build:** [Date/commit hash]
```

---

## ✅ SIGN-OFF

When testing complete, verify:
- [ ] All Priority 1 features work
- [ ] No crashes or errors
- [ ] Performance acceptable
- [ ] Visual quality good
- [ ] Documentation accurate

---

**Happy Testing!** 🎮

For questions or issues, refer to:
- `docs/progress/PHASE4_IMPLEMENTATION_SUMMARY.md` - Complete implementation details
- `PROJECT_STATUS.md` - Project status
- `README.md` - General project information
