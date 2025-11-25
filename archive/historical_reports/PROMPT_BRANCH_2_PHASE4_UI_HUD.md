# Prompt for Branch 2: Phase 4 - UI/HUD Implementation

## Context
You are working on the 'barzikol/kkurs' repository, a C++ SFML-based road trip game project. This branch is dedicated to implementing Phase 4: UI/HUD system without graphical assets (using Unicode icons and colored rectangles).

## Current Project State (from main branch after Phase 1-4 merge)

### ✅ Completed Systems (Already Working):
1. **Menu & Character Creation** - Fallout-style terminal, 3 backstories, 3 cars
2. **NPC & Experience System** - Team management, leveling, abilities
3. **Navigation** - NodeScene (city driving), MapScene (route selection)
4. **Travel & Events** - Cinematic travel with parallax, fuel consumption, random events
5. **Quest System** - 6 objective types, auto-tracking, progress calculation
6. **Location System** - 7 location types with resource transactions

### 📊 Project Metrics:
- **Tests**: 46/46 passing
- **Performance**: 60 FPS, ~15MB memory
- **Completion**: 55% → **Target 70%** after Phase 4

## Your Mission: Implement UI/HUD System

### 🎯 Phase 4 Goals (from NEXT_PHASE_PLAN.md):

#### **Deliverable 1: Enhanced HUDPanel Component** ✨
**Current State**: Basic HUDPanel exists in `src/UI/HUDPanel.cpp` but is minimal.

**Requirements**:
1. **Resource Displays** (top-right corner):
   - Money: `💰 1,250₽` (yellow text)
   - Fuel: `⛽ 45.5/100L` (orange text, with progress bar)
   - Energy: `⚡ 70/100` (cyan text, with progress bar)
   - Cargo: `📦 25/64` (white text)

2. **Progress Bars**:
   - 100px wide × 10px high
   - Border: 2px white
   - Fill color based on percentage:
     - Green (>70%)
     - Yellow (40-70%)
     - Red (<40%)
   - Smooth fill animation

3. **Layout**:
   ```
   Screen (800×600)
   ┌─────────────────────────────────────┐
   │                    💰 1,250₽        │ ← Top-right
   │                    ⛽ 45.5/100L      │
   │                    [████████░░] 45% │ ← Fuel bar
   │                    ⚡ 70/100         │
   │                    [████████░░] 70% │ ← Energy bar
   │                    📦 25/64          │
   │                                     │
   │         [Game Content]              │
   │                                     │
   └─────────────────────────────────────┘
   ```

4. **Update Frequency**: Every frame (from GameState)

#### **Deliverable 2: ResourceBar Component** 🎨
**New File**: `include/UI/ResourceBar.h` and `src/UI/ResourceBar.cpp`

**Class Structure**:
```cpp
class ResourceBar {
private:
    sf::RectangleShape border;
    sf::RectangleShape fill;
    sf::Vector2f position;
    sf::Vector2f size;
    float currentValue;
    float maxValue;
    
    sf::Color getBorderColor() const;
    sf::Color getFillColor(float percentage) const;
    
public:
    ResourceBar(sf::Vector2f pos, sf::Vector2f size);
    
    void setValue(float current, float max);
    void setPosition(sf::Vector2f pos);
    void update(float deltaTime); // Smooth animation
    void draw(sf::RenderWindow& window);
    
    float getPercentage() const;
};
```

**Implementation Details**:
- **Smooth transitions**: Use lerp for value changes
- **Color gradient**: Red → Yellow → Green based on percentage
- **Animation**: 0.2s transition time
- **Border**: 2px white outline

#### **Deliverable 3: Scene Integration** 🔗
**Integrate HUD into ALL scenes**:

1. **NodeScene** (`src/NodeScene.cpp`):
   - Show fuel, cargo, money
   - Update every frame
   - Position: top-right (650, 10)

