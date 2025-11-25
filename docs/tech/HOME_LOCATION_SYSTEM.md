# Home Location System - Implementation Documentation

## Overview

This document describes the implementation of the Keep Driving style home location generation system, including procedural structure placement, resource management, and first-day tutorial integration.

## System Architecture

### Core Components

#### 1. Game Type System (`GameTypes.h/.cpp`)

**Purpose**: Define and manage game-wide type enumerations and conversions.

**Key Types:**
- `BiomeType`: Environmental regions (Desert, Forest, Coastal, Mountain, Urban, Suburban)
- `OriginType`: Character backgrounds (Nomad, Hitchhiker, Ex-Racer)
- `CarType`: Vehicle types (Old Sedan, Offroad Van, Sports Coupe)
- `StartingResources`: Resource allocation based on origin/car combination

**Resource Calculations:**
```cpp
// Example: Nomad with Old Sedan
Origin: Nomad -> Money: $150, Fuel: 80%, Energy: 90%
Car: Old Sedan -> Fuel +20% = 100%
Final: $150, 100% fuel, 90% energy

// Example: Ex-Racer with Sports Coupe
Origin: Ex-Racer -> Money: $300, Fuel: 60%, Energy: 70%
Car: Sports Coupe -> Fuel -20%, Money +$100
Final: $400, 40% fuel, 70% energy
```

#### 2. Game State (`GameState.h`)

**Purpose**: Singleton for sharing player choices across scenes.

**Features:**
- Stores origin and car type from character creation
- Accessible globally via `GameState::getInstance()`
- Resets on new game

**Usage:**
```cpp
GameState& state = GameState::getInstance();
state.setOrigin(OriginType::NOMAD);
state.setCarType(CarType::OLD_SEDAN);

if (state.isCharacterCreated()) {
    // Use stored values
}
```

#### 3. Location Generator (`LocationGenerator.h/.cpp`)

**Purpose**: Procedurally generate home area with biome-appropriate structures.

**Features:**
- Biome selection based on character origin
- 3-4 structures placed around home
- Semi-circular layout (180° arc to the right of home)
- Random distance variation (200-350 pixels)
- Biome-specific structure types
- Neighbor structure logic (complementary pairings)

**Structure Distribution by Biome:**
```
Desert:    Gas Station, Store, Diner, Motel, Mechanic
Forest:    Gas Station, Store, Garage, Diner
Coastal:   Gas Station, Store, Diner, Motel
Mountain:  Gas Station, Store, Mechanic, Garage, Motel
Urban:     All types available
Suburban:  All types available
```

**Placement Algorithm:**
```
1. Place home at (15% screen width, 45% screen height)
2. Determine number of structures (3-4, random)
3. For each structure:
   - Select type based on biome
   - Calculate angle: -45° to +135° (spread evenly)
   - Random distance: 200-350 pixels from home
   - Apply screen bounds checking
4. Avoid duplicates (3 retry attempts)
```

#### 4. Enhanced Player State (`PlayerState.h/.cpp`)

**Purpose**: Track player resources and character context.

**New Features:**
- Origin and car type storage
- `initializeResources()` method
- Resource initialization based on character choices

**Resource Management:**
- Energy: 0-100% (drains 2% per second)
- Fuel: 0-100% (drains 0.5% per second when moving)
- Money: No upper limit, cannot go negative

#### 5. Enhanced Interaction Menu (`InteractionMenu.h/.cpp`)

**Purpose**: Context-sensitive structure interaction with resource costs.

**New Features:**
- `InteractionAction` struct with costs and effects
- Affordability checking
- Visual feedback (grayed out unaffordable actions)
- Result messages
- Resource consumption on action execution

**Action Examples:**
```cpp
// Gas Station - Refuel
Cost: $50
Effect: Fuel +100%
Result: "Tank is full!"

// Diner - Eat Meal
Cost: $15
Effect: Energy +50%
Result: "Delicious!"

// Home - Rest
Cost: Free
Effect: Energy +100%
Result: "You feel refreshed!"
```

