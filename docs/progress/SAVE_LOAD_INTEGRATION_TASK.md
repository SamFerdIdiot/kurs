# Save/Load Integration Task for Ability Tree System

## Overview
This document specifies what needs to be saved/loaded for the Ability Tree System when integrated with the existing save/load branch.

## Date Created
2025-11-17

## Target Branch
Save/Load system branch (to be merged with this branch later)

## What Needs to Be Saved

### 1. Unlocked Abilities
**Data:**
- List of ability IDs that have been unlocked by the player
- Each ID is a string identifier (e.g., "driving_economical_1")

**JSON Format:**
```json
{
  "abilityTree": {
    "unlockedAbilities": [
      "driving_economical_1",
      "driving_economical_2",
      "trading_trader_1",
      "survival_efficiency",
      "perk_energy_saver"
    ]
  }
}
```

### 2. Available Skill Points
**Data:**
- Integer count of unspent skill points
- Earned through leveling up (1 point per level)

**JSON Format:**
```json
{
  "abilityTree": {
    "availableSkillPoints": 3
  }
}
```

### 3. Active Perk Cooldowns
**Data:**
- Array of perks currently on cooldown
- Each entry contains perk ID and remaining cooldown time in minutes

**JSON Format:**
```json
{
  "abilityTree": {
    "activeCooldowns": [
      {
        "perkId": "perk_energy_saver",
        "remainingMinutes": 25.5
      },
      {
        "perkId": "perk_fuel_saver",
        "remainingMinutes": 12.3
      }
    ]
  }
}
```

## Complete JSON Structure

```json
{
  "playerData": {
    "name": "Иван",
    "level": 5,
    "experience": 450,
    "money": 2500.0,
    "fuel": 45.5,
    "energy": 68.2
  },
  "abilityTree": {
    "unlockedAbilities": [
      "driving_economical_1",
      "driving_economical_2",
      "driving_fuel_reserve",
      "perk_fuel_saver",
      "trading_trader_1",
      "survival_efficiency",
      "perk_energy_saver"
    ],
    "availableSkillPoints": 3,
    "activeCooldowns": [
      {
        "perkId": "perk_energy_saver",
        "remainingMinutes": 25.5
      },
      {
        "perkId": "perk_fuel_saver",
        "remainingMinutes": 12.3
      }
    ]
  },
  "inventory": {
    // ... existing inventory data
  },
  "quests": {
    // ... existing quest data
  }
}
```

## Implementation Methods Needed

### AbilityTreeSystem Methods to Implement

```cpp
// Save current state to JSON
void AbilityTreeSystem::save() const {
    // Implementation:
    // 1. Collect all unlocked ability IDs
    // 2. Get available skill points
    // 3. Collect active cooldowns
    // 4. Serialize to JSON
    // 5. Pass to SaveManager or write to file
}

// Load state from JSON
void AbilityTreeSystem::load() {
    // Implementation:
    // 1. Read JSON from SaveManager or file
    // 2. Clear current state
    // 3. Restore unlocked abilities
    // 4. Restore skill points
    // 5. Restore active cooldowns
    // 6. Validate data integrity
}
```

### Helper Methods

```cpp
// Serialize unlocked abilities to JSON array
std::vector<std::string> AbilityTreeSystem::getUnlockedAbilityIds() const {
    std::vector<std::string> ids;
    for (const auto& ability : m_abilities) {
        if (ability.isUnlocked()) {
            ids.push_back(ability.getId());
        }
    }
    return ids;
}

// Restore unlocked state from ID list
void AbilityTreeSystem::restoreUnlockedAbilities(const std::vector<std::string>& ids) {
    for (const auto& id : ids) {
        AbilityNode* ability = getAbilityMutable(id);
        if (ability) {
            ability->setUnlocked(true);
        }
    }
}

// Serialize cooldowns
std::vector<ActivePerkCooldown> AbilityTreeSystem::getActiveCooldowns() const {
    return m_activeCooldowns;
}

// Restore cooldowns
void AbilityTreeSystem::restoreActiveCooldowns(const std::vector<ActivePerkCooldown>& cooldowns) {
    m_activeCooldowns = cooldowns;
}
```

