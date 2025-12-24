#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include "PlayerState.h"
#include <string>
#include <vector>










class SaveSystem {
public:
    SaveSystem();



    bool saveGame(const PlayerState& playerState);


    bool loadGame(PlayerState& playerState);


    bool loadBackup(PlayerState& playerState, int backupIndex);


    bool hasSaveFile() const;


    bool hasBackup(int backupIndex) const;


    struct BackupInfo {
        int index;
        std::string timestamp;
        int nodeId;
        bool exists;
    };
    std::vector<BackupInfo> getBackupList() const;


    void clearAllSaves();

private:

    std::string serializePlayerState(const PlayerState& playerState) const;
    bool deserializePlayerState(const std::string& data, PlayerState& playerState) const;


    bool writeToFile(const std::string& filename, const std::string& data);
    std::string readFromFile(const std::string& filename) const;
    bool fileExists(const std::string& filename) const;


    void rotateBackups();


    std::string getSaveFilePath() const;
    std::string getBackupFilePath(int index) const;


    std::string m_saveDirectory;
    static constexpr int MAX_BACKUPS = 4;
};

#endif