#### 6. Tutorial System (`TutorialSystem.h/.cpp`)

**Purpose**: Guide players through first-day experience.

**Features:**
- 5-step tutorial sequence
- Auto-start on first DrivingScene entry
- Skip anytime with 'T' key
- Continue with SPACE
- Contextual hint system
- Non-intrusive top-of-screen display

**Tutorial Steps:**
1. Welcome and introduction
2. Movement controls
3. Resource management explanation
4. Interaction guidance
5. Ready to explore

## Integration Flow

### Character Creation → Driving Scene

```
1. Player selects background and car in CharacterCreationScene
2. CharacterCreationScene saves to GameState
3. DrivingScene constructor reads GameState
4. PlayerState.initializeResources() called
5. StartZone.generate(origin) called
6. LocationGenerator creates biome-appropriate area
7. Tutorial starts automatically (first time only)
```

### Resource Consumption Flow

```
Every frame in DrivingScene.update():
1. Check if menu is visible
2. If not in menu:
   - Energy drains (2% per second)
   - If moving: Fuel drains (0.5% per second)
3. Update HUD display
4. Check for low resource warnings
```

### Interaction Flow

```
1. Player drives near structure
2. Press 'E' to open InteractionMenu
3. Menu displays actions with costs
4. Affordable actions: white text
5. Unaffordable actions: gray text
6. Player selects action:
   - If affordable: Apply costs/effects, show result
   - If not: Show "Not enough money!" message
7. Press ESC or select "Leave" to close
```

## HUD Integration

### Real-time Updates

The HUD displays live player state:
- **Fuel Gauge**: Red/yellow/green based on level
- **Energy Display**: Shows percentage
- **Money Counter**: Current funds
- **LCD Screen**: Context-sensitive messages

### Warning System

```cpp
if (fuel < 15%) {
    LCD: "!! LOW FUEL !!"
    fuelWarning: true
}
else if (energy < 20%) {
    LCD: "!! TIRED !!"
    checkEngine: true
}
else {
    LCD: "DRIVING..."
}
```

## Extension Points

### 1. Event System (Prepared)

The architecture supports future random events:
- Hook points in DrivingScene.update()
- Tutorial hint system can announce events
- Structure types can be extended

### 2. Quest System (Prepared)

TutorialSystem can be extended for quests:
- Add quest steps alongside tutorial steps
- Track completion with step.dismissed
- Show contextual hints for quest objectives

### 3. Save/Load (Prepared)

GameState and PlayerState ready for serialization:
- Origin and car type
- Current resources
- Tutorial completion status

### 4. Additional Structures (Prepared)

LocationGenerator supports new structure types:
- Add to LocationType enum
- Define in getStructureTypesForBiome()
- Add actions in InteractionMenu.updateActionList()

## Testing Recommendations

### Manual Testing Checklist

1. **Character Creation**
   - [ ] Test all 3 backgrounds × 3 cars = 9 combinations
   - [ ] Verify resources match expected values
   - [ ] Check biome selection matches origin

2. **Location Generation**
   - [ ] Home always present
   - [ ] 3-4 structures generated
   - [ ] Structures within screen bounds
   - [ ] No overlapping buildings
   - [ ] Biome-appropriate structure types

3. **Resource Management**
   - [ ] Fuel drains when moving
   - [ ] Energy drains over time
   - [ ] Consumption pauses in menus
   - [ ] Resources can't go negative
   - [ ] HUD updates in real-time

4. **Interactions**
   - [ ] All structure types have actions
   - [ ] Costs deducted correctly
   - [ ] Unaffordable actions grayed out
   - [ ] Result messages appear
   - [ ] "Leave" closes menu

5. **Tutorial**
   - [ ] Starts automatically on first entry
   - [ ] All 5 steps display correctly
   - [ ] SPACE advances steps
   - [ ] T skips tutorial
   - [ ] Doesn't repeat on re-entry