## Integration Points with SaveManager

### SaveManager Interface (Expected)

```cpp
class SaveManager {
public:
    // Save game data
    void saveGame(const std::string& saveSlot);
    
    // Load game data
    void loadGame(const std::string& saveSlot);
    
    // Register save/load callbacks
    void registerSaveCallback(std::function<nlohmann::json()> callback);
    void registerLoadCallback(std::function<void(const nlohmann::json&)> callback);
};
```

### Registration in Game Initialization

```cpp
void GameState::initialize() {
    // ... other initialization
    
    // Register ability tree system with save manager
    saveManager.registerSaveCallback([]() {
        return AbilityTreeSystem::getInstance().toJSON();
    });
    
    saveManager.registerLoadCallback([](const nlohmann::json& data) {
        AbilityTreeSystem::getInstance().fromJSON(data);
    });
}
```

## Data Validation

### On Load
1. Verify all ability IDs exist in the current ability tree
2. Validate skill point count (should be >= 0)
3. Validate cooldown times (should be >= 0)
4. Check for impossible states (e.g., ability unlocked without prerequisites)
5. If validation fails, log error and use default state

### Validation Method

```cpp
bool AbilityTreeSystem::validateLoadedData() const {
    // Check if unlocked abilities have their prerequisites unlocked
    for (const auto& ability : m_abilities) {
        if (!ability.isUnlocked()) continue;
        
        for (const auto& prereqId : ability.getRequirement().prerequisiteIds) {
            if (!isAbilityUnlocked(prereqId)) {
                std::cerr << "ERROR: Ability " << ability.getId() 
                         << " unlocked without prerequisite " << prereqId << std::endl;
                return false;
            }
        }
    }
    
    // Check skill points are non-negative
    if (m_availableSkillPoints < 0) {
        std::cerr << "ERROR: Negative skill points: " << m_availableSkillPoints << std::endl;
        return false;
    }
    
    // Check cooldown times are non-negative
    for (const auto& cooldown : m_activeCooldowns) {
        if (cooldown.remainingMinutes < 0) {
            std::cerr << "ERROR: Negative cooldown for " << cooldown.perkId << std::endl;
            return false;
        }
    }
    
    return true;
}
```

## Testing Requirements

### Unit Tests
1. Test serialization of empty ability tree
2. Test serialization with some unlocked abilities
3. Test serialization with active cooldowns
4. Test deserialization and state restoration
5. Test validation of invalid data

### Integration Tests
1. Test full save/load cycle with ability tree data
2. Test loading old save files (backward compatibility)
3. Test handling of corrupted save data
4. Test concurrent save operations

## Migration Strategy

### Backward Compatibility
- Old save files without ability tree data should load successfully
- Default to empty ability tree (no unlocked abilities, 0 skill points)
- Log warning when loading old format

### Forward Compatibility
- Version field in save file for future changes
- Ability to handle missing or additional fields gracefully

```json
{
  "version": "1.0",
  "abilityTree": {
    // ... data
  }
}
```

## Priority
**Medium-High** - Should be implemented after EventManager and TravelCinematicScene integrations are complete and tested, but before full release.

## Dependencies
- SaveManager system (from save/load branch)
- JSON library (nlohmann/json or similar)
- File I/O utilities

## Status
- [ ] Implement `save()` method
- [ ] Implement `load()` method
- [ ] Implement helper serialization methods
- [ ] Implement validation methods
- [ ] Add unit tests
- [ ] Add integration tests
- [ ] Test backward compatibility
- [ ] Document save file format

## Notes
- Consider compressing save files if size becomes an issue
- Ability tree state is relatively small (~100-500 bytes per save)
- Auto-save after ability unlock recommended
- Consider cloud save integration for future updates

## Contact
For questions or clarifications, contact the main developer or refer to the save/load branch documentation.
