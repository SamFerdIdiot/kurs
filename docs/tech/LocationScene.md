# LocationScene Documentation

## Overview

`LocationScene` is a unified scene class that handles player interactions with various location types in the game. This implements Stage 6 requirements from README_COMPREHENSIVE.md for location structures and interactions.

## Purpose

Provides a consistent interface for entering and interacting with different types of locations encountered during the road trip journey:
- Gas Stations (⛽)
- General Stores (🏪)
- Diners (🍔)
- Garages/Mechanics (🔧)
- Motels (🏨)

## Architecture

### Class Hierarchy
```
Scene (base interface)
  └── LocationScene
```

### Key Components

**Visual Elements:**
- Background interior
- Location-specific objects (fuel pumps, shelves, tables, tools, etc.)
- Service counter
- Player character
- Exit door
- Interactive object labels

**Interaction System:**
- Integration with InteractionMenu
- Location-specific action handlers
- Resource transaction validation
- Context-sensitive options based on location type

**State Management:**
- PlayerState integration for resources (money, fuel, energy)
- Scene transition handling
- Interaction menu toggle

## Location Types and Services

### Gas Station
```
Available Actions:
- Fill Tank: $50 (sets fuel to 100%)
- Buy Snacks: Opens shop interface
- Talk to Attendant: Dialogue interaction
- Leave: Return to driving
```

### Store
```
Available Actions:
- Browse Items: Opens inventory/shop
- Talk to Clerk: Dialogue interaction
- Leave: Return to driving
```

### Diner
```
Available Actions:
- Order Meal: $15 (+30% energy)
- Coffee: $3 (+10% energy, quick option)
- Talk to Staff: Dialogue interaction
- Leave: Return to driving
```

### Garage/Mechanic
```
Available Actions:
- Quick Repair: $50 (minor repairs)
- Full Service: $150 (major repairs)
- Talk to Mechanic: Dialogue interaction
- Leave: Return to driving
```

### Motel
```
Available Actions:
- Rest: $20 (restore 100% energy)
- Check Inventory: Opens inventory scene
- Leave: Return to driving
```

## Usage Example

```cpp
// Create a gas station scene
auto gasStation = std::make_unique<LocationScene>(
    LocationType::GAS_STATION,
    "Joe's Gas Station"
);

// The scene will automatically:
// - Display gas station interior
// - Set up fuel pump and shop objects
// - Configure appropriate interaction options
// - Handle refuel and shop transactions
```

## Integration with Game Systems

### Scene Manager
LocationScene is registered in SceneManager with the `SceneType::LOCATION` enum value.

```cpp
case SceneType::LOCATION:
    return std::make_unique<LocationScene>(
        LocationType::GAS_STATION, 
        "Roadside Gas Station"
    );
```

### Resource Management
All resource transactions go through PlayerState:
- Money: `getMoney()`, `addMoney(amount)`
- Fuel: `getFuel()`, `setFuel(amount)`, `addFuel(amount)`
- Energy: `getEnergy()`, `setEnergy(amount)`, `addEnergy(amount)`

### Scene Transitions
- **From:** DrivingScene, SideViewScene (when player presses E near a location)
- **To:** DrivingScene (when player leaves location)
- **Potential:** DialogueScene (for NPC conversations), InventoryScene (for shopping)

## Controls

```
[E]       - Toggle interaction menu
[Up/Down] - Navigate menu options (when menu open)
[Enter]   - Select menu option
[ESC]     - Leave location (return to driving)
```

## Status Display

Real-time resource display in top-right corner:
```
Money: $XXX
Fuel: XX%
Energy: XX%
```

## Visual Design

The scene follows the existing project aesthetic:
- Dark interior backgrounds
- Colored rectangular placeholders for objects
- White outlines for clarity
- Yellow status text
- Location-specific color schemes:
  - Gas Station: Orange/Red tones
  - Store: Blue tones
  - Diner: Yellow/Warm tones
  - Garage: Gray/Metal tones
  - Motel: Purple/Warm tones

## Future Enhancements

### Recommended Additions
1. **Sprite Assets**: Replace placeholder rectangles with actual sprites
2. **Enhanced InteractionMenu**: Full menu implementation with selection indicators
3. **NPC System**: Add location-specific NPCs with dialogues
4. **Inventory Integration**: Direct shopping interface
5. **Sound Effects**: Location-specific ambient sounds and action sounds
6. **Save Points**: Auto-save functionality at motels
7. **Quest Integration**: Location-based quest triggers
8. **Dynamic Pricing**: Resource costs based on location and economy
9. **Time System**: Track time spent in locations
10. **Weather Effects**: Visual weather changes visible through windows

### Extensibility
New location types can be easily added by:
1. Adding new value to `LocationType` enum
2. Adding case in `setupLocationElements()` for visual setup
3. Adding case in `setupInteractionOptions()` for menu options
4. Adding case in `handleInput()` for action routing
5. Implementing handler methods for new actions

## Code Structure

```
LocationScene.h
├── Constructor (LocationType, name)
├── Scene Interface Methods
│   ├── handleInput()
│   ├── update()
│   ├── render()
│   ├── getNextScene()
│   └── isFinished()
└── Private Methods
    ├── setupLocationElements()      // Visual setup
    ├── setupInteractionOptions()    // Menu configuration
    ├── handleRefuel()               // Gas station service
    ├── handleShop()                 // Store/shopping
    ├── handleRepair()               // Garage services
    ├── handleEat()                  // Diner services
    ├── handleRest()                 // Motel services
    ├── handleTalk()                 // NPC dialogue
    ├── handleLeave()                // Exit location
    └── getLocationDescription()     // Flavor text
```

## Testing

To test LocationScene:
1. Build the project: `./scripts/build.sh`
2. Modify `RouteSelectionScene` to transition to `SceneType::LOCATION`
3. Or modify `DrivingScene` to transition when pressing a test key

### Test Cases
- [ ] Can enter different location types
- [ ] Visual elements render correctly
- [ ] Status display updates in real-time
- [ ] Can toggle interaction menu with E key
- [ ] Resource transactions work (money, fuel, energy)
- [ ] Can't make purchases without enough money
- [ ] Exit returns to driving scene
- [ ] State persists correctly across scenes

## References

- Main Documentation: `README.md` (Section: Локации)
- Comprehensive Guide: `README_COMPREHENSIVE.md` (Stage 6)
- Base Scene Interface: `include/Scene.h`
- Location Data Model: `include/Location.h`
- Resource Management: `include/PlayerState.h`
- Interaction System: `include/InteractionMenu.h`

## Authors

Implemented as part of Stage 6 requirements for unified location interaction system.

---

**Note:** This is a foundation implementation. Many features like full InteractionMenu integration, NPC dialogues, and shopping systems are prepared but require additional implementation in their respective systems.
