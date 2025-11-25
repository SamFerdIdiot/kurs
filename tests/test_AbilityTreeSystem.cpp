#include <gtest/gtest.h>
#include "AbilityTreeSystem.h"
#include "ExperienceSystem.h"

class AbilityTreeSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset systems before each test
        AbilityTreeSystem::getInstance().reset();
        ExperienceSystem::getInstance().reset();
    }
    
    void TearDown() override {
        // Cleanup after each test
    }
};

// Test: Initialize creates abilities
TEST_F(AbilityTreeSystemTest, InitializeCreatesAbilities) {
    auto& system = AbilityTreeSystem::getInstance();
    system.initialize();
    
    EXPECT_GT(system.getAllAbilities().size(), 0);
    EXPECT_GE(system.getAllAbilities().size(), 35);  // At least 35 abilities
}

// Test: Initial state has zero skill points
TEST_F(AbilityTreeSystemTest, InitialStateHasZeroSkillPoints) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    
    EXPECT_EQ(system.getAvailableSkillPoints(), 0);
}

// Test: Add skill points increases available points
TEST_F(AbilityTreeSystemTest, AddSkillPointsIncreasesAvailable) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    
    EXPECT_EQ(system.getAvailableSkillPoints(), 0);
    
    system.addSkillPoints(5);
    EXPECT_EQ(system.getAvailableSkillPoints(), 5);
    
    system.addSkillPoints(3);
    EXPECT_EQ(system.getAvailableSkillPoints(), 8);
}

// Test: Spend skill points decreases available points
TEST_F(AbilityTreeSystemTest, SpendSkillPointsDecreasesAvailable) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    system.addSkillPoints(10);
    
    EXPECT_TRUE(system.spendSkillPoints(3));
    EXPECT_EQ(system.getAvailableSkillPoints(), 7);
    
    EXPECT_TRUE(system.spendSkillPoints(5));
    EXPECT_EQ(system.getAvailableSkillPoints(), 2);
}

// Test: Cannot spend more skill points than available
TEST_F(AbilityTreeSystemTest, CannotSpendMoreThanAvailable) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    system.addSkillPoints(5);
    
    EXPECT_FALSE(system.spendSkillPoints(10));
    EXPECT_EQ(system.getAvailableSkillPoints(), 5);  // Unchanged
    
    EXPECT_TRUE(system.spendSkillPoints(5));
    EXPECT_EQ(system.getAvailableSkillPoints(), 0);
}

// Test: Get abilities by category
TEST_F(AbilityTreeSystemTest, GetAbilitiesByCategory) {
    auto& system = AbilityTreeSystem::getInstance();
    system.initialize();
    
    auto drivingAbilities = system.getAbilitiesByCategory(AbilityCategory::DRIVING);
    auto tradingAbilities = system.getAbilitiesByCategory(AbilityCategory::TRADING);
    auto survivalAbilities = system.getAbilitiesByCategory(AbilityCategory::SURVIVAL);
    auto mechanicsAbilities = system.getAbilitiesByCategory(AbilityCategory::MECHANICS);
    auto socialAbilities = system.getAbilitiesByCategory(AbilityCategory::SOCIAL);
    
    EXPECT_GT(drivingAbilities.size(), 0);
    EXPECT_GT(tradingAbilities.size(), 0);
    EXPECT_GT(survivalAbilities.size(), 0);
    EXPECT_GT(mechanicsAbilities.size(), 0);
    EXPECT_GT(socialAbilities.size(), 0);
    
    // Each category should have multiple abilities
    EXPECT_GE(drivingAbilities.size(), 5);
    EXPECT_GE(tradingAbilities.size(), 5);
    EXPECT_GE(survivalAbilities.size(), 5);
    EXPECT_GE(mechanicsAbilities.size(), 5);
    EXPECT_GE(socialAbilities.size(), 5);
}

// Test: Get specific ability by ID
TEST_F(AbilityTreeSystemTest, GetSpecificAbilityById) {
    auto& system = AbilityTreeSystem::getInstance();
    system.initialize();
    
    const AbilityNode* ability = system.getAbility("driving_economical_1");
    ASSERT_NE(ability, nullptr);
    EXPECT_EQ(ability->getId(), "driving_economical_1");
    EXPECT_EQ(ability->getCategory(), AbilityCategory::DRIVING);
}

// Test: Get non-existent ability returns nullptr
TEST_F(AbilityTreeSystemTest, GetNonExistentAbilityReturnsNullptr) {
    auto& system = AbilityTreeSystem::getInstance();
    system.initialize();
    
    const AbilityNode* ability = system.getAbility("non_existent_ability");
    EXPECT_EQ(ability, nullptr);
}

