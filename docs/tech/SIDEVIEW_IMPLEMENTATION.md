# SideViewScene Implementation

## Overview

The **SideViewScene** is a fully-featured side-scrolling game scene with layered rendering, parallax effects, automatic structure generation, and complete integration with the game's HUD and interaction systems.

## Features

### 1. Layered Rendering System

The scene implements 7 distinct rendering layers drawn in the correct order:

1. **Background Layer** (Furthest)
   - Sky (light blue)
   - Mountains (gray-blue)
   - Parallax speed: 0.1x (slowest)

2. **Second Background Layer**
   - Hills (green)
   - Trees (dark green)
   - Parallax speed: 0.3x (medium)

3. **Structures Layer**
   - Procedurally generated buildings via StructurePlacement
   - 8 structure types: Gas Station, Store, Garage, Motel, Diner, Mechanic, Rest Stop, Billboard
   - Automatic generation and cleanup

4. **Vehicles Layer**
   - Oncoming traffic (moving towards player)
   - Passing traffic (slower than player)
   - Dynamic spawning and removal

5. **Road and Player Layer**
   - Gray road surface
   - White lane markings
   - Player's red car (fixed screen position)

6. **HUD Layer**
   - Integrated HUDPanel showing:
     - Speed, Fuel, Energy, Money
     - Temperature, Odometer
     - LCD messages
     - Warning indicators

7. **Menu Layer** (Top)
   - InteractionMenu popup when near structures
   - Context-sensitive actions based on location type

### 2. StructurePlacement System

Automatic structure generation along the road:

- **Procedural Generation**: Structures spawn ahead of camera
- **Random Spacing**: 800-2000 units between structures
- **8 Structure Types**:
  - Gas Station (Red) - Refuel and buy supplies
  - Store (Blue) - Buy/sell items
  - Garage (Gray) - Repair and upgrade car
  - Motel (Purple) - Rest and save game
  - Diner (Orange) - Eat meals and coffee
  - Mechanic (Dark Gray) - Advanced repairs
  - Rest Stop (Green) - Free rest
  - Billboard (Yellow) - Non-interactive scenery

- **Automatic Cleanup**: Structures 2000+ units behind camera are removed
- **Proximity Detection**: Player can interact when within 200 units
- **Collision Detection**: Uses distance calculation for interaction range

### 3. Camera System

Smooth camera tracking with proper position management:

- **World Space**: Player position tracked in world coordinates (cameraX)
- **Screen Space**: Player car stays at fixed screen position (x=300)
- **Smooth Following**: Camera interpolates to target position
- **Boundary Check**: Camera never goes negative
- **Parallax Calculation**: Each layer offset calculated based on camera position

### 4. Parallax Scrolling

Multi-layer parallax creates depth effect:

- Background layers move slower than foreground
- Parallax speeds: 0.1x (far), 0.3x (medium), 1.0x (near)
- Infinite scrolling via modulo wrapping
- Seamless tiling for backgrounds

### 5. Player Controls

Full movement control within the game world:

- **Horizontal Movement**: Left/Right or A/D keys
  - Accelerate: Right/D (max speed 120 km/h)
  - Decelerate: Left/A (min speed 0)
  - Auto-deceleration when no input
  
- **Vertical Movement**: Up/Down or W/S keys
  - Move up/down within road boundaries (460-600 pixels)
  - Prevents leaving road area

- **Interaction**: E key
  - Checks for nearby structures
  - Opens InteractionMenu with context-sensitive actions

- **Scene Transitions**:
  - Escape: Pause menu
  - I: Inventory
  - T: Town (test mode)

### 6. Integration with Existing Systems

#### HUDPanel Integration
- Real-time HUD updates every frame
- Displays current player state:
  - Speed from player movement
  - Fuel, Energy, Money from PlayerState
  - Odometer calculated from world position
  - Warning indicators (low fuel, etc.)

