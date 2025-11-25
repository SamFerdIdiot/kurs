# UI/HUD System Usage Guide

## Overview

This guide explains how to use the UI/HUD system implemented in Phase 4 of the kkurs project. The system provides a clean, animated resource display using Unicode icons and colored progress bars.

## Components

### 1. ResourceBar

**Purpose**: Animated progress bar for displaying resource levels (fuel, energy, etc.)

**Location**: 
- Header: `include/UI/ResourceBar.h`
- Implementation: `src/UI/ResourceBar.cpp`

**Features**:
- Smooth transitions using linear interpolation (lerp)
- Color gradient based on percentage:
  - Green (>70%): Healthy
  - Yellow (40-70%): Warning
  - Red (<40%): Critical
- 2px white border
- Customizable position and size

**Example Usage**:

```cpp
#include "UI/ResourceBar.h"

// Create a resource bar
ResourceBar fuelBar(sf::Vector2f(700, 50), sf::Vector2f(100, 10));

// Set current and max values
fuelBar.setValue(45.5f, 100.0f);  // 45.5% fuel

// Update animation (call every frame)
fuelBar.update(deltaTime);

// Draw to window
fuelBar.draw(window);

// Get current percentage
float percentage = fuelBar.getPercentage();  // Returns 45.5
```

### 2. HUDPanel

**Purpose**: Top-right resource display panel with Unicode icons and progress bars

**Location**:
- Header: `include/UI/HUDPanel.hpp`
- Implementation: `src/UI/HUDPanel.cpp`

