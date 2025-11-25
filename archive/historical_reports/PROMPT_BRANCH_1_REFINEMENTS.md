# Prompt for Branch 1: Refinements and Task Work

## Context
You are working on the 'barzikol/kkurs' repository, a C++ SFML-based road trip game project. This branch is for refinements, bug fixes, and small task work that doesn't belong to major feature development.

## Current Project State (from main branch after Phase 1-4 merge)

### ✅ Completed Systems:
1. **Menu & Character Creation** - Fallout-style terminal, 3 backstories, 3 cars (BMW E30, Volvo 940, Mercedes Sprinter)
2. **NPC & Experience System** - Team of 4 NPCs, levels 1-3, abilities, hero progression
3. **Navigation** - NodeScene (WASD car control), MapScene (route selection)
4. **Travel & Events** - TravelCinematicScene (parallax, fuel consumption 1.5-2.8L/100km), 5 default events
5. **Quest System** - QuestManager (6 objective types, auto-tracking, 3 default quests)
6. **Location System** - LocationScene (7 location types: gas station, store, garage, diner, motel, mechanic, home)

### 📊 Project Metrics:
- **Code**: ~15,000 lines production code
- **Tests**: 46/46 passing (100% pass rate)
- **Build**: GCC 13.3.0, C++17, SFML 2.6.1
- **Performance**: 60 FPS, ~15MB memory
- **Completion**: 55% of roadmap complete

### 🗂️ Key Files & Structure:
```
include/
  - Scene.h (base class + scene types)
  - TravelCinematicScene.h
  - QuestManager.h
  - Car.h, NPC.h, ExperienceSystem.h, EventManager.h
  - DialogueManager.h, GameTypes.h
src/
  - All implementations (.cpp)
  - UI/HUDPanel.cpp
tests/
  - unit/ (46 tests)
docs/
  - archive/ (16 historical documents)
Root documentation:
  - PROJECT_STATUS.md (main status document)
  - FUNCTIONAL_REPORT.md (21KB functional report)
  - NEXT_PHASE_PLAN.md (Phase 4 UI/HUD plan)
  - IMPLEMENTATION_PLAN.md (full roadmap)
  - CAR_SPECIFICATIONS.md, DRIVING_SCENE_SPECIFICATION.md, WORLD_MAP_FORMAT.md, VISUAL_DIAGRAM.md
```

## Your Mission: Refinements & Task Work

### 🎯 Primary Goals:
1. **Bug Fixes** - Address any bugs found in existing systems
2. **Code Quality** - Refactor for better maintainability
3. **Small Improvements** - Polish existing features
4. **Technical Debt** - Clean up TODOs and code smells
5. **Test Coverage** - Add missing tests for edge cases

### 📋 Suggested Tasks (Priority Order):

#### High Priority:
1. **Add more default quests** (currently only 3)
   - Create 5-7 more quests with varied objectives
   - Test quest progress tracking
   - Update QuestManager initialization

2. **Expand event library** (currently only 5 events)
   - Add 10-15 more diverse road events
   - Include seasonal/weather variations
   - Test event triggering probability

3. **Improve error handling**
   - Add validation for edge cases
   - Better null pointer checks
   - Graceful degradation for missing resources

4. **Add unit tests**
   - TravelCinematicScene tests
   - LocationScene tests
   - EventManager tests
   - Edge case coverage

#### Medium Priority:
5. **Code documentation**
   - Add Doxygen comments to public APIs
   - Document complex algorithms
   - Create usage examples

6. **Performance optimization**
   - Profile slow functions
   - Reduce unnecessary copying
   - Optimize rendering loops

7. **Refactoring**
   - Extract magic numbers to constants
   - Simplify complex functions
   - Improve naming consistency

#### Low Priority:
8. **Code style consistency**
   - Format according to .clang-format
   - Fix any compiler warnings
   - Remove dead code

9. **Build improvements**
   - Add more CMake options
   - Improve build time
   - Better dependency management

10. **Documentation updates**
    - Keep FUNCTIONAL_REPORT.md up to date
    - Update PROJECT_STATUS.md metrics
    - Archive completed work

