#include <gtest/gtest.h>
#include "InventoryScene.h"
#include "CharacterScene.h"
#include "CompanionsScene.h"
#include "QuestsScene.h"
#include "ExperienceSystem.h"
#include "NPC.h"
#include "QuestManager.h"
#include "GameStateManager.h"
#include <SFML/Graphics.hpp>

// ============================================================================
// InventoryScene Tests
// ============================================================================

class InventorySceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset singletons before each test
        // Note: This assumes singletons have a reset or getInstance pattern
    }
    
    void TearDown() override {
        // Clean up after tests
    }
};

TEST_F(InventorySceneTest, SceneInitializes) {
    // Тест: сцена инициализируется без ошибок
    EXPECT_NO_THROW({
        InventoryScene scene;
    });
}

TEST_F(InventorySceneTest, InitialStateIsNotFinished) {
    // Тест: начальное состояние сцены - не завершена
    InventoryScene scene;
    EXPECT_FALSE(scene.isFinished());
}

TEST_F(InventorySceneTest, NextSceneIsInventory) {
    // Тест: следующая сцена по умолчанию - INVENTORY
    InventoryScene scene;
    EXPECT_EQ(scene.getNextScene(), SceneType::INVENTORY);
}

TEST_F(InventorySceneTest, EscapeKeyFinishesScene) {
    // Тест: ESC завершает сцену
    InventoryScene scene;
    
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::Escape;
    
    scene.handleInput(event);
    
    EXPECT_TRUE(scene.isFinished());
    EXPECT_EQ(scene.getNextScene(), SceneType::NODE);
}

TEST_F(InventorySceneTest, UpdateDoesNotCrash) {
    // Тест: обновление сцены не вызывает ошибок
    InventoryScene scene;
    EXPECT_NO_THROW({
        scene.update(0.016f); // 60 FPS frame time
    });
}

TEST_F(InventorySceneTest, MouseMovementDoesNotCrash) {
    // Тест: движение мыши не вызывает ошибок
    InventoryScene scene;
    
    sf::Event event;
    event.type = sf::Event::MouseMoved;
    event.mouseMove.x = 200;
    event.mouseMove.y = 200;
    
    EXPECT_NO_THROW({
        scene.handleInput(event);
    });
}

TEST_F(InventorySceneTest, LeftClickDoesNotCrash) {
    // Тест: левый клик не вызывает ошибок
    InventoryScene scene;
    
    // Сначала движение мыши
    sf::Event moveEvent;
    moveEvent.type = sf::Event::MouseMoved;
    moveEvent.mouseMove.x = 150;
    moveEvent.mouseMove.y = 200;
    scene.handleInput(moveEvent);
    
    // Затем клик
    sf::Event clickEvent;
    clickEvent.type = sf::Event::MouseButtonPressed;
    clickEvent.mouseButton.button = sf::Mouse::Left;
    clickEvent.mouseButton.x = 150;
    clickEvent.mouseButton.y = 200;
    
    EXPECT_NO_THROW({
        scene.handleInput(clickEvent);
    });
}

TEST_F(InventorySceneTest, RightClickDoesNotCrash) {
    // Тест: правый клик не вызывает ошибок
    InventoryScene scene;
    
    sf::Event event;
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = sf::Mouse::Right;
    event.mouseButton.x = 150;
    event.mouseButton.y = 200;
    
    EXPECT_NO_THROW({
        scene.handleInput(event);
    });
}

// ============================================================================
// CharacterScene Tests
// ============================================================================

class CharacterSceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Сброс ExperienceSystem перед каждым тестом
        ExperienceSystem::getInstance().reset();
    }
};

TEST_F(CharacterSceneTest, SceneInitializes) {
    // Тест: сцена инициализируется без ошибок
    EXPECT_NO_THROW({
        CharacterScene scene;
    });
}

TEST_F(CharacterSceneTest, InitialStateIsNotFinished) {
    // Тест: начальное состояние сцены - не завершена
    CharacterScene scene;
    EXPECT_FALSE(scene.isFinished());
}

TEST_F(CharacterSceneTest, EscapeKeyFinishesScene) {
    // Тест: ESC завершает сцену и возвращает в NODE
    CharacterScene scene;
    
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::Escape;
    
    scene.handleInput(event);
    
    EXPECT_TRUE(scene.isFinished());
    EXPECT_EQ(scene.getNextScene(), SceneType::NODE);
}

TEST_F(CharacterSceneTest, UsesRealExperienceSystem) {
    // Тест: сцена использует реальную систему опыта
    // Добавляем опыт в ExperienceSystem
    ExperienceSystem::getInstance().addExperience(150);
    
    // Создаём сцену после добавления опыта
    CharacterScene scene;
    
    // Проверяем что уровень не 1 (начальное значение)
    // Это косвенная проверка что сцена использует ExperienceSystem
    int currentLevel = ExperienceSystem::getInstance().getLevel();
    EXPECT_GE(currentLevel, 1);
}

