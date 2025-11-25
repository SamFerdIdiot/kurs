#include <gtest/gtest.h>
#include "NPC.h"
#include "DialogueManager.h"
#include "ExperienceSystem.h"

// ============================================================================
// NPC Tests
// ============================================================================

class NPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up NPCManager before each test
        NPCManager::getInstance().clear();
    }
};

// Test NPC creation and basic properties
TEST_F(NPCTest, NPCCreation) {
    NPC npc("test_id", "Test NPC", "Test bio", 1);
    
    EXPECT_EQ(npc.getId(), "test_id");
    EXPECT_EQ(npc.getName(), "Test NPC");
    EXPECT_EQ(npc.getBio(), "Test bio");
    EXPECT_EQ(npc.getLevel(), 1);
    EXPECT_EQ(npc.getExperience(), 0);
}

// Test leveling system
TEST_F(NPCTest, LevelingSystem) {
    NPC npc("test_id", "Test NPC", "Test bio", 1);
    
    // Test experience for next level
    EXPECT_EQ(npc.getExperienceForNextLevel(), 100);
    
    // Add experience
    npc.addExperience(50);
    EXPECT_EQ(npc.getExperience(), 50);
    EXPECT_EQ(npc.getLevel(), 1);
    
    // Level up
    npc.addExperience(50);
    EXPECT_EQ(npc.getLevel(), 2);
}

// Test level boundaries
TEST_F(NPCTest, LevelBoundaries) {
    NPC npc("test_id", "Test NPC", "Test bio", 1);
    
    // Add enough experience to reach level 3
    npc.addExperience(500);
    
    // Should be at level 3 (max)
    EXPECT_EQ(npc.getLevel(), 3);
    
    // Adding more experience shouldn't go beyond level 3
    npc.addExperience(1000);
    EXPECT_EQ(npc.getLevel(), 3);
}

// Test abilities
TEST_F(NPCTest, Abilities) {
    NPC npc("test_id", "Test NPC", "Test bio", 1);
    
    // Initially no abilities
    EXPECT_FALSE(npc.hasPassiveAbility());
    EXPECT_FALSE(npc.hasActiveAbility());
    
    // Set abilities
    PassiveAbility passive("passive_1", "Test Passive", "Description", 
                          PassiveAbilityAttachment::NPC);
    npc.setPassiveAbility(passive);
    EXPECT_TRUE(npc.hasPassiveAbility());
    EXPECT_EQ(npc.getPassiveAbility().id, "passive_1");
    
    ActiveAbility active("active_1", "Test Active", "Description", 60.0f);
    npc.setActiveAbility(active);
    EXPECT_TRUE(npc.hasActiveAbility());
    EXPECT_EQ(npc.getActiveAbility().id, "active_1");
}

// Test quests
TEST_F(NPCTest, Quests) {
    NPC npc("test_id", "Test NPC", "Test bio", 1);
    
    // Initially no quests
    EXPECT_FALSE(npc.hasQuest("quest_1"));
    
    // Attach quest
    npc.attachQuest("quest_1");
    EXPECT_TRUE(npc.hasQuest("quest_1"));
    EXPECT_EQ(npc.getAttachedQuests().size(), 1);
    
    // Detach quest
    npc.detachQuest("quest_1");
    EXPECT_FALSE(npc.hasQuest("quest_1"));
}

// Test NPCManager singleton
TEST_F(NPCTest, NPCManagerSingleton) {
    NPCManager& manager1 = NPCManager::getInstance();
    NPCManager& manager2 = NPCManager::getInstance();
    
    EXPECT_EQ(&manager1, &manager2);
}

// Test adding NPCs to manager
TEST_F(NPCTest, AddNPCToManager) {
    NPCManager& manager = NPCManager::getInstance();
    manager.setMaxTeamSize(3);
    
    auto npc1 = std::make_unique<NPC>("id1", "NPC1", "Bio1", 1);
    EXPECT_TRUE(manager.addNPC(std::move(npc1)));
    EXPECT_EQ(manager.getTeamSize(), 1);
    
    auto npc2 = std::make_unique<NPC>("id2", "NPC2", "Bio2", 1);
    EXPECT_TRUE(manager.addNPC(std::move(npc2)));
    EXPECT_EQ(manager.getTeamSize(), 2);
}

// Test team size limit
TEST_F(NPCTest, TeamSizeLimit) {
    NPCManager& manager = NPCManager::getInstance();
    manager.setMaxTeamSize(2);
    
    auto npc1 = std::make_unique<NPC>("id1", "NPC1", "Bio1", 1);
    EXPECT_TRUE(manager.addNPC(std::move(npc1)));
    
    auto npc2 = std::make_unique<NPC>("id2", "NPC2", "Bio2", 1);
    EXPECT_TRUE(manager.addNPC(std::move(npc2)));
    
    // Should fail - team is full
    auto npc3 = std::make_unique<NPC>("id3", "NPC3", "Bio3", 1);
    EXPECT_FALSE(manager.addNPC(std::move(npc3)));
    EXPECT_EQ(manager.getTeamSize(), 2);
}

