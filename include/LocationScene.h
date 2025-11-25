#ifndef LOCATION_SCENE_H
#define LOCATION_SCENE_H

#include "Scene.h"
#include "Location.h"
#include "InteractionMenu.h"
#include "UI/HUDPanel.hpp"
#include "UILayout.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// LocationScene - Generic scene for entering and interacting with locations
// Purpose: Unified scene for all location types (gas stations, stores, diners, garages, etc.)
// Features:
//   - Display location interior with contextual objects
//   - Show available interactions based on location type
//   - Handle resource transactions (fuel, money, energy)
//   - Support for buying items, services, and resting
//   - Integration with InteractionMenu for service selection
//   - Return to previous scene (driving/side view) when finished
class LocationScene : public Scene {
public:
    // Constructor that takes the location type to configure the scene
    LocationScene(LocationType locationType, const std::string& locationName = "");
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Initialize location-specific elements based on type
    void setupLocationElements();
    
    // Setup interaction options based on location type
    void setupInteractionOptions();
    
    // Handle specific interactions
    void handleRefuel();          // Gas station: refuel vehicle
    void handleShop();            // Store: buy items
    void handleRepair();          // Garage/Mechanic: repair vehicle
    void handleEat();             // Diner: buy food/drinks
    void handleRest();            // Any location: quick rest if available
    void handleTalk();            // Talk to NPC if present
    void handleLeave();           // Exit location and return to driving
    
    // Visual elements
    sf::RectangleShape m_background;        // Location interior background
    sf::RectangleShape m_counterBox;        // Counter/service area
    sf::RectangleShape m_playerBox;         // Player character
    sf::RectangleShape m_exitDoorBox;       // Exit door
    
    // Location-specific objects (displayed based on type)
    std::vector<sf::RectangleShape> m_objects;  // Shelves, pumps, tools, tables, etc.
    std::vector<sf::Text> m_objectLabels;       // Labels for interactive objects
    
    // UI elements
    sf::Font m_font;
    sf::Text m_locationNameText;        // Display location name (e.g., "Joe's Gas Station")
    sf::Text m_instructionsText;        // Instructions for player
    sf::Text m_statusText;              // Display current resources
    bool m_fontLoaded;
    
    // Interaction system
    std::unique_ptr<InteractionMenu> m_interactionMenu;
    bool m_showInteractionMenu;
    
    // Player state (maintains resources across interactions)
    PlayerState m_playerState;
    
    // Scene state
    LocationType m_locationType;        // Type of location (gas station, store, etc.)
    std::string m_locationName;         // Custom name for this location
    bool m_isFinished;                  // Scene completion flag
    SceneType m_nextScene;              // Next scene to transition to
    
    // HUD Panel for displaying resources
    HUDPanel m_hudPanel;
    
    // Helper to get location description
    std::string getLocationDescription() const;
};

#endif // LOCATION_SCENE_H
