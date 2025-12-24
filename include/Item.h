#ifndef ITEM_H
#define ITEM_H

#include <string>



enum class ItemCategory {
    CONSUMABLE,
    TOOL,
    QUEST,
    JUNK,
    EQUIPMENT
};



enum class ItemRarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};









struct Item {
    std::string name;
    std::string description;
    ItemCategory category;
    ItemRarity rarity;
    int value;
    float weight;
    bool isStackable;
    int maxStackSize;


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



    bool operator==(const Item& other) const {
        return name == other.name &&
               category == other.category &&
               rarity == other.rarity &&
               value == other.value &&
               weight == other.weight &&
               isStackable == other.isStackable &&
               maxStackSize == other.maxStackSize;
    }


    bool operator!=(const Item& other) const {
        return !(*this == other);
    }











};








struct InventorySlot {
    Item item;
    int count;
    bool isEmpty;


    InventorySlot()
        : item(),
          count(0),
          isEmpty(true) {}


    InventorySlot(const Item& slotItem, int itemCount = 1)
        : item(slotItem),
          count(itemCount),
          isEmpty(false) {}


    void clear() {
        count = 0;
        isEmpty = true;
    }



    int addItems(int amount) {
        if (!item.isStackable) {
            return amount;
        }

        int newCount = count + amount;
        if (newCount > item.maxStackSize) {
            count = item.maxStackSize;
            return newCount - item.maxStackSize;
        }

        count = newCount;
        return 0;
    }



    int removeItems(int amount) {
        int removed = (amount > count) ? count : amount;
        count -= removed;

        if (count <= 0) {
            clear();
        }

        return removed;
    }
};

#endif
