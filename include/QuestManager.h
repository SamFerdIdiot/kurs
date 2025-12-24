#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <functional>


enum class QuestObjectiveType {
    COLLECT_ITEM,
    DELIVER_ITEM,
    TRAVEL_TO,
    TALK_TO_NPC,
    COMPLETE_EVENT,
    EARN_MONEY,
    CUSTOM
};


enum class QuestStatus {
    NOT_STARTED,
    ACTIVE,
    COMPLETED,
    FAILED
};


enum class QuestDifficulty {
    EASY,
    MEDIUM,
    HARD,
    EXTREME
};


struct QuestObjective {
    QuestObjectiveType type;
    std::string description;
    std::string targetId;
    int targetAmount;
    int currentAmount;
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


    bool checkCompletion();


    int getProgress() const;
};


struct QuestReward {
    int money;
    int experience;
    std::vector<std::string> itemIds;

    QuestReward()
        : money(0), experience(0) {}

    QuestReward(int m, int exp, const std::vector<std::string>& items = {})
        : money(m), experience(exp), itemIds(items) {}
};


struct Quest {
    std::string id;
    std::string title;
    std::string description;
    std::string questGiverNpcId;
    std::string completionNpcId;
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


    bool areAllObjectivesComplete() const;


    int getOverallProgress() const;
};


class QuestManager {
public:
    static QuestManager& getInstance();


    void registerQuest(const Quest& quest);
    void registerQuestsFromFile(const std::string& jsonPath);


    Quest* getQuest(const std::string& questId);
    void startQuest(const std::string& questId);
    void completeQuest(const std::string& questId);
    void failQuest(const std::string& questId);
    void resetQuest(const std::string& questId);


    std::vector<Quest*> getActiveQuests();
    std::vector<Quest*> getCompletedQuests();
    std::vector<Quest*> getAvailableQuests();
    std::vector<Quest*> getQuestsFromNPC(const std::string& npcId);


    void updateObjective(const std::string& questId, int objectiveIndex, int progress);
    void incrementObjective(const std::string& questId, int objectiveIndex, int amount = 1);
    void completeObjective(const std::string& questId, int objectiveIndex);


    void notifyItemCollected(const std::string& itemId, int amount = 1);
    void notifyItemDelivered(const std::string& itemId, const std::string& location, int amount = 1);
    void notifyLocationVisited(const std::string& locationId);
    void notifyNPCTalkedTo(const std::string& npcId);
    void notifyEventCompleted(const std::string& eventId);
    void notifyMoneyEarned(int amount);


    void grantReward(const std::string& questId);


    void initializeDefaultQuests();


    void clear();

private:
    QuestManager() = default;
    QuestManager(const QuestManager&) = delete;
    QuestManager& operator=(const QuestManager&) = delete;

    std::map<std::string, Quest> m_quests;


    void checkObjectiveCompletion(Quest& quest);
    void checkQuestCompletion(Quest& quest);
};

#endif
