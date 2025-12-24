#ifndef INVENTORY_SYSTEM_H
#define INVENTORY_SYSTEM_H

#include "Item.h"
#include <vector>
#include <string>



















class InventorySystem {
public:




    InventorySystem(int maxSlots = 24, float maxWeight = 100.0f);






    bool addItem(const Item& item, int count = 1);






    bool removeItem(int slotIndex, int count = 1);






    int removeItemByName(const std::string& itemName, int count);






    bool useItem(int slotIndex);





    const InventorySlot& getSlot(int slotIndex) const;


    InventorySlot& getSlotMutable(int slotIndex);






    bool hasItem(const std::string& itemName, int minCount = 1) const;





    int getItemCount(const std::string& itemName) const;



    float getTotalWeight() const;



    float getMaxWeight() const;


    void setMaxWeight(float maxWeight);



    int getSlotCount() const;



    int getEmptySlotCount() const;



    bool isFull() const;






    bool canAddItem(const Item& item, int count = 1) const;





    int findItem(const std::string& itemName) const;





    std::vector<int> findAllItems(const std::string& itemName) const;





    std::vector<int> findItemsByCategory(ItemCategory category) const;



    void sortInventory();


    void compactInventory();


    void clear();






    bool swapSlots(int slotIndex1, int slotIndex2);






    bool dropItem(int slotIndex, int count = -1);














private:
    std::vector<InventorySlot> m_slots;
    int m_maxSlots;
    float m_maxWeight;



    int findEmptySlot() const;





    int findStackableSlot(const Item& item) const;



    bool isValidSlot(int slotIndex) const;
};

#endif
