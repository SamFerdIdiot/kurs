#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "PlayerState.h"
#include "SaveSystem.h"
#include "GameTypes.h"
#include "Location.h"
#include <memory>








class GameStateManager {
public:

    static GameStateManager& getInstance() {
        static GameStateManager instance;
        return instance;
    }


    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;


    PlayerState& getPlayerState() { return m_playerState; }
    const PlayerState& getPlayerState() const { return m_playerState; }


    SaveSystem& getSaveSystem() { return m_saveSystem; }
    const SaveSystem& getSaveSystem() const { return m_saveSystem; }




    bool hasSaveGame() const;


    void startNewGame(OriginType origin, CarType carType);


    bool loadGame();


    void autoSave();


    bool loadBackup(int backupIndex);


    std::vector<SaveSystem::BackupInfo> getBackupList() const;


    void setCurrentLocationType(LocationType type) { m_currentLocationType = type; }
    LocationType getCurrentLocationType() const { return m_currentLocationType; }

private:
    GameStateManager();

    PlayerState m_playerState;
    SaveSystem m_saveSystem;
    LocationType m_currentLocationType;
};

#endif
