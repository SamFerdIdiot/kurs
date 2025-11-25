#include "SceneManager.h"
#include "MainMenuScene.h"
#include "CharacterCreationScene.h"
#include "CinematicEventScene.h"
#include "NodeScene.h"
#include "CharacterScene.h"
#include "CompanionsScene.h"
#include "QuestsScene.h"
#include "AbilityTreeScene.h"
#include "MapScene.h"
#include "InventoryScene.h"
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
            
        case SceneType::CHARACTER_CREATION:
            std::cout << "Creating Character Creation Scene" << std::endl;
            return std::make_unique<CharacterCreationScene>();
            
        case SceneType::CINEMATIC_EVENT:
            std::cout << "Creating Cinematic Event Scene" << std::endl;
            return std::make_unique<CinematicEventScene>();
            
        case SceneType::NODE:
            std::cout << "Creating Node Scene" << std::endl;
            // Create node scene with starting node ID and name
            // In a real implementation, these would be determined by game state
            return std::make_unique<NodeScene>("node_0", "Starting Node");
            
        case SceneType::CHARACTER:
            std::cout << "Creating Character Scene" << std::endl;
            return std::make_unique<CharacterScene>();
            
        case SceneType::COMPANIONS:
            std::cout << "Creating Companions Scene" << std::endl;
            return std::make_unique<CompanionsScene>();
            
        case SceneType::QUESTS:
            std::cout << "Creating Quests Scene" << std::endl;
            return std::make_unique<QuestsScene>();
            
        case SceneType::ABILITY_TREE:
            std::cout << "Creating Ability Tree Scene" << std::endl;
            return std::make_unique<AbilityTreeScene>();
            
        case SceneType::WORLD_MAP:
            std::cout << "Creating World Map Scene" << std::endl;
            return std::make_unique<MapScene>();
            
        case SceneType::INVENTORY:
            std::cout << "Creating Inventory Scene" << std::endl;
            return std::make_unique<InventoryScene>();
            
        case SceneType::EXIT:
            std::cout << "Exit requested" << std::endl;
            return nullptr;
            
        // Add more scenes as they are implemented
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
        
        // Handle events
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                return;
            }
            
            // Pass event to current scene
            m_currentScene->handleInput(event);
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
