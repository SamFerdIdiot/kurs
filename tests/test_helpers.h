#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "ExperienceSystem.h"
#include "NPC.h"
#include "QuestManager.h"
#include "GameStateManager.h"

// ============================================================================
// TEST MACROS - Упрощают создание типовых тестов
// ============================================================================

// Макрос для тестирования инициализации сцены
#define TEST_SCENE_INITIALIZES(SceneClass) \
    TEST_F(SceneClass##Test, SceneInitializes) { \
        EXPECT_NO_THROW({ \
            SceneClass scene; \
        }); \
    }

// Макрос для тестирования начального состояния (не завершена)
#define TEST_SCENE_INITIAL_STATE_NOT_FINISHED(SceneClass) \
    TEST_F(SceneClass##Test, InitialStateIsNotFinished) { \
        SceneClass scene; \
        EXPECT_FALSE(scene.isFinished()); \
    }

// Макрос для тестирования ESC навигации
#define TEST_SCENE_ESC_RETURNS_TO_NODE(SceneClass) \
    TEST_F(SceneClass##Test, EscapeKeyFinishesScene) { \
        SceneClass scene; \
        sf::Event event; \
        event.type = sf::Event::KeyPressed; \
        event.key.code = sf::Keyboard::Escape; \
        scene.handleInput(event); \
        EXPECT_TRUE(scene.isFinished()); \
        EXPECT_EQ(scene.getNextScene(), SceneType::NODE); \
    }

// Макрос для тестирования update без краша
#define TEST_SCENE_UPDATE_NO_CRASH(SceneClass) \
    TEST_F(SceneClass##Test, UpdateDoesNotCrash) { \
        SceneClass scene; \
        EXPECT_NO_THROW({ \
            scene.update(0.016f); \
        }); \
    }

// Комбинированный макрос для всех базовых тестов сцены
#define TEST_SCENE_BASIC_FUNCTIONALITY(SceneClass) \
    TEST_SCENE_INITIALIZES(SceneClass) \
    TEST_SCENE_INITIAL_STATE_NOT_FINISHED(SceneClass) \
    TEST_SCENE_ESC_RETURNS_TO_NODE(SceneClass) \
    TEST_SCENE_UPDATE_NO_CRASH(SceneClass)

// ============================================================================
// FIXTURE КЛАССЫ - Общая настройка для тестов
// ============================================================================

// Базовый фикстур для всех тестов сцен
class SceneTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Сброс всех синглтонов перед каждым тестом
        ExperienceSystem::getInstance().reset();
        NPCManager::getInstance().clear();
        QuestManager::getInstance().clear();
    }
    
    void TearDown() override {
        // Очистка после теста
    }
};

// Фикстур для тестов с ExperienceSystem
class ExperienceSystemTestFixture : public SceneTestFixture {
protected:
    void SetUp() override {
        SceneTestFixture::SetUp();
        // Дополнительная настройка для ExperienceSystem
    }
    
    void addExperience(int amount) {
        ExperienceSystem::getInstance().addExperience(amount);
    }
    
    int getCurrentLevel() {
        return ExperienceSystem::getInstance().getLevel();
    }
};

// Фикстур для тестов с NPCManager
class NPCManagerTestFixture : public SceneTestFixture {
protected:
    void SetUp() override {
        SceneTestFixture::SetUp();
        // Дополнительная настройка для NPCManager
    }
    
    void addTestNPC(const std::string& type = "mechanic") {
        NPCManager& manager = NPCManager::getInstance();
        if (type == "mechanic") {
            manager.addNPC(NPCFactory::createMechanic());
        } else if (type == "trader") {
            manager.addNPC(NPCFactory::createTrader());
        } else if (type == "companion") {
            manager.addNPC(NPCFactory::createCompanion());
        }
    }
    
    int getNPCCount() {
        return NPCManager::getInstance().getTeamSize();
    }
};

// Фикстур для тестов с QuestManager
class QuestManagerTestFixture : public SceneTestFixture {
protected:
    void SetUp() override {
        SceneTestFixture::SetUp();
        QuestManager::getInstance().initializeDefaultQuests();
    }
    
