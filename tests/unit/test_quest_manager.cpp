// Unit tests for QuestManager
#include <gtest/gtest.h>
#include "QuestManager.h"

// Test fixture for QuestManager tests
class QuestManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clear any existing quests before each test
        QuestManager::getInstance().clear();
    }
    
    void TearDown() override {
        // Clean up after each test
        QuestManager::getInstance().clear();
    }
};

// Test: QuestManager singleton
TEST_F(QuestManagerTest, Singleton) {
    QuestManager& manager1 = QuestManager::getInstance();
    QuestManager& manager2 = QuestManager::getInstance();
    
    EXPECT_EQ(&manager1, &manager2);
}

// Test: Register and retrieve quest
TEST_F(QuestManagerTest, RegisterQuest) {
    Quest quest("test_quest", "Test Quest", "This is a test quest", "npc_001");
    
    QuestManager::getInstance().registerQuest(quest);
    
    Quest* retrieved = QuestManager::getInstance().getQuest("test_quest");
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->id, "test_quest");
    EXPECT_EQ(retrieved->title, "Test Quest");
}

// Test: Start quest
TEST_F(QuestManagerTest, StartQuest) {
    Quest quest("test_quest", "Test Quest", "Description", "npc_001");
    QuestManager::getInstance().registerQuest(quest);
    
    QuestManager::getInstance().startQuest("test_quest");
    
    Quest* retrieved = QuestManager::getInstance().getQuest("test_quest");
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->status, QuestStatus::ACTIVE);
}

// Test: Quest objectives
TEST_F(QuestManagerTest, QuestObjectives) {
    Quest quest("test_quest", "Test Quest", "Description", "npc_001");
    
    QuestObjective obj1(QuestObjectiveType::COLLECT_ITEM, "Collect apples", "item_apple", 5);
    quest.objectives.push_back(obj1);
    
    QuestManager::getInstance().registerQuest(quest);
    QuestManager::getInstance().startQuest("test_quest");
    
    // Update objective
    QuestManager::getInstance().incrementObjective("test_quest", 0, 3);
    
    Quest* retrieved = QuestManager::getInstance().getQuest("test_quest");
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->objectives[0].currentAmount, 3);
    EXPECT_FALSE(retrieved->objectives[0].isCompleted);
}

// Test: Complete objective
TEST_F(QuestManagerTest, CompleteObjective) {
    Quest quest("test_quest", "Test Quest", "Description", "npc_001");
    
    QuestObjective obj1(QuestObjectiveType::TRAVEL_TO, "Go to Moscow", "moscow", 1);
    quest.objectives.push_back(obj1);
    
    QuestManager::getInstance().registerQuest(quest);
    QuestManager::getInstance().startQuest("test_quest");
    
    // Complete objective
    QuestManager::getInstance().completeObjective("test_quest", 0);
    
    Quest* retrieved = QuestManager::getInstance().getQuest("test_quest");
    ASSERT_NE(retrieved, nullptr);
    EXPECT_TRUE(retrieved->objectives[0].isCompleted);
    EXPECT_TRUE(retrieved->areAllObjectivesComplete());
}

// Test: Complete quest
TEST_F(QuestManagerTest, CompleteQuest) {
    Quest quest("test_quest", "Test Quest", "Description", "npc_001");
    quest.reward = QuestReward(1000, 100);
    
    QuestObjective obj1(QuestObjectiveType::TALK_TO_NPC, "Talk to trader", "npc_trader", 1);
    quest.objectives.push_back(obj1);
    
    QuestManager::getInstance().registerQuest(quest);
    QuestManager::getInstance().startQuest("test_quest");
    QuestManager::getInstance().completeObjective("test_quest", 0);
    
    // Complete the quest
    QuestManager::getInstance().completeQuest("test_quest");
    
    Quest* retrieved = QuestManager::getInstance().getQuest("test_quest");
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->status, QuestStatus::COMPLETED);
}

// Test: Get active quests
TEST_F(QuestManagerTest, GetActiveQuests) {
    Quest quest1("quest_1", "Quest 1", "Description 1", "npc_001");
    Quest quest2("quest_2", "Quest 2", "Description 2", "npc_002");
    Quest quest3("quest_3", "Quest 3", "Description 3", "npc_003");
    
    QuestManager::getInstance().registerQuest(quest1);
    QuestManager::getInstance().registerQuest(quest2);
    QuestManager::getInstance().registerQuest(quest3);
    
    QuestManager::getInstance().startQuest("quest_1");
    QuestManager::getInstance().startQuest("quest_3");
    
    auto activeQuests = QuestManager::getInstance().getActiveQuests();
    EXPECT_EQ(activeQuests.size(), 2);
}