### Automated Testing (Future)

Suggested test cases:
```cpp
TEST(GameTypes, StartingResources) {
    auto res = getStartingResources(OriginType::NOMAD, CarType::OLD_SEDAN);
    EXPECT_EQ(res.money, 150.0f);
    EXPECT_EQ(res.fuel, 100.0f);  // 80 + 20 bonus
}

TEST(LocationGenerator, BiomeSelection) {
    EXPECT_EQ(getPreferredBiome(OriginType::NOMAD), BiomeType::DESERT);
    EXPECT_EQ(getPreferredBiome(OriginType::EX_RACER), BiomeType::URBAN);
}

TEST(InteractionAction, Affordability) {
    PlayerState state;
    state.setMoney(40.0f);
    
    InteractionAction expensive("Test", 50.0f);
    EXPECT_FALSE(expensive.canAfford(state));
    
    InteractionAction affordable("Test", 30.0f);
    EXPECT_TRUE(affordable.canAfford(state));
}
```

## Performance Considerations

### Optimization Notes

1. **Location Generation**: One-time cost at scene creation
2. **Resource Updates**: Simple floating-point operations per frame
3. **HUD Updates**: Only updates when values change (optimization possible)
4. **Tutorial Rendering**: Only when active

### Memory Usage

Estimated per-scene memory:
- LocationGenerator: ~1 KB (temporary)
- StartZone: ~100 bytes per location (5-10 locations)
- TutorialSystem: ~2 KB (steps + UI)
- Total additional: < 10 KB

## Configuration Options

### Tunable Parameters

In source files, easily adjustable:

**Resource Consumption** (`DrivingScene.cpp`):
```cpp
float energyDrain = 2.0f * deltaTime;      // Adjust rate
float fuelConsumption = 0.5f * deltaTime;  // Adjust rate
```

**Starting Resources** (`GameTypes.cpp`):
```cpp
case OriginType::NOMAD:
    resources.money = 150.0f;   // Adjust starting money
    resources.fuel = 80.0f;     // Adjust starting fuel
```

**Structure Placement** (`LocationGenerator.cpp`):
```cpp
std::uniform_int_distribution<int> structureCountDist(3, 4);  // Adjust count
std::uniform_real_distribution<float> distanceDist(200.0f, 350.0f);  // Adjust distance
```

**Tutorial Duration** (`TutorialSystem.cpp`):
```cpp
// In initializeTutorialSteps(), add/remove/modify steps
```

## Known Limitations

1. **Static Home Position**: Home is always at same screen position
2. **No Structure Variety**: Each type has one appearance
3. **Simple Resource Model**: No advanced needs (hunger, thirst, etc.)
4. **No Structure Removal**: Structures persist entire scene
5. **Tutorial Only Once**: No replay option (by design)

## Future Enhancements

### Planned Features

1. **Dynamic Events**: Random encounters while driving
2. **Weather System**: Effects on resources and driving
3. **Day/Night Cycle**: Time-based mechanics
4. **Advanced Needs**: Hunger, thirst, vehicle condition
5. **Multiple Home Locations**: Unlock new starting areas
6. **Structure Upgrades**: Improve building services
7. **NPC Interactions**: Characters at structures
8. **Quest System**: Story-driven objectives

### Architecture Support

The current system is designed to support:
- Event system hooks (TODO comments in DrivingScene)
- Extended tutorial/hint system
- Additional structure types
- Complex resource models
- Save/load functionality

## Conclusion

This implementation provides a solid foundation for the Keep Driving style home location system with:
- ✅ Procedural generation based on character choices
- ✅ Resource management with real-time updates
- ✅ Context-sensitive structure interactions
- ✅ First-day tutorial for new players
- ✅ Extensible architecture for future features

All core requirements from the problem statement have been implemented and integrated into the main branch workflow.
