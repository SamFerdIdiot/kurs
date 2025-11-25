#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "PlayerState.h"
#include "SaveSystem.h"
#include <memory>

// GameStateManager - Singleton to manage global game state and save/load
// Purpose: Coordinate PlayerState and SaveSystem, provide global access
// Features:
//   - Singleton pattern for easy access across scenes
//   - Manages PlayerState instance
//   - Handles auto-save and manual save/load
//   - New game initialization
class GameStateManager {
public:
    // Get singleton instance
    static GameStateManager& getInstance() {
        static GameStateManager instance;
        return instance;
    }
    
    // Prevent copying
    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;
    
    // Access to player state
    PlayerState& getPlayerState() { return m_playerState; }
    const PlayerState& getPlayerState() const { return m_playerState; }
    
    // Access to save system
    SaveSystem& getSaveSystem() { return m_saveSystem; }
    const SaveSystem& getSaveSystem() const { return m_saveSystem; }
    
    // Game lifecycle operations
    
    // Check if a save file exists
    bool hasSaveGame() const;
    
    // Start a new game (clears saves and initializes fresh state)
    void startNewGame(OriginType origin, CarType carType);
    
    // Load existing game from save
    bool loadGame();
    
    // Auto-save current game state
    void autoSave();
    
    // Load from specific backup
    bool loadBackup(int backupIndex);
    
    // Get backup list for UI
    std::vector<SaveSystem::BackupInfo> getBackupList() const;
    
private:
    GameStateManager();
    
    PlayerState m_playerState;
    SaveSystem m_saveSystem;
};

#endif // GAME_STATE_MANAGER_H
