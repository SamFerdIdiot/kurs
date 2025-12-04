#include "InventorySystem.h"
#include <algorithm>

// Constructor
InventorySystem::InventorySystem(int maxSlots, float maxWeight)
    : m_maxSlots(maxSlots), m_maxWeight(maxWeight) {
    m_slots.resize(maxSlots);
}

// Add item to inventory
bool InventorySystem::addItem(const Item& item, int count) {
    if (count <= 0) return false;
    
    // Check if adding would exceed weight capacity
    if (!canAddItem(item, count)) {
        return false;
    }
    
    int remaining = count;
    
    // Try to stack with existing items first
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
    
    // Add to empty slots
    while (remaining > 0) {
        int emptySlot = findEmptySlot();
        if (emptySlot == -1) {
            return false; // No more empty slots
        }
        
        int toAdd = item.isStackable ? std::min(remaining, item.maxStackSize) : 1;
        m_slots[emptySlot].item = item;
        m_slots[emptySlot].count = toAdd;
        m_slots[emptySlot].isEmpty = false;
        remaining -= toAdd;
    }
    
    return true;
}

// Remove item from slot
bool InventorySystem::removeItem(int slotIndex, int count) {
    if (!isValidSlot(slotIndex)) return false;
    if (m_slots[slotIndex].isEmpty) return false;
    
    if (count <= 0) count = m_slots[slotIndex].count;
    
    if (count >= m_slots[slotIndex].count) {
        // Remove entire stack
        m_slots[slotIndex].isEmpty = true;
        m_slots[slotIndex].count = 0;
    } else {
        // Remove partial stack
        m_slots[slotIndex].count -= count;
    }
    
    return true;
}

// Remove items by name
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

// Use item from slot
bool InventorySystem::useItem(int slotIndex) {
    if (!isValidSlot(slotIndex)) return false;
    if (m_slots[slotIndex].isEmpty) return false;

    // Note: Item effects/callbacks system not yet implemented
    // Items are consumed but no special effects are applied

    // Consume one item
    return removeItem(slotIndex, 1);
}

// Get slot at index
const InventorySlot& InventorySystem::getSlot(int slotIndex) const {
    static InventorySlot emptySlot;
    if (!isValidSlot(slotIndex)) return emptySlot;
    return m_slots[slotIndex];
}

// Get mutable slot reference
InventorySlot& InventorySystem::getSlotMutable(int slotIndex) {
    static InventorySlot emptySlot;
    if (!isValidSlot(slotIndex)) return emptySlot;
    return m_slots[slotIndex];
}

// Check if inventory has item
bool InventorySystem::hasItem(const std::string& itemName, int minCount) const {
    return getItemCount(itemName) >= minCount;
}

// Get total count of item
int InventorySystem::getItemCount(const std::string& itemName) const {
    int total = 0;
    for (const auto& slot : m_slots) {
        if (!slot.isEmpty && slot.item.name == itemName) {
            total += slot.count;
        }
    }
    return total;
}

// Get total weight
float InventorySystem::getTotalWeight() const {
    float totalWeight = 0.0f;
    for (const auto& slot : m_slots) {
        if (!slot.isEmpty) {
            totalWeight += slot.item.weight * slot.count;
        }
    }
    return totalWeight;
}

// Get max weight
float InventorySystem::getMaxWeight() const {
    return m_maxWeight;
}

// Set max weight
void InventorySystem::setMaxWeight(float maxWeight) {
    m_maxWeight = maxWeight;
}

// Get slot count
int InventorySystem::getSlotCount() const {
    return m_maxSlots;
}

// Get empty slot count
int InventorySystem::getEmptySlotCount() const {
    int count = 0;
    for (const auto& slot : m_slots) {
        if (slot.isEmpty) count++;
    }
    return count;
}

// Check if inventory is full
bool InventorySystem::isFull() const {
    return getEmptySlotCount() == 0;
}

// Check if item can be added
bool InventorySystem::canAddItem(const Item& item, int count) const {
    float itemWeight = item.weight * count;
    float currentWeight = getTotalWeight();
    
    return (currentWeight + itemWeight) <= m_maxWeight;
}

// Find first occurrence of item
int InventorySystem::findItem(const std::string& itemName) const {
    for (int i = 0; i < m_maxSlots; i++) {
        if (!m_slots[i].isEmpty && m_slots[i].item.name == itemName) {
            return i;
        }
    }
    return -1;
}

// Find all occurrences of item
std::vector<int> InventorySystem::findAllItems(const std::string& itemName) const {
    std::vector<int> indices;
    for (int i = 0; i < m_maxSlots; i++) {
        if (!m_slots[i].isEmpty && m_slots[i].item.name == itemName) {
            indices.push_back(i);
        }
    }
    return indices;
}

// Find items by category
std::vector<int> InventorySystem::findItemsByCategory(ItemCategory category) const {
    std::vector<int> indices;
    for (int i = 0; i < m_maxSlots; i++) {
        if (!m_slots[i].isEmpty && m_slots[i].item.category == category) {
            indices.push_back(i);
        }
    }
    return indices;
}

// Sort inventory (stub)
void InventorySystem::sortInventory() {
    // Sorting not yet implemented - use compactInventory() to organize items
}

// Compact inventory
void InventorySystem::compactInventory() {
    std::vector<InventorySlot> nonEmptySlots;
    
    // Collect all non-empty slots
    for (const auto& slot : m_slots) {
        if (!slot.isEmpty) {
            nonEmptySlots.push_back(slot);
        }
    }
    
    // Clear all slots
    clear();
    
    // Refill with non-empty slots
    for (size_t i = 0; i < nonEmptySlots.size() && i < static_cast<size_t>(m_maxSlots); i++) {
        m_slots[i] = nonEmptySlots[i];
    }
}

// Clear inventory
void InventorySystem::clear() {
    for (auto& slot : m_slots) {
        slot.isEmpty = true;
        slot.count = 0;
    }
}

// Swap slots
bool InventorySystem::swapSlots(int slotIndex1, int slotIndex2) {
    if (!isValidSlot(slotIndex1) || !isValidSlot(slotIndex2)) {
        return false;
    }
    
    std::swap(m_slots[slotIndex1], m_slots[slotIndex2]);
    return true;
}

// Drop item
bool InventorySystem::dropItem(int slotIndex, int count) {
    return removeItem(slotIndex, count);
}

// Find empty slot
int InventorySystem::findEmptySlot() const {
    for (int i = 0; i < m_maxSlots; i++) {
        if (m_slots[i].isEmpty) {
            return i;
        }
    }
    return -1;
}

// Find stackable slot
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

// Validate slot index
bool InventorySystem::isValidSlot(int slotIndex) const {
    return slotIndex >= 0 && slotIndex < m_maxSlots;
}
