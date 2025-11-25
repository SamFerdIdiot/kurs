#ifndef INVENTORY_SCENE_H
#define INVENTORY_SCENE_H

#include "Scene.h"
#include "GridInventorySystem.h"
#include "UILayout.h"
#include <SFML/Graphics.hpp>
#include <memory>

// Inventory scene with Tarkov-style grid inventory
// Purpose: Manage player's items in a spatial 2D grid
// Features:
//   - Grid-based inventory (6x4 = 24 cells)
//   - Items occupy multiple cells based on size
//   - Mouse-only interaction within inventory bounds
//   - Drag and drop item placement
//   - Item rotation (90 degrees)
//   - Collision detection (no overlapping)
//   - Visual grid rendering with color-coded cells
//   - Item details panel
//   - Weight and capacity display
// ⚠️ MOUSE INTERACTION ONLY - Mouse restricted to inventory bounds
class InventoryScene : public Scene {
public:
    InventoryScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Grid inventory system
    std::unique_ptr<GridInventorySystem> m_gridInventory;
    
    // UI elements
    sf::RectangleShape m_backgroundBox;     // Semi-transparent overlay
    sf::RectangleShape m_gridContainer;     // Grid container panel
    sf::RectangleShape m_detailPanel;       // Item details panel
    sf::RectangleShape m_statsPanel;        // Weight/capacity stats panel
    sf::RectangleShape m_closeButton;       // Close button
    
    // Grid rendering
    std::vector<sf::RectangleShape> m_gridCells;  // Visual grid cells
    const float m_cellSize = 80.0f;               // Size of each grid cell
    const float m_gridStartX = 100.0f;            // Grid origin X
    const float m_gridStartY = 150.0f;            // Grid origin Y
    
    // Mouse interaction
    sf::Vector2i m_mousePosition;           // Current mouse position
    int m_hoveredCellX;                     // Hovered grid cell X (-1 if none)
    int m_hoveredCellY;                     // Hovered grid cell Y (-1 if none)
    int m_draggedItemId;                    // ID of item being dragged (-1 if none)
    sf::Vector2i m_dragOffset;              // Offset from item origin to mouse
    bool m_isDragging;                      // Is currently dragging an item
    
    // Font and text elements
    sf::Font m_font;
    sf::Text m_titleText;
    sf::Text m_detailText;
    sf::Text m_statsText;
    sf::Text m_closeButtonText;
    sf::Text m_controlsText;
    bool m_fontLoaded;
    
    // Scene state
    bool m_isFinished;
    SceneType m_nextScene;
    SceneType m_returnToScene;              // Scene to return to when closing
    
    // Helper methods
    void initializeGridCells();
    void updateHoveredCell();
    void renderGrid(sf::RenderWindow& window);
    void renderItems(sf::RenderWindow& window);
    void renderDraggedItem(sf::RenderWindow& window);
    void renderItemDetails(sf::RenderWindow& window);
    void updateStats();
    
    // Mouse interaction helpers
    bool isMouseInInventoryBounds(int mouseX, int mouseY) const;
    void screenToGrid(int screenX, int screenY, int& gridX, int& gridY) const;
    void gridToScreen(int gridX, int gridY, float& screenX, float& screenY) const;
    void handleLeftClick();
    void handleRightClick();
    void startDragging(int itemId);
    void stopDragging();
    
    // Add some test items for demonstration
    void addTestItems();
};

#endif // INVENTORY_SCENE_H