// Test removing NPCs
TEST_F(NPCTest, RemoveNPC) {
    NPCManager& manager = NPCManager::getInstance();
    
    auto npc1 = std::make_unique<NPC>("id1", "NPC1", "Bio1", 1);
    manager.addNPC(std::move(npc1));
    EXPECT_EQ(manager.getTeamSize(), 1);
    
    manager.removeNPC("id1");
    EXPECT_EQ(manager.getTeamSize(), 0);
}

// Test team experience
TEST_F(NPCTest, TeamExperience) {
    NPCManager& manager = NPCManager::getInstance();
    
    auto npc1 = std::make_unique<NPC>("id1", "NPC1", "Bio1", 1);
    auto npc2 = std::make_unique<NPC>("id2", "NPC2", "Bio2", 1);
    
    manager.addNPC(std::move(npc1));
    manager.addNPC(std::move(npc2));
    
    // Add experience to all team members
    manager.addExperienceToTeam(100);
    
    // Both NPCs should level up
    EXPECT_EQ(manager.getNPC("id1")->getLevel(), 2);
    EXPECT_EQ(manager.getNPC("id2")->getLevel(), 2);
}

// Test NPC factory
TEST_F(NPCTest, NPCFactory) {
    auto mechanic = NPCFactory::createMechanic();
    EXPECT_EQ(mechanic->getLevel(), 1);
    EXPECT_FALSE(mechanic->getName().empty());
    EXPECT_TRUE(mechanic->hasPassiveAbility());
    EXPECT_TRUE(mechanic->hasActiveAbility());
    
    auto trader = NPCFactory::createTrader();
    EXPECT_TRUE(trader->hasPassiveAbility());
    
    auto companion = NPCFactory::createCompanion();
    EXPECT_TRUE(companion->hasPassiveAbility());
    
    auto medic = NPCFactory::createMedic();
    EXPECT_TRUE(medic->hasPassiveAbility());
    
    auto navigator = NPCFactory::createNavigator();
    EXPECT_TRUE(navigator->hasPassiveAbility());
}

// ============================================================================
// Experience System Tests
// ============================================================================

class ExperienceSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        ExperienceSystem::getInstance().reset();
    }
};

TEST_F(ExperienceSystemTest, Initialization) {
    ExperienceSystem& exp = ExperienceSystem::getInstance();
    
    EXPECT_EQ(exp.getLevel(), 1);
    EXPECT_EQ(exp.getExperience(), 0);
}

TEST_F(ExperienceSystemTest, LevelingUp) {
    ExperienceSystem& exp = ExperienceSystem::getInstance();
    
    int expForLevel2 = exp.getExperienceForNextLevel();
    exp.addExperience(expForLevel2);
    
    EXPECT_EQ(exp.getLevel(), 2);
}

TEST_F(ExperienceSystemTest, AbilityTree) {
    ExperienceSystem& exp = ExperienceSystem::getInstance();
    
    EXPECT_FALSE(exp.getAbilityTree().empty());
    
    // Try to unlock an ability
    exp.unlockAbility("ability_driving_1");
    EXPECT_TRUE(exp.isAbilityUnlocked("ability_driving_1"));
}

// ============================================================================
// DialogueManager Tests
// ============================================================================

class DialogueManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up DialogueManager before each test
        DialogueManager::getInstance().clear();
    }
};

// Test DialogueManager singleton
TEST_F(DialogueManagerTest, Singleton) {
    DialogueManager& manager1 = DialogueManager::getInstance();
    DialogueManager& manager2 = DialogueManager::getInstance();
    
    EXPECT_EQ(&manager1, &manager2);
}

// Test registering dialogue
TEST_F(DialogueManagerTest, RegisterDialogue) {
    DialogueManager& manager = DialogueManager::getInstance();
    
    DialogueTree tree;
    tree.id = "test_dialogue";
    tree.title = "Test";
    
    manager.registerDialogue("test_dialogue", tree);
    EXPECT_TRUE(manager.hasDialogue("test_dialogue"));
}

// Test starting dialogue
TEST_F(DialogueManagerTest, StartDialogue) {
    DialogueManager& manager = DialogueManager::getInstance();
    
    // Create simple dialogue
    DialogueTree tree;
    tree.id = "test";
    tree.startNodeId = "start";
    
    DialogueNode startNode;
    startNode.id = "start";
    startNode.speakerName = "Test Speaker";
    startNode.text = "Test text";
    
    tree.nodes["start"] = startNode;
    
    manager.registerDialogue("test", tree);
    EXPECT_TRUE(manager.startDialogue("test"));
    EXPECT_TRUE(manager.isActive());
}