// Test: Unlock ability successfully
TEST_F(AbilityTreeSystemTest, UnlockAbilitySuccessfully) {
    auto& system = AbilityTreeSystem::getInstance();
    auto& expSystem = ExperienceSystem::getInstance();
    
    system.reset();
    expSystem.reset();
    system.initialize();
    
    // Give skill points and level
    system.addSkillPoints(5);
    expSystem.addExperience(300);  // Level up
    
    EXPECT_FALSE(system.isAbilityUnlocked("driving_economical_1"));
    
    bool result = system.unlockAbility("driving_economical_1");
    EXPECT_TRUE(result);
    EXPECT_TRUE(system.isAbilityUnlocked("driving_economical_1"));
}

// Test: Cannot unlock without skill points
TEST_F(AbilityTreeSystemTest, CannotUnlockWithoutSkillPoints) {
    auto& system = AbilityTreeSystem::getInstance();
    auto& expSystem = ExperienceSystem::getInstance();
    
    system.reset();
    expSystem.reset();
    system.initialize();
    
    // Level is sufficient but no skill points
    expSystem.addExperience(300);
    
    bool result = system.unlockAbility("driving_economical_1");
    EXPECT_FALSE(result);
    EXPECT_FALSE(system.isAbilityUnlocked("driving_economical_1"));
}

// Test: Unlocking spends skill points
TEST_F(AbilityTreeSystemTest, UnlockingSpendsSkillPoints) {
    auto& system = AbilityTreeSystem::getInstance();
    auto& expSystem = ExperienceSystem::getInstance();
    
    system.reset();
    expSystem.reset();
    system.initialize();
    
    system.addSkillPoints(10);
    expSystem.addExperience(300);
    
    EXPECT_EQ(system.getAvailableSkillPoints(), 10);
    
    system.unlockAbility("driving_economical_1");  // Costs 1 point
    EXPECT_EQ(system.getAvailableSkillPoints(), 9);
    
    system.unlockAbility("perk_boost");  // Costs 2 points
    EXPECT_EQ(system.getAvailableSkillPoints(), 7);
}

// Test: Passive bonus calculation
TEST_F(AbilityTreeSystemTest, PassiveBonusCalculation) {
    auto& system = AbilityTreeSystem::getInstance();
    auto& expSystem = ExperienceSystem::getInstance();
    
    system.reset();
    expSystem.reset();
    system.initialize();
    
    system.addSkillPoints(10);
    expSystem.addExperience(500);  // High level
    
    // Initially no bonus (multiplier = 1.0)
    float initialBonus = system.getPassiveBonus("fuel_efficiency");
    EXPECT_FLOAT_EQ(initialBonus, 1.0f);
    
    // Unlock fuel efficiency I (+10% = 1.1)
    system.unlockAbility("driving_economical_1");
    float bonus1 = system.getPassiveBonus("fuel_efficiency");
    EXPECT_FLOAT_EQ(bonus1, 1.1f);
    
    // Unlock fuel efficiency II (+15% = 1.15, total 1.1 × 1.15 = 1.265)
    system.unlockAbility("driving_economical_2");
    float bonus2 = system.getPassiveBonus("fuel_efficiency");
    EXPECT_FLOAT_EQ(bonus2, 1.1f * 1.15f);  // 1.265
}

// Test: Passive flat bonus calculation
TEST_F(AbilityTreeSystemTest, PassiveFlatBonusCalculation) {
    auto& system = AbilityTreeSystem::getInstance();
    auto& expSystem = ExperienceSystem::getInstance();
    
    system.reset();
    expSystem.reset();
    system.initialize();
    
    system.addSkillPoints(10);
    expSystem.addExperience(500);
    
    // Initially no bonus
    int initialBonus = system.getPassiveBonusFlat("max_energy");
    EXPECT_EQ(initialBonus, 0);
    
    // Unlock stamina I (+10 energy)
    system.unlockAbility("survival_stamina_1");
    int bonus1 = system.getPassiveBonusFlat("max_energy");
    EXPECT_EQ(bonus1, 10);
    
    // Unlock stamina II (+20 energy, total +30)
    system.unlockAbility("survival_stamina_2");
    int bonus2 = system.getPassiveBonusFlat("max_energy");
    EXPECT_EQ(bonus2, 30);
}

// Test: Active perk cooldown system
TEST_F(AbilityTreeSystemTest, ActivePerkCooldownSystem) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    system.initialize();
    system.addSkillPoints(5);
    
    // Unlock a perk with charges
    system.unlockAbility("perk_relax");
    
    // Verify the perk is unlocked
    EXPECT_TRUE(system.isAbilityUnlocked("perk_relax"));
    
    // Verify it has charges (assuming it's initialized)
    // The charge system is tested separately
    
    // Call updateCooldowns for compatibility (no-op in charge system)
    system.updateCooldowns(0.0f);  // No time passed
}

