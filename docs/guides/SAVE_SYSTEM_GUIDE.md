# Save/Load System Documentation

## Overview

The save/load system provides automatic game state persistence with backup rotation. The system is designed to work independently from player actions, auto-saving when arriving at nodes.

## Components

### SaveSystem (`include/SaveSystem.h`, `src/SaveSystem.cpp`)

Manages file I/O and backup rotation.

**Features:**
- JSON-like save format (human-readable)
- 1 main save file: `saves/savegame.sav`
- 4 backup files: `saves/savegame_backup_0.sav` through `saves/savegame_backup_3.sav`
- Automatic backup rotation (oldest backup is replaced)
- Rollback support (load from any backup)

**Key Methods:**
- `saveGame(playerState)` - Saves game and rotates backups
- `loadGame(playerState)` - Loads from main save
- `loadBackup(playerState, index)` - Loads from specific backup (0-3)
- `hasSaveFile()` - Check if save exists
- `clearAllSaves()` - Delete all saves (for new game)

### GameStateManager (`include/GameStateManager.h`, `src/GameStateManager.cpp`)

Singleton that coordinates PlayerState and SaveSystem.

**Key Methods:**
- `startNewGame(origin, carType)` - Initialize new game with auto-save
- `loadGame()` - Load existing game
- `autoSave()` - Auto-save current state
- `loadBackup(index)` - Rollback to specific backup
- `hasSaveGame()` - Check if save exists

## Game Flow

### New Game
1. Player selects "New Game" from main menu
2. Character creation scene shown
3. Player chooses origin and car type
4. `GameStateManager::startNewGame()` called:
   - Clears any existing saves
   - Initializes PlayerState with chosen values
   - Auto-saves initial state
5. Transitions to first node scene

### Continue Game
1. Main menu checks if save exists on startup
2. If save exists:
   - "Continue" option enabled (green)
   - Player selects "Continue"
   - `GameStateManager::loadGame()` called
   - Transitions to saved node
3. If no save exists:
   - "Continue" option disabled (grayed out)
   - Selection ignored

### Auto-Save
- Triggered automatically when entering a node
- Happens in `NodeScene` constructor
- Creates backup of previous save before saving
- No player action required

### Backup Rotation
When saving (e.g., entering 6th node):
1. Remove `savegame_backup_3.sav` (oldest)
2. Rename `savegame_backup_2.sav` → `savegame_backup_3.sav`
3. Rename `savegame_backup_1.sav` → `savegame_backup_2.sav`
4. Rename `savegame_backup_0.sav` → `savegame_backup_1.sav`
5. Copy `savegame.sav` → `savegame_backup_0.sav`
6. Write new save to `savegame.sav`

This keeps the 5 most recent saves (1 main + 4 backups).

## Save File Format

Extended JSON-like format with full game state:

```json
{
  "version": 1,
  "timestamp": 1700000000,
  "energy": 85.5,
  "money": 1250.0,
  "fuel": 70.0,
  "origin": 0,
  "carType": 1,
  "currentNodeId": 3,
  "inventory": {
    "totalWeight": 15.5,
    "maxWeight": 100.0,
    "slotCount": 24,
    "items": [...]
  },
  "experience": {
    "level": 5,
    "exp": 2400,
    "unlockedAbilities": ["ability1", "ability2"]
  },
  "quests": [
    {
      "id": "quest_1",
      "status": 1,
      "objectives": [
        {
          "type": 0,
          "currentAmount": 3,
          "targetAmount": 5,
          "completed": false
        }
      ]
    }
  ],
  "dialogueHistory": [],
  "npcs": [
    {
      "id": "companion_1",
      "name": "Alex",
      "level": 3,
      "experience": 450,
      "passiveAbilityId": "navigation",
      "activeAbilityId": "repair"
    }
  ]
}
```

Fields:
- `version` - Save format version (for future compatibility)
- `timestamp` - Unix timestamp of save
- `energy` - Player energy (0-100)
- `money` - Player money
- `fuel` - Car fuel (0-100)
- `origin` - OriginType enum value (0=NOMAD, 1=HITCHHIKER, 2=EX_RACER)
- `carType` - CarType enum value (0=OLD_SEDAN, 1=OFFROAD_VAN, 2=SPORTS_COUPE)
- `currentNodeId` - Current node ID
- `inventory` - Player inventory data:
  - `totalWeight` - Current total weight
  - `maxWeight` - Maximum carrying capacity
  - `slotCount` - Number of inventory slots
  - `items` - Array of items with their properties
- `experience` - Player experience and abilities:
  - `level` - Player level
  - `exp` - Current experience points
  - `unlockedAbilities` - Array of unlocked ability IDs (perks)
- `quests` - Array of quests:
  - `id` - Quest ID
  - `status` - Status (0=not started, 1=active, 2=completed, 3=failed)
  - `objectives` - Array of objectives with progress
- `dialogueHistory` - Dialogue history (to be implemented)
- `npcs` - Array of NPC team members:
  - `id` - NPC ID
  - `name` - NPC name
  - `level` - NPC level
  - `experience` - NPC experience
  - `passiveAbilityId` - Passive ability ID
  - `activeAbilityId` - Active ability ID

## What Gets Saved

**Current Implementation:**
- ✅ Basic player state (energy, money, fuel)
- ✅ Character origin and car type
- ✅ Current node/location
- ✅ Complete inventory with all items
- ✅ Item properties (name, category, weight, value, stack info)
- ✅ **Player level and experience** ← NEW
- ✅ **Unlocked abilities/perks** ← NEW
- ✅ **Quest progress and status** ← NEW
- ✅ **NPC team members with levels** ← NEW
- ✅ **NPC abilities and experience** ← NEW

**Requires Additional Work:**
- ⚠️ Dialogue history (structure ready, needs DialogueManager access method)

## Usage Examples

### Access Game State
```cpp
// Get singleton instance
GameStateManager& gsm = GameStateManager::getInstance();

// Access player state
PlayerState& player = gsm.getPlayerState();
player.setEnergy(50.0f);
player.addMoney(100.0f);
```

### Check for Save
```cpp
if (GameStateManager::getInstance().hasSaveGame()) {
    // Enable "Continue" button
} else {
    // Disable "Continue" button
}
```

### Load Backup (Rollback)
```cpp
// Get list of available backups
auto backups = GameStateManager::getInstance().getBackupList();

for (const auto& backup : backups) {
    if (backup.exists) {
        std::cout << "Backup " << backup.index 
                  << " from node " << backup.nodeId << std::endl;
    }
}

// Load specific backup
if (GameStateManager::getInstance().loadBackup(2)) {
    // Successfully loaded backup 2
    // Transition to appropriate scene
}
```

## Testing

Comprehensive unit tests in `tests/unit/test_save_system.cpp`:
- Save and load functionality
- Backup rotation
- Multiple backups
- Error handling
- Invalid operations

Run tests:
```bash
cd build
ctest --output-on-failure
```

## Future Enhancements

Potential improvements:
1. Add proper JSON library (e.g., nlohmann/json) for robust parsing
2. Serialize inventory contents
3. Serialize quest progress
4. Serialize NPC relationships
5. Add save file versioning for forward compatibility
6. Add save file corruption detection/recovery
7. Add cloud save support
8. Add multiple save slots
9. Add save file metadata display in UI
10. Add manual save option (in addition to auto-save)