// Test: Get quests from NPC
TEST_F(QuestManagerTest, GetQuestsFromNPC) {
    Quest quest1("quest_1", "Quest 1", "Description 1", "npc_trader");
    Quest quest2("quest_2", "Quest 2", "Description 2", "npc_trader");
    Quest quest3("quest_3", "Quest 3", "Description 3", "npc_mechanic");
    
    QuestManager::getInstance().registerQuest(quest1);
    QuestManager::getInstance().registerQuest(quest2);
    QuestManager::getInstance().registerQuest(quest3);
    
    auto traderQuests = QuestManager::getInstance().getQuestsFromNPC("npc_trader");
    EXPECT_EQ(traderQuests.size(), 2);
    
    auto mechanicQuests = QuestManager::getInstance().getQuestsFromNPC("npc_mechanic");
    EXPECT_EQ(mechanicQuests.size(), 1);
}

// Test: Notify item collected
TEST_F(QuestManagerTest, NotifyItemCollected) {
    Quest quest("test_quest", "Test Quest", "Description", "npc_001");
    
    QuestObjective obj1(QuestObjectiveType::COLLECT_ITEM, "Collect apples", "item_apple", 5);
    quest.objectives.push_back(obj1);
    
    QuestManager::getInstance().registerQuest(quest);
    QuestManager::getInstance().startQuest("test_quest");
    
    // Notify items collected
    QuestManager::getInstance().notifyItemCollected("item_apple", 3);
    QuestManager::getInstance().notifyItemCollected("item_apple", 2);
    
    Quest* retrieved = QuestManager::getInstance().getQuest("test_quest");
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->objectives[0].currentAmount, 5);
    EXPECT_TRUE(retrieved->objectives[0].isCompleted);
}

// Test: Initialize default quests
TEST_F(QuestManagerTest, InitializeDefaultQuests) {
    QuestManager::getInstance().initializeDefaultQuests();
    
    auto allQuests = QuestManager::getInstance().getAvailableQuests();
    EXPECT_GE(allQuests.size(), 10); // At least 10 default quests
}

// Test: Quest progress calculation
TEST_F(QuestManagerTest, QuestProgress) {
    Quest quest("test_quest", "Test Quest", "Description", "npc_001");
    
    QuestObjective obj1(QuestObjectiveType::COLLECT_ITEM, "Collect apples", "item_apple", 10);
    QuestObjective obj2(QuestObjectiveType::EARN_MONEY, "Earn money", "", 1000);
    
    quest.objectives.push_back(obj1);
    quest.objectives.push_back(obj2);
    
    QuestManager::getInstance().registerQuest(quest);
    QuestManager::getInstance().startQuest("test_quest");
    
    // Update objectives
    QuestManager::getInstance().incrementObjective("test_quest", 0, 5); // 50% of obj1
    QuestManager::getInstance().incrementObjective("test_quest", 1, 500); // 50% of obj2
    
    Quest* retrieved = QuestManager::getInstance().getQuest("test_quest");
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->getOverallProgress(), 50); // Average of 50% and 50%
}

// Test: Fuel Crisis quest (quest_004)
TEST_F(QuestManagerTest, FuelCrisisQuest) {
    QuestManager::getInstance().initializeDefaultQuests();
    
    auto quest = QuestManager::getInstance().getQuest("quest_004");
    ASSERT_NE(quest, nullptr);
    EXPECT_EQ(quest->id, "quest_004");
    EXPECT_EQ(quest->difficulty, QuestDifficulty::EASY);
    EXPECT_EQ(quest->repeatable, true);
    EXPECT_EQ(quest->objectives.size(), 1);
    EXPECT_EQ(quest->objectives[0].type, QuestObjectiveType::COLLECT_ITEM);
    EXPECT_EQ(quest->objectives[0].targetId, "item_fuel");
    EXPECT_EQ(quest->objectives[0].targetAmount, 50);
    
    // Start quest and simulate collecting fuel
    QuestManager::getInstance().startQuest("quest_004");
    QuestManager::getInstance().notifyItemCollected("item_fuel", 30);
    EXPECT_EQ(quest->objectives[0].currentAmount, 30);
    EXPECT_FALSE(quest->objectives[0].isCompleted);
    
    QuestManager::getInstance().notifyItemCollected("item_fuel", 20);
    EXPECT_EQ(quest->objectives[0].currentAmount, 50);
    EXPECT_TRUE(quest->objectives[0].isCompleted);
}

