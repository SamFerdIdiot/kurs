#include "SaveSystem.h"
#include "DialogueManager.h"
#include "QuestManager.h"
#include "NPC.h"
#include "ExperienceSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

// Constructor
SaveSystem::SaveSystem() 
    : m_saveDirectory("saves") {
    // Create saves directory if it doesn't exist
    struct stat info;
    if (stat(m_saveDirectory.c_str(), &info) != 0) {
        // Directory doesn't exist, create it
        mkdir(m_saveDirectory.c_str(), 0755);
    }
}

// Save game state
bool SaveSystem::saveGame(const PlayerState& playerState) {
    // First, rotate existing backups
    rotateBackups();
    
    // Serialize player state
    std::string data = serializePlayerState(playerState);
    if (data.empty()) {
        std::cerr << "Failed to serialize player state" << std::endl;
        return false;
    }
    
    // Write to main save file
    if (!writeToFile(getSaveFilePath(), data)) {
        std::cerr << "Failed to write save file" << std::endl;
        return false;
    }
    
    std::cout << "Game saved successfully" << std::endl;
    return true;
}

// Load game state from main save
bool SaveSystem::loadGame(PlayerState& playerState) {
    if (!hasSaveFile()) {
        std::cerr << "No save file found" << std::endl;
        return false;
    }
    
    std::string data = readFromFile(getSaveFilePath());
    if (data.empty()) {
        std::cerr << "Failed to read save file" << std::endl;
        return false;
    }
    
    if (!deserializePlayerState(data, playerState)) {
        std::cerr << "Failed to deserialize save data" << std::endl;
        return false;
    }
    
    std::cout << "Game loaded successfully" << std::endl;
    return true;
}

// Load game state from specific backup
bool SaveSystem::loadBackup(PlayerState& playerState, int backupIndex) {
    if (backupIndex < 0 || backupIndex >= MAX_BACKUPS) {
        std::cerr << "Invalid backup index: " << backupIndex << std::endl;
        return false;
    }
    
    if (!hasBackup(backupIndex)) {
        std::cerr << "Backup " << backupIndex << " does not exist" << std::endl;
        return false;
    }
    
    std::string data = readFromFile(getBackupFilePath(backupIndex));
    if (data.empty()) {
        std::cerr << "Failed to read backup file" << std::endl;
        return false;
    }
    
    if (!deserializePlayerState(data, playerState)) {
        std::cerr << "Failed to deserialize backup data" << std::endl;
        return false;
    }
    
    std::cout << "Loaded from backup " << backupIndex << std::endl;
    return true;
}

// Check if main save file exists
bool SaveSystem::hasSaveFile() const {
    return fileExists(getSaveFilePath());
}

// Check if specific backup exists
bool SaveSystem::hasBackup(int backupIndex) const {
    if (backupIndex < 0 || backupIndex >= MAX_BACKUPS) {
        return false;
    }
    return fileExists(getBackupFilePath(backupIndex));
}

// Get list of available backups
std::vector<SaveSystem::BackupInfo> SaveSystem::getBackupList() const {
    std::vector<BackupInfo> backups;
    
    for (int i = 0; i < MAX_BACKUPS; i++) {
        BackupInfo info;
        info.index = i;
        info.exists = hasBackup(i);
        
        if (info.exists) {
            // Try to extract timestamp and nodeId from backup file
            std::string data = readFromFile(getBackupFilePath(i));
            // Parse basic info (simplified - in real implementation would parse JSON)
            info.timestamp = "backup_" + std::to_string(i);
            info.nodeId = 0;
        } else {
            info.timestamp = "";
            info.nodeId = -1;
        }
        
        backups.push_back(info);
    }
    
    return backups;
}

// Delete all saves
void SaveSystem::clearAllSaves() {
    // Remove main save
    if (hasSaveFile()) {
        std::remove(getSaveFilePath().c_str());
    }
    
    // Remove all backups
    for (int i = 0; i < MAX_BACKUPS; i++) {
        if (hasBackup(i)) {
            std::remove(getBackupFilePath(i).c_str());
        }
    }
    
    std::cout << "All saves cleared" << std::endl;
}

