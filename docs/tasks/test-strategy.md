# Testing Strategy
# Road Trip Adventure RPG

**Version:** 1.0  
**Last Updated:** 2025-11-11

---

## Table of Contents

1. [Testing Overview](#testing-overview)
2. [Unit Testing](#unit-testing)
3. [Integration Testing](#integration-testing)
4. [Manual QA Testing](#manual-qa-testing)
5. [Performance Testing](#performance-testing)
6. [Test Automation](#test-automation)

---

## Testing Overview

### Testing Goals

- Ensure code quality and reliability
- Catch bugs early in development
- Maintain 60 FPS performance target
- Verify feature completeness
- Validate user experience

### Testing Pyramid

```
                  /\
                 /  \
                /    \
               / E2E  \
              /────────\
             / Integration\
            /──────────────\
           /   Unit Tests   \
          /──────────────────\
         
         Unit Tests: 70%
         Integration Tests: 20%
         Manual/E2E Tests: 10%
```

### Coverage Goals

- **Unit Tests:** > 70% code coverage
- **Integration Tests:** All scene transitions
- **Manual QA:** All user-facing features
- **Performance:** All gameplay scenarios

---

## Unit Testing

### Framework

Using Google Test (gtest) for C++ unit testing.

### Setup

```cmake
# CMakeLists.txt addition
find_package(GTest REQUIRED)

add_executable(kkurs_tests
    tests/ResourceManagerTest.cpp
    tests/GridInventoryTest.cpp
    tests/EventManagerTest.cpp
    tests/NPCManagerTest.cpp
    # ... more test files
)

target_link_libraries(kkurs_tests
    GTest::GTest
    GTest::Main
    kkurs_lib
)
```

### Test Categories

#### 1. Resource Management Tests

```cpp
TEST(ResourceManager, InitialValues) {
    ResourceManager rm;
    EXPECT_EQ(rm.getFuel(), 100.0f);
    EXPECT_EQ(rm.getMoney(), 200);
    EXPECT_EQ(rm.getEnergy(), 100.0f);
}

TEST(ResourceManager, FuelModification) {
    ResourceManager rm;
    rm.setFuel(50.0f);
    rm.modifyFuel(-10.0f);
    EXPECT_EQ(rm.getFuel(), 40.0f);
}

TEST(ResourceManager, FuelBoundaries) {
    ResourceManager rm;
    rm.modifyFuel(-200.0f);  // Try to go negative
    EXPECT_GE(rm.getFuel(), 0.0f);  // Should clamp to 0
    
    rm.modifyFuel(200.0f);  // Try to exceed max
    EXPECT_LE(rm.getFuel(), 100.0f);  // Should clamp to max
}

TEST(ResourceManager, MoneyOperations) {
    ResourceManager rm;
    rm.setMoney(100);
    EXPECT_TRUE(rm.canAfford(50));
    EXPECT_FALSE(rm.canAfford(150));
    
    rm.modifyMoney(-50);
    EXPECT_EQ(rm.getMoney(), 50);
}
```

#### 2. Inventory System Tests

```cpp
TEST(GridInventory, ItemPlacement) {
    GridInventorySystem inv(6, 4);  // 6x4 grid
    Item testItem = createTestItem("Wrench", 2, 1);
    
    EXPECT_TRUE(inv.canPlaceItem(0, 0, 2, 1, -1));
    EXPECT_TRUE(inv.placeItem(testItem, 0, 0));
    EXPECT_FALSE(inv.canPlaceItem(0, 0, 2, 1, -1));  // Occupied
}

TEST(GridInventory, ItemRotation) {
    GridInventorySystem inv(6, 4);
    Item testItem = createTestItem("Tool", 3, 1);
    
    int itemId = inv.addItem(testItem, 0, 0);
    ASSERT_NE(itemId, -1);
    
    EXPECT_TRUE(inv.rotateItem(itemId));  // 1x3 now
    
    GridItem* rotated = inv.getItemById(itemId);
    EXPECT_EQ(rotated->gridWidth, 1);
    EXPECT_EQ(rotated->gridHeight, 3);
}

TEST(GridInventory, CollisionDetection) {
    GridInventorySystem inv(6, 4);
    
    Item item1 = createTestItem("Item1", 2, 2);
    Item item2 = createTestItem("Item2", 2, 2);
    
    inv.addItem(item1, 0, 0);  // Top-left
    
    EXPECT_FALSE(inv.canPlaceItem(1, 1, 2, 2, -1));  // Overlaps
    EXPECT_TRUE(inv.canPlaceItem(2, 0, 2, 2, -1));   // Adjacent, OK
}

TEST(GridInventory, WeightLimit) {
    GridInventorySystem inv(6, 4, 100.0f);  // 100kg max
    
    Item heavy = createTestItem("Heavy", 1, 1, 60.0f);
    Item medium = createTestItem("Medium", 1, 1, 30.0f);
    Item light = createTestItem("Light", 1, 1, 20.0f);
    
    EXPECT_TRUE(inv.addItem(heavy, 0, 0) != -1);
    EXPECT_TRUE(inv.addItem(medium, 1, 0) != -1);
    EXPECT_FALSE(inv.addItem(light, 2, 0) != -1);  // Exceeds weight
    
    EXPECT_EQ(inv.getTotalWeight(), 90.0f);
}
```

#### 3. Event System Tests

```cpp
TEST(EventManager, ConditionEvaluation) {
    EventManager em;
    Event event = createTestEvent();
    event.condition.minFuel = 20.0f;
    event.condition.maxFuel = 80.0f;
    
    GameState state1;
    state1.fuel = 50.0f;
    EXPECT_TRUE(em.evaluateCondition(event.condition, state1));
    
    GameState state2;
    state2.fuel = 10.0f;
    EXPECT_FALSE(em.evaluateCondition(event.condition, state2));
}

TEST(EventManager, EventProbability) {
    EventManager em;
    Event event;
    event.probability = 0.5f;  // 50%
    
    int triggered = 0;
    int total = 1000;
    
    for (int i = 0; i < total; ++i) {
        if (em.rollProbability(event.probability)) {
            triggered++;
        }
    }
    
    // Should be around 500 +/- margin
    EXPECT_NEAR(triggered, 500, 100);
}

TEST(EventManager, OutcomeApplication) {
    EventManager em;
    GameState state;
    state.fuel = 50.0f;
    state.money = 100;
    
    EventOutcome outcome;
    outcome.resources.fuelChange = -10.0f;
    outcome.resources.moneyChange = 50;
    
    em.applyOutcome(outcome, state);
    
    EXPECT_EQ(state.fuel, 40.0f);
    EXPECT_EQ(state.money, 150);
}
```

#### 4. NPC System Tests

```cpp
TEST(NPCManager, MoraleCalculation) {
    NPCManager nm;
    NPC npc = createTestNPC("Joe", NPCType::MECHANIC);
    npc.morale = 80;
    
    nm.registerNPC(npc);
    nm.modifyMorale("Joe", -20);
    
    EXPECT_EQ(nm.getNPC("Joe")->morale, 60);
}

TEST(NPCManager, PartyManagement) {
    NPCManager nm;
    nm.setPartyCapacity(3);
    
    EXPECT_TRUE(nm.hasPartySlot());
    EXPECT_TRUE(nm.addToParty("npc1"));
    EXPECT_TRUE(nm.addToParty("npc2"));
    EXPECT_TRUE(nm.addToParty("npc3"));
    EXPECT_FALSE(nm.addToParty("npc4"));  // Full
    
    EXPECT_EQ(nm.getPartySize(), 3);
}

TEST(NPCManager, AbilityCheck) {
    NPCManager nm;
    NPC mechanic = createTestNPC("Joe", NPCType::MECHANIC);
    mechanic.abilities.push_back(Ability::REPAIR_DISCOUNT);
    
    nm.registerNPC(mechanic);
    nm.addToParty("Joe");
    
    EXPECT_TRUE(nm.partyHasAbility(Ability::REPAIR_DISCOUNT));
    EXPECT_FALSE(nm.partyHasAbility(Ability::NAVIGATION_BONUS));
}
```

### Running Unit Tests

```bash
# Build with tests
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make

# Run all tests
./kkurs_tests

# Run specific test suite
./kkurs_tests --gtest_filter=ResourceManager.*

# Run with verbose output
./kkurs_tests --gtest_verbose

# Generate coverage report
gcov kkurs_tests
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
```

---

## Integration Testing

### Scene Transition Tests

```cpp
TEST(SceneIntegration, MainMenuToCharacterCreation) {
    SceneManager sm(window);
    sm.createScene(SceneType::MAIN_MENU);
    
    // Simulate selecting "New Game"
    sm.transitionTo(SceneType::CHARACTER_CREATION);
    
    EXPECT_EQ(sm.getCurrentSceneType(), SceneType::CHARACTER_CREATION);
}

TEST(SceneIntegration, CompleteGameFlow) {
    SceneManager sm(window);
    PlayerState& ps = PlayerState::getInstance();
    
    // Main menu
    sm.createScene(SceneType::MAIN_MENU);
    sm.transitionTo(SceneType::CHARACTER_CREATION);
    
    // Character creation
    ps.setArchetype(CharacterArchetype::NOMAD);
    ps.setVehicleType(VehicleType::SEDAN);
    sm.transitionTo(SceneType::ROUTE_SELECTION);
    
    // Route selection
    sm.transitionTo(SceneType::WORLD_MAP);
    
    // Verify state
    EXPECT_EQ(ps.getArchetype(), CharacterArchetype::NOMAD);
    EXPECT_EQ(ps.getVehicleType(), VehicleType::SEDAN);
    EXPECT_EQ(sm.getCurrentSceneType(), SceneType::WORLD_MAP);
}
```

### System Integration Tests

```cpp
TEST(SystemIntegration, EventAffectsResources) {
    EventManager& em = EventManager::getInstance();
    ResourceManager& rm = ResourceManager::getInstance();
    
    rm.setFuel(50.0f);
    rm.setMoney(100);
    
    Event event = createTestEvent();
    EventOutcome outcome;
    outcome.resources.fuelChange = -10.0f;
    outcome.resources.moneyChange = 50;
    
    em.applyOutcome(outcome);
    
    EXPECT_EQ(rm.getFuel(), 40.0f);
    EXPECT_EQ(rm.getMoney(), 150);
}

TEST(SystemIntegration, InventoryItemUsage) {
    GridInventorySystem& inv = GridInventorySystem::getInstance();
    EventManager& em = EventManager::getInstance();
    
    Item tool = createTestItem("Wrench", 1, 1);
    int itemId = inv.addItem(tool, 0, 0);
    ASSERT_NE(itemId, -1);
    
    // Use item in event
    Event event = createTestEvent();
    event.choices[0].requiredItems.push_back(ItemRequirement{"Wrench", 1, true});
    
    EXPECT_TRUE(em.canSelectChoice(event.choices[0]));
    
    em.processChoice(event.choices[0]);
    
    EXPECT_FALSE(inv.hasItem("Wrench"));  // Consumed
}
```

---

## Manual QA Testing

### Test Case Template

```
Test Case ID: TC-XXX
Title: [Descriptive title]
Priority: [High / Medium / Low]
Type: [Functional / UI / Performance / Security]

Preconditions:
- [List any setup required]

Test Steps:
1. [Step 1]
2. [Step 2]
3. [Step 3]

Expected Result:
- [What should happen]

Actual Result:
- [What actually happened]

Status: [Pass / Fail / Blocked]
Notes: [Any additional information]
```

### Critical Path Tests

**CP-001: Start New Game**
```
1. Launch application
2. Click "New Game"
3. Select archetype "Nomad"
4. Select vehicle "Sedan"
5. Click "Start"

Expected: Game starts at home location with:
- Fuel: 100L
- Money: $200
- Energy: 100%
```

**CP-002: Complete Journey**
```
1. From home, open world map
2. Select adjacent location
3. Confirm journey
4. Wait for travel to complete
5. Verify arrival at destination

Expected: 
- Travel scene displays
- Events may trigger
- Arrive at selected location
- Resources updated correctly
```

**CP-003: Manage Inventory**
```
1. Press 'I' to open inventory
2. Pick up item with left-click
3. Rotate with right-click
4. Place in new position
5. Close inventory with ESC

Expected:
- Grid displays correctly
- Rotation works smoothly
- Placement validated
- Changes persist
```

### UI/UX Tests

**UI-001: HUD Visibility**
- All HUD elements readable at 1366x768
- No overlapping elements
- Colors provide clear contrast
- Icons are recognizable

**UI-002: Mouse Interaction**
- Cursor changes appropriately
- Hover effects visible
- Click feedback immediate
- No input lag

**UI-003: Keyboard Controls**
- A/D movement responsive
- E interaction works near structures
- I opens inventory
- ESC returns/pauses

### Edge Case Tests

**EDGE-001: Zero Fuel**
```
Scenario: Fuel reaches zero during travel
Expected: Event triggers offering help or game over
```

**EDGE-002: Full Inventory**
```
Scenario: Try to add item when inventory full
Expected: Clear error message, item not added
```

**EDGE-003: No Money for Service**
```
Scenario: Try to buy with insufficient funds
Expected: Transaction blocked, clear message
```

---

## Performance Testing

### Performance Benchmarks

**Target Metrics:**
- Frame Rate: 60 FPS minimum
- Frame Time: < 16.67ms per frame
- Memory Usage: < 512 MB
- Load Time: < 3 seconds
- Input Latency: < 50ms

### Performance Test Scenarios

**PERF-001: World Map Rendering**
```
Test: Render world map with 50+ nodes
Measure: FPS, frame time
Target: 60 FPS stable
```

**PERF-002: Inventory with Full Grid**
```
Test: Render 24/24 items in inventory
Measure: FPS, frame time
Target: 60 FPS stable
```

**PERF-003: Event Triggering**
```
Test: Trigger 10 events in sequence
Measure: Processing time per event
Target: < 1ms per event
```

**PERF-004: Save/Load**
```
Test: Save and load game state
Measure: Time taken
Target: < 2 seconds each
```

### Profiling Tools

```bash
# CPU Profiling (Linux)
valgrind --tool=callgrind ./kkurs
kcachegrind callgrind.out.*

# Memory Profiling
valgrind --leak-check=full --show-leak-kinds=all ./kkurs

# Performance Monitoring
perf record ./kkurs
perf report
```

---

## Test Automation

### CI/CD Pipeline

```yaml
# .github/workflows/tests.yml
name: Test Suite

on: [push, pull_request]

jobs:
  unit-tests:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install dependencies
        run: |
          sudo apt-get install libsfml-dev cmake libgtest-dev
      - name: Build
        run: |
          mkdir build && cd build
          cmake -DBUILD_TESTS=ON ..
          make
      - name: Run unit tests
        run: ./build/kkurs_tests
      - name: Generate coverage
        run: |
          gcov build/kkurs_tests
          bash <(curl -s https://codecov.io/bash)
  
  integration-tests:
    runs-on: ubuntu-latest
    needs: unit-tests
    steps:
      - uses: actions/checkout@v2
      - name: Build
        run: ./scripts/build.sh
      - name: Run integration tests
        run: ./scripts/run_integration_tests.sh
  
  static-analysis:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Run cppcheck
        run: cppcheck --enable=all --suppress=missingInclude src/
      - name: Run clang-tidy
        run: clang-tidy src/*.cpp -- -I include/
```

### Automated Test Scripts

```bash
#!/bin/bash
# scripts/run_all_tests.sh

echo "Running unit tests..."
./build/kkurs_tests

if [ $? -ne 0 ]; then
    echo "Unit tests failed!"
    exit 1
fi

echo "Running integration tests..."
./scripts/run_integration_tests.sh

if [ $? -ne 0 ]; then
    echo "Integration tests failed!"
    exit 1
fi

echo "Running static analysis..."
cppcheck --enable=all --error-exitcode=1 src/

if [ $? -ne 0 ]; then
    echo "Static analysis failed!"
    exit 1
fi

echo "All tests passed!"
```

---

## Test Reporting

### Test Report Template

```markdown
# Test Report - [Date]

## Summary
- Total Tests: X
- Passed: Y
- Failed: Z
- Coverage: A%

## Failed Tests
1. Test Name - Reason
2. Test Name - Reason

## Performance Metrics
- Average FPS: X
- Memory Usage: Y MB
- Load Time: Z seconds

## Issues Found
- Critical: X
- High: Y
- Medium: Z
- Low: W

## Recommendations
1. [Recommendation 1]
2. [Recommendation 2]
```

---

**Document Status:** Complete  
**Last Review:** 2025-11-11  
**Next Review:** After Phase 2 implementation