// Test: Social Butterfly quest (quest_006) with multiple NPCs
TEST_F(QuestManagerTest, SocialButterflyQuest) {
    QuestManager::getInstance().initializeDefaultQuests();
    
    auto quest = QuestManager::getInstance().getQuest("quest_006");
    ASSERT_NE(quest, nullptr);
    EXPECT_EQ(quest->id, "quest_006");
    EXPECT_EQ(quest->objectives.size(), 5); // 5 NPCs to talk to
    
    // Start quest and simulate talking to NPCs
    QuestManager::getInstance().startQuest("quest_006");
    
    // Talk to first 3 NPCs
    QuestManager::getInstance().notifyNPCTalkedTo("npc_traveler_1");
    QuestManager::getInstance().notifyNPCTalkedTo("npc_traveler_2");
    QuestManager::getInstance().notifyNPCTalkedTo("npc_traveler_3");
    
    EXPECT_TRUE(quest->objectives[0].isCompleted);
    EXPECT_TRUE(quest->objectives[1].isCompleted);
    EXPECT_TRUE(quest->objectives[2].isCompleted);
    EXPECT_FALSE(quest->objectives[3].isCompleted);
    EXPECT_FALSE(quest->objectives[4].isCompleted);
    EXPECT_FALSE(quest->areAllObjectivesComplete());
    
    // Complete remaining objectives
    QuestManager::getInstance().notifyNPCTalkedTo("npc_traveler_4");
    QuestManager::getInstance().notifyNPCTalkedTo("npc_traveler_5");
    
    EXPECT_TRUE(quest->areAllObjectivesComplete());
}

// Test: Entrepreneur quest (quest_008) - Hard difficulty money quest
TEST_F(QuestManagerTest, EntrepreneurQuest) {
    QuestManager::getInstance().initializeDefaultQuests();
    
    auto quest = QuestManager::getInstance().getQuest("quest_008");
    ASSERT_NE(quest, nullptr);
    EXPECT_EQ(quest->difficulty, QuestDifficulty::HARD);
    EXPECT_EQ(quest->objectives.size(), 1);
    EXPECT_EQ(quest->objectives[0].type, QuestObjectiveType::EARN_MONEY);
    EXPECT_EQ(quest->objectives[0].targetAmount, 5000);
    
    // Start quest and simulate earning money
    QuestManager::getInstance().startQuest("quest_008");
    QuestManager::getInstance().notifyMoneyEarned(2000);
    EXPECT_EQ(quest->objectives[0].currentAmount, 2000);
    
    QuestManager::getInstance().notifyMoneyEarned(3000);
    EXPECT_EQ(quest->objectives[0].currentAmount, 5000);
    EXPECT_TRUE(quest->objectives[0].isCompleted);
}

// Test: Collector quest (quest_009) - Multiple collection objectives
TEST_F(QuestManagerTest, CollectorQuest) {
    QuestManager::getInstance().initializeDefaultQuests();
    
    auto quest = QuestManager::getInstance().getQuest("quest_009");
    ASSERT_NE(quest, nullptr);
    EXPECT_EQ(quest->objectives.size(), 3); // Food, water, spare parts
    
    QuestManager::getInstance().startQuest("quest_009");
    
    // Collect some items
    QuestManager::getInstance().notifyItemCollected("item_food", 10);
    QuestManager::getInstance().notifyItemCollected("item_water", 5);
    QuestManager::getInstance().notifyItemCollected("item_spare_parts", 3);
    
    EXPECT_TRUE(quest->objectives[0].isCompleted); // Food complete
    EXPECT_FALSE(quest->objectives[1].isCompleted); // Water incomplete
    EXPECT_FALSE(quest->objectives[2].isCompleted); // Spare parts incomplete
    
    // Complete remaining
    QuestManager::getInstance().notifyItemCollected("item_water", 5);
    QuestManager::getInstance().notifyItemCollected("item_spare_parts", 2);
    
    EXPECT_TRUE(quest->areAllObjectivesComplete());
}

// Test: Long Haul quest (quest_010) - Multi-objective journey
TEST_F(QuestManagerTest, LongHaulQuest) {
    QuestManager::getInstance().initializeDefaultQuests();
    
    auto quest = QuestManager::getInstance().getQuest("quest_010");
    ASSERT_NE(quest, nullptr);
    EXPECT_EQ(quest->difficulty, QuestDifficulty::HARD);
    EXPECT_EQ(quest->objectives.size(), 4); // 3 cities + 1 delivery
    
    QuestManager::getInstance().startQuest("quest_010");
    
    // Visit cities
    QuestManager::getInstance().notifyLocationVisited("loc_moscow");
    QuestManager::getInstance().notifyLocationVisited("loc_vladimir");
    
    EXPECT_TRUE(quest->objectives[0].isCompleted);
    EXPECT_TRUE(quest->objectives[1].isCompleted);
    EXPECT_FALSE(quest->objectives[2].isCompleted);
    EXPECT_FALSE(quest->areAllObjectivesComplete());
}

