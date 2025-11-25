#include "QuestManager.h"
#include <algorithm>
#include <iostream>

// QuestObjective methods
bool QuestObjective::checkCompletion() {
    if (currentAmount >= targetAmount) {
        isCompleted = true;
    }
    return isCompleted;
}

int QuestObjective::getProgress() const {
    if (targetAmount <= 0) return 100;
    return static_cast<int>((static_cast<float>(currentAmount) / targetAmount) * 100);
}

// Quest methods
bool Quest::areAllObjectivesComplete() const {
    for (const auto& obj : objectives) {
        if (!obj.isCompleted) {
            return false;
        }
    }
    return true;
}

int Quest::getOverallProgress() const {
    if (objectives.empty()) return 100;
    
    int totalProgress = 0;
    for (const auto& obj : objectives) {
        totalProgress += obj.getProgress();
    }
    return totalProgress / static_cast<int>(objectives.size());
}

// QuestManager singleton
QuestManager& QuestManager::getInstance() {
    static QuestManager instance;
    return instance;
}

// Quest registration
void QuestManager::registerQuest(const Quest& quest) {
    m_quests[quest.id] = quest;
    std::cout << "Quest registered: " << quest.title << " (ID: " << quest.id << ")" << std::endl;
}

void QuestManager::registerQuestsFromFile(const std::string& jsonPath) {
    // TODO: Implement JSON parsing for quests
    std::cerr << "Quest loading from JSON not yet implemented: " << jsonPath << std::endl;
}

// Quest management
Quest* QuestManager::getQuest(const std::string& questId) {
    auto it = m_quests.find(questId);
    if (it != m_quests.end()) {
        return &it->second;
    }
    return nullptr;
}

void QuestManager::startQuest(const std::string& questId) {
    Quest* quest = getQuest(questId);
    if (quest && quest->status == QuestStatus::NOT_STARTED) {
        quest->status = QuestStatus::ACTIVE;
        std::cout << "Quest started: " << quest->title << std::endl;
    }
}

void QuestManager::completeQuest(const std::string& questId) {
    Quest* quest = getQuest(questId);
    if (quest && quest->status == QuestStatus::ACTIVE) {
        if (quest->areAllObjectivesComplete()) {
            quest->status = QuestStatus::COMPLETED;
            grantReward(questId);
            std::cout << "Quest completed: " << quest->title << std::endl;
        } else {
            std::cerr << "Cannot complete quest - objectives not finished: " << quest->title << std::endl;
        }
    }
}

void QuestManager::failQuest(const std::string& questId) {
    Quest* quest = getQuest(questId);
    if (quest && quest->status == QuestStatus::ACTIVE) {
        quest->status = QuestStatus::FAILED;
        std::cout << "Quest failed: " << quest->title << std::endl;
    }
}

void QuestManager::resetQuest(const std::string& questId) {
    Quest* quest = getQuest(questId);
    if (quest && quest->repeatable) {
        quest->status = QuestStatus::NOT_STARTED;
        for (auto& obj : quest->objectives) {
            obj.currentAmount = 0;
            obj.isCompleted = false;
        }
        std::cout << "Quest reset: " << quest->title << std::endl;
    }
}

// Quest queries
std::vector<Quest*> QuestManager::getActiveQuests() {
    std::vector<Quest*> activeQuests;
    for (auto& pair : m_quests) {
        if (pair.second.status == QuestStatus::ACTIVE) {
            activeQuests.push_back(&pair.second);
        }
    }
    return activeQuests;
}

std::vector<Quest*> QuestManager::getCompletedQuests() {
    std::vector<Quest*> completedQuests;
    for (auto& pair : m_quests) {
        if (pair.second.status == QuestStatus::COMPLETED) {
            completedQuests.push_back(&pair.second);
        }
    }
    return completedQuests;
}

std::vector<Quest*> QuestManager::getAvailableQuests() {
    std::vector<Quest*> availableQuests;
    for (auto& pair : m_quests) {
        if (pair.second.status == QuestStatus::NOT_STARTED) {
            availableQuests.push_back(&pair.second);
        }
    }
    return availableQuests;
}

std::vector<Quest*> QuestManager::getQuestsFromNPC(const std::string& npcId) {
    std::vector<Quest*> npcQuests;
    for (auto& pair : m_quests) {
        if (pair.second.questGiverNpcId == npcId) {
            npcQuests.push_back(&pair.second);
        }
    }
    return npcQuests;
}

// Objective updates
void QuestManager::updateObjective(const std::string& questId, int objectiveIndex, int progress) {
    Quest* quest = getQuest(questId);
    if (quest && quest->status == QuestStatus::ACTIVE) {
        if (objectiveIndex >= 0 && objectiveIndex < static_cast<int>(quest->objectives.size())) {
            quest->objectives[objectiveIndex].currentAmount = progress;
            checkObjectiveCompletion(*quest);
            checkQuestCompletion(*quest);
        }
    }
}

