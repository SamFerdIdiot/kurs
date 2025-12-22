#include "SceneManager.h"
#include "MainMenuScene.h"
// UI Scenes (commented out - will be implemented as Notebook submenus)
// #include "CharacterScene.h"
// #include "CompanionsScene.h"
// #include "QuestsScene.h"
// #include "AbilityTreeScene.h"
// #include "InventoryScene.h"
#include "Notebook/NotebookScene.h"  // Main gameplay system
#include "GameStateManager.h"
#include "EventHelper.h"
#include <stdexcept>
#include <iostream>

// Constructor
SceneManager::SceneManager(sf::RenderWindow& window)
    : m_window(window),
      m_currentSceneType(SceneType::MAIN_MENU) {
    
    // Start with main menu
    m_currentScene = createScene(SceneType::MAIN_MENU);
    
    if (!m_currentScene) {
        throw std::runtime_error("Failed to create main menu scene");
    }
}

// Create scene based on type
std::unique_ptr<Scene> SceneManager::createScene(SceneType type) {
    switch (type) {
        case SceneType::MAIN_MENU:
            std::cout << "Creating Main Menu Scene" << std::endl;
            return std::make_unique<MainMenuScene>();

        // UI Scenes - commented out, will be implemented as Notebook submenus
        // case SceneType::CHARACTER:
        //     std::cout << "Creating Character Scene" << std::endl;
        //     return std::make_unique<CharacterScene>();
        //
        // case SceneType::COMPANIONS:
        //     std::cout << "Creating Companions Scene" << std::endl;
        //     return std::make_unique<CompanionsScene>();
        //
        // case SceneType::QUESTS:
        //     std::cout << "Creating Quests Scene" << std::endl;
        //     return std::make_unique<QuestsScene>();
        //
        // case SceneType::ABILITY_TREE:
        //     std::cout << "Creating Ability Tree Scene" << std::endl;
        //     return std::make_unique<AbilityTreeScene>();
        //
        // case SceneType::INVENTORY:
        //     std::cout << "Creating Inventory Scene" << std::endl;
        //     return std::make_unique<InventoryScene>();

        case SceneType::NOTEBOOK: {
            std::cout << "Creating Notebook Scene" << std::endl;
            // Get player state from GameStateManager
            auto& playerState = GameStateManager::getInstance().getPlayerState();
            // Use saved entry ID, or default to tutorial_start
            std::string entryId = playerState.getCurrentNotebookEntryId();
            if (entryId.empty()) {
                entryId = "tutorial_start";
            }
            std::cout << "[SceneManager] Loading notebook entry: " << entryId << std::endl;
            return std::make_unique<NotebookScene>(&playerState, entryId);
        }

        case SceneType::EXIT:
            std::cout << "Exit requested" << std::endl;
            return nullptr;

        // Future features (Motel)
        case SceneType::MOTEL:
            std::cerr << "Motel scene not yet implemented, returning to main menu" << std::endl;
            return std::make_unique<MainMenuScene>();

        default:
            std::cerr << "Scene type not implemented yet, returning to main menu" << std::endl;
            return std::make_unique<MainMenuScene>();
    }
}

// Run game loop
void SceneManager::run() {
    sf::Clock clock;

    while (m_window.isOpen() && m_currentScene) {
        float deltaTime = clock.restart().asSeconds();

        // [MVP] Update playtime tracking
        auto& playerState = GameStateManager::getInstance().getPlayerState();
        playerState.updatePlayTime(deltaTime);

        // Handle events (SFML 3.x returns std::optional<sf::Event>)
        while (const std::optional<sf::Event> event = m_window.pollEvent()) {
            if (EventHelper::isClosed(*event)) {
                m_window.close();
                return;
            }

            // Pass event to current scene
            m_currentScene->handleInput(*event);
        }

        // Update current scene
        m_currentScene->update(deltaTime);
        
        // Check if scene is finished
        if (m_currentScene->isFinished()) {
            SceneType nextSceneType = m_currentScene->getNextScene();
            
            // Handle exit
            if (nextSceneType == SceneType::EXIT) {
                m_window.close();
                return;
            }
            
            // Transition to next scene
            m_currentSceneType = nextSceneType;
            m_currentScene = createScene(nextSceneType);
            
            if (!m_currentScene) {
                // If scene creation failed, close
                m_window.close();
                return;
            }
        }
        
        // Render current scene
        m_window.clear();
        m_currentScene->render(m_window);
        m_window.display();
    }
}
