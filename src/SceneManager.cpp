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
#include "LocationScene.h"
#include "TravelSelectionScene.h"
#include "TravelCinematicScene.h"  // [MVP] Travel cinematic scene
#include "NotesScene.h"
#include "EndGameScene.h"  // [MVP] End game scene
#include "Journal/JournalScene.h"  // [NEW] Unified journal system
#include "Notebook/NotebookScene.h"  // [NEWEST] Notebook with text printing animation
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

        case SceneType::END_GAME: {
            std::cout << "Creating End Game Scene" << std::endl;
            // [MVP] Get stats from GameStateManager
            auto& playerState = GameStateManager::getInstance().getPlayerState();
            float totalTime = playerState.getTotalPlayTime();
            int citiesVisited = playerState.getCurrentCityIndex() + 1;
            return std::make_unique<EndGameScene>(totalTime, citiesVisited);
        }

        case SceneType::INVENTORY:
            std::cout << "Creating Inventory Scene" << std::endl;
            return std::make_unique<InventoryScene>();

        case SceneType::LOCATION: {
            std::cout << "Creating Location Scene" << std::endl;
            // Get location type from GameStateManager
            LocationType locationType = GameStateManager::getInstance().getCurrentLocationType();
            return std::make_unique<LocationScene>(locationType);
        }

        case SceneType::TRAVEL_SELECTION:
            std::cout << "Creating Travel Selection Scene" << std::endl;
            // Hardcoded to "moscow" - dynamic city tracking not yet implemented
            return std::make_unique<TravelSelectionScene>("moscow");

        case SceneType::TRAVEL_CINEMATIC: {
            std::cout << "Creating Travel Cinematic Scene" << std::endl;

            // Get player's current city
            auto& playerState = GameStateManager::getInstance().getPlayerState();
            int currentCity = playerState.getCurrentCityIndex();
            // [MVP] Always travel forward to next city (currentCity + 1)
            int destinationCity = currentCity + 1;

            // Build road data with proper city IDs
            RoadData road;
            road.fromNodeId = "city_" + std::to_string(currentCity);
            road.toNodeId = "city_" + std::to_string(destinationCity);
            road.roadType = "highway";
            road.distance = 100.0f;
            road.baseEventChance = 0.0f;  // [MVP] no events yet

            std::cout << "[MVP] Traveling from " << road.fromNodeId << " to " << road.toNodeId << std::endl;

            return std::make_unique<TravelCinematicScene>(road, &playerState);
        }

        case SceneType::NOTES:
            std::cout << "Creating Notes Scene" << std::endl;
            return std::make_unique<NotesScene>();

        case SceneType::MAP:
            std::cout << "Creating Map Scene" << std::endl;
            return std::make_unique<MapScene>();

        case SceneType::JOURNAL: {
            std::cout << "Creating Journal Scene" << std::endl;
            // Get player state from GameStateManager
            auto& playerState = GameStateManager::getInstance().getPlayerState();
            // Start in Moscow
            return std::make_unique<JournalScene>(&playerState, "moscow");
        }

        case SceneType::NOTEBOOK: {
            std::cout << "Creating Notebook Scene" << std::endl;
            // Get player state from GameStateManager
            auto& playerState = GameStateManager::getInstance().getPlayerState();
            // Start with tutorial entry
            return std::make_unique<NotebookScene>(&playerState, "tutorial_start");
        }

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
