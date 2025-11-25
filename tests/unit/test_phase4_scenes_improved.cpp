#include <gtest/gtest.h>
#include "test_helpers.h"
#include "InventoryScene.h"
#include "CharacterScene.h"
#include "CompanionsScene.h"
#include "QuestsScene.h"

// ============================================================================
// InventoryScene Tests - Using Test Helpers (Additional Tests)
// ============================================================================

class InventorySceneImprovedTest : public SceneTestFixture {};

TEST_F(InventorySceneImprovedTest, MouseInteractionWorks) {
    InventoryScene scene;
    // Использование helper функции для тестирования мыши
    TestHelpers::testMouseHandling(scene, 150, 200);
}

TEST_F(InventorySceneImprovedTest, MultipleFramesWork) {
    InventoryScene scene;
    // Симуляция 60 кадров
    TestHelpers::simulateFrames(scene, 60);
}

TEST_F(InventorySceneImprovedTest, BasicsWork) {
    InventoryScene scene;
    // Использование helper функции для базовых проверок
    TestHelpers::testSceneBasics(scene);
}

// ============================================================================
// CharacterScene Tests - Using ExperienceSystem Fixture (Additional Tests)
// ============================================================================

class CharacterSceneImprovedTest : public ExperienceSystemTestFixture {};

TEST_F(CharacterSceneImprovedTest, ExperienceIntegration) {
    // Использование helper метода из фикстура
    addExperience(150);
    
    CharacterScene scene;
    
    // Проверка что уровень обновился
    EXPECT_GE(getCurrentLevel(), 1);
}

TEST_F(CharacterSceneImprovedTest, MultipleUpdates) {
    CharacterScene scene;
    TestHelpers::simulateFrames(scene, 30);
}

// ============================================================================
// CompanionsScene Tests - Using NPCManager Fixture (Additional Tests)
// ============================================================================

class CompanionsSceneImprovedTest : public NPCManagerTestFixture {};

TEST_F(CompanionsSceneImprovedTest, MultipleCompanions) {
    // Добавление нескольких NPC
    addTestNPC("mechanic");
    addTestNPC("trader");
    
    CompanionsScene scene;
    
    EXPECT_EQ(getNPCCount(), 2);
}

TEST_F(CompanionsSceneImprovedTest, ThreeCompanions) {
    addTestNPC("mechanic");
    addTestNPC("trader");
    addTestNPC("companion");
    
    CompanionsScene scene;
    
    EXPECT_EQ(getNPCCount(), 3);
}

// ============================================================================
// QuestsScene Tests - Using QuestManager Fixture (Additional Tests)
// ============================================================================

class QuestsSceneImprovedTest : public QuestManagerTestFixture {};

TEST_F(QuestsSceneImprovedTest, HasDefaultQuests) {
    // QuestManager needs to be initialized first
    QuestManager::getInstance().initializeDefaultQuests();
    
    QuestsScene scene;
    
    // Проверка что квесты загружены
    EXPECT_GT(getActiveQuestsCount(), 0);
}

TEST_F(QuestsSceneImprovedTest, MultipleUpdates) {
    QuestsScene scene;
    TestHelpers::simulateFrames(scene, 30);
}

// ============================================================================
// Integration Tests - Using Multiple Systems
// ============================================================================

class IntegrationTest : public SceneTestFixture {};

TEST_F(IntegrationTest, ExperienceAndNPCTogether) {
    // Тест интеграции нескольких систем
    ExperienceSystem::getInstance().addExperience(100);
    NPCManager::getInstance().addNPC(NPCFactory::createMechanic());
    
    EXPECT_GE(ExperienceSystem::getInstance().getLevel(), 1);
    EXPECT_EQ(NPCManager::getInstance().getTeamSize(), 1);
}

TEST_F(IntegrationTest, AllSystemsInitialize) {
    // Проверка что все системы инициализируются корректно
    EXPECT_NO_THROW({
        ExperienceSystem::getInstance();
        NPCManager::getInstance();
        QuestManager::getInstance();
    });
}