2. **MapScene** (`src/MapScene.cpp`):
   - Show money, current location
   - Display route distance and fuel requirements
   - Position: top-right (650, 10)

3. **TravelCinematicScene** (`src/TravelCinematicScene.cpp`):
   - Show fuel decreasing in real-time
   - Progress bar for distance traveled
   - Event countdown timer
   - Position: top-right (650, 10)

4. **LocationScene** (`src/LocationScene.cpp`):
   - Show money, cargo, fuel
   - Highlight resource changes (flash effect)
   - Position: top-right (650, 10)

5. **CinematicEventScene** (`src/CinematicEventScene.cpp`):
   - Show current resources
   - Display event rewards/costs
   - Position: top-right (650, 10)

#### **Deliverable 4: GameState HUD Bridge** 🌉
**Modify**: `src/GameState.cpp`

**Add HUD update method**:
```cpp
void GameState::updateHUD(HUDPanel& hud) {
    hud.setMoney(hero.getMoney());
    hud.setFuel(car.getCurrentFuel(), car.getFuelCapacity());
    hud.setEnergy(hero.getEnergy(), hero.getMaxEnergy());
    hud.setCargo(car.getCurrentCargo(), car.getMaxCargo());
}
```

**Call from every scene's update()**:
```cpp
void NodeScene::update(float deltaTime) {
    // ... existing code ...
    gameState.updateHUD(hudPanel);
    hudPanel.update(deltaTime);
}
```

### 📋 Implementation Plan (4 Days)

#### **Day 1: ResourceBar Component** ✅
- [ ] Create `include/UI/ResourceBar.h`
- [ ] Create `src/UI/ResourceBar.cpp`
- [ ] Implement constructor, setters, getters
- [ ] Implement color gradient logic
- [ ] Implement smooth animation (lerp)
- [ ] Implement draw method
- [ ] Add to CMakeLists.txt
- [ ] Write unit tests (5+ tests)
- [ ] Test: Build and run
- **Validation**: ResourceBar renders with smooth transitions

#### **Day 2: Enhanced HUDPanel** ✨
- [ ] Refactor `src/UI/HUDPanel.cpp`
- [ ] Add ResourceBar members (fuel, energy)
- [ ] Implement layout (top-right positioning)
- [ ] Add Unicode icons (💰⛽⚡📦)
- [ ] Implement update method
- [ ] Implement draw method with all resources
- [ ] Test with mock data
- **Validation**: HUD displays all resources correctly

#### **Day 3: Scene Integration (Part 1)** 🔗
- [ ] Integrate HUD into NodeScene
- [ ] Integrate HUD into MapScene
- [ ] Integrate HUD into TravelCinematicScene
- [ ] Add GameState::updateHUD() method
- [ ] Test each scene individually
- **Validation**: HUD appears in all scenes, updates correctly

#### **Day 4: Scene Integration (Part 2) + Polish** ✨
- [ ] Integrate HUD into LocationScene
- [ ] Integrate HUD into CinematicEventScene
- [ ] Add flash effects for resource changes
- [ ] Polish animations and colors
- [ ] Final testing (all scenes)
- [ ] Update documentation
- [ ] Commit and push
- **Validation**: Full gameplay loop with working HUD

### 🛠️ Technical Specifications

#### **Unicode Icons**:
- Money: `💰` (U+1F4B0) - #FFD700 (gold)
- Fuel: `⛽` (U+26FD) - #FFA500 (orange)
- Energy: `⚡` (U+26A1) - #00FFFF (cyan)
- Cargo: `📦` (U+1F4E6) - #FFFFFF (white)

#### **Color Palette**:
- **Green**: `sf::Color(0, 255, 0)` - >70%
- **Yellow**: `sf::Color(255, 255, 0)` - 40-70%
- **Red**: `sf::Color(255, 0, 0)` - <40%
- **White**: `sf::Color(255, 255, 255)` - borders, text
- **Gray**: `sf::Color(128, 128, 128)` - background

