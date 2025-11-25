# Prompt for Branch 3: Phase 5 - Character Ability Tree System Implementation

## Context
You are working on the 'barzikol/kkurs' repository, a C++ SFML-based road trip game project. This branch is dedicated to implementing Phase 5: Character Ability Tree System with charges-based perks, rest mechanics, and Pip-Boy style UI.

## Current Project State (from main branch)

### ✅ Completed Systems (Already Working):
1. **Menu & Character Creation** - Fallout-style terminal, 3 backstories, 3 cars
2. **NPC & Experience System** - Team management, leveling 1-3, abilities
3. **Navigation** - NodeScene (city driving), MapScene (route selection)
4. **Travel & Events** - Cinematic travel with parallax, fuel consumption, random events
5. **Quest System** - 6 objective types, auto-tracking, progress calculation
6. **Location System** - 7 location types with resource transactions
7. **UI/HUD System** - ResourceBar, HUDPanel with progress bars, Unicode icons

### ⚠️ CRITICAL: What's Already Implemented in THIS Branch (copilot/add-character-skill-branch)

**DO NOT RE-IMPLEMENT THESE - THEY ALREADY EXIST AND WORK:**

#### Core Ability Tree System ✅
- **Files**: `include/AbilityTypes.h`, `include/AbilityNode.h`, `include/AbilityTreeSystem.h`
- **Implementation**: `src/AbilityNode.cpp`, `src/AbilityTreeSystem.cpp`
- **Status**: ~20 balanced abilities across 5 categories (Driving, Trading, Survival, Mechanics, Social)
- **Features**: 
  - Level-gated progression (levels 1-8+)
  - Prerequisite chains
  - Skill points economy (1 point per level)
  - Passive bonuses (3-5% per level)
  - Active perks with charges (1-5 charges per perk)

#### Charges System ✅
- **Replaces**: Cooldown system (no more cooldowns!)
- **Mechanism**: Each active perk has 1-5 charges
- **Restoration**: Only through rest (car or motel)
- **API**:
  - `getCurrentCharges(perkId)`
  - `getMaxCharges(perkId)`
  - `hasCharges(perkId)`
  - `restoreCharges()` - restores ALL charges

#### Rest System ✅
- **File**: `src/RestScene.cpp`
- **Types**:
  1. **Car Rest** - Free, 4h, +30 energy, -5% vehicle, 30% negative status chance
  2. **Motel Rest** - 300₽, 8h, full restoration, +10% efficiency buff
- **Integration**: RestScene UI with Pip-Boy terminal style

#### Event Integration ✅
- **Files**: Modified `src/EventManager.cpp`, `src/CinematicEventScene.cpp`
- **Feature**: Perks appear as event choices with 💡 icon
- **Display**: Shows charges like `[Charges: 3/5]`
- **Mechanism**: Perks filter by ResourcePreservationType (CHARACTER_ENERGY, FUEL, MONEY, VEHICLE_CONDITION)

