#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <functional>

// Quest objective types
enum class QuestObjectiveType {
    COLLECT_ITEM,       // Collect specific items
    DELIVER_ITEM,       // Deliver items to a location
    TRAVEL_TO,          // Travel to a specific location
    TALK_TO_NPC,        // Talk to a specific NPC
    COMPLETE_EVENT,     // Complete a specific event
    EARN_MONEY,         // Earn a certain amount of money
    CUSTOM              // Custom objective with custom check
};

// Quest status
enum class QuestStatus {
    NOT_STARTED,
    ACTIVE,
    COMPLETED,
    FAILED
};

// Quest difficulty
enum class QuestDifficulty {
    EASY,
    MEDIUM,
    HARD,
    EXTREME
};

// Quest objective structure
struct QuestObjective {
    QuestObjectiveType type;
    std::string description;
    std::string targetId;       // Item ID, NPC ID, location ID, etc.
    int targetAmount;           // Amount needed
    int currentAmount;          // Current progress
    bool isCompleted;
    
    QuestObjective()
        : type(QuestObjectiveType::CUSTOM),
          targetAmount(1),
          currentAmount(0),
          isCompleted(false) {}
    
    QuestObjective(QuestObjectiveType t, const std::string& desc,
                  const std::string& target = "", int amount = 1)
        : type(t), description(desc), targetId(target),
          targetAmount(amount), currentAmount(0), isCompleted(false) {}
    
    // Check if objective is complete
    bool checkCompletion();
    
    // Get progress percentage (0-100)
    int getProgress() const;
};

// Quest reward structure
struct QuestReward {
    int money;
    int experience;
    std::vector<std::string> itemIds;   // Items to give as reward
    
    QuestReward()
        : money(0), experience(0) {}
    
    QuestReward(int m, int exp, const std::vector<std::string>& items = {})
        : money(m), experience(exp), itemIds(items) {}
};

// Quest structure
struct Quest {
    std::string id;
    std::string title;
    std::string description;
    std::string questGiverNpcId;        // NPC who gives the quest
    std::string completionNpcId;        // NPC to complete quest (can be same)
    QuestDifficulty difficulty;
    QuestStatus status;
    std::vector<QuestObjective> objectives;
    QuestReward reward;
    bool repeatable;
    
    Quest()
        : difficulty(QuestDifficulty::MEDIUM),
          status(QuestStatus::NOT_STARTED),
          repeatable(false) {}
    
    Quest(const std::string& qid, const std::string& t,
          const std::string& desc, const std::string& giver)
        : id(qid), title(t), description(desc),
          questGiverNpcId(giver), completionNpcId(giver),
          difficulty(QuestDifficulty::MEDIUM),
          status(QuestStatus::NOT_STARTED),
          repeatable(false) {}
    
    // Check if all objectives are complete
    bool areAllObjectivesComplete() const;
    
    // Get overall progress (0-100)
    int getOverallProgress() const;
};

// Quest manager - manages all quests in the game
class QuestManager {
public:
    static QuestManager& getInstance();
    
    // Quest registration
    void registerQuest(const Quest& quest);
    void registerQuestsFromFile(const std::string& jsonPath);
    
    // Quest management
    Quest* getQuest(const std::string& questId);
    void startQuest(const std::string& questId);
    void completeQuest(const std::string& questId);
    void failQuest(const std::string& questId);
    void resetQuest(const std::string& questId);
    
    // Quest queries
    std::vector<Quest*> getActiveQuests();
    std::vector<Quest*> getCompletedQuests();
    std::vector<Quest*> getAvailableQuests();  // Quests from NPCs
    std::vector<Quest*> getQuestsFromNPC(const std::string& npcId);
    
    // Objective updates
    void updateObjective(const std::string& questId, int objectiveIndex, int progress);
    void incrementObjective(const std::string& questId, int objectiveIndex, int amount = 1);
    void completeObjective(const std::string& questId, int objectiveIndex);
    
    // Convenience methods for common objective types
    void notifyItemCollected(const std::string& itemId, int amount = 1);
    void notifyItemDelivered(const std::string& itemId, const std::string& location, int amount = 1);
    void notifyLocationVisited(const std::string& locationId);
    void notifyNPCTalkedTo(const std::string& npcId);
    void notifyEventCompleted(const std::string& eventId);
    void notifyMoneyEarned(int amount);
    
    // Reward handling
    void grantReward(const std::string& questId);
    
    // Initialize default quests
    void initializeDefaultQuests();
    
    // Clear all quests
    void clear();
    
private:
    QuestManager() = default;
    QuestManager(const QuestManager&) = delete;
    QuestManager& operator=(const QuestManager&) = delete;
    
    std::map<std::string, Quest> m_quests;
    
    // Helper to check and auto-complete objectives
    void checkObjectiveCompletion(Quest& quest);
    void checkQuestCompletion(Quest& quest);
};

#endif // QUEST_MANAGER_H
