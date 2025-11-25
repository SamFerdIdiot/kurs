#include <gtest/gtest.h>
#include "AbilityNode.h"
#include "AbilityTypes.h"

class AbilityNodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test data
    }
    
    void TearDown() override {
        // Cleanup
    }
};

// Test: Default constructor creates valid node
TEST_F(AbilityNodeTest, DefaultConstructorCreatesValidNode) {
    AbilityNode node;
    
    EXPECT_EQ(node.getType(), AbilityType::PASSIVE);
    EXPECT_EQ(node.getCategory(), AbilityCategory::DRIVING);
    EXPECT_FALSE(node.isUnlocked());
}

// Test: Parameterized constructor initializes all fields
TEST_F(AbilityNodeTest, ParameterizedConstructorInitializesFields) {
    AbilityRequirement req(3, {"prereq1"}, 2);
    AbilityEffect effect;
    effect.effectType = "fuel_efficiency";
    effect.bonusMultiplier = 1.1f;
    
    AbilityNode node("test_id", "Test Name", "Test Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    EXPECT_EQ(node.getId(), "test_id");
    EXPECT_EQ(node.getName(), "Test Name");
    EXPECT_EQ(node.getDescription(), "Test Description");
    EXPECT_EQ(node.getType(), AbilityType::PASSIVE);
    EXPECT_EQ(node.getCategory(), AbilityCategory::DRIVING);
    EXPECT_FALSE(node.isUnlocked());
    EXPECT_EQ(node.getRequirement().requiredLevel, 3);
    EXPECT_EQ(node.getRequirement().skillPointCost, 2);
    EXPECT_EQ(node.getEffect().effectType, "fuel_efficiency");
    EXPECT_FLOAT_EQ(node.getEffect().bonusMultiplier, 1.1f);
}

// Test: Can unlock with all requirements met
TEST_F(AbilityNodeTest, CanUnlockWithRequirementsMet) {
    AbilityRequirement req(3, {"ability_1"}, 1);
    AbilityEffect effect;
    AbilityNode node("test_ability", "Test", "Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    std::vector<std::string> unlocked = {"ability_1"};
    EXPECT_TRUE(node.canUnlock(3, 1, unlocked));
}

// Test: Cannot unlock without sufficient level
TEST_F(AbilityNodeTest, CannotUnlockWithInsufficientLevel) {
    AbilityRequirement req(5, {}, 1);
    AbilityEffect effect;
    AbilityNode node("test_ability", "Test", "Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    std::vector<std::string> unlocked;
    EXPECT_FALSE(node.canUnlock(3, 1, unlocked));  // Level 3 < 5
}

// Test: Cannot unlock without skill points
TEST_F(AbilityNodeTest, CannotUnlockWithoutSkillPoints) {
    AbilityRequirement req(1, {}, 2);
    AbilityEffect effect;
    AbilityNode node("test_ability", "Test", "Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    std::vector<std::string> unlocked;
    EXPECT_FALSE(node.canUnlock(1, 1, unlocked));  // Has 1, needs 2
    EXPECT_TRUE(node.canUnlock(1, 2, unlocked));   // Has 2, needs 2
}

// Test: Cannot unlock without prerequisites
TEST_F(AbilityNodeTest, CannotUnlockWithoutPrerequisites) {
    AbilityRequirement req(3, {"ability_1", "ability_2"}, 1);
    AbilityEffect effect;
    AbilityNode node("test_ability", "Test", "Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    // Missing ability_2
    std::vector<std::string> unlocked = {"ability_1"};
    EXPECT_FALSE(node.canUnlock(3, 1, unlocked));
    
    // Has both
    unlocked.push_back("ability_2");
    EXPECT_TRUE(node.canUnlock(3, 1, unlocked));
}

// Test: Cannot unlock if already unlocked
TEST_F(AbilityNodeTest, CannotUnlockIfAlreadyUnlocked) {
    AbilityRequirement req(1, {}, 1);
    AbilityEffect effect;
    AbilityNode node("test_ability", "Test", "Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    std::vector<std::string> unlocked;
    EXPECT_TRUE(node.canUnlock(1, 1, unlocked));
    
    // Unlock the node
    node.setUnlocked(true);
    EXPECT_TRUE(node.isUnlocked());
    
    // Now it cannot be unlocked again
    EXPECT_FALSE(node.canUnlock(1, 1, unlocked));
}

// Test: Unlock/lock state management
TEST_F(AbilityNodeTest, UnlockStateManagement) {
    AbilityNode node;
    
    EXPECT_FALSE(node.isUnlocked());
    
    node.setUnlocked(true);
    EXPECT_TRUE(node.isUnlocked());
    
    node.setUnlocked(false);
    EXPECT_FALSE(node.isUnlocked());
}

// Test: Icon path management
TEST_F(AbilityNodeTest, IconPathManagement) {
    AbilityNode node;
    
    EXPECT_EQ(node.getIconPath(), "");
    
    node.setIconPath("assets/icons/ability.png");
    EXPECT_EQ(node.getIconPath(), "assets/icons/ability.png");
}

// Test: Passive ability effect
TEST_F(AbilityNodeTest, PassiveAbilityEffect) {
    AbilityRequirement req(1, {}, 1);
    AbilityEffect effect;
    effect.effectType = "fuel_efficiency";
    effect.bonusMultiplier = 1.15f;  // +15%
    effect.bonusFlat = 0;
    
    AbilityNode node("passive_test", "Passive Test", "Test",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    EXPECT_EQ(node.getType(), AbilityType::PASSIVE);
    EXPECT_EQ(node.getEffect().effectType, "fuel_efficiency");
    EXPECT_FLOAT_EQ(node.getEffect().bonusMultiplier, 1.15f);
}

// Test: Active ability effect with costs
TEST_F(AbilityNodeTest, ActiveAbilityEffectWithCosts) {
    AbilityRequirement req(3, {}, 2);
    AbilityEffect effect;
    effect.effectType = "temporary_speed_boost";
    effect.energyCost = 10;
    effect.moneyCost = 50;
    effect.itemRequirements = {"tool_kit"};
    effect.effectDescription = "Speed boost";
    effect.targetType = "vehicle";
    effect.duration = 0.5f;
    
    AbilityNode node("active_test", "Active Test", "Test",
                    AbilityType::ACTIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    EXPECT_EQ(node.getType(), AbilityType::ACTIVE);
    EXPECT_EQ(node.getEffect().energyCost, 10);
    EXPECT_EQ(node.getEffect().moneyCost, 50);
    EXPECT_EQ(node.getEffect().itemRequirements.size(), 1);
    EXPECT_EQ(node.getEffect().itemRequirements[0], "tool_kit");
    EXPECT_EQ(node.getEffect().targetType, "vehicle");
    EXPECT_FLOAT_EQ(node.getEffect().duration, 0.5f);
}

// Test: Multiple prerequisites validation
TEST_F(AbilityNodeTest, MultiplePrerequisitesValidation) {
    AbilityRequirement req(5, {"req1", "req2", "req3"}, 2);
    AbilityEffect effect;
    AbilityNode node("multi_prereq", "Test", "Test",
                    AbilityType::PASSIVE, AbilityCategory::SURVIVAL,
                    req, effect);
    
    std::vector<std::string> unlocked1 = {"req1"};
    EXPECT_FALSE(node.canUnlock(5, 2, unlocked1));
    
    std::vector<std::string> unlocked2 = {"req1", "req2"};
    EXPECT_FALSE(node.canUnlock(5, 2, unlocked2));
    
    std::vector<std::string> unlocked3 = {"req1", "req2", "req3"};
    EXPECT_TRUE(node.canUnlock(5, 2, unlocked3));
    
    // Order doesn't matter
    std::vector<std::string> unlocked4 = {"req3", "req1", "req2"};
    EXPECT_TRUE(node.canUnlock(5, 2, unlocked4));
}