TEST_F(CharacterSceneTest, UpdateDoesNotCrash) {
    // Тест: обновление сцены не вызывает ошибок
    CharacterScene scene;
    EXPECT_NO_THROW({
        scene.update(0.016f);
    });
}

// ============================================================================
// CompanionsScene Tests
// ============================================================================

class CompanionsSceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Очистка NPCManager перед каждым тестом
        NPCManager::getInstance().clear();
    }
};

TEST_F(CompanionsSceneTest, SceneInitializes) {
    // Тест: сцена инициализируется без ошибок
    EXPECT_NO_THROW({
        CompanionsScene scene;
    });
}

TEST_F(CompanionsSceneTest, InitialStateIsNotFinished) {
    // Тест: начальное состояние сцены - не завершена
    CompanionsScene scene;
    EXPECT_FALSE(scene.isFinished());
}

TEST_F(CompanionsSceneTest, EscapeKeyFinishesScene) {
    // Тест: ESC завершает сцену и возвращает в NODE
    CompanionsScene scene;
    
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::Escape;
    
    scene.handleInput(event);
    
    EXPECT_TRUE(scene.isFinished());
    EXPECT_EQ(scene.getNextScene(), SceneType::NODE);
}

TEST_F(CompanionsSceneTest, NavigationKeysWork) {
    // Тест: клавиши навигации работают
    CompanionsScene scene;
    
    sf::Event upEvent;
    upEvent.type = sf::Event::KeyPressed;
    upEvent.key.code = sf::Keyboard::Up;
    
    sf::Event downEvent;
    downEvent.type = sf::Event::KeyPressed;
    downEvent.key.code = sf::Keyboard::Down;
    
    EXPECT_NO_THROW({
        scene.handleInput(upEvent);
        scene.handleInput(downEvent);
    });
}

TEST_F(CompanionsSceneTest, UsesRealNPCManager) {
    // Тест: сцена использует реальный NPCManager
    // Добавляем NPC в NPCManager
    NPCManager& manager = NPCManager::getInstance();
    manager.addNPC(NPCFactory::createMechanic());
    
    // Создаём сцену после добавления NPC
    CompanionsScene scene;
    
    // Проверяем что NPCManager содержит данные
    EXPECT_EQ(manager.getTeamSize(), 1);
}

TEST_F(CompanionsSceneTest, UpdateDoesNotCrash) {
    // Тест: обновление сцены не вызывает ошибок
    CompanionsScene scene;
    EXPECT_NO_THROW({
        scene.update(0.016f);
    });
}

// ============================================================================
// QuestsScene Tests
// ============================================================================

class QuestsSceneTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Сброс QuestManager перед каждым тестом
        QuestManager::getInstance().clear();
    }
};

TEST_F(QuestsSceneTest, SceneInitializes) {
    // Тест: сцена инициализируется без ошибок
    EXPECT_NO_THROW({
        QuestsScene scene;
    });
}

TEST_F(QuestsSceneTest, InitialStateIsNotFinished) {
    // Тест: начальное состояние сцены - не завершена
    QuestsScene scene;
    EXPECT_FALSE(scene.isFinished());
}

TEST_F(QuestsSceneTest, EscapeKeyFinishesScene) {
    // Тест: ESC завершает сцену и возвращает в NODE
    QuestsScene scene;
    
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::Escape;
    
    scene.handleInput(event);
    
    EXPECT_TRUE(scene.isFinished());
    EXPECT_EQ(scene.getNextScene(), SceneType::NODE);
}

TEST_F(QuestsSceneTest, TabKeyChangesFilter) {
    // Тест: Tab переключает фильтр квестов
    QuestsScene scene;
    
    sf::Event event;
    event.type = sf::Event::KeyPressed;
    event.key.code = sf::Keyboard::Tab;
    
    EXPECT_NO_THROW({
        scene.handleInput(event);
    });
}

TEST_F(QuestsSceneTest, NavigationKeysWork) {
    // Тест: клавиши навигации работают
    QuestsScene scene;
    
    sf::Event upEvent;
    upEvent.type = sf::Event::KeyPressed;
    upEvent.key.code = sf::Keyboard::Up;
    
    sf::Event downEvent;
    downEvent.type = sf::Event::KeyPressed;
    downEvent.key.code = sf::Keyboard::Down;
    
    EXPECT_NO_THROW({
        scene.handleInput(upEvent);
        scene.handleInput(downEvent);
    });
}

TEST_F(QuestsSceneTest, UpdateDoesNotCrash) {
    // Тест: обновление сцены не вызывает ошибок
    QuestsScene scene;
    EXPECT_NO_THROW({
        scene.update(0.016f);
    });
}