### 🚫 What NOT to Do:
- ❌ Do NOT implement Phase 4 (UI/HUD) - that's for Branch 2
- ❌ Do NOT add new major features
- ❌ Do NOT break existing tests
- ❌ Do NOT modify core architecture without discussion
- ❌ Do NOT add new dependencies without approval

### 🛠️ Development Workflow:
1. **Understand**: Read FUNCTIONAL_REPORT.md to understand how systems work
2. **Plan**: Choose a task from the list above
3. **Implement**: Make minimal, focused changes
4. **Test**: Run tests frequently (`cmake --build build && ctest --test-dir build`)
5. **Commit**: Use descriptive commit messages
6. **Document**: Update relevant documentation

### 📏 Code Standards:
- **Style**: Follow existing code style (RAII, smart pointers, const correctness)
- **Testing**: All changes must have tests (current coverage: 46 tests)
- **Performance**: No regressions (60 FPS baseline)
- **Memory**: Use RAII, avoid manual memory management
- **Safety**: No buffer overflows, validate all inputs

### 🔧 Build Commands:
```bash
# Clean build
rm -rf build && mkdir build && cd build
cmake .. && cmake --build .

# Run tests
ctest --test-dir build --output-on-failure

# Run game
./build/kkurs
```

### 📚 Key Documentation to Read:
1. **FUNCTIONAL_REPORT.md** - How all systems work (21KB, essential reading)
2. **PROJECT_STATUS.md** - Current project status and metrics
3. **IMPLEMENTATION_PLAN.md** - Full roadmap (know what's planned)
4. **CAR_SPECIFICATIONS.md** - Car system details

### ✅ Success Criteria:
- All 46+ tests passing
- No new compiler warnings
- No performance regressions
- Code follows project style
- Documentation updated
- Changes reviewed and validated

### 🆘 If You Get Stuck:
- Check FUNCTIONAL_REPORT.md for system details
- Review existing tests for usage examples
- Look at similar code for patterns
- Ask for clarification if needed

---

## Example Task: Add 3 New Quests

### Step 1: Understand QuestManager
Read `src/QuestManager.cpp` lines 1-100 to see initialization.

### Step 2: Design New Quests
Quest 1: "Fuel Crisis" - Collect 100L fuel (COLLECT_ITEM)
Quest 2: "Speed Demon" - Travel 500km (TRAVEL_TO with distance check)
Quest 3: "Social Butterfly" - Talk to 5 NPCs (TALK_TO_NPC)

### Step 3: Implement
Edit `src/QuestManager.cpp`, add to `initializeDefaultQuests()`:
```cpp
// Quest 4: Fuel Crisis
Quest quest4;
quest4.id = 4;
quest4.title = "Топливный кризис / Fuel Crisis";
quest4.description = "Запасись топливом на случай долгого пути / Stock up on fuel for the long road";
quest4.questGiverNpcId = 1;
quest4.status = QuestStatus::ACTIVE;
quest4.difficulty = QuestDifficulty::EASY;

QuestObjective obj4;
obj4.id = 1;
obj4.type = QuestObjectiveType::COLLECT_ITEM;
obj4.description = "Собрать 100L топлива / Collect 100L fuel";
obj4.targetValue = 100.0f;
obj4.currentValue = 0.0f;
obj4.completed = false;
quest4.objectives.push_back(obj4);

quest4.rewards.money = 500.0f;
quest4.rewards.experience = 100.0f;
registerQuest(quest4);
```

### Step 4: Test
Write test in `tests/unit/test_quest_manager.cpp`:
```cpp
TEST_CASE("QuestManager - Fuel Crisis Quest", "[quest]") {
    QuestManager& qm = QuestManager::getInstance();
    qm.reset();
    qm.initializeDefaultQuests();
    
    auto quest = qm.getQuest(4);
    REQUIRE(quest != nullptr);
    REQUIRE(quest->id == 4);
    REQUIRE(quest->objectives.size() == 1);
    
    // Simulate collecting fuel
    qm.notifyItemCollected("fuel", 100.0f);
    REQUIRE(quest->objectives[0].completed == true);
}
```

### Step 5: Validate
```bash
cmake --build build && ctest --test-dir build
```

---

**Remember**: This branch is for REFINEMENTS only. Major features go to Branch 2. Keep changes small, focused, and well-tested!
