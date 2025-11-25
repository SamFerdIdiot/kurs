#ifndef GARAGE_SERVICE_H
#define GARAGE_SERVICE_H

#include "Service.h"
#include "PlayerState.h"
#include <SFML/Graphics.hpp>
#include <vector>

// GarageService - Garage where player can repair car and refuel
// Purpose: Allow player to maintain their vehicle
// Features:
//   - Refuel car
//   - Repair car damage
//   - Display service prices and player's money
//   - Handle service selection and purchase
class GarageService : public Service {
public:
    GarageService();
    
    ServiceType getType() const override { return ServiceType::GARAGE; }
    std::string getName() const override { return "Garage"; }
    
    void displayMenu(sf::RenderWindow& window) override;
    bool handleInput(const sf::Event& event) override;
    bool execute(PlayerState& playerState) override;
    bool isAvailable(const PlayerState& playerState) const override;
    void update(float deltaTime) override;
    
private:
    // Garage service structure
    struct GarageOption {
        std::string name;
        float price;
        float fuelAmount;
    };
    
    std::vector<GarageOption> m_options;  // Available services
    int m_selectedOption;                  // Currently selected option
    bool m_isFinished;                     // Service interaction complete flag
    
    // Placeholder visual elements
    sf::RectangleShape m_backgroundBox;    // Garage background
    sf::RectangleShape m_menuBox;          // Menu container
    sf::RectangleShape m_optionBox1;       // Option 1 placeholder
    sf::RectangleShape m_optionBox2;       // Option 2 placeholder
    sf::RectangleShape m_optionBox3;       // Option 3 placeholder
    sf::RectangleShape m_selectorBox;      // Selection indicator
    sf::RectangleShape m_infoBox;          // Info/status box
    sf::RectangleShape m_carBox;           // Car display
};

#endif // GARAGE_SERVICE_H