    int getActiveQuestsCount() {
        return static_cast<int>(QuestManager::getInstance().getActiveQuests().size());
    }
};

// ============================================================================
// HELPER ФУНКЦИИ - Упрощают создание тестовых данных
// ============================================================================

namespace TestHelpers {
    
    // Создание тестового события клавиатуры
    inline sf::Event createKeyEvent(sf::Keyboard::Key key, bool pressed = true) {
        sf::Event event;
        event.type = pressed ? sf::Event::KeyPressed : sf::Event::KeyReleased;
        event.key.code = key;
        return event;
    }
    
    // Создание тестового события мыши (движение)
    inline sf::Event createMouseMoveEvent(int x, int y) {
        sf::Event event;
        event.type = sf::Event::MouseMoved;
        event.mouseMove.x = x;
        event.mouseMove.y = y;
        return event;
    }
    
    // Создание тестового события мыши (клик)
    inline sf::Event createMouseClickEvent(sf::Mouse::Button button, int x, int y, bool pressed = true) {
        sf::Event event;
        event.type = pressed ? sf::Event::MouseButtonPressed : sf::Event::MouseButtonReleased;
        event.mouseButton.button = button;
        event.mouseButton.x = x;
        event.mouseButton.y = y;
        return event;
    }
    
    // Проверка, что сцена корректно обрабатывает навигационные клавиши
    template<typename SceneType>
    void testNavigationKeys(SceneType& scene) {
        // Тест Up
        sf::Event upEvent = createKeyEvent(sf::Keyboard::Up);
        EXPECT_NO_THROW(scene.handleInput(upEvent));
        
        // Тест Down
        sf::Event downEvent = createKeyEvent(sf::Keyboard::Down);
        EXPECT_NO_THROW(scene.handleInput(downEvent));
        
        // Тест Left (если применимо)
        sf::Event leftEvent = createKeyEvent(sf::Keyboard::Left);
        EXPECT_NO_THROW(scene.handleInput(leftEvent));
        
        // Тест Right (если применимо)
        sf::Event rightEvent = createKeyEvent(sf::Keyboard::Right);
        EXPECT_NO_THROW(scene.handleInput(rightEvent));
    }
    
    // Проверка, что сцена обрабатывает мышь без краша
    template<typename SceneType>
    void testMouseHandling(SceneType& scene, int gridX = 200, int gridY = 200) {
        // Тест движения мыши
        sf::Event moveEvent = createMouseMoveEvent(gridX, gridY);
        EXPECT_NO_THROW(scene.handleInput(moveEvent));
        
        // Тест левого клика
        sf::Event leftClick = createMouseClickEvent(sf::Mouse::Left, gridX, gridY);
        EXPECT_NO_THROW(scene.handleInput(leftClick));
        
        // Тест правого клика
        sf::Event rightClick = createMouseClickEvent(sf::Mouse::Right, gridX, gridY);
        EXPECT_NO_THROW(scene.handleInput(rightClick));
    }
    
    // Создание тестового NPC с заданными параметрами
    inline std::unique_ptr<NPC> createTestNPC(
        const std::string& name = "Test NPC",
        const std::string& bio = "Test Bio",
        int level = 1
    ) {
        return std::make_unique<NPC>("test_npc_id", name, bio, level);
    }
    
    // Симуляция нескольких обновлений сцены (для тестирования анимаций)
    template<typename SceneType>
    void simulateFrames(SceneType& scene, int frameCount = 60, float deltaTime = 0.016f) {
        for (int i = 0; i < frameCount; ++i) {
            EXPECT_NO_THROW(scene.update(deltaTime));
        }
    }
    
    // Проверка базовой функциональности любой сцены
    template<typename SceneType>
    void testSceneBasics(SceneType& scene) {
        // Проверка начального состояния
        EXPECT_FALSE(scene.isFinished());
        
        // Проверка update
        EXPECT_NO_THROW(scene.update(0.016f));
        
        // Проверка ESC
        sf::Event escEvent = createKeyEvent(sf::Keyboard::Escape);
        scene.handleInput(escEvent);
        EXPECT_TRUE(scene.isFinished());
    }
    
} // namespace TestHelpers

#endif // TEST_HELPERS_H
