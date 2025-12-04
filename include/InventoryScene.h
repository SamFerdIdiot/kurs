#ifndef INVENTORY_SCENE_H
#define INVENTORY_SCENE_H

#include "Scene.h"
#include "Item.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

// Inventory scene with Fallout 4-style list inventory
// Purpose: Manage player's items in a simple list format
// Features:
//   - Vertical list of items
//   - Shows: Name, Weight, Value
//   - W/S navigation
//   - Enter to use/examine item
//   - Simple and lightweight
class InventoryScene : public Scene {
public:
    InventoryScene();

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;

private:
    // Inventory data
    std::vector<InventorySlot> m_items;      // List of items in inventory
    int m_selectedIndex;                      // Currently selected item index

    // UI elements
    sf::RectangleShape m_backgroundBox;      // Semi-transparent overlay
    sf::RectangleShape m_listContainer;      // List container panel
    sf::RectangleShape m_headerBar;          // Header bar for labels
    sf::RectangleShape m_selectionHighlight; // Selection highlight bar

    // Font and text elements
    sf::Font m_font;
    std::optional<sf::Text> m_titleText;        // Title "INVENTORY"
    std::optional<sf::Text> m_headerNameText;   // Header "NAME"
    std::optional<sf::Text> m_headerWeightText; // Header "WEIGHT"
    std::optional<sf::Text> m_headerValueText;  // Header "VALUE"
    std::optional<sf::Text> m_totalWeightText;  // Total weight display
    std::optional<sf::Text> m_controlsText;     // Controls help text
    bool m_fontLoaded;

    // Scene state
    bool m_isFinished;
    SceneType m_nextScene;
    SceneType m_returnToScene;              // Scene to return to when closing

    // List rendering
    const float m_listStartY = 200.0f;      // Y position where list starts
    const float m_itemHeight = 40.0f;       // Height of each item row
    const int m_maxVisibleItems = 12;       // Max items visible at once
    int m_scrollOffset;                     // Scroll offset for long lists

    // Helper methods
    void selectPrevious();
    void selectNext();
    void useSelectedItem();
    void renderItemList(sf::RenderWindow& window);
    void updateTotalWeight();

    // Add some test items for demonstration
    void addTestItems();
};

#endif // INVENTORY_SCENE_H
