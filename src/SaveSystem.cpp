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


SaveSystem::SaveSystem()
    : m_saveDirectory("saves") {

    struct stat info;
    if (stat(m_saveDirectory.c_str(), &info) != 0) {

        mkdir(m_saveDirectory.c_str(), 0755);
    }
}


bool SaveSystem::saveGame(const PlayerState& playerState) {

    rotateBackups();


    std::string data = serializePlayerState(playerState);
    if (data.empty()) {
        std::cerr << "Failed to serialize player state" << std::endl;
        return false;
    }


    if (!writeToFile(getSaveFilePath(), data)) {
        std::cerr << "Failed to write save file" << std::endl;
        return false;
    }

    std::cout << "Game saved successfully" << std::endl;
    return true;
}


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


bool SaveSystem::hasSaveFile() const {
    return fileExists(getSaveFilePath());
}


bool SaveSystem::hasBackup(int backupIndex) const {
    if (backupIndex < 0 || backupIndex >= MAX_BACKUPS) {
        return false;
    }
    return fileExists(getBackupFilePath(backupIndex));
}


std::vector<SaveSystem::BackupInfo> SaveSystem::getBackupList() const {
    std::vector<BackupInfo> backups;

    for (int i = 0; i < MAX_BACKUPS; i++) {
        BackupInfo info;
        info.index = i;
        info.exists = hasBackup(i);

        if (info.exists) {

            std::string data = readFromFile(getBackupFilePath(i));

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


void SaveSystem::clearAllSaves() {

    if (hasSaveFile()) {
        std::remove(getSaveFilePath().c_str());
    }


    for (int i = 0; i < MAX_BACKUPS; i++) {
        if (hasBackup(i)) {
            std::remove(getBackupFilePath(i).c_str());
        }
    }

    std::cout << "All saves cleared" << std::endl;
}


std::string SaveSystem::serializePlayerState(const PlayerState& playerState) const {
    std::ostringstream oss;


    std::time_t now = std::time(nullptr);


    oss << "{\n";
    oss << "  \"version\": 1,\n";
    oss << "  \"timestamp\": " << now << ",\n";


    oss << "  \"energy\": " << playerState.getEnergy() << ",\n";
    oss << "  \"money\": " << playerState.getMoney() << ",\n";
    oss << "  \"fuel\": " << playerState.getFuel() << ",\n";
    oss << "  \"vehicleCondition\": " << playerState.getVehicleCondition() << ",\n";
    oss << "  \"mood\": " << playerState.getMood() << ",\n";
    oss << "  \"reputation\": " << playerState.getReputation() << ",\n";
    oss << "  \"origin\": " << static_cast<int>(playerState.getOrigin()) << ",\n";
    oss << "  \"carType\": " << static_cast<int>(playerState.getCarType()) << ",\n";
    oss << "  \"currentNodeId\": " << playerState.getCurrentNodeId() << ",\n";
    oss << "  \"currentNotebookEntryId\": \"" << playerState.getCurrentNotebookEntryId() << "\",\n";


































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


    QuestManager& questMgr = QuestManager::getInstance();
    oss << "  \"quests\": [\n";
    bool firstQuest = true;


    auto activeQuests = questMgr.getActiveQuests();
    auto completedQuests = questMgr.getCompletedQuests();


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


    DialogueManager& dialogueMgr = DialogueManager::getInstance();
    oss << "  \"dialogueHistory\": [\n";




    oss << "  ],\n";


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
    oss << "\n  ],\n";


    oss << "  \"principles\": [\n";
    bool firstPrinciple = true;
    for (const auto& principle : playerState.getPrinciples()) {
        if (!firstPrinciple) oss << ",\n";
        oss << "    \"" << principle << "\"";
        firstPrinciple = false;
    }
    oss << "\n  ],\n";


    oss << "  \"traits\": [\n";
    bool firstTrait = true;
    for (const auto& trait : playerState.getTraits()) {
        if (!firstTrait) oss << ",\n";
        oss << "    \"" << trait << "\"";
        firstTrait = false;
    }
    oss << "\n  ],\n";


    oss << "  \"storyItems\": [\n";
    bool firstItem = true;
    for (const auto& item : playerState.getStoryItems()) {
        if (!firstItem) oss << ",\n";
        oss << "    \"" << item << "\"";
        firstItem = false;
    }
    oss << "\n  ]\n";

    oss << "}\n";

    return oss.str();
}


bool SaveSystem::deserializePlayerState(const std::string& data, PlayerState& playerState) const {








    std::istringstream iss(data);
    std::string line;

    float energy = 0.0f, money = 0.0f, fuel = 0.0f;
    float vehicleCondition = 100.0f, mood = 70.0f;
    int origin = 0, carType = 0, currentNodeId = 0;
    int reputation = 0;
    std::string currentNotebookEntryId = "tutorial_start";
    std::vector<std::string> principles, traits, storyItems;
    std::string currentArray = "";

    while (std::getline(iss, line)) {

        if (line.find("{") != std::string::npos ||
            line.find("}") != std::string::npos ||
            line.empty()) {
            continue;
        }


        if (line.find("\"principles\"") != std::string::npos) {
            currentArray = "principles";
            continue;
        } else if (line.find("\"traits\"") != std::string::npos) {
            currentArray = "traits";
            continue;
        } else if (line.find("\"storyItems\"") != std::string::npos) {
            currentArray = "storyItems";
            continue;
        }


        if (line.find("]") != std::string::npos) {
            currentArray = "";
            continue;
        }


        if (!currentArray.empty()) {

            size_t firstQuote = line.find("\"");
            if (firstQuote != std::string::npos) {
                size_t secondQuote = line.find("\"", firstQuote + 1);
                if (secondQuote != std::string::npos) {
                    std::string value = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
                    if (currentArray == "principles") {
                        principles.push_back(value);
                    } else if (currentArray == "traits") {
                        traits.push_back(value);
                    } else if (currentArray == "storyItems") {
                        storyItems.push_back(value);
                    }
                }
            }
            continue;
        }


        if (line.find("[") != std::string::npos) {
            continue;
        }


        size_t colonPos = line.find(":");
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);


        key.erase(0, key.find_first_not_of(" \t\""));
        key.erase(key.find_last_not_of(" \t\"") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t,") + 1);


        if (key == "energy") {
            energy = std::stof(value);
        } else if (key == "money") {
            money = std::stof(value);
        } else if (key == "fuel") {
            fuel = std::stof(value);
        } else if (key == "vehicleCondition") {
            vehicleCondition = std::stof(value);
        } else if (key == "mood") {
            mood = std::stof(value);
        } else if (key == "reputation") {
            reputation = std::stoi(value);
        } else if (key == "origin") {
            origin = std::stoi(value);
        } else if (key == "carType") {
            carType = std::stoi(value);
        } else if (key == "currentNodeId") {
            currentNodeId = std::stoi(value);
        } else if (key == "currentNotebookEntryId") {

            value.erase(0, value.find_first_not_of("\""));
            value.erase(value.find_last_not_of("\"") + 1);
            currentNotebookEntryId = value;
        }


    }


    playerState.setEnergy(energy);
    playerState.setMoney(money);
    playerState.setFuel(fuel);
    playerState.setVehicleCondition(vehicleCondition);
    playerState.setMood(mood);
    playerState.setReputation(reputation);
    playerState.setOrigin(static_cast<OriginType>(origin));
    playerState.setCarType(static_cast<CarType>(carType));
    playerState.setCurrentNodeId(currentNodeId);
    playerState.setCurrentNotebookEntryId(currentNotebookEntryId);


    for (const auto& principle : principles) {
        playerState.addPrinciple(principle);
    }
    for (const auto& trait : traits) {
        playerState.addTrait(trait);
    }
    for (const auto& item : storyItems) {
        playerState.addStoryItem(item);
    }

    return true;
}


bool SaveSystem::writeToFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << data;
    file.close();
    return true;
}


std::string SaveSystem::readFromFile(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}


bool SaveSystem::fileExists(const std::string& filename) const {
    std::ifstream file(filename);
    return file.good();
}


void SaveSystem::rotateBackups() {

    if (hasBackup(MAX_BACKUPS - 1)) {
        std::remove(getBackupFilePath(MAX_BACKUPS - 1).c_str());
    }


    for (int i = MAX_BACKUPS - 1; i > 0; i--) {
        if (hasBackup(i - 1)) {
            std::rename(getBackupFilePath(i - 1).c_str(), getBackupFilePath(i).c_str());
        }
    }


    if (hasSaveFile()) {
        std::string data = readFromFile(getSaveFilePath());
        writeToFile(getBackupFilePath(0), data);
    }
}


std::string SaveSystem::getSaveFilePath() const {
    return m_saveDirectory + "/savegame.sav";
}


std::string SaveSystem::getBackupFilePath(int index) const {
    return m_saveDirectory + "/savegame_backup_" + std::to_string(index) + ".sav";
}
