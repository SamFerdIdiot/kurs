#ifndef LOCATION_SCENE_H
#define LOCATION_SCENE_H

#include "Scene.h"
#include "Location.h"
#include "PlayerState.h"
#include "FuelSystem.h"
#include "UI/HUDPanel.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <optional>

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
    void handleShop();            // Store: buy items (opens shop UI)
    void handleRepair();          // Garage/Mechanic: repair vehicle
    void handleEat();             // Diner: buy food/drinks
    void handleRest();            // Any location: quick rest if available
    void handleTalk();            // Talk to NPC if present
    void handleLeave();           // Exit location and return to driving

    // Shop system
    void initializeStoreInventory();  // Setup items for sale
    void enterShopMode();             // Enter shopping interface
    void exitShopMode();              // Exit shopping interface
    void handleShopInput(const sf::Event& event);  // Handle shop navigation
    void buySelectedItem();           // Buy from store
    void sellSelectedItem();          // Sell from inventory
    void renderShopUI(sf::RenderWindow& window);  // Render shop interface
    
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
    std::optional<sf::Text> m_locationNameText;  // SFML 3.x: Text requires font in constructor
    std::optional<sf::Text> m_instructionsText;  // SFML 3.x: Text requires font in constructor
    std::optional<sf::Text> m_statusText;        // SFML 3.x: Text requires font in constructor
    bool m_fontLoaded;
    
    // Interaction system (simple menu instead of InteractionMenu)
    bool m_showInteractionMenu;

    // Scene state
    LocationType m_locationType;        // Type of location (gas station, store, etc.)
    std::string m_locationName;         // Custom name for this location
    bool m_isFinished;                  // Scene completion flag
    SceneType m_nextScene;              // Next scene to transition to
    
    // HUD Panel for displaying resources
    HUDPanel m_hudPanel;

    // Shop system state
    bool m_isInShopMode;                    // Whether we're in shopping UI
    InventorySystem m_storeInventory;       // Store's inventory (items for sale)
    int m_selectedStoreIndex;               // Selected item in store list
    int m_selectedPlayerIndex;              // Selected item in player inventory
    int m_storeScrollOffset;                // Scroll offset for store list
    int m_playerScrollOffset;               // Scroll offset for player inventory
    bool m_isStoreListActive;               // Which list is active (true = store, false = inventory)

    // Refuel system state
    bool m_isInRefuelMode;                  // Whether we're in refuel UI
    std::vector<RefuelOption> m_refuelOptions;  // Available refuel options
    int m_selectedRefuelOption;             // Currently selected option

    // Refuel methods
    void enterRefuelMode();
    void exitRefuelMode();
    void handleRefuelInput(const sf::Event& event);
    void selectRefuelOption(int index);
    void confirmRefuel();
    void renderRefuelUI(sf::RenderWindow& window);

    // Helper to get location description
    std::string getLocationDescription() const;
};

#endif // LOCATION_SCENE_H