#### **Positioning** (800×600 window):
- **HUD Origin**: (650, 10) - top-right corner
- **Resource Spacing**: 25px vertical
- **Bar Width**: 100px
- **Bar Height**: 10px
- **Icon Size**: 16×16px
- **Text Size**: 14px

#### **Animation Parameters**:
- **Transition Time**: 0.2 seconds
- **Lerp Factor**: 5.0 * deltaTime
- **Flash Duration**: 0.5 seconds
- **Flash Color**: White (alpha 128)

### 🔧 Code Examples

#### **Example 1: ResourceBar Usage**
```cpp
// In HUDPanel.h
class HUDPanel {
private:
    ResourceBar fuelBar;
    ResourceBar energyBar;
    sf::Text moneyText;
    sf::Text cargoText;
    
public:
    HUDPanel();
    void setFuel(float current, float max);
    void setEnergy(float current, float max);
    void setMoney(float amount);
    void setCargo(float current, float max);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};

// In HUDPanel.cpp constructor
HUDPanel::HUDPanel() 
    : fuelBar(sf::Vector2f(700, 50), sf::Vector2f(100, 10)),
      energyBar(sf::Vector2f(700, 80), sf::Vector2f(100, 10)) {
    // Initialize text, load font, etc.
}

void HUDPanel::setFuel(float current, float max) {
    fuelBar.setValue(current, max);
}

void HUDPanel::update(float deltaTime) {
    fuelBar.update(deltaTime);
    energyBar.update(deltaTime);
}

void HUDPanel::draw(sf::RenderWindow& window) {
    window.draw(moneyText);
    fuelBar.draw(window);
    window.draw(cargoText);
    energyBar.draw(window);
}
```

#### **Example 2: Scene Integration**
```cpp
// In NodeScene.cpp
void NodeScene::enter(GameState& gs) {
    gameState = &gs;
    hudPanel.setPosition(sf::Vector2f(650, 10));
}

void NodeScene::update(float deltaTime) {
    // Update game logic...
    
    // Update HUD
    hudPanel.setMoney(gameState->hero.getMoney());
    hudPanel.setFuel(gameState->car.getCurrentFuel(), 
                     gameState->car.getFuelCapacity());
    hudPanel.setEnergy(gameState->hero.getEnergy(), 
                       gameState->hero.getMaxEnergy());
    hudPanel.setCargo(gameState->car.getCurrentCargo(),
                      gameState->car.getMaxCargo());
    hudPanel.update(deltaTime);
}

void NodeScene::draw(sf::RenderWindow& window) {
    // Draw scene content...
    
    // Draw HUD on top
    hudPanel.draw(window);
}
```

### 🧪 Testing Strategy

#### **Unit Tests** (add to `tests/unit/test_ui.cpp`):
```cpp
TEST_CASE("ResourceBar - Color Gradient", "[ui]") {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    
    bar.setValue(80, 100); // 80%
    REQUIRE(bar.getPercentage() == 80.0f);
    // Should be green
    
    bar.setValue(50, 100); // 50%
    REQUIRE(bar.getPercentage() == 50.0f);
    // Should be yellow
    
    bar.setValue(20, 100); // 20%
    REQUIRE(bar.getPercentage() == 20.0f);
    // Should be red
}

TEST_CASE("ResourceBar - Smooth Transition", "[ui]") {
    ResourceBar bar(sf::Vector2f(0, 0), sf::Vector2f(100, 10));
    bar.setValue(100, 100);
    
    bar.setValue(50, 100);
    bar.update(0.1f); // 0.1 second
    
    // Value should be transitioning, not instant
    REQUIRE(bar.getPercentage() > 50.0f);
    REQUIRE(bar.getPercentage() < 100.0f);
}

TEST_CASE("HUDPanel - Resource Display", "[ui]") {
    HUDPanel hud;
    hud.setMoney(1250.0f);
    hud.setFuel(45.5f, 100.0f);
    hud.setEnergy(70.0f, 100.0f);
    hud.setCargo(25.0f, 64.0f);
    
    // HUD should store and display all values
    REQUIRE(hud.getMoney() == 1250.0f);
    REQUIRE(hud.getFuel() == 45.5f);
}
```