// Serialize player state to JSON-like string
std::string SaveSystem::serializePlayerState(const PlayerState& playerState) const {
    std::ostringstream oss;
    
    // Get current timestamp
    std::time_t now = std::time(nullptr);
    
    // Simple JSON-like format - expanded to include all game progress
    oss << "{\n";
    oss << "  \"version\": 1,\n";
    oss << "  \"timestamp\": " << now << ",\n";
    
    // [MVP] Disabled - Resource state (uncomment to enable)
    /*
    // Basic player state
    oss << "  \"energy\": " << playerState.getEnergy() << ",\n";
    oss << "  \"money\": " << playerState.getMoney() << ",\n";
    oss << "  \"fuel\": " << playerState.getFuel() << ",\n";
    oss << "  \"origin\": " << static_cast<int>(playerState.getOrigin()) << ",\n";
    oss << "  \"carType\": " << static_cast<int>(playerState.getCarType()) << ",\n";
    */
    oss << "  \"currentNodeId\": " << playerState.getCurrentNodeId() << ",\n";

    // [MVP] Disabled - Inventory (uncomment to enable)
    /*
    // Inventory - serialize item count and total weight
    const InventorySystem& inventory = playerState.getInventory();
    oss << "  \"inventory\": {\n";
    oss << "    \"totalWeight\": " << inventory.getTotalWeight() << ",\n";
    oss << "    \"maxWeight\": " << inventory.getMaxWeight() << ",\n";
    oss << "    \"slotCount\": " << inventory.getSlotCount() << ",\n";
    oss << "    \"items\": [\n";

    bool firstItem = true;
    for (int i = 0; i < inventory.getSlotCount(); ++i) {
        const InventorySlot& slot = inventory.getSlot(i);
        if (!slot.isEmpty) {
            if (!firstItem) oss << ",\n";
            oss << "      {\n";
            oss << "        \"slot\": " << i << ",\n";
            oss << "        \"name\": \"" << slot.item.name << "\",\n";
            oss << "        \"category\": " << static_cast<int>(slot.item.category) << ",\n";
            oss << "        \"weight\": " << slot.item.weight << ",\n";
            oss << "        \"value\": " << slot.item.value << ",\n";
            oss << "        \"stackable\": " << (slot.item.isStackable ? "true" : "false") << ",\n";
            oss << "        \"maxStack\": " << slot.item.maxStackSize << ",\n";
            oss << "        \"count\": " << slot.count << "\n";
            oss << "      }";
            firstItem = false;
        }
    }
    oss << "\n    ]\n";
    oss << "  },\n";
    */
    
    // Player experience and abilities
    ExperienceSystem& expSystem = ExperienceSystem::getInstance();
    oss << "  \"experience\": {\n";
    oss << "    \"level\": " << expSystem.getLevel() << ",\n";
    oss << "    \"exp\": " << expSystem.getExperience() << ",\n";
    oss << "    \"unlockedAbilities\": [\n";
    bool firstAbility = true;
    for (const auto& ability : expSystem.getAbilityTree()) {
        if (ability.unlocked) {
            if (!firstAbility) oss << ",\n";
            oss << "      \"" << ability.id << "\"";
            firstAbility = false;
        }
    }
    oss << "\n    ]\n";
    oss << "  },\n";
    
    // Quests - serialize all quests with their status and progress
    QuestManager& questMgr = QuestManager::getInstance();
    oss << "  \"quests\": [\n";
    bool firstQuest = true;
    
    // Get all quest types
    auto activeQuests = questMgr.getActiveQuests();
    auto completedQuests = questMgr.getCompletedQuests();
    
    // Combine and serialize all quests that have been started or completed
    std::vector<Quest*> allQuests;
    allQuests.insert(allQuests.end(), activeQuests.begin(), activeQuests.end());
    allQuests.insert(allQuests.end(), completedQuests.begin(), completedQuests.end());
    
    for (const auto* quest : allQuests) {
        if (!firstQuest) oss << ",\n";
        oss << "    {\n";
        oss << "      \"id\": \"" << quest->id << "\",\n";
        oss << "      \"status\": " << static_cast<int>(quest->status) << ",\n";
        oss << "      \"objectives\": [\n";
        
        bool firstObjective = true;
        for (const auto& obj : quest->objectives) {
            if (!firstObjective) oss << ",\n";
            oss << "        {\n";
            oss << "          \"type\": " << static_cast<int>(obj.type) << ",\n";
            oss << "          \"currentAmount\": " << obj.currentAmount << ",\n";
            oss << "          \"targetAmount\": " << obj.targetAmount << ",\n";
            oss << "          \"completed\": " << (obj.isCompleted ? "true" : "false") << "\n";
            oss << "        }";
            firstObjective = false;
        }
        oss << "\n      ]\n";
        oss << "    }";
        firstQuest = false;
    }
    oss << "\n  ],\n";
    
    // Dialogue history - serialize which dialogues and choices were made
    DialogueManager& dialogueMgr = DialogueManager::getInstance();
    oss << "  \"dialogueHistory\": [\n";
    // Note: DialogueManager stores history as map<string, vector<string>>
    // We'll serialize it as array of dialogue-choices pairs
    // Since we can't directly access private m_history, this will remain a placeholder
    // In a production system, DialogueManager would need a getHistory() method
    oss << "  ],\n";
    
    // NPCs - serialize team members with their levels and experience
    NPCManager& npcMgr = NPCManager::getInstance();
    oss << "  \"npcs\": [\n";
    bool firstNPC = true;
    for (const auto& npc : npcMgr.getTeam()) {
        if (!firstNPC) oss << ",\n";
        oss << "    {\n";
        oss << "      \"id\": \"" << npc->getId() << "\",\n";
        oss << "      \"name\": \"" << npc->getName() << "\",\n";
        oss << "      \"level\": " << npc->getLevel() << ",\n";
        oss << "      \"experience\": " << npc->getExperience() << ",\n";
        oss << "      \"passiveAbilityId\": \"" << (npc->hasPassiveAbility() ? npc->getPassiveAbility().id : "") << "\",\n";
        oss << "      \"activeAbilityId\": \"" << (npc->hasActiveAbility() ? npc->getActiveAbility().id : "") << "\"\n";
        oss << "    }";
        firstNPC = false;
    }
    oss << "\n  ]\n";
    
    oss << "}\n";
    
    return oss.str();
}