void QuestManager::incrementObjective(const std::string& questId, int objectiveIndex, int amount) {
    Quest* quest = getQuest(questId);
    if (quest && quest->status == QuestStatus::ACTIVE) {
        if (objectiveIndex >= 0 && objectiveIndex < static_cast<int>(quest->objectives.size())) {
            quest->objectives[objectiveIndex].currentAmount += amount;
            checkObjectiveCompletion(*quest);
            checkQuestCompletion(*quest);
        }
    }
}

void QuestManager::completeObjective(const std::string& questId, int objectiveIndex) {
    Quest* quest = getQuest(questId);
    if (quest && quest->status == QuestStatus::ACTIVE) {
        if (objectiveIndex >= 0 && objectiveIndex < static_cast<int>(quest->objectives.size())) {
            quest->objectives[objectiveIndex].isCompleted = true;
            quest->objectives[objectiveIndex].currentAmount = quest->objectives[objectiveIndex].targetAmount;
            checkQuestCompletion(*quest);
        }
    }
}

// Convenience methods for common objective types
void QuestManager::notifyItemCollected(const std::string& itemId, int amount) {
    for (auto& pair : m_quests) {
        Quest& quest = pair.second;
        if (quest.status == QuestStatus::ACTIVE) {
            for (size_t i = 0; i < quest.objectives.size(); ++i) {
                auto& obj = quest.objectives[i];
                if (obj.type == QuestObjectiveType::COLLECT_ITEM && obj.targetId == itemId) {
                    incrementObjective(quest.id, static_cast<int>(i), amount);
                }
            }
        }
    }
}

void QuestManager::notifyItemDelivered(const std::string& itemId, const std::string& location, int amount) {
    for (auto& pair : m_quests) {
        Quest& quest = pair.second;
        if (quest.status == QuestStatus::ACTIVE) {
            for (size_t i = 0; i < quest.objectives.size(); ++i) {
                auto& obj = quest.objectives[i];
                if (obj.type == QuestObjectiveType::DELIVER_ITEM && 
                    obj.targetId == itemId) {
                    incrementObjective(quest.id, static_cast<int>(i), amount);
                }
            }
        }
    }
}

void QuestManager::notifyLocationVisited(const std::string& locationId) {
    for (auto& pair : m_quests) {
        Quest& quest = pair.second;
        if (quest.status == QuestStatus::ACTIVE) {
            for (size_t i = 0; i < quest.objectives.size(); ++i) {
                auto& obj = quest.objectives[i];
                if (obj.type == QuestObjectiveType::TRAVEL_TO && obj.targetId == locationId) {
                    completeObjective(quest.id, static_cast<int>(i));
                }
            }
        }
    }
}

void QuestManager::notifyNPCTalkedTo(const std::string& npcId) {
    for (auto& pair : m_quests) {
        Quest& quest = pair.second;
        if (quest.status == QuestStatus::ACTIVE) {
            for (size_t i = 0; i < quest.objectives.size(); ++i) {
                auto& obj = quest.objectives[i];
                if (obj.type == QuestObjectiveType::TALK_TO_NPC && obj.targetId == npcId) {
                    completeObjective(quest.id, static_cast<int>(i));
                }
            }
        }
    }
}

void QuestManager::notifyEventCompleted(const std::string& eventId) {
    for (auto& pair : m_quests) {
        Quest& quest = pair.second;
        if (quest.status == QuestStatus::ACTIVE) {
            for (size_t i = 0; i < quest.objectives.size(); ++i) {
                auto& obj = quest.objectives[i];
                if (obj.type == QuestObjectiveType::COMPLETE_EVENT && obj.targetId == eventId) {
                    completeObjective(quest.id, static_cast<int>(i));
                }
            }
        }
    }
}

void QuestManager::notifyMoneyEarned(int amount) {
    for (auto& pair : m_quests) {
        Quest& quest = pair.second;
        if (quest.status == QuestStatus::ACTIVE) {
            for (size_t i = 0; i < quest.objectives.size(); ++i) {
                auto& obj = quest.objectives[i];
                if (obj.type == QuestObjectiveType::EARN_MONEY) {
                    incrementObjective(quest.id, static_cast<int>(i), amount);
                }
            }
        }
    }
}

// Reward handling
void QuestManager::grantReward(const std::string& questId) {
    Quest* quest = getQuest(questId);
    if (quest) {
        std::cout << "Quest reward granted for: " << quest->title << std::endl;
        std::cout << "  Money: " << quest->reward.money << std::endl;
        std::cout << "  Experience: " << quest->reward.experience << std::endl;
        
        // TODO: Actually grant the reward to player
        // This would require PlayerState reference
        // For now, just log it
        
        if (!quest->reward.itemIds.empty()) {
            std::cout << "  Items: ";
            for (const auto& itemId : quest->reward.itemIds) {
                std::cout << itemId << " ";
            }
            std::cout << std::endl;
        }
    }
}

