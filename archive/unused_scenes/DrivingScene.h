#ifndef DRIVING_SCENE_H
#define DRIVING_SCENE_H

#include "Scene.h"
#include "DrivingController.h"
#include "MainSceneOverlay.h"
#include "StartZone.h"
#include "InteractionMenu.h"
#include "PlayerState.h"
#include "TutorialSystem.h"
#include "POIManager.h"
#include "EventManager.h"
#include "UI/HUDPanel.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

// Driving/Road scene
// Purpose: Main gameplay scene where player drives on the road
// Features:
//   - Display scrolling road environment
//   - Show player's car and other vehicles
//   - Display HUD with speed, fuel, money, energy, time
//   - Handle driving controls (acceleration, braking, lane changes)
//   - Random events and encounters (NPCs, roadblocks, towns, motels)
//   - Transition to town/motel when arriving at destinations
//   - Fuel consumption and energy depletion
//   - POI-based stopping system
//   - Random event triggering with cinematic mode
class DrivingScene : public Scene {
public:
    DrivingScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Прямоугольники-заглушки для дорожного окружения
    sf::RectangleShape m_skyBox;            // Sky background
    sf::RectangleShape m_mountainBox;       // Mountains/hills in background
    sf::RectangleShape m_roadBox;           // Road surface
    sf::RectangleShape m_roadLine1;         // Lane marking 1
    sf::RectangleShape m_roadLine2;         // Lane marking 2
    sf::RectangleShape m_playerCarBox;      // Player's car
    sf::RectangleShape m_npcCar1Box;        // NPC car 1
    sf::RectangleShape m_npcCar2Box;        // NPC car 2
    
    // Элементы HUD
    sf::RectangleShape m_hudBackground;     // HUD background bar
    sf::RectangleShape m_speedBox;          // Speed indicator
    sf::RectangleShape m_fuelBox;           // Fuel gauge
    sf::RectangleShape m_moneyBox;          // Money counter
    sf::RectangleShape m_energyBox;         // Energy/stamina bar
    sf::RectangleShape m_timeBox;           // Time/date display
    sf::RectangleShape m_weatherBox;        // Weather indicator
    sf::RectangleShape m_inventoryIcon;     // Inventory button/icon
    
    // Font and text elements for HUD
    sf::Font m_font;                        // Font for text rendering
    sf::Text m_speedText;                   // Speed text
    sf::Text m_fuelText;                    // Fuel text
    sf::Text m_moneyText;                   // Money text
    sf::Text m_energyText;                  // Energy text
    sf::Text m_timeText;                    // Time text
    sf::Text m_weatherText;                 // Weather text
    sf::Text m_inventoryText;               // Inventory text
    bool m_fontLoaded;                      // Font loading status
    
    // Переменные состояния игры
    float m_speed;                          // Current speed
    bool m_isFinished;                      // Scene completion flag
    SceneType m_nextScene;                  // Next scene to transition to
    PlayerState m_playerState;              // Player resources (fuel, money, energy)
    bool m_firstTime;                       // Track if this is first time entering scene
    
    // POI and Event systems
    std::unique_ptr<POIManager> m_poiManager;        // Manages points of interest
    std::unique_ptr<EventManager> m_eventManager;    // Manages random events
    float m_travelDistance;                          // Distance traveled
    float m_eventCheckDistance;                      // Distance since last event check
    GameEvent* m_pendingEvent;                       // Event waiting to be shown
    
    // Модульные игровые компоненты
    std::unique_ptr<DrivingController> m_drivingController;  // Handles player car movement
    std::unique_ptr<MainSceneOverlay> m_overlay;             // Manages overlay for events/pauses
    std::unique_ptr<StartZone> m_startZone;                  // Start location and structures
    std::unique_ptr<InteractionMenu> m_interactionMenu;      // Popup menu for interactions
    std::unique_ptr<HUDPanel> m_hudPanel;                    // Retro-style HUD panel
    std::unique_ptr<TutorialSystem> m_tutorialSystem;        // First-day tutorial
    
    // Вспомогательные методы
    void checkForNearbyLocations();
    void handleInteractionMenuAction(const std::string& action);
    void updateHUDDisplay();
    void initializePOIs();
    void checkForEvents();
    void triggerEvent(GameEvent* event);
};

#endif // DRIVING_SCENE_H