#### InteractionMenu Integration
- Converts StructurePlacement structures to Location objects
- Uses existing menu system with proper action lists
- Handles resource transactions (money, fuel, energy)
- Actions based on structure type:
  - Gas Station: Refuel ($50), Buy Supplies
  - Diner: Eat Meal ($15), Coffee ($5)
  - Motel: Rest ($20), Save Game
  - And more...

#### PlayerState Integration
- Fuel consumption simulation
- Money transactions for services
- Energy management
- All values properly tracked and updated

## How to Access SideViewScene

The SideViewScene is accessible from the game's Route Selection screen:

1. Start the game
2. Select "New Game" from main menu
3. Complete character creation (2 steps)
4. In Route Selection, navigate to the **5th route** (purple)
   - Route name: "Side View Highway"
   - Description: "Full side-scrolling adventure with parallax"
5. Press Enter to select and enter SideViewScene

## Technical Implementation

### File Structure

- **include/SideViewScene.h** - Scene class definition
- **src/SideViewScene.cpp** - Scene implementation
- **include/StructurePlacement.h** - Structure generation system
- **src/StructurePlacement.cpp** - Structure generation implementation

### Key Methods

#### SideViewScene
- `initializeLayers()` - Setup all rendering layers
- `updateCamera(float deltaTime)` - Smooth camera tracking
- `updatePlayerMovement(float deltaTime)` - Handle player controls
- `updateVehicles(float deltaTime)` - Update traffic
- `calculateParallaxOffset(float parallaxSpeed)` - Calculate layer offset
- `renderBackground()`, `renderSecondBackground()`, etc. - Layer rendering
- `checkNearbyStructures()` - Proximity detection
- `handleInteractionMenuAction()` - Process menu selections

#### StructurePlacement
- `generateInitialStructures(float start, float end)` - Create initial structures
- `update(float cameraX, float deltaTime)` - Generate/cleanup structures
- `render(sf::RenderWindow& window, float cameraX, float screenWidth)` - Draw visible structures
- `getNearbyStructure(float x, float y, float range)` - Find nearby structure
- `cleanupStructures(float cameraX)` - Remove off-screen structures

### Scene State Management

The scene maintains:
- Camera position (m_cameraX)
- Player world position (m_playerCarX, m_playerCarY)
- Player speed (m_playerSpeed)
- Vehicle list (m_vehicles)
- Player resources via PlayerState
- Scene transition state (m_isFinished, m_nextScene)

### Performance Considerations

- Structures automatically cleaned up when off-screen
- Vehicles removed when far from camera
- Efficient parallax calculation using modulo
- Layer rendering only draws visible elements
- Fixed 60 FPS frame rate

## Future Enhancements

Potential improvements for future development:

1. **Asset Integration**
   - Replace placeholder rectangles with actual sprites
   - Add texture atlases for structures
   - Animated vehicles

2. **Enhanced Physics**
   - Collision detection between player and vehicles
   - More realistic vehicle movement
   - Weather effects (rain, snow)

3. **Dynamic Events**
   - Random encounters
   - Traffic accidents
   - Roadblocks
   - NPCs on the road

4. **Expanded Interactions**
   - Mini-games at locations
   - NPC dialogues
   - Quest system integration

5. **Visual Effects**
   - Particle systems (exhaust, dust)
   - Day/night cycle
   - Dynamic lighting
   - Screen shake effects

## Testing

The implementation has been tested for:
- ✓ Successful compilation
- ✓ Scene creation and initialization
- ✓ Integration with SceneManager
- ✓ Proper scene transitions
- ✓ Layer rendering order
- ✓ Structure generation and cleanup
- ✓ Camera tracking
- ✓ Player movement
- ✓ HUD integration
- ✓ Menu integration

## Notes

- All code follows the existing project structure
- Minimal changes to existing files
- Full backward compatibility maintained
- Ready for merge to main branch

---

**Implementation Date**: 2025-11-10  
**Status**: Complete and Ready for Review
