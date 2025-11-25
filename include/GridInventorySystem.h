#ifndef GRID_INVENTORY_SYSTEM_H
#define GRID_INVENTORY_SYSTEM_H

#include "Item.h"
#include <vector>
#include <string>
#include <memory>

// GridCell struct
// Purpose: Represents a single cell in the 2D inventory grid
// Each cell tracks if it's occupied and which item occupies it
struct GridCell {
    bool occupied;      // Is this cell occupied by an item
    int itemId;         // ID of the item occupying this cell (-1 if empty)
    
    GridCell() : occupied(false), itemId(-1) {}
};

// GridItem struct
// Purpose: Represents an item in the grid-based inventory with spatial properties
// Extends Item with grid position and size information
struct GridItem {
    Item item;              // The base item data
    int id;                 // Unique ID for this item instance
    int gridX;              // Top-left grid cell X position
    int gridY;              // Top-left grid cell Y position
    int gridWidth;          // Width in grid cells
    int gridHeight;         // Height in grid cells
    bool isRotated;         // Is the item rotated 90 degrees
    int stackCount;         // Number of items in stack (for stackables)
    
    GridItem()
        : item(), id(-1), gridX(0), gridY(0), 
          gridWidth(1), gridHeight(1), isRotated(false), stackCount(1) {}
    
    GridItem(const Item& baseItem, int itemId, int width, int height)
        : item(baseItem), id(itemId), gridX(0), gridY(0),
          gridWidth(width), gridHeight(height), isRotated(false), 
          stackCount(1) {}
};

// GridInventorySystem class
// Purpose: Tarkov-style grid-based inventory with spatial item placement
// Features:
//   - 2D grid layout (default 6x4 = 24 cells)
//   - Items occupy multiple cells based on size
//   - Item rotation (90 degrees)
//   - Drag and drop placement
//   - Collision detection (items cannot overlap)
//   - Weight-based capacity management
//   - Visual grid representation
//
// ⚠️ MOUSE-ONLY INTERACTION within inventory bounds
class GridInventorySystem {
public:
    // Constructor
    // Parameters:
    //   - gridWidth: Number of columns in the grid (default: 6)
    //   - gridHeight: Number of rows in the grid (default: 4)
    //   - maxWeight: Maximum carrying capacity (default: 100.0)
    GridInventorySystem(int gridWidth = 6, int gridHeight = 4, float maxWeight = 100.0f);
    
    // Add an item to the inventory at a specific grid position
    // Parameters:
    //   - item: The item to add
    //   - width: Item width in cells
    //   - height: Item height in cells
    //   - gridX: Target X position (if -1, auto-find position)
    //   - gridY: Target Y position (if -1, auto-find position)
    //   - count: Number of items to add (for stackables)
    // Returns: Item ID if successful, -1 if failed
    int addItem(const Item& item, int width, int height, int gridX = -1, int gridY = -1, int count = 1);
    
    // Check if an item can be placed at a specific position
    // Parameters:
    //   - gridX: Target X position
    //   - gridY: Target Y position
    //   - width: Item width in cells
    //   - height: Item height in cells
    //   - ignoreItemId: Item ID to ignore in collision check (for moving items)
    // Returns: True if placement is valid
    bool canPlaceItem(int gridX, int gridY, int width, int height, int ignoreItemId = -1) const;
    
    // Place an item at a specific grid position
    // Parameters:
    //   - itemId: ID of the item to place
    //   - gridX: Target X position
    //   - gridY: Target Y position
    // Returns: True if placement successful
    bool placeItem(int itemId, int gridX, int gridY);
    
    // Remove an item from the grid
    // Parameters:
    //   - itemId: ID of the item to remove
    // Returns: True if removal successful
    bool removeItem(int itemId);
    
    // Get item at a specific grid cell
    // Parameters:
    //   - gridX: Cell X position
    //   - gridY: Cell Y position
    // Returns: Pointer to GridItem if found, nullptr otherwise
    GridItem* getItemAtCell(int gridX, int gridY);
    const GridItem* getItemAtCell(int gridX, int gridY) const;
    
    // Get item by ID
    // Parameters:
    //   - itemId: Item ID
    // Returns: Pointer to GridItem if found, nullptr otherwise
    GridItem* getItemById(int itemId);
    const GridItem* getItemById(int itemId) const;
    
    // Rotate an item 90 degrees
    // Parameters:
    //   - itemId: ID of the item to rotate
    // Returns: True if rotation successful
    bool rotateItem(int itemId);
    
    // Find first available position for an item
    // Parameters:
    //   - width: Item width in cells
    //   - height: Item height in cells
    //   - outX: Output X position
    //   - outY: Output Y position
    // Returns: True if position found
    bool findAvailablePosition(int width, int height, int& outX, int& outY) const;
    
    // Get total weight of all items in inventory
    // Returns: Total weight
    float getTotalWeight() const;
    
    // Get maximum weight capacity
    // Returns: Max weight
    float getMaxWeight() const;
    
    // Set maximum weight capacity
    void setMaxWeight(float maxWeight);
    
    // Get grid dimensions
    int getGridWidth() const { return m_gridWidth; }
    int getGridHeight() const { return m_gridHeight; }
    
    // Get number of items in inventory
    int getItemCount() const { return static_cast<int>(m_items.size()); }
    
    // Get all items
    const std::vector<GridItem>& getItems() const { return m_items; }
    
    // Check if inventory has specific item
    // Parameters:
    //   - itemName: Name of the item
    //   - minCount: Minimum count required
    // Returns: True if item exists with at least minCount
    bool hasItem(const std::string& itemName, int minCount = 1) const;
    
    // Get total count of a specific item
    // Parameters:
    //   - itemName: Name of the item
    // Returns: Total count across all stacks
    int getItemCount(const std::string& itemName) const;
    
    // Clear the entire inventory
    void clear();
    
    // Drop an item (remove without returning)
    // Parameters:
    //   - itemId: ID of item to drop
    // Returns: True if dropped successfully
    bool dropItem(int itemId);
    
private:
    std::vector<std::vector<GridCell>> m_grid;  // 2D grid of cells
    std::vector<GridItem> m_items;              // All items in inventory
    int m_gridWidth;                            // Grid width in cells
    int m_gridHeight;                           // Grid height in cells
    float m_maxWeight;                          // Maximum carrying capacity
    int m_nextItemId;                           // Next item ID to assign
    
    // Validate grid position
    // Returns: True if position is within grid bounds
    bool isValidPosition(int gridX, int gridY) const;
    
    // Mark grid cells as occupied by an item
    void occupyCells(int itemId, int gridX, int gridY, int width, int height);
    
    // Clear grid cells occupied by an item
    void clearCells(int itemId);
    
    // Get actual dimensions considering rotation
    void getActualDimensions(const GridItem& item, int& width, int& height) const;
};

#endif // GRID_INVENTORY_SYSTEM_H
