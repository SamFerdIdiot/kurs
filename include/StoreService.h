#ifndef STORE_SERVICE_H
#define STORE_SERVICE_H

#include "Service.h"
#include "PlayerState.h"
#include <SFML/Graphics.hpp>
#include <vector>

// StoreService - Store where player can buy items and supplies
// Purpose: Allow player to purchase items that restore energy or provide benefits
// Features:
//   - Display available items for purchase
//   - Show item prices and player's money
//   - Handle item selection and purchase
//   - Update player inventory and money
class StoreService : public Service {
public:
    StoreService();
    
    ServiceType getType() const override { return ServiceType::STORE; }
    std::string getName() const override { return "Store"; }
    
    void displayMenu(sf::RenderWindow& window) override;
    bool handleInput(const sf::Event& event) override;
    bool execute(PlayerState& playerState) override;
    bool isAvailable(const PlayerState& playerState) const override;
    void update(float deltaTime) override;
    
private:
    // Store item structure
    struct Item {
        std::string name;
        float price;
        float energyRestore;
    };
    
    std::vector<Item> m_items;           // Available items
    int m_selectedItem;                   // Currently selected item
    bool m_isFinished;                    // Service interaction complete flag
    
    // Placeholder visual elements
    sf::RectangleShape m_backgroundBox;   // Store background
    sf::RectangleShape m_menuBox;         // Menu container
    sf::RectangleShape m_itemBox1;        // Item 1 placeholder
    sf::RectangleShape m_itemBox2;        // Item 2 placeholder
    sf::RectangleShape m_itemBox3;        // Item 3 placeholder
    sf::RectangleShape m_selectorBox;     // Selection indicator
    sf::RectangleShape m_infoBox;         // Info/status box
};

#endif // STORE_SERVICE_H