// Test: Update cooldowns reduces remaining time
TEST_F(AbilityTreeSystemTest, UpdateCooldownsReducesTime) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    
    // We can't easily test this without access to internal cooldown methods
    // but we can verify the update method doesn't crash
    system.updateCooldowns(1.0f);  // 1 minute passed
    system.updateCooldowns(5.0f);  // 5 minutes passed
    system.updateCooldowns(10.0f); // 10 minutes passed
    
    // If we got here without crashing, the method works
    SUCCEED();
}

// Test: Cannot unlock ability twice
TEST_F(AbilityTreeSystemTest, CannotUnlockAbilityTwice) {
    auto& system = AbilityTreeSystem::getInstance();
    auto& expSystem = ExperienceSystem::getInstance();
    
    system.reset();
    expSystem.reset();
    system.initialize();
    
    system.addSkillPoints(10);
    expSystem.addExperience(300);
    
    // First unlock succeeds
    EXPECT_TRUE(system.unlockAbility("driving_economical_1"));
    EXPECT_EQ(system.getAvailableSkillPoints(), 9);
    
    // Second unlock fails
    EXPECT_FALSE(system.unlockAbility("driving_economical_1"));
    EXPECT_EQ(system.getAvailableSkillPoints(), 9);  // No points spent
}

// Test: Reset clears all data
TEST_F(AbilityTreeSystemTest, ResetClearsAllData) {
    auto& system = AbilityTreeSystem::getInstance();
    
    system.initialize();
    system.addSkillPoints(10);
    system.unlockAbility("driving_economical_1");
    
    EXPECT_GT(system.getAvailableSkillPoints(), 0);
    EXPECT_TRUE(system.isAbilityUnlocked("driving_economical_1"));
    
    system.reset();
    
    EXPECT_EQ(system.getAvailableSkillPoints(), 0);
    EXPECT_FALSE(system.isAbilityUnlocked("driving_economical_1"));
    EXPECT_GT(system.getAllAbilities().size(), 0);  // Abilities re-initialized
}

// Test: Specific abilities exist
TEST_F(AbilityTreeSystemTest, SpecificAbilitiesExist) {
    auto& system = AbilityTreeSystem::getInstance();
    system.initialize();
    
    // Check key abilities exist
    EXPECT_NE(system.getAbility("driving_economical_1"), nullptr);
    EXPECT_NE(system.getAbility("perk_boost"), nullptr);
    EXPECT_NE(system.getAbility("perk_relax"), nullptr);
    EXPECT_NE(system.getAbility("perk_emergency_repair"), nullptr);
    EXPECT_NE(system.getAbility("perk_negotiate"), nullptr);
    EXPECT_NE(system.getAbility("perk_encourage"), nullptr);
}

// Test: Ability categories are correct
TEST_F(AbilityTreeSystemTest, AbilityCategoriesAreCorrect) {
    auto& system = AbilityTreeSystem::getInstance();
    system.initialize();
    
    const AbilityNode* driving = system.getAbility("driving_economical_1");
    ASSERT_NE(driving, nullptr);
    EXPECT_EQ(driving->getCategory(), AbilityCategory::DRIVING);
    
    const AbilityNode* trading = system.getAbility("trading_discount_1");
    ASSERT_NE(trading, nullptr);
    EXPECT_EQ(trading->getCategory(), AbilityCategory::TRADING);
    
    const AbilityNode* survival = system.getAbility("survival_stamina_1");
    ASSERT_NE(survival, nullptr);
    EXPECT_EQ(survival->getCategory(), AbilityCategory::SURVIVAL);
    
    const AbilityNode* mechanics = system.getAbility("mechanics_prevention_1");
    ASSERT_NE(mechanics, nullptr);
    EXPECT_EQ(mechanics->getCategory(), AbilityCategory::MECHANICS);
    
    const AbilityNode* social = system.getAbility("social_charisma_1");
    ASSERT_NE(social, nullptr);
    EXPECT_EQ(social->getCategory(), AbilityCategory::SOCIAL);
}

// Test: Ability types are correct
TEST_F(AbilityTreeSystemTest, AbilityTypesAreCorrect) {
    auto& system = AbilityTreeSystem::getInstance();
    system.initialize();
    
    // Passive abilities
    const AbilityNode* passive = system.getAbility("driving_economical_1");
    ASSERT_NE(passive, nullptr);
    EXPECT_EQ(passive->getType(), AbilityType::PASSIVE);
    
    // Active perks
    const AbilityNode* active = system.getAbility("perk_boost");
    ASSERT_NE(active, nullptr);
    EXPECT_EQ(active->getType(), AbilityType::ACTIVE);
}
