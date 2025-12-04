#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include "Item.h"
#include <vector>
#include <string>

// InventorySystem class
// Purpose: Manages the player's inventory of items
// Features:
//   - Add items to inventory (with auto-stacking for stackables)
//   - Remove items from inventory
//   - Use/consume items
//   - Get item at specific slot
//   - Check if inventory has specific item
//   - Get total weight and capacity management
//   - Sort and organize inventory
//   - Find items by name or category
//
// Заметки по дизайну:
//   - Uses a fixed-size vector of slots (can be made dynamic later)
//   - Supports automatic stacking of stackable items
//   - Weight-based carrying capacity system
//   - Can be extended with multiple inventory pages/tabs
//   - Events can be added for UI updates
class InventorySystem {
public:
    // Конструктор
    // Parameters:
    //   - maxSlots: Maximum number of inventory slots (default: 24)
    //   - maxWeight: Maximum carrying capacity in weight units (default: 100.0)
    InventorySystem(int maxSlots = 24, float maxWeight = 100.0f);
    
    // Add an item to the inventory
    // Parameters:
    //   - item: The item to add
    //   - count: Number of items to add (default: 1)
    // Returns: True if item was added successfully, false if inventory is full
    bool addItem(const Item& item, int count = 1);
    
    // Remove an item from a specific slot
    // Parameters:
    //   - slotIndex: Index of the slot to remove from
    //   - count: Number of items to remove (default: 1)
    // Returns: True if items were removed successfully
    bool removeItem(int slotIndex, int count = 1);
    
    // Remove items by name (searches all slots)
    // Parameters:
    //   - itemName: Name of the item to remove
    //   - count: Number of items to remove
    // Returns: Number of items actually removed
    int removeItemByName(const std::string& itemName, int count);
    
    // Use/consume an item from a specific slot
    // Parameters:
    //   - slotIndex: Index of the slot
    // Returns: True if item was used successfully
    // Note: Item effects/callbacks system not yet implemented
    bool useItem(int slotIndex);
    
    // Get the item at a specific slot
    // Parameters:
    //   - slotIndex: Index of the slot
    // Returns: Reference to the inventory slot (check isEmpty before using)
    const InventorySlot& getSlot(int slotIndex) const;
    
    // Get mutable reference to a slot (for advanced operations)
    InventorySlot& getSlotMutable(int slotIndex);
    
    // Check if inventory has a specific item
    // Parameters:
    //   - itemName: Name of the item to search for
    //   - minCount: Minimum count required (default: 1)
    // Returns: True if item exists with at least minCount
    bool hasItem(const std::string& itemName, int minCount = 1) const;
    
    // Get total count of a specific item across all slots
    // Parameters:
    //   - itemName: Name of the item to count
    // Returns: Total count of the item
    int getItemCount(const std::string& itemName) const;
    
    // Get the current total weight of all items
    // Returns: Total weight of inventory
    float getTotalWeight() const;
    
    // Get the maximum carrying capacity
    // Returns: Maximum weight capacity
    float getMaxWeight() const;
    
    // Set the maximum carrying capacity
    void setMaxWeight(float maxWeight);
    
    // Get the number of slots
    // Returns: Total number of inventory slots
    int getSlotCount() const;
    
    // Get the number of empty slots
    // Returns: Count of empty slots
    int getEmptySlotCount() const;
    
    // Check if inventory is full (no empty slots)
    // Returns: True if inventory has no empty slots
    bool isFull() const;
    
    // Check if adding an item would exceed weight capacity
    // Parameters:
    //   - item: The item to check
    //   - count: Number of items
    // Returns: True if item can be added without exceeding weight
    bool canAddItem(const Item& item, int count = 1) const;
    
    // Find first slot containing a specific item
    // Parameters:
    //   - itemName: Name of the item to find
    // Returns: Slot index, or -1 if not found
    int findItem(const std::string& itemName) const;
    
    // Find all slots containing a specific item
    // Parameters:
    //   - itemName: Name of the item to find
    // Returns: Vector of slot indices
    std::vector<int> findAllItems(const std::string& itemName) const;
    
    // Find items by category
    // Parameters:
    //   - category: Category to search for
    // Returns: Vector of slot indices
    std::vector<int> findItemsByCategory(ItemCategory category) const;
    
    // Sort inventory (stub for future implementation)
    // Note: Sorting not yet implemented - use compactInventory() instead
    void sortInventory();
    
    // Compact inventory (move items to fill empty slots)
    void compactInventory();
    
    // Clear all items from inventory
    void clear();
    
    // Swap items between two slots
    // Parameters:
    //   - slotIndex1: First slot index
    //   - slotIndex2: Second slot index
    // Returns: True if swap was successful
    bool swapSlots(int slotIndex1, int slotIndex2);
    
    // Drop an item (remove from inventory without return)
    // Parameters:
    //   - slotIndex: Slot to drop from
    //   - count: Number to drop (default: all)
    // Returns: True if items were dropped
    bool dropItem(int slotIndex, int count = -1);
    
    // TODO: Future extensions
    // - Add inventory events/callbacks for UI updates
    // - Add item filters and search
    // - Add favorite/locked item flags
    // - Add quick slots/hotbar system
    // - Add inventory pages/tabs
    // - Add item comparison features
    // - Add item tooltips generation
    // - Add save/load functionality
    // - Add item crafting system integration
    // - Add item equipment slots (separate from main inventory)
    // - Add item trading/transfer between inventories
    
private:
    std::vector<InventorySlot> m_slots;    // Inventory slots
    int m_maxSlots;                        // Maximum number of slots
    float m_maxWeight;                     // Maximum carrying capacity
    
    // Find first empty slot
    // Returns: Index of empty slot, or -1 if inventory is full
    int findEmptySlot() const;
    
    // Find first slot with matching stackable item that isn't at max stack
    // Parameters:
    //   - item: The item to find
    // Returns: Index of slot, or -1 if no stackable slot found
    int findStackableSlot(const Item& item) const;
    
    // Validate slot index
    // Returns: True if index is valid
    bool isValidSlot(int slotIndex) const;
};

#endif // INVENTORY_SYSTEM_H