// Test dialogue not found
TEST_F(DialogueManagerTest, DialogueNotFound) {
    DialogueManager& manager = DialogueManager::getInstance();
    
    EXPECT_FALSE(manager.startDialogue("nonexistent"));
    EXPECT_FALSE(manager.isActive());
}

// Test getting current node
TEST_F(DialogueManagerTest, GetCurrentNode) {
    DialogueManager& manager = DialogueManager::getInstance();
    
    DialogueTree tree;
    tree.id = "test";
    tree.startNodeId = "start";
    
    DialogueNode startNode;
    startNode.id = "start";
    startNode.speakerName = "Speaker";
    startNode.text = "Text";
    
    tree.nodes["start"] = startNode;
    
    manager.registerDialogue("test", tree);
    manager.startDialogue("test");
    
    const DialogueNode* node = manager.getCurrentNode();
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->id, "start");
    EXPECT_EQ(node->text, "Text");
}

// Test ending dialogue
TEST_F(DialogueManagerTest, EndDialogue) {
    DialogueManager& manager = DialogueManager::getInstance();
    
    DialogueTree tree;
    tree.id = "test";
    tree.startNodeId = "start";
    
    DialogueNode startNode;
    startNode.id = "start";
    tree.nodes["start"] = startNode;
    
    manager.registerDialogue("test", tree);
    manager.startDialogue("test");
    EXPECT_TRUE(manager.isActive());
    
    manager.endDialogue();
    EXPECT_FALSE(manager.isActive());
}

// Test dialogue factory - mechanic
TEST_F(DialogueManagerTest, MechanicDialogueFactory) {
    DialogueTree tree = DialogueFactory::createMechanicDialogue();
    
    EXPECT_EQ(tree.id, "mechanic_repair");
    EXPECT_FALSE(tree.nodes.empty());
    EXPECT_TRUE(tree.nodes.find("start") != tree.nodes.end());
}

// Test dialogue factory - trader
TEST_F(DialogueManagerTest, TraderDialogueFactory) {
    DialogueTree tree = DialogueFactory::createTraderDialogue();
    
    EXPECT_EQ(tree.id, "trader_shop");
    EXPECT_FALSE(tree.nodes.empty());
}

// Test dialogue factory - companion
TEST_F(DialogueManagerTest, CompanionDialogueFactory) {
    DialogueTree tree = DialogueFactory::createCompanionDialogue();
    
    EXPECT_EQ(tree.id, "companion_chat");
    EXPECT_FALSE(tree.nodes.empty());
}

// Test dialogue factory - recruit
TEST_F(DialogueManagerTest, RecruitDialogueFactory) {
    DialogueTree tree = DialogueFactory::createRecruitDialogue("npc_id", "Test NPC", 100.0f);
    
    EXPECT_EQ(tree.id, "recruit_npc_id");
    EXPECT_FALSE(tree.nodes.empty());
}

// Test dialogue factory - conflict
TEST_F(DialogueManagerTest, ConflictDialogueFactory) {
    DialogueTree tree = DialogueFactory::createConflictDialogue();
    
    EXPECT_EQ(tree.id, "team_conflict");
    EXPECT_FALSE(tree.nodes.empty());
}

// Test dialogue factory - info
TEST_F(DialogueManagerTest, InfoDialogueFactory) {
    DialogueTree tree = DialogueFactory::createInfoDialogue("test_info", "Speaker", "Message");
    
    EXPECT_EQ(tree.id, "test_info");
    EXPECT_FALSE(tree.nodes.empty());
}

// Test choice availability
TEST_F(DialogueManagerTest, ChoiceAvailability) {
    DialogueManager& manager = DialogueManager::getInstance();
    
    DialogueTree tree;
    tree.id = "test";
    tree.startNodeId = "start";
    
    DialogueNode startNode;
    startNode.id = "start";
    
    DialogueChoice choice1;
    choice1.text = "Always available";
    
    DialogueChoice choice2;
    choice2.text = "Never available";
    choice2.condition = DialogueCondition([]() { return false; }, "Test condition");
    
    startNode.choices.push_back(choice1);
    startNode.choices.push_back(choice2);
    
    tree.nodes["start"] = startNode;
    
    manager.registerDialogue("test", tree);
    manager.startDialogue("test");
    
    EXPECT_TRUE(manager.isChoiceAvailable(0));
    EXPECT_FALSE(manager.isChoiceAvailable(1));
}
