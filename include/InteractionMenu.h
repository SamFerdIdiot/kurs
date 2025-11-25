#ifndef INTERACTION_MENU_H
#define INTERACTION_MENU_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "Location.h"
#include "PlayerState.h"

// Action structure with resource requirements
struct InteractionAction {
    std::string name;        // Display name
    float moneyCost;         // Money cost (0 if free)
    float energyChange;      // Energy change (negative = cost, positive = gain)
    float fuelChange;        // Fuel change (negative = cost, positive = gain)
    std::string result;      // Result message to display
    
    InteractionAction(const std::string& n, float money = 0.0f, 
                     float energy = 0.0f, float fuel = 0.0f, 
                     const std::string& res = "")
        : name(n), moneyCost(money), energyChange(energy), 
          fuelChange(fuel), result(res) {}
    
    // Check if player can afford this action
    bool canAfford(const PlayerState& state) const;
    
    // Get display string with cost info
    std::string getDisplayString() const;
};

// InteractionMenu displays a popup menu when player is near a structure
class InteractionMenu {
public:
    InteractionMenu();
    
    // Show menu for a specific location
    void show(std::shared_ptr<Location> location);
    
    // Hide the menu
    void hide();
    
    // Проверить видимо ли меню в данный момент
    bool isVisible() const { return m_isVisible; }
    
    // Get the current location being interacted with
    std::shared_ptr<Location> getCurrentLocation() const { return m_currentLocation; }
    
    // Handle input events (returns true if input was consumed)
    bool handleInput(const sf::Event& event);
    
    // Update menu state
    void update(float deltaTime);
    
    // Render the menu
    void render(sf::RenderWindow& window);
    
    // Get selected action (returns nullptr if none or "Leave")
    const InteractionAction* getSelectedAction() const;
    
    // Set player state reference for checking affordability
    void setPlayerState(PlayerState* state) { m_playerState = state; }
    
    // Set result message (displayed after action)
    void setResultMessage(const std::string& message) { m_resultMessage = message; }
    
private:
    bool m_isVisible;
    std::shared_ptr<Location> m_currentLocation;
    std::vector<InteractionAction> m_actions;
    int m_selectedIndex;
    PlayerState* m_playerState;
    std::string m_resultMessage;
    float m_resultMessageTimer;
    
    // Visual elements
    sf::RectangleShape m_background;
    sf::RectangleShape m_titleBar;
    sf::Font m_font;
    bool m_fontLoaded;
    sf::Text m_titleText;
    std::vector<sf::Text> m_actionTexts;
    sf::RectangleShape m_selectionIndicator;
    sf::Text m_resultText;
    
    // Helper functions
    void setupMenu();
    void updateActionList();
    void updateVisualElements();
};

#endif // INTERACTION_MENU_H
