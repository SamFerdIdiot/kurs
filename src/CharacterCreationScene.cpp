#include "CharacterCreationScene.h"
#include "GameStateManager.h"
#include "GameTypes.h"
#include <iostream>

// Constructor
CharacterCreationScene::CharacterCreationScene()
    : m_isFinished(false),
      m_nextScene(SceneType::CHARACTER_CREATION) {
    
    // Initialize the menu
    if (!m_menu.initialize("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        std::cerr << "Failed to initialize character creation menu" << std::endl;
    }
}

// Handle input
void CharacterCreationScene::handleInput(const sf::Event& event) {
    m_menu.handleInput(event);
    
    // Check if menu is complete or user wants to go back
    if (m_menu.isComplete()) {
        // Get selections and apply to player state
        int backgroundIndex = m_menu.getSelectedBackground();
        int carIndex = m_menu.getSelectedCar();
        
        // Map to OriginType and CarType
        OriginType origin;
        switch (backgroundIndex) {
            case 0: origin = OriginType::EX_RACER; break;
            case 1: origin = OriginType::NOMAD; break; // Student -> Nomad mapping
            case 2: origin = OriginType::HITCHHIKER; break; // Unemployed -> Hitchhiker mapping
            default: origin = OriginType::UNKNOWN; break;
        }
        
        CarType carType;
        switch (carIndex) {
            case 0: carType = CarType::OLD_SEDAN; break; // Volga
            case 1: carType = CarType::OFFROAD_VAN; break; // Zhiguli
            case 2: carType = CarType::SPORTS_COUPE; break; // Moskvich
            default: carType = CarType::UNKNOWN; break;
        }
        
        // Start new game with chosen origin and car type
        GameStateManager::getInstance().startNewGame(origin, carType);
        
        // Transition to first game scene (node scene at starting location)
        m_nextScene = SceneType::NODE;
        m_isFinished = true;
        
        std::cout << "Character created: Background=" << backgroundIndex 
                  << ", Car=" << carIndex << std::endl;
    }
    
    if (m_menu.wantsToGoBack()) {
        m_nextScene = SceneType::MAIN_MENU;
        m_isFinished = true;
    }
}

// Update
void CharacterCreationScene::update(float deltaTime) {
    m_menu.update(deltaTime);
}

// Render
void CharacterCreationScene::render(sf::RenderWindow& window) {
    m_menu.render(window);
}

// Get next scene
SceneType CharacterCreationScene::getNextScene() const {
    return m_nextScene;
}

// Check if finished
bool CharacterCreationScene::isFinished() const {
    return m_isFinished;
}