**Features**:
- Top-right positioning (default: 650, 10)
- Unicode icons with colors:
  - 💰 Money (gold #FFD700)
  - ⛽ Fuel (orange #FFA500)
  - ⚡ Energy (cyan #00FFFF)
  - 📦 Cargo (white #FFFFFF)
- Animated progress bars for Fuel and Energy
- Enable/disable functionality
- Backward compatible with existing bottom panel

**Example Usage**:

```cpp
#include "UI/HUDPanel.hpp"

// Create HUD panel with window dimensions
HUDPanel hudPanel(800.0f, 600.0f);

// Update resources
hudPanel.setMoney(1250.0f);
hudPanel.setFuel(45.5f, 100.0f);  // current, max
hudPanel.setEnergy(70.0f, 100.0f);
hudPanel.setCargo(25.0f, 64.0f);

// Update animations (call every frame)
hudPanel.updateTopRight(deltaTime);

// Render HUD
hudPanel.render(window);

// Optional: Enable/disable top-right display
hudPanel.setTopRightDisplayEnabled(true);

// Get current values
float money = hudPanel.getMoney();
float fuel = hudPanel.getFuel();
```

## Scene Integration

### Pattern for All Scenes

Every scene that displays the HUD should follow this pattern:

#### 1. Include Header

```cpp
#include "UI/HUDPanel.hpp"
```

#### 2. Add Member Variable

```cpp
private:
    HUDPanel m_hudPanel;
```

#### 3. Initialize in Constructor

```cpp
MyScene::MyScene()
    : m_hudPanel(800.0f, 600.0f) {  // Window dimensions
    // Other initialization...
}
```

#### 4. Update in update() Method

```cpp
void MyScene::update(float deltaTime) {
    // Update game logic...
    
    // Update HUD values
    m_hudPanel.setMoney(money);
    m_hudPanel.setFuel(currentFuel, maxFuel);
    m_hudPanel.setEnergy(currentEnergy, maxEnergy);
    m_hudPanel.setCargo(currentCargo, maxCargo);
    
    // Update animations
    m_hudPanel.updateTopRight(deltaTime);
}
```

#### 5. Render in render() Method

```cpp
void MyScene::render(sf::RenderWindow& window) {
    // Draw scene content...
    
    // Draw HUD on top
    m_hudPanel.render(window);
}
```

### Integration Examples

#### Example 1: Scene with PlayerState Access

```cpp
void TravelCinematicScene::update(float deltaTime) {
    // Game logic...
    
    // Update HUD with real values from PlayerState
    if (m_playerState) {
        m_hudPanel.setMoney(m_playerState->getMoney());
        m_hudPanel.setFuel(m_playerState->getFuel(), 100.0f);
        m_hudPanel.setEnergy(m_playerState->getEnergy(), 100.0f);
        
        // Get cargo weight from inventory
        float cargoWeight = m_playerState->getInventory().getTotalWeight();
        m_hudPanel.setCargo(cargoWeight, 100.0f);
    }
    
    m_hudPanel.updateTopRight(deltaTime);
}
```

#### Example 2: Scene without PlayerState

```cpp
void NodeScene::update(float deltaTime) {
    // Game logic...
    
    // Update HUD with example/mock values
    m_hudPanel.setMoney(1250.0f);
    m_hudPanel.setFuel(75.0f, 100.0f);
    m_hudPanel.setEnergy(85.0f, 100.0f);
    m_hudPanel.setCargo(30.0f, 64.0f);
    
    m_hudPanel.updateTopRight(deltaTime);
}
```

## Layout Specifications

### Screen Coordinates (800×600 window)

```
Screen (800×600)
┌─────────────────────────────────────┐
│                    💰 1,250₽        │ ← (650, 10)
│                    ⛽ 45.5/100L      │ ← (650, 35)
│                    [████████░░] 45% │ ← (700, 50) - Fuel bar
│                    ⚡ 70/100         │ ← (650, 70)
│                    [████████░░] 70% │ ← (700, 85) - Energy bar
│                    📦 25/64          │ ← (650, 110)
│                                     │
│         [Game Content]              │
│                                     │
└─────────────────────────────────────┘
```

### Component Dimensions

- **Resource Bars**: 100px × 10px
- **Border Thickness**: 2px
- **Text Size**: 14px
- **Vertical Spacing**: 25px between elements

## Animation Parameters

- **Transition Time**: 0.2 seconds
- **Lerp Speed**: 5.0 × deltaTime
- **Target FPS**: 60

## Color Palette

### Resource Bar Colors
- **Green** (>70%): `sf::Color(0, 255, 0)`
- **Yellow** (40-70%): `sf::Color(255, 255, 0)`
- **Red** (<40%): `sf::Color(255, 0, 0)`
- **Border**: `sf::Color::White`
- **Background**: `sf::Color(40, 40, 40, 200)`

### Icon Colors
- **Money (💰)**: `sf::Color(255, 215, 0)` - Gold
- **Fuel (⛽)**: `sf::Color(255, 165, 0)` - Orange
- **Energy (⚡)**: `sf::Color(0, 255, 255)` - Cyan
- **Cargo (📦)**: `sf::Color::White`

## Best Practices

### 1. Update Frequency
- Always call `updateTopRight(deltaTime)` every frame
- This ensures smooth animations
- DeltaTime should be in seconds (typically 0.016 for 60 FPS)

### 2. Resource Values
- Fuel and Energy should be in range [0, max]
- Money can be any positive float
- Cargo should represent current weight vs. max capacity

### 3. Performance
- HUD rendering is lightweight (~0.1ms per frame)
- No dynamic allocations in update/render loops
- Safe to render every frame at 60 FPS

### 4. Testing
- Unit tests available in `tests/unit/test_ui.cpp`
- Run tests with: `ctest --output-on-failure`
- Current coverage: 14 tests (9 ResourceBar + 5 HUDPanel)

## Troubleshooting

### Problem: HUD not visible
**Solution**: Check that you're calling `hudPanel.render(window)` after drawing other scene elements.

### Problem: Progress bars not animating
**Solution**: Ensure `updateTopRight(deltaTime)` is called every frame with a valid deltaTime value.

### Problem: Unicode icons not displaying
**Solution**: Make sure a Unicode-compatible font is loaded. DejaVu Sans is loaded by default.

### Problem: Colors not changing with percentage
**Solution**: Verify that you're calling `setValue(current, max)` with correct values where max > 0.

## Testing Your Integration

### Quick Test Checklist
- [ ] HUD appears in top-right corner
- [ ] All 4 resources display (💰⛽⚡📦)
- [ ] Progress bars animate smoothly
- [ ] Colors change based on percentage
- [ ] HUD doesn't block important game elements
- [ ] No FPS drop (maintains 60 FPS)

### Example Test Code

```cpp
void testHUDIntegration() {
    HUDPanel hud(800, 600);
    
    // Test all resources
    hud.setMoney(1250.0f);
    hud.setFuel(45.5f, 100.0f);
    hud.setEnergy(70.0f, 100.0f);
    hud.setCargo(25.0f, 64.0f);
    
    // Simulate frame updates
    for (int i = 0; i < 60; ++i) {
        hud.updateTopRight(0.016f);  // 60 FPS
    }
    
    // Verify values
    assert(hud.getMoney() == 1250.0f);
    assert(hud.getFuel() == 45.5f);
    // etc.
}
```

## Future Enhancements

Possible future improvements (not yet implemented):

1. **Flash Effects**: Highlight resources when they change
2. **Tooltips**: Show detailed info on hover
3. **Customizable Position**: Allow scenes to specify HUD position
4. **Additional Resources**: Health, stamina, etc.
5. **Graphical Assets**: Replace Unicode icons with sprites

## Support

For questions or issues:
1. Check existing unit tests in `tests/unit/test_ui.cpp`
2. Review source code documentation in headers
3. See `PROJECT_STATUS.md` for overall project status
4. See `NEXT_PHASE_PLAN.md` for roadmap

## Version History

- **v1.0** (2025-11-16): Initial implementation
  - ResourceBar component
  - Enhanced HUDPanel with top-right display
  - Integration into 5 scenes
  - 14 unit tests
