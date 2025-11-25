#ifndef ITEM_H
#define ITEM_H

#include <string>

// Enum defining item categories/types
// Used for organizing and filtering items in inventory
enum class ItemCategory {
    CONSUMABLE,     // Items that are used up (food, water, medicine)
    TOOL,           // Items that provide utility (repair kit, map, flashlight)
    QUEST,          // Quest-related items
    JUNK,           // Sellable items with no direct use
    EQUIPMENT       // Items that can be equipped (spare tire, roof rack)
};

// Enum defining item rarity
// Affects item value, drop rates, and visual presentation
enum class ItemRarity {
    COMMON,         // White - frequently found
    UNCOMMON,       // Green - less common
    RARE,           // Blue - hard to find
    EPIC,           // Purple - very rare
    LEGENDARY       // Orange - extremely rare, unique items
};

// Item struct
// Purpose: Data structure representing an item in the game
//
// Заметки по дизайну:
//   - Lightweight struct for easy copying and passing
//   - Can be extended with additional properties as needed
//   - Consider using unique IDs for item instances in the future
//   - Effects/stats can be added for consumables and equipment
struct Item {
    std::string name;               // Display name of the item
    std::string description;        // Detailed description shown in UI
    ItemCategory category;          // Category for organization
    ItemRarity rarity;             // Rarity level
    int value;                     // Monetary value (for buying/selling)
    float weight;                  // Weight (affects carrying capacity)
    bool isStackable;              // Can multiple items stack in one slot
    int maxStackSize;              // Maximum stack size (if stackable)
    
    // Конструктор со значениями по умолчанию
    Item(const std::string& itemName = "Unknown Item",
         const std::string& itemDescription = "No description available.",
         ItemCategory itemCategory = ItemCategory::JUNK,
         ItemRarity itemRarity = ItemRarity::COMMON,
         int itemValue = 0,
         float itemWeight = 1.0f,
         bool stackable = false,
         int maxStack = 1)
        : name(itemName),
          description(itemDescription),
          category(itemCategory),
          rarity(itemRarity),
          value(itemValue),
          weight(itemWeight),
          isStackable(stackable),
          maxStackSize(maxStack) {}
    
    // Equality operator for comparing items (used for stacking)
    // Items are considered equal if all their properties match
    bool operator==(const Item& other) const {
        return name == other.name &&
               category == other.category &&
               rarity == other.rarity &&
               value == other.value &&
               weight == other.weight &&
               isStackable == other.isStackable &&
               maxStackSize == other.maxStackSize;
    }
    
    // Inequality operator
    bool operator!=(const Item& other) const {
        return !(*this == other);
    }
    
    // TODO: Future extensions
    // - Add item effects (e.g., heal 20 HP, restore 50 fuel)
    // - Add item requirements (e.g., requires tool skill level 2)
    // - Add item durability for tools and equipment
    // - Add item sprites/icons
    // - Add item sound effects
    // - Add item usage cooldowns
    // - Add item crafting recipes
    // - Add item enchantments/modifiers
    // - Add item set bonuses
};

// InventorySlot struct
// Purpose: Represents a single slot in the inventory that can hold items
//
// Заметки по дизайну:
//   - Supports stacking for stackable items
//   - Empty slots have nullptr or count of 0
//   - Can add slot restrictions in the future (equipment slots, etc.)
struct InventorySlot {
    Item item;                     // The item in this slot
    int count;                     // Number of items (for stackable items)
    bool isEmpty;                  // Whether this slot is empty
    
    // Конструктор
    InventorySlot()
        : item(),
          count(0),
          isEmpty(true) {}
    
    // Constructor with item
    InventorySlot(const Item& slotItem, int itemCount = 1)
        : item(slotItem),
          count(itemCount),
          isEmpty(false) {}
    
    // Clear the slot
    void clear() {
        count = 0;
        isEmpty = true;
    }
    
    // Add items to this slot (for stackable items)
    // Returns: Number of items that couldn't be added (overflow)
    int addItems(int amount) {
        if (!item.isStackable) {
            return amount; // Can't stack, return all items
        }
        
        int newCount = count + amount;
        if (newCount > item.maxStackSize) {
            count = item.maxStackSize;
            return newCount - item.maxStackSize; // Return overflow
        }
        
        count = newCount;
        return 0; // All items added successfully
    }
    
    // Remove items from this slot
    // Returns: Number of items actually removed
    int removeItems(int amount) {
        int removed = (amount > count) ? count : amount;
        count -= removed;
        
        if (count <= 0) {
            clear();
        }
        
        return removed;
    }
};

#endif // ITEM_H
