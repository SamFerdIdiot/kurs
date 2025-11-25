#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include "PlayerState.h"
#include <string>
#include <vector>

// SaveSystem - Manages game save/load with automatic backups
// Purpose: Provide auto-save functionality with backup rotation
// Features:
//   - Auto-save when arriving at nodes
//   - 1 main save + 4 backup files
//   - Backup rotation (oldest backup gets replaced)
//   - Load from main save or any backup
//   - JSON format for human-readable saves
//   - Independent from player actions (no manual save)
class SaveSystem {
public:
    SaveSystem();
    
    // Save game state
    // Creates main save and rotates backups
    bool saveGame(const PlayerState& playerState);
    
    // Load game state from main save
    bool loadGame(PlayerState& playerState);
    
    // Load game state from specific backup (0-3)
    bool loadBackup(PlayerState& playerState, int backupIndex);
    
    // Check if a save file exists
    bool hasSaveFile() const;
    
    // Check if specific backup exists
    bool hasBackup(int backupIndex) const;
    
    // Get list of available backups with metadata
    struct BackupInfo {
        int index;
        std::string timestamp;
        int nodeId;
        bool exists;
    };
    std::vector<BackupInfo> getBackupList() const;
    
    // Delete all saves (for new game)
    void clearAllSaves();
    
private:
    // Save/load helpers
    std::string serializePlayerState(const PlayerState& playerState) const;
    bool deserializePlayerState(const std::string& data, PlayerState& playerState) const;
    
    // File operations
    bool writeToFile(const std::string& filename, const std::string& data);
    std::string readFromFile(const std::string& filename) const;
    bool fileExists(const std::string& filename) const;
    
    // Backup rotation
    void rotateBackups();
    
    // File paths
    std::string getSaveFilePath() const;
    std::string getBackupFilePath(int index) const;
    
    // Save directory
    std::string m_saveDirectory;
    static constexpr int MAX_BACKUPS = 4;
};

#endif // SAVE_SYSTEM_H