#### **Manual Testing Checklist**:
- [ ] HUD appears in NodeScene
- [ ] HUD appears in MapScene
- [ ] HUD appears in TravelCinematicScene (fuel decreases)
- [ ] HUD appears in LocationScene (money changes on purchase)
- [ ] HUD appears in CinematicEventScene
- [ ] Progress bars animate smoothly
- [ ] Colors change correctly (green → yellow → red)
- [ ] Unicode icons render correctly
- [ ] No performance impact (60 FPS maintained)
- [ ] Window resize doesn't break layout

### 🚫 What NOT to Do:
- ❌ Do NOT add graphical assets (PNG/JPG) - Unicode only!
- ❌ Do NOT implement ability tree UI (that's Phase 5-7)
- ❌ Do NOT add new game mechanics
- ❌ Do NOT break existing tests
- ❌ Do NOT modify core game logic
- ❌ Do NOT change GameState structure (only add HUD update method)

### 📏 Code Standards:
- **Style**: Match existing code style
- **SFML**: Use sf::RectangleShape, sf::Text, sf::Color
- **Performance**: No allocations in draw/update loops
- **Memory**: RAII, no manual memory management
- **Testing**: Add tests for all new classes

### 🔍 Key Files to Modify:
1. **New Files**:
   - `include/UI/ResourceBar.h`
   - `src/UI/ResourceBar.cpp`

2. **Modify**:
   - `src/UI/HUDPanel.cpp` (enhance)
   - `include/UI/HUDPanel.h` (add ResourceBar members)
   - `src/NodeScene.cpp` (integrate HUD)
   - `src/MapScene.cpp` (integrate HUD)
   - `src/TravelCinematicScene.cpp` (integrate HUD)
   - `src/LocationScene.cpp` (integrate HUD)
   - `src/CinematicEventScene.cpp` (integrate HUD)
   - `src/GameState.cpp` (add updateHUD method)
   - `CMakeLists.txt` (add ResourceBar.cpp)

### ✅ Success Criteria:
- [ ] ResourceBar class implemented and tested
- [ ] HUDPanel enhanced with progress bars
- [ ] HUD integrated into all 5 scenes
- [ ] All resources display correctly
- [ ] Smooth animations working
- [ ] Color gradients correct
- [ ] Unicode icons render properly
- [ ] All 46+ tests passing
- [ ] No performance regression (60 FPS)
- [ ] Documentation updated

### 📚 Documentation to Update:
- Update **PROJECT_STATUS.md** (55% → 70%)
- Update **FUNCTIONAL_REPORT.md** (add UI/HUD section)
- Create **UI_HUD_GUIDE.md** (usage guide for future developers)

### 🆘 If You Get Stuck:
- Review existing `src/UI/HUDPanel.cpp` for patterns
- Check SFML documentation for sf::RectangleShape
- Test ResourceBar in isolation first
- Integrate one scene at a time
- Ask for clarification if needed

---

## Quick Start Commands

```bash
# Create ResourceBar files
touch include/UI/ResourceBar.h src/UI/ResourceBar.cpp

# Add to CMakeLists.txt
# In src/CMakeLists.txt, add: UI/ResourceBar.cpp

# Build
cmake --build build

# Test
ctest --test-dir build --output-on-failure

# Run
./build/kkurs
```

---

**Remember**: This is Phase 4 - UI/HUD ONLY. No assets, no ability trees, no new mechanics. Just clean, functional UI with Unicode icons and colored rectangles. Keep it simple, keep it working!
