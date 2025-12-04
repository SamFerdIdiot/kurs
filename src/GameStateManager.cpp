#include "GameStateManager.h"
#include <iostream>

// Constructor
GameStateManager::GameStateManager()
    : m_currentLocationType(LocationType::UNKNOWN) {
    // Constructor intentionally empty - state will be initialized
    // either by loading a save or starting a new game
}

// Check if a save file exists
bool GameStateManager::hasSaveGame() const {
    return m_saveSystem.hasSaveFile();
}

// Start a new game
void GameStateManager::startNewGame(OriginType origin, CarType carType) {
    std::cout << "Starting new game..." << std::endl;
    
    // Clear any existing saves
    m_saveSystem.clearAllSaves();
    
    // [MVP] Disabled - Resource initialization (uncomment to enable)
    /*
    // Initialize player state with chosen origin and car type
    m_playerState.initializeResources(origin, carType);
    */
    m_playerState.setCurrentNodeId(0); // Start at node 0
    /*
    m_playerState.initializeStartingInventory();
    */
    
    // Save the initial state
    autoSave();
    
    std::cout << "New game started successfully" << std::endl;
}

// Load existing game
bool GameStateManager::loadGame() {
    std::cout << "Loading game..." << std::endl;
    
    if (!m_saveSystem.loadGame(m_playerState)) {
        std::cerr << "Failed to load game" << std::endl;
        return false;
    }
    
    std::cout << "Game loaded successfully" << std::endl;
    return true;
}

// Auto-save current state
void GameStateManager::autoSave() {
    std::cout << "Auto-saving game..." << std::endl;
    
    if (!m_saveSystem.saveGame(m_playerState)) {
        std::cerr << "Failed to auto-save game" << std::endl;
        return;
    }
    
    std::cout << "Auto-save completed" << std::endl;
}

// Load from specific backup
bool GameStateManager::loadBackup(int backupIndex) {
    std::cout << "Loading backup " << backupIndex << "..." << std::endl;
    
    if (!m_saveSystem.loadBackup(m_playerState, backupIndex)) {
        std::cerr << "Failed to load backup " << backupIndex << std::endl;
        return false;
    }
    
    std::cout << "Backup " << backupIndex << " loaded successfully" << std::endl;
    return true;
}

// Get backup list
std::vector<SaveSystem::BackupInfo> GameStateManager::getBackupList() const {
    return m_saveSystem.getBackupList();
}
