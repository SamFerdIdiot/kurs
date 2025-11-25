#ifndef TOWN_MENU_H
#define TOWN_MENU_H

#include "Service.h"
#include "PlayerState.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// TownMenu - Menu system for town services
// Purpose: Display and manage town service interactions
// Features:
//   - Display list of available services
//   - Handle service selection
//   - Show player resources
//   - Manage service execution
//   - Coordinate service transitions
class TownMenu {
public:
    TownMenu();
    ~TownMenu();
    
    // Initialize menu with player state
    void initialize(PlayerState* playerState);
    
    // Display the town menu
    void display(sf::RenderWindow& window);
    
    // Handle input for menu navigation
    bool handleInput(const sf::Event& event);
    
    // Update menu state
    void update(float deltaTime);
    
    // Проверить активно ли меню (показывает услугу)
    bool isActive() const { return m_activeService != nullptr; }
    
    // Проверить завершено ли меню
    bool isFinished() const { return m_isFinished; }
    
    // Get the number of available services
    int getServiceCount() const { return m_services.size(); }
    
    // Get selected service index
    int getSelectedService() const { return m_selectedService; }
    
private:
    PlayerState* m_playerState;                          // Reference to player state
    std::vector<std::unique_ptr<Service>> m_services;    // Available services
    Service* m_activeService;                            // Currently active service
    int m_selectedService;                               // Currently selected service
    bool m_isFinished;                                   // Menu interaction complete flag
    
    // Placeholder visual elements for menu
    sf::RectangleShape m_backgroundBox;                  // Menu background
    sf::RectangleShape m_titleBox;                       // Title box
    sf::RectangleShape m_serviceBox1;                    // Service 1 placeholder
    sf::RectangleShape m_serviceBox2;                    // Service 2 placeholder
    sf::RectangleShape m_serviceBox3;                    // Service 3 placeholder
    sf::RectangleShape m_selectorBox;                    // Selection indicator
    sf::RectangleShape m_statsBox;                       // Player stats display
};

#endif // TOWN_MENU_H