#### AbilityTreeScene UI ✅
- **Files**: `include/AbilityTreeScene.h`, `src/AbilityTreeScene.cpp`
- **Style**: Pip-Boy terminal (green monochrome #00FF00 on #000000)
- **Navigation**: Category tabs (1-5 keys), W/S arrows, Enter to unlock
- **Display**: Tree structure with [✓][ ][✗] symbols

#### Tests ✅
- **Files**: `tests/test_AbilityNode.cpp`, `tests/test_AbilityTreeSystem.cpp`
- **Count**: 38 unit tests (16 + 22)

#### Documentation ✅
- `docs/ABILITY_TREE_SYSTEM.md` (33KB)
- `docs/ABILITY_TREE_TECHNICAL_SPEC.md` (37KB)
- `docs/ABILITY_TREE_UI_PIPBOY.md` (19KB)
- `docs/ABILITY_TREE_VISUALIZATION.md` (28KB)
- `docs/RESOURCE_PRESERVATION_SYSTEM.md` (16KB)
- `docs/REST_AND_CHARGES_SYSTEM.md` (12KB)
- `docs/progress/SAVE_LOAD_INTEGRATION_TASK.md` (8KB)
- `docs/progress/ability_tree_implementation.md` (14KB)

---

## Your Mission: Next Phase Implementation

### 🎯 What Still Needs to Be Done

Based on the comment from @denisov2956151998_barzikol requesting "что у нас дальше по плану можешь сделать промт файл для дальнейшей реализации и открытия новой ветки", here are the priority tasks for the NEXT branch:

---

### Priority 1: Status/Buff System 🌟
**Why**: Rest system creates positive/negative effects but no system to apply them
**Impact**: HIGH - Enables rest mechanics to actually work

#### Deliverables:
1. **StatusEffect Class** (`include/StatusEffect.h`)
   ```cpp
   enum class StatusType {
       POSITIVE,  // Buffs (from motel)
       NEGATIVE,  // Debuffs (from car rest)
       NEUTRAL
   };
   
   enum class StatusCategory {
       EFFICIENCY,      // Affects action efficiency
       ENERGY,          // Max energy modifiers
       MORALE,          // Morale effects
       REGENERATION,    // Regen rate modifiers
       VEHICLE          // Vehicle wear/fuel effects
   };
   
   class StatusEffect {
   private:
       std::string id;
       std::string name;
       std::string description;
       StatusType type;
       StatusCategory category;
       float magnitude;         // e.g., -10 for max energy, +10% for efficiency
       float durationMinutes;   // Remaining duration
       bool isPermanent;
       
   public:
       StatusEffect(std::string id, std::string name, StatusType type, 
                    StatusCategory cat, float mag, float duration);
       
       void update(float deltaMinutes);
       bool isExpired() const;
       float getMagnitude() const;
       StatusCategory getCategory() const;
   };
   ```

2. **StatusManager Singleton** (`include/StatusManager.h`)
   ```cpp
   class StatusManager {
   private:
       static StatusManager* instance;
       std::vector<StatusEffect> activeStatuses;
       
   public:
       static StatusManager& getInstance();
       
       void addStatus(const StatusEffect& status);
       void removeStatus(const std::string& statusId);
       void updateStatuses(float deltaMinutes);
       void clearExpired();
       
       std::vector<StatusEffect> getActiveStatuses() const;
       float getTotalModifier(StatusCategory category) const;
       bool hasStatus(const std::string& statusId) const;
       
       void reset();  // For testing
   };
   ```

3. **Integration with RestScene**
   - Modify `src/RestScene.cpp` to add statuses:
   ```cpp
   // Car rest negative effects (30% chance)
   if (rand() % 100 < 30) {
       StatusEffect badSleep("uncomfortable_sleep", "Неудобный сон", 
                            StatusType::NEGATIVE, StatusCategory::ENERGY,
                            -10.0f, 720.0f);  // -10 max energy for 12h
       StatusManager::getInstance().addStatus(badSleep);
   }
   
   // Motel rest positive effects (always)
   StatusEffect greatSleep("great_sleep", "Отличный сон",
                          StatusType::POSITIVE, StatusCategory::EFFICIENCY,
                          10.0f, 1440.0f);  // +10% efficiency for 24h
   StatusManager::getInstance().addStatus(greatSleep);
   ```

4. **Apply Status Effects in Gameplay**
   - Modify `PlayerState` to use status modifiers:
   ```cpp
   float PlayerState::getMaxEnergy() const {
       float base = baseMaxEnergy;
       float modifier = StatusManager::getInstance()
           .getTotalModifier(StatusCategory::ENERGY);
       return base + modifier;
   }
   ```

5. **UI Display for Statuses** (optional - can be TODO)
   - Add status icons to HUDPanel
   - Show remaining duration
   - Color-code positive/negative

#### Testing:
- 10+ unit tests for StatusEffect
- 10+ unit tests for StatusManager
- Integration test: rest → status → effect application

---

### Priority 2: Save/Load System 💾
**Why**: Players need to save progress, especially ability unlocks and charges
**Impact**: HIGH - Core functionality for any game

#### Deliverables:
1. **SaveManager Singleton** (`include/SaveManager.h`)
   ```cpp
   class SaveManager {
   private:
       static SaveManager* instance;
       std::string savePath;
       
   public:
       static SaveManager& getInstance();
       
       bool save(const std::string& slotName, const GameState& state);
       bool load(const std::string& slotName, GameState& state);
       
       std::vector<std::string> listSaves() const;
       bool deleteSave(const std::string& slotName);
       bool saveExists(const std::string& slotName) const;
       
       // Ability tree specific
       nlohmann::json serializeAbilityTree() const;
       void deserializeAbilityTree(const nlohmann::json& data);
   };
   ```

2. **JSON Format for Ability Tree** (from docs/progress/SAVE_LOAD_INTEGRATION_TASK.md):
   ```json
   {
       "abilityTree": {
           "unlockedAbilities": ["driving_economical_1", "driving_economical_2"],
           "skillPoints": 3,
           "perkCharges": {
               "perk_fuel_saver": {"current": 2, "max": 3},
               "perk_energy_saver": {"current": 5, "max": 5}
           }
       },
       "playerState": {
           "level": 5,
           "experience": 350,
           "money": 1250.0,
           "energy": 70.0,
           "maxEnergy": 100.0
       },
       "car": {
           "type": "bmw_e30",
           "fuel": 45.5,
           "condition": 85.0
       },
       "activeStatuses": [
           {
               "id": "great_sleep",
               "remainingMinutes": 1200.0
           }
       ]
   }
   ```

3. **Integration Points**:
   - Modify `AbilityTreeSystem` to add:
     - `nlohmann::json save() const;`
     - `void load(const nlohmann::json& data);`
   - Modify `GameState` to add:
     - `bool saveGame(const std::string& slotName);`
     - `bool loadGame(const std::string& slotName);`

4. **SaveLoadScene UI** (Pip-Boy style)
   - List of save slots (3-5 slots)
   - Auto-save slot
   - Save date/time display
   - Player level and location
   - Navigation: W/S, Enter to load, DEL to delete

#### Testing:
- 10+ unit tests for SaveManager
- Integration test: save → load → verify state
- Test corrupted file handling
- Test backward compatibility

---

### Priority 3: PlayerState Passive Bonus Integration 🎮
**Why**: Passive abilities don't actually affect gameplay yet
**Impact**: MEDIUM - Makes abilities feel impactful

#### Deliverables:
1. **Modify PlayerState** (`src/PlayerState.cpp`):
   ```cpp
   float PlayerState::getFuelConsumptionRate() const {
       float base = baseFuelConsumption;
       
       // Apply passive bonuses from ability tree
       float fuelEfficiency = AbilityTreeSystem::getInstance()
           .getPassiveBonus("fuel_efficiency");
       
       // Apply status modifiers
       float statusMod = StatusManager::getInstance()
           .getTotalModifier(StatusCategory::EFFICIENCY);
       
       return base * (1.0f / fuelEfficiency) * (1.0f - statusMod / 100.0f);
   }
   
   float PlayerState::getMaxEnergy() const {
       float base = baseMaxEnergy;
       
       // Apply passive bonuses
       float energyBonus = AbilityTreeSystem::getInstance()
           .getPassiveFlatBonus("max_energy");
       
       // Apply status modifiers
       float statusMod = StatusManager::getInstance()
           .getTotalModifier(StatusCategory::ENERGY);
       
       return base + energyBonus + statusMod;
   }
   ```

2. **Key Methods to Modify**:
   - `getFuelConsumptionRate()` - Apply fuel efficiency passive
   - `getMaxEnergy()` - Apply stamina passives
   - `getEnergyRegenRate()` - Apply fast recovery passives
   - `getRepairCost()` - Apply mechanic discount passives
   - `getTradingDiscount()` - Apply trader passives

3. **Testing**:
   - Test each passive bonus type
   - Test stacking multiple passives
   - Test bonus removal when ability locked

---

### Priority 4: HUDPanel Perk Display Enhancement 📊
**Why**: Players need to see active perks and their charges
**Impact**: MEDIUM - Improves UX

#### Deliverables:
1. **Add to HUDPanel** (`src/UI/HUDPanel.cpp`):
   - Display active perks (unlocked abilities with charges)
   - Show charges like `⚡ Energy Saver [3/5]`
   - Color-code by charge level:
     - Green (>70% charges)
     - Yellow (30-70% charges)
     - Red (<30% charges)
   - Position: Below resource displays

2. **Perk Icon Display**:
   ```
   ┌────────────────────────┐
   │ 💰 1,250₽              │
   │ ⛽ 45.5/100L            │
   │ [████████░░] 45%       │
   │ ⚡ 70/100               │
   │ [████████░░] 70%       │
   │                        │
   │ ПЕРКИ / PERKS:         │
   │ ⛽ Fuel Saver [2/3]    │ ← New
   │ ⚡ Energy Saver [5/5]  │ ← New
   │ 💰 Money Saver [0/4]   │ ← New (depleted)
   └────────────────────────┘
   ```

3. **Hotkey Hints** (optional):
   - Show which keys activate perks during events
   - Flash when perk is available

#### Testing:
- Visual verification in all scenes
- Test with 0 charges, partial charges, full charges
- Test with no perks unlocked

---

### Priority 5: Additional Abilities & Content 🎯
**Why**: More content = more replayability
**Impact**: LOW - Can be done incrementally

#### Deliverables:
1. **Expand Ability Count** (from ~20 to 30-40):
   - Add higher-tier abilities (levels 9-15)
   - Add prestige abilities (level 15+)
   - Add combo abilities (require multiple prerequisites)

2. **New Ability Categories** (optional):
   - Leadership (affects NPC team efficiency)
   - Luck (better event outcomes)
   - Survival (weather/season resistance)

3. **Ability Synergies**:
   - Define synergy bonuses (e.g., Economical Driving I+II+III = extra 2% bonus)
   - Display synergies in AbilityTreeScene

#### Testing:
- Balance testing with game metrics
- Playtest different builds

---

## ⚡ COMPRESSED Implementation Plan for NEXT Branch

### Branch Name Suggestion: `feature/status-buff-system`

### ⚠️ URGENT Timeline: 1.5 Weeks (11 Days)

**Context**: Other branches will merge soon. Must complete quickly for integration before next phase.

---

### Days 1-3: Status/Buff System (PRIORITY 1) 🌟
**Goal**: Make rest effects actually work

**Day 1**: Core Implementation
- [ ] Create `include/StatusEffect.h` + `src/StatusEffect.cpp`
- [ ] Create `include/StatusManager.h` + `src/StatusManager.cpp`
- [ ] Add to CMakeLists.txt
- [ ] 8 core unit tests (constructor, add/remove, expiry)

**Day 2**: Integration
- [ ] Integrate with RestScene (create negative/positive effects)
- [ ] Integrate with PlayerState (apply efficiency/energy modifiers)
- [ ] Integrate with TravelCinematicScene (updateStatuses call)
- [ ] 7+ integration tests

**Day 3**: Testing & Polish
- [ ] End-to-end testing (car rest → negative status → affects gameplay)
- [ ] Documentation in `docs/STATUS_BUFF_SYSTEM.md`
- [ ] Code review

---

### Days 4-6: Passive Bonus Integration (PRIORITY 2) 🎮
**Goal**: Passive abilities affect gameplay

**Day 4**: PlayerState Modifications
- [ ] Modify PlayerState.cpp - add passive bonus checks
- [ ] Implement getFuelConsumptionRate() with bonuses
- [ ] Implement getMaxEnergy() with bonuses
- [ ] Test fuel efficiency abilities work

**Day 5**: Trading & More
- [ ] Implement getTradingDiscount() with bonuses
- [ ] Implement vehicle wear reduction
- [ ] Test all passive categories
- [ ] 5+ tests per bonus type

**Day 6**: Integration Testing
- [ ] Test combinations (multiple passives stacking)
- [ ] Test edge cases (no abilities unlocked)
- [ ] Performance check (O(N) lookups acceptable)
- [ ] Documentation in `docs/PASSIVE_BONUS_INTEGRATION.md`

---

### Days 7-9: Save/Load Core (PRIORITY 3) 💾
**Goal**: Ability progress persists

⚠️ **SCOPE**: AbilityTreeSystem ONLY (SaveLoadScene UI deferred to main save/load branch)

**Day 7**: AbilityTreeSystem Methods
- [ ] Add save() method to AbilityTreeSystem
- [ ] Add load() method to AbilityTreeSystem
- [ ] JSON format: unlocked abilities, skill points, charges
- [ ] 4+ save/load unit tests

**Day 8**: JSON Serialization
- [ ] Implement toJSON() for ability data
- [ ] Implement fromJSON() for ability data
- [ ] Validation (prevent corrupt data)
- [ ] Test backward compatibility (empty save file)

**Day 9**: Integration & Testing
- [ ] Test save → quit → load → verify state
- [ ] Test edge cases (invalid JSON, missing fields)
- [ ] Documentation in `docs/progress/ability_tree_save_load.md`
- [ ] Coordinate JSON format with main save/load branch

---

### Days 10-11: HUD Perk Display (PRIORITY 4 - OPTIONAL) 📊
**Goal**: Players see active perks

**Day 10**: Basic Implementation
- [ ] Add perk icons to HUDPanel (simple colored boxes)
- [ ] Show charge counters (e.g., "⚡ 3/5")
- [ ] Color-code by charge level (green: full, yellow: <50%, red: <25%)
- [ ] Test display updates when charges consumed

**Day 11**: Polish & Buffer
- [ ] Add tooltips on hover (perk name + effect)
- [ ] Handle depleted perks (grey out or hide)
- [ ] Documentation
- [ ] **BUFFER**: Fix any blockers from previous days

---

### ⏩ Deferred to Other Branches

**EXPLICITLY NOT IN THIS BRANCH**:
- ❌ SaveLoadScene UI (handled by main save/load branch)
- ❌ Comprehensive StatusManager UI panel
- ❌ Content expansion (30-40 abilities)
- ❌ Advanced HUD polish and animations
- ❌ Full save/load for all game systems (just AbilityTreeSystem)

**Coordination Required**:
- 🔗 Save/load branch: JSON format compatibility
- 🔗 UI/HUD branch: HUD enhancement after merge
- 🔗 Content branch: Ability expansion in future phase

---

### 📊 Scope Comparison

| Task | Original (4 weeks) | Compressed (1.5 weeks) |
|------|-------------------|------------------------|
| **Status System** | Full + UI | Core only |
| **Passive Bonuses** | Full integration | Core integration |
| **Save/Load** | Complete system | AbilityTree only |
| **HUD Display** | Full polish | Basic display |
| **Tests** | 50+ tests | 33+ critical tests |
| **Polish** | Extensive | Minimal |

**Achievable**: ✅ Core functionality working  
**Deferred**: ⏩ UI polish and expansion

---

## Technical Specifications

### Status System Architecture:
```
StatusManager (Singleton)
    ├── std::vector<StatusEffect> activeStatuses
    ├── updateStatuses(deltaMinutes)
    └── getTotalModifier(category) → aggregates all statuses

PlayerState
    ├── Uses StatusManager::getTotalModifier()
    ├── Applies to resource calculations
    └── No direct status storage

RestScene
    ├── Creates StatusEffect objects
    ├── Adds to StatusManager
    └── Calls restoreCharges()
```

### Save/Load Data Flow:
```
GameState::saveGame()
    ├── AbilityTreeSystem::save() → JSON
    ├── StatusManager::save() → JSON
    ├── PlayerState::save() → JSON
    ├── Car::save() → JSON
    └── SaveManager::save(slot, mergedJSON)

GameState::loadGame()
    ├── SaveManager::load(slot) → JSON
    ├── AbilityTreeSystem::load(JSON)
    ├── StatusManager::load(JSON)
    ├── PlayerState::load(JSON)
    └── Car::load(JSON)
```

### JSON Library:
Use `nlohmann/json` (already popular in C++ projects):
```bash
# Add to CMakeLists.txt
find_package(nlohmann_json 3.11.0 REQUIRED)
target_link_libraries(kkurs PRIVATE nlohmann_json::nlohmann_json)
```

---

## Code Examples

### Example 1: Creating and Adding a Status
```cpp
// In RestScene.cpp - after car rest
if (shouldApplyNegativeEffect) {
    StatusEffect uncomfortableSleep(
        "uncomfortable_sleep",
        "Неудобный сон / Uncomfortable Sleep",
        StatusType::NEGATIVE,
        StatusCategory::ENERGY,
        -10.0f,     // -10 max energy
        720.0f      // 12 hours (720 minutes)
    );
    StatusManager::getInstance().addStatus(uncomfortableSleep);
}
```

### Example 2: Applying Status Modifiers
```cpp
// In PlayerState.cpp
float PlayerState::getMaxEnergy() const {
    float base = 100.0f;
    
    // Passive abilities
    float stamina = AbilityTreeSystem::getInstance()
        .getPassiveFlatBonus("max_energy");
    
    // Status effects
    float statusMod = StatusManager::getInstance()
        .getTotalModifier(StatusCategory::ENERGY);
    
    return base + stamina + statusMod;  // e.g., 100 + 15 - 10 = 105
}
```

### Example 3: Saving Ability Tree
```cpp
// In AbilityTreeSystem.cpp
nlohmann::json AbilityTreeSystem::save() const {
    nlohmann::json data;
    
    data["unlockedAbilities"] = nlohmann::json::array();
    for (const auto& [id, node] : abilities) {
        if (node.isUnlocked()) {
            data["unlockedAbilities"].push_back(id);
        }
    }
    
    data["skillPoints"] = availableSkillPoints;
    
    data["perkCharges"] = nlohmann::json::object();
    for (const auto& [perkId, charges] : perkCharges) {
        data["perkCharges"][perkId] = {
            {"current", charges.current},
            {"max", charges.max}
        };
    }
    
    return data;
}
```

---

## Testing Strategy

### Unit Tests (50+ new tests):
- `tests/test_StatusEffect.cpp` (10 tests)
- `tests/test_StatusManager.cpp` (15 tests)
- `tests/test_SaveManager.cpp` (15 tests)
- `tests/test_PlayerStateIntegration.cpp` (10 tests)

### Integration Tests:
- Rest → Status → Effect → Gameplay
- Save → Load → Verify all state
- Ability unlock → Passive bonus → Resource calculation

### Manual Testing:
- [ ] Rest in car → get negative status → see effect
- [ ] Rest in motel → get positive status → see buff
- [ ] Unlock passive ability → see resource change
- [ ] Save game → close → load → verify state
- [ ] Deplete perk charges → rest → charges restored

---

## 🚫 What NOT to Do

### Don't Re-implement These (Already Done):
- ❌ AbilityTreeSystem (already complete)
- ❌ Charges system (already implemented)
- ❌ RestScene UI (already exists)
- ❌ EventManager perk integration (already done)
- ❌ AbilityTreeScene UI (already complete)

### Don't Add These (Out of Scope):
- ❌ Graphics/sprites (use Unicode icons only)
- ❌ Sound system (separate phase)
- ❌ Multiplayer (not planned)
- ❌ New game mechanics (focus on existing systems)

---

## Success Criteria (COMPRESSED)

### Days 1-3 Success (Status System):
- [ ] StatusEffect and StatusManager implemented
- [ ] 15+ tests passing (reduced from 20+)
- [ ] Rest creates statuses (negative from car, positive from motel)
- [ ] Statuses affect gameplay (energy/efficiency modifiers work)

### Days 4-6 Success (Passive Bonuses):
- [ ] getFuelConsumptionRate() applies passive bonuses
- [ ] getMaxEnergy() applies passive bonuses
- [ ] getTradingDiscount() applies passive bonuses
- [ ] 10+ integration tests passing
- [ ] Gameplay feels different with abilities unlocked

### Days 7-9 Success (Save/Load):
- [ ] AbilityTreeSystem.save() and load() work
- [ ] JSON serialization for unlocked abilities, charges, skill points
- [ ] 8+ save/load tests passing
- [ ] Save → quit → load → state restored correctly

### Days 10-11 Success (HUD Display - OPTIONAL):
- [ ] HUDPanel shows active perks with charge display
- [ ] Color-coding works (green/yellow/red by charge level)
- [ ] Basic tooltips on hover
- [ ] Updates when charges consumed/restored

---

## Expected Deliverables (COMPRESSED)

### Test Count:
- **Original Plan**: 50+ tests over 4 weeks
- **Compressed Plan**: 33+ critical tests over 1.5 weeks
- **Breakdown**:
  - Status System: 15 tests
  - Passive Bonuses: 10 tests
  - Save/Load: 8 tests
  - OPTIONAL: HUD Integration tests if time permits

### File Count:
- **New Headers**: 4 files (StatusEffect.h, StatusManager.h, SaveManager deferred)
- **New Implementation**: 4 files (StatusEffect.cpp, StatusManager.cpp, etc.)
- **New Tests**: 4 files (test_StatusEffect, test_StatusManager, etc.)
- **Modified Files**: 3-4 files (RestScene, PlayerState, TravelCinematicScene, HUDPanel)

### Documentation:
- **New Docs**: 2 files (STATUS_BUFF_SYSTEM.md, PASSIVE_BONUS_INTEGRATION.md)
- **Updated Docs**: ability_tree_implementation.md progress report
- **Deferred Docs**: Full SAVE_LOAD_SYSTEM.md (main save/load branch handles this)

### Code Size Estimate:
- **Status System**: ~800 lines (classes + tests)
- **Passive Integration**: ~300 lines (modifications + tests)
- **Save/Load Core**: ~400 lines (AbilityTreeSystem methods + tests)
- **HUD Display**: ~200 lines (basic display + tests)
- **TOTAL**: ~1,700 lines (vs original 3,000+)

---

## Documentation to Create (COMPRESSED)

### New Documentation Files (2 files):
1. `docs/STATUS_BUFF_SYSTEM.md` - Status/buff system design (~8KB)
2. `docs/PASSIVE_BONUS_INTEGRATION.md` - How passives affect gameplay (~6KB)

### Update Existing Files:
- `docs/progress/ability_tree_implementation.md` - Add status/passive integration sections

### Deferred Documentation:
- ⏩ `docs/SAVE_LOAD_SYSTEM.md` - Main save/load branch handles comprehensive docs
- ⏩ SaveLoadScene UI documentation - Deferred to save/load branch

---

## Key Files to Create (COMPRESSED)

### New Header Files (2-3 files):
1. `include/StatusEffect.h` ✅ REQUIRED
2. `include/StatusManager.h` ✅ REQUIRED
3. `include/SaveManager.h` ⏩ DEFERRED to main save/load branch

### New Implementation Files (2-3 files):
1. `src/StatusEffect.cpp` ✅ REQUIRED
2. `src/StatusManager.cpp` ✅ REQUIRED
3. `src/SaveManager.cpp` ⏩ DEFERRED to main save/load branch

### New Test Files (4 files):
1. `tests/test_StatusEffect.cpp` ✅ REQUIRED (8 tests)
2. `tests/test_StatusManager.cpp` ✅ REQUIRED (7 tests)
3. `tests/test_AbilityTreeSaveLoad.cpp` ✅ REQUIRED (8 tests)
4. `tests/test_PlayerStateIntegration.cpp` ✅ REQUIRED (10 tests)

### Files to Modify (3-4 files):
1. `src/RestScene.cpp` - Add StatusManager::addStatus() calls
2. `src/PlayerState.cpp` - Apply StatusManager modifiers to resources
3. `src/TravelCinematicScene.cpp` - Add StatusManager::updateStatuses() call
4. `src/HUDPanel.cpp` - Add basic perk display (OPTIONAL, if time permits)
5. `src/AbilityTreeSystem.cpp` - Add save()/load() methods

---
4. `src/GameState.cpp` - Add save/load methods
5. `src/UI/HUDPanel.cpp` - Display perks and statuses
6. `CMakeLists.txt` - Add new files

---

## 🆘 If You Get Stuck

### Resources:
- Review existing `AbilityTreeSystem` for patterns
- Check `docs/REST_AND_CHARGES_SYSTEM.md` for rest mechanics
- Read `docs/progress/SAVE_LOAD_INTEGRATION_TASK.md` for save/load specs
- Look at SFML documentation for UI rendering

### Common Issues:
1. **Status not applying**: Check StatusManager::getTotalModifier() calculation
2. **Save/load fails**: Validate JSON format with schema
3. **Charges not restoring**: Verify RestScene calls AbilityTreeSystem::restoreCharges()
4. **Passive bonus not working**: Check PlayerState method modifications

---

## Quick Start Commands

```bash
# Create new branch from current branch
git checkout -b feature/status-buff-system

# Create new files
touch include/StatusEffect.h src/StatusEffect.cpp
touch include/StatusManager.h src/StatusManager.cpp
touch tests/test_StatusEffect.cpp

# Build and test
cmake --build build
ctest --test-dir build --output-on-failure

# Run game
./build/kkurs
```

---

**Remember**: 
- This branch builds ON TOP of the ability tree system (already complete)
- Focus on Status/Buff system first (Week 1)
- Then Save/Load (Week 2)
- Then integration and polish (Week 3-4)
- Keep tests passing, keep code clean, keep commits small!

**Good luck! 🚀**