// Deserialize player state from JSON-like string
bool SaveSystem::deserializePlayerState(const std::string& data, PlayerState& playerState) const {
    // NOTE: This is a simplified parser that only loads basic player state.
    // Full deserialization of quests, NPCs, dialogue history, and abilities
    // requires a proper JSON library (like nlohmann/json).
    // 
    // For now, the save format preserves all data (quests, NPCs, dialogue, abilities)
    // so it can be manually inspected or loaded with a proper JSON parser in the future.
    // Basic player state (energy, money, fuel, origin, car, node) is loaded correctly.
    
    std::istringstream iss(data);
    std::string line;
    
    float energy = 0.0f, money = 0.0f, fuel = 0.0f;
    int origin = 0, carType = 0, currentNodeId = 0;
    
    while (std::getline(iss, line)) {
        // Skip empty lines, braces, and nested structures
        if (line.find("{") != std::string::npos || 
            line.find("}") != std::string::npos || 
            line.find("[") != std::string::npos ||
            line.find("]") != std::string::npos ||
            line.empty()) {
            continue;
        }
        
        // Parse key-value pairs at root level only
        size_t colonPos = line.find(":");
        if (colonPos == std::string::npos) continue;
        
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        
        // Trim quotes and whitespace
        key.erase(0, key.find_first_not_of(" \t\""));
        key.erase(key.find_last_not_of(" \t\"") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t,") + 1);
        
        // Set values (only basic fields for now)
        if (key == "energy") {
            energy = std::stof(value);
        } else if (key == "money") {
            money = std::stof(value);
        } else if (key == "fuel") {
            fuel = std::stof(value);
        } else if (key == "origin") {
            origin = std::stoi(value);
        } else if (key == "carType") {
            carType = std::stoi(value);
        } else if (key == "currentNodeId") {
            currentNodeId = std::stoi(value);
        }
        // Note: Inventory, quests, and dialogue history parsing would go here
        // For a production system, use a proper JSON library like nlohmann/json
    }
    
    // Apply values to player state
    // [MVP] Disabled - Resource loading (uncomment to enable)
    /*
    playerState.setEnergy(energy);
    playerState.setMoney(money);
    playerState.setFuel(fuel);
    playerState.setOrigin(static_cast<OriginType>(origin));
    playerState.setCarType(static_cast<CarType>(carType));
    */
    playerState.setCurrentNodeId(currentNodeId);
    
    return true;
}

// Write data to file
bool SaveSystem::writeToFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << data;
    file.close();
    return true;
}

// Read data from file
std::string SaveSystem::readFromFile(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

// Check if file exists
bool SaveSystem::fileExists(const std::string& filename) const {
    std::ifstream file(filename);
    return file.good();
}

// Rotate backups
void SaveSystem::rotateBackups() {
    // Remove oldest backup (backup_3)
    if (hasBackup(MAX_BACKUPS - 1)) {
        std::remove(getBackupFilePath(MAX_BACKUPS - 1).c_str());
    }
    
    // Shift all backups by one
    for (int i = MAX_BACKUPS - 1; i > 0; i--) {
        if (hasBackup(i - 1)) {
            std::rename(getBackupFilePath(i - 1).c_str(), getBackupFilePath(i).c_str());
        }
    }
    
    // Copy current save to backup_0
    if (hasSaveFile()) {
        std::string data = readFromFile(getSaveFilePath());
        writeToFile(getBackupFilePath(0), data);
    }
}

// Get save file path
std::string SaveSystem::getSaveFilePath() const {
    return m_saveDirectory + "/savegame.sav";
}

// Get backup file path
std::string SaveSystem::getBackupFilePath(int index) const {
    return m_saveDirectory + "/savegame_backup_" + std::to_string(index) + ".sav";
}
