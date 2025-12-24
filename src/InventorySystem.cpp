#include "InventorySystem.h"
#include <algorithm>


InventorySystem::InventorySystem(int maxSlots, float maxWeight)
    : m_maxSlots(maxSlots), m_maxWeight(maxWeight) {
    m_slots.resize(maxSlots);
}


bool InventorySystem::addItem(const Item& item, int count) {
    if (count <= 0) return false;


    if (!canAddItem(item, count)) {
        return false;
    }

    int remaining = count;


    if (item.isStackable) {
        int slotIndex = findStackableSlot(item);
        while (slotIndex != -1 && remaining > 0) {
            InventorySlot& slot = m_slots[slotIndex];
            int spaceInStack = item.maxStackSize - slot.count;
            int toAdd = std::min(remaining, spaceInStack);

            slot.count += toAdd;
            remaining -= toAdd;

            if (remaining > 0) {
                slotIndex = findStackableSlot(item);
            }
        }
    }


    while (remaining > 0) {
        int emptySlot = findEmptySlot();
        if (emptySlot == -1) {
            return false;
        }

        int toAdd = item.isStackable ? std::min(remaining, item.maxStackSize) : 1;
        m_slots[emptySlot].item = item;
        m_slots[emptySlot].count = toAdd;
        m_slots[emptySlot].isEmpty = false;
        remaining -= toAdd;
    }

    return true;
}


bool InventorySystem::removeItem(int slotIndex, int count) {
    if (!isValidSlot(slotIndex)) return false;
    if (m_slots[slotIndex].isEmpty) return false;

    if (count <= 0) count = m_slots[slotIndex].count;

    if (count >= m_slots[slotIndex].count) {

        m_slots[slotIndex].isEmpty = true;
        m_slots[slotIndex].count = 0;
    } else {

        m_slots[slotIndex].count -= count;
    }

    return true;
}


int InventorySystem::removeItemByName(const std::string& itemName, int count) {
    int removed = 0;

    for (int i = 0; i < m_maxSlots && removed < count; i++) {
        if (!m_slots[i].isEmpty && m_slots[i].item.name == itemName) {
            int toRemove = std::min(count - removed, m_slots[i].count);
            removeItem(i, toRemove);
            removed += toRemove;
        }
    }

    return removed;
}


bool InventorySystem::useItem(int slotIndex) {
    if (!isValidSlot(slotIndex)) return false;
    if (m_slots[slotIndex].isEmpty) return false;





    return removeItem(slotIndex, 1);
}


const InventorySlot& InventorySystem::getSlot(int slotIndex) const {
    static InventorySlot emptySlot;
    if (!isValidSlot(slotIndex)) return emptySlot;
    return m_slots[slotIndex];
}


InventorySlot& InventorySystem::getSlotMutable(int slotIndex) {
    static InventorySlot emptySlot;
    if (!isValidSlot(slotIndex)) return emptySlot;
    return m_slots[slotIndex];
}


bool InventorySystem::hasItem(const std::string& itemName, int minCount) const {
    return getItemCount(itemName) >= minCount;
}


int InventorySystem::getItemCount(const std::string& itemName) const {
    int total = 0;
    for (const auto& slot : m_slots) {
        if (!slot.isEmpty && slot.item.name == itemName) {
            total += slot.count;
        }
    }
    return total;
}


float InventorySystem::getTotalWeight() const {
    float totalWeight = 0.0f;
    for (const auto& slot : m_slots) {
        if (!slot.isEmpty) {
            totalWeight += slot.item.weight * slot.count;
        }
    }
    return totalWeight;
}


float InventorySystem::getMaxWeight() const {
    return m_maxWeight;
}


void InventorySystem::setMaxWeight(float maxWeight) {
    m_maxWeight = maxWeight;
}


int InventorySystem::getSlotCount() const {
    return m_maxSlots;
}


int InventorySystem::getEmptySlotCount() const {
    int count = 0;
    for (const auto& slot : m_slots) {
        if (slot.isEmpty) count++;
    }
    return count;
}


bool InventorySystem::isFull() const {
    return getEmptySlotCount() == 0;
}


bool InventorySystem::canAddItem(const Item& item, int count) const {
    float itemWeight = item.weight * count;
    float currentWeight = getTotalWeight();

    return (currentWeight + itemWeight) <= m_maxWeight;
}


int InventorySystem::findItem(const std::string& itemName) const {
    for (int i = 0; i < m_maxSlots; i++) {
        if (!m_slots[i].isEmpty && m_slots[i].item.name == itemName) {
            return i;
        }
    }
    return -1;
}


std::vector<int> InventorySystem::findAllItems(const std::string& itemName) const {
    std::vector<int> indices;
    for (int i = 0; i < m_maxSlots; i++) {
        if (!m_slots[i].isEmpty && m_slots[i].item.name == itemName) {
            indices.push_back(i);
        }
    }
    return indices;
}


std::vector<int> InventorySystem::findItemsByCategory(ItemCategory category) const {
    std::vector<int> indices;
    for (int i = 0; i < m_maxSlots; i++) {
        if (!m_slots[i].isEmpty && m_slots[i].item.category == category) {
            indices.push_back(i);
        }
    }
    return indices;
}


void InventorySystem::sortInventory() {

}


void InventorySystem::compactInventory() {
    std::vector<InventorySlot> nonEmptySlots;


    for (const auto& slot : m_slots) {
        if (!slot.isEmpty) {
            nonEmptySlots.push_back(slot);
        }
    }


    clear();


    for (size_t i = 0; i < nonEmptySlots.size() && i < static_cast<size_t>(m_maxSlots); i++) {
        m_slots[i] = nonEmptySlots[i];
    }
}


void InventorySystem::clear() {
    for (auto& slot : m_slots) {
        slot.isEmpty = true;
        slot.count = 0;
    }
}


bool InventorySystem::swapSlots(int slotIndex1, int slotIndex2) {
    if (!isValidSlot(slotIndex1) || !isValidSlot(slotIndex2)) {
        return false;
    }

    std::swap(m_slots[slotIndex1], m_slots[slotIndex2]);
    return true;
}


bool InventorySystem::dropItem(int slotIndex, int count) {
    return removeItem(slotIndex, count);
}


int InventorySystem::findEmptySlot() const {
    for (int i = 0; i < m_maxSlots; i++) {
        if (m_slots[i].isEmpty) {
            return i;
        }
    }
    return -1;
}


int InventorySystem::findStackableSlot(const Item& item) const {
    if (!item.isStackable) return -1;

    for (int i = 0; i < m_maxSlots; i++) {
        if (!m_slots[i].isEmpty &&
            m_slots[i].item.name == item.name &&
            m_slots[i].count < item.maxStackSize) {
            return i;
        }
    }
    return -1;
}


bool InventorySystem::isValidSlot(int slotIndex) const {
    return slotIndex >= 0 && slotIndex < m_maxSlots;
}
