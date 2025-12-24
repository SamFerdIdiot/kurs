#include "GameStateManager.h"
#include <iostream>


GameStateManager::GameStateManager()
    : m_currentLocationType(LocationType::UNKNOWN) {


}


bool GameStateManager::hasSaveGame() const {
    return m_saveSystem.hasSaveFile();
}


void GameStateManager::startNewGame(OriginType origin, CarType carType) {
    std::cout << "Starting new game..." << std::endl;


    m_saveSystem.clearAllSaves();






    m_playerState.setCurrentNodeId(0);





    autoSave();

    std::cout << "New game started successfully" << std::endl;
}


bool GameStateManager::loadGame() {
    std::cout << "Loading game..." << std::endl;

    if (!m_saveSystem.loadGame(m_playerState)) {
        std::cerr << "Failed to load game" << std::endl;
        return false;
    }

    std::cout << "Game loaded successfully" << std::endl;
    return true;
}


void GameStateManager::autoSave() {
    std::cout << "Auto-saving game..." << std::endl;

    if (!m_saveSystem.saveGame(m_playerState)) {
        std::cerr << "Failed to auto-save game" << std::endl;
        return;
    }

    std::cout << "Auto-save completed" << std::endl;
}


bool GameStateManager::loadBackup(int backupIndex) {
    std::cout << "Loading backup " << backupIndex << "..." << std::endl;

    if (!m_saveSystem.loadBackup(m_playerState, backupIndex)) {
        std::cerr << "Failed to load backup " << backupIndex << std::endl;
        return false;
    }

    std::cout << "Backup " << backupIndex << " loaded successfully" << std::endl;
    return true;
}


std::vector<SaveSystem::BackupInfo> GameStateManager::getBackupList() const {
    return m_saveSystem.getBackupList();
}