// Initialize default quests
void QuestManager::initializeDefaultQuests() {
    // Example quest 1: Deliver package
    Quest quest1("quest_001", "Delivery to Vladimir", 
                "Take this package to Vladimir and deliver it to the local shop.",
                "npc_trader_moscow");
    quest1.completionNpcId = "npc_shopkeeper_vladimir";
    quest1.difficulty = QuestDifficulty::EASY;
    quest1.reward = QuestReward(500, 100);
    
    QuestObjective obj1(QuestObjectiveType::TRAVEL_TO, 
                       "Travel to Vladimir", "loc_vladimir", 1);
    QuestObjective obj2(QuestObjectiveType::DELIVER_ITEM,
                       "Deliver package", "item_package", 1);
    
    quest1.objectives.push_back(obj1);
    quest1.objectives.push_back(obj2);
    
    registerQuest(quest1);
    
    // Example quest 2: Make money
    Quest quest2("quest_002", "Earn Your Keep",
                "Earn 1000 rubles to prove you can survive on the road.",
                "npc_mechanic_moscow");
    quest2.difficulty = QuestDifficulty::MEDIUM;
    quest2.reward = QuestReward(200, 150);
    quest2.repeatable = true;
    
    QuestObjective obj3(QuestObjectiveType::EARN_MONEY,
                       "Earn 1000 rubles", "", 1000);
    quest2.objectives.push_back(obj3);
    
    registerQuest(quest2);
    
    // Example quest 3: Talk to NPCs
    Quest quest3("quest_003", "Meeting the Locals",
                "Meet and talk to the mechanic in the next town.",
                "npc_guide_moscow");
    quest3.difficulty = QuestDifficulty::EASY;
    quest3.reward = QuestReward(100, 50);
    
    QuestObjective obj4(QuestObjectiveType::TALK_TO_NPC,
                       "Talk to the mechanic", "npc_mechanic_vladimir", 1);
    quest3.objectives.push_back(obj4);
    
    registerQuest(quest3);
    
    // Quest 4: Fuel Crisis - Collect fuel
    Quest quest4("quest_004", "Топливный кризис / Fuel Crisis",
                "Запасись топливом на случай долгого пути. Собери достаточно бензина.\n"
                "Stock up on fuel for the long road ahead. Collect enough gasoline.",
                "npc_gas_attendant");
    quest4.difficulty = QuestDifficulty::EASY;
    quest4.reward = QuestReward(300, 75);
    quest4.repeatable = true;
    
    QuestObjective obj5(QuestObjectiveType::COLLECT_ITEM,
                       "Собрать 50L топлива / Collect 50L fuel", "item_fuel", 50);
    quest4.objectives.push_back(obj5);
    
    registerQuest(quest4);
    
    // Quest 5: Speed Demon - Travel distance
    Quest quest5("quest_005", "Скоростной дьявол / Speed Demon",
                "Покажи свои навыки вождения. Проедь 500 километров.\n"
                "Show your driving skills. Travel 500 kilometers.",
                "npc_racer_moscow");
    quest5.difficulty = QuestDifficulty::MEDIUM;
    quest5.reward = QuestReward(800, 200);
    
    QuestObjective obj6(QuestObjectiveType::TRAVEL_TO,
                       "Проехать 500 км / Travel 500 km", "distance_500km", 1);
    quest5.objectives.push_back(obj6);
    
    registerQuest(quest5);
    
    // Quest 6: Social Butterfly - Talk to multiple NPCs
    Quest quest6("quest_006", "Социальная бабочка / Social Butterfly",
                "Познакомься с людьми на дороге. Поговори с 5 разными людьми.\n"
                "Meet people on the road. Talk to 5 different people.",
                "npc_journalist");
    quest6.difficulty = QuestDifficulty::EASY;
    quest6.reward = QuestReward(400, 120);
    
    QuestObjective obj7(QuestObjectiveType::TALK_TO_NPC,
                       "Поговорить с попутчиком 1 / Talk to traveler 1", "npc_traveler_1", 1);
    QuestObjective obj8(QuestObjectiveType::TALK_TO_NPC,
                       "Поговорить с попутчиком 2 / Talk to traveler 2", "npc_traveler_2", 1);
    QuestObjective obj9(QuestObjectiveType::TALK_TO_NPC,
                       "Поговорить с попутчиком 3 / Talk to traveler 3", "npc_traveler_3", 1);
    QuestObjective obj10(QuestObjectiveType::TALK_TO_NPC,
                        "Поговорить с попутчиком 4 / Talk to traveler 4", "npc_traveler_4", 1);
    QuestObjective obj11(QuestObjectiveType::TALK_TO_NPC,
                        "Поговорить с попутчиком 5 / Talk to traveler 5", "npc_traveler_5", 1);
    quest6.objectives.push_back(obj7);
    quest6.objectives.push_back(obj8);
    quest6.objectives.push_back(obj9);
    quest6.objectives.push_back(obj10);
    quest6.objectives.push_back(obj11);
    
    registerQuest(quest6);
    
    // Quest 7: Roadside Adventures - Complete events
    Quest quest7("quest_007", "Приключения на дороге / Roadside Adventures",
                "Переживи события на дороге. Успешно пройди 3 случайных события.\n"
                "Experience the road. Successfully complete 3 random events.",
                "npc_storyteller");
    quest7.difficulty = QuestDifficulty::MEDIUM;
    quest7.reward = QuestReward(600, 180);
    
    QuestObjective obj12(QuestObjectiveType::COMPLETE_EVENT,
                        "Пройти событие 1 / Complete event 1", "any_event", 3);
    quest7.objectives.push_back(obj12);
    
    registerQuest(quest7);
    
    // Quest 8: Entrepreneur - Earn big money
    Quest quest8("quest_008", "Предприниматель / Entrepreneur",
                "Докажи, что ты можешь заработать. Собери 5000 рублей.\n"
                "Prove you can make money. Earn 5000 rubles.",
                "npc_businessman");
    quest8.difficulty = QuestDifficulty::HARD;
    quest8.reward = QuestReward(1000, 300);
    
    QuestObjective obj13(QuestObjectiveType::EARN_MONEY,
                        "Заработать 5000 рублей / Earn 5000 rubles", "", 5000);
    quest8.objectives.push_back(obj13);
    
    registerQuest(quest8);
    
    // Quest 9: Collector - Gather supplies
    Quest quest9("quest_009", "Коллекционер / Collector",
                "Собери припасы для долгого путешествия.\n"
                "Gather supplies for the long journey.",
                "npc_merchant");
    quest9.difficulty = QuestDifficulty::MEDIUM;
    quest9.reward = QuestReward(700, 190);
    
    QuestObjective obj14(QuestObjectiveType::COLLECT_ITEM,
                        "Собрать еду / Collect food", "item_food", 10);
    QuestObjective obj15(QuestObjectiveType::COLLECT_ITEM,
                        "Собрать воду / Collect water", "item_water", 10);
    QuestObjective obj16(QuestObjectiveType::COLLECT_ITEM,
                        "Собрать запчасти / Collect spare parts", "item_spare_parts", 5);
    quest9.objectives.push_back(obj14);
    quest9.objectives.push_back(obj15);
    quest9.objectives.push_back(obj16);
    
    registerQuest(quest9);
    
    // Quest 10: Long Haul - Multi-city journey
    Quest quest10("quest_010", "Дальнобойщик / Long Haul",
                 "Совершите путешествие через несколько городов.\n"
                 "Make a journey through multiple cities.",
                 "npc_truck_driver");
    quest10.difficulty = QuestDifficulty::HARD;
    quest10.reward = QuestReward(1200, 350);
    
    QuestObjective obj17(QuestObjectiveType::TRAVEL_TO,
                        "Посетить Москву / Visit Moscow", "loc_moscow", 1);
    QuestObjective obj18(QuestObjectiveType::TRAVEL_TO,
                        "Посетить Владимир / Visit Vladimir", "loc_vladimir", 1);
    QuestObjective obj19(QuestObjectiveType::TRAVEL_TO,
                        "Посетить Нижний Новгород / Visit Nizhny Novgorod", "loc_nizhny", 1);
    QuestObjective obj20(QuestObjectiveType::DELIVER_ITEM,
                        "Доставить груз / Deliver cargo", "item_cargo", 1);
    quest10.objectives.push_back(obj17);
    quest10.objectives.push_back(obj18);
    quest10.objectives.push_back(obj19);
    quest10.objectives.push_back(obj20);
    
    registerQuest(quest10);
    
    std::cout << "Initialized " << m_quests.size() << " default quests" << std::endl;
}

// Clear all quests
void QuestManager::clear() {
    m_quests.clear();
}

// Helper methods
void QuestManager::checkObjectiveCompletion(Quest& quest) {
    for (auto& obj : quest.objectives) {
        obj.checkCompletion();
    }
}

void QuestManager::checkQuestCompletion(Quest& quest) {
    if (quest.areAllObjectivesComplete()) {
        std::cout << "All objectives complete for quest: " << quest.title << std::endl;
        std::cout << "Return to " << quest.completionNpcId << " to complete the quest." << std::endl;
    }
}
