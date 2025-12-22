#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include "GameTypes.h"
#include "InventorySystem.h"
#include <memory>

// PlayerState - Manages player resources and game state
// Purpose: Track player's energy, money, fuel, and other resources
// Features:
//   - Energy management (affects ability to continue driving)
//   - Money tracking (for purchases and services)
//   - Fuel management (for car operation)
//   - Resource validation (prevent negative values)
//   - Query methods for resource levels
//   - Character origin and car type tracking
//   - Starting resources based on character choices
//   - Inventory management (items, weight, capacity)
class PlayerState {
public:
    PlayerState();

    // Energy management
    float getEnergy() const { return m_energy; }
    void setEnergy(float energy);
    void addEnergy(float amount);
    void modifyEnergy(float amount) { addEnergy(amount); }
    bool hasEnergy() const { return m_energy > 0.0f; }

    // Money management
    float getMoney() const { return m_money; }
    void setMoney(float money);
    void addMoney(float amount);
    void modifyMoney(float amount) { addMoney(amount); }
    bool canAfford(float cost) const { return m_money >= cost; }

    // Fuel management
    float getFuel() const { return m_fuel; }
    void setFuel(float fuel);
    void addFuel(float amount);
    bool hasFuel() const { return m_fuel > 0.0f; }

    // Vehicle condition management
    float getVehicleCondition() const { return m_vehicleCondition; }
    void setVehicleCondition(float condition);
    void modifyVehicleCondition(float amount);

    // Mood management (0-100%)
    float getMood() const { return m_mood; }
    void setMood(float mood);
    void modifyMood(float amount);

    // Reputation management (-50..+50)
    int getReputation() const { return m_reputation; }
    void setReputation(int reputation);
    void modifyReputation(int amount);

    // Character and car type management
    OriginType getOrigin() const { return m_origin; }
    void setOrigin(OriginType origin) { m_origin = origin; }

    CarType getCarType() const { return m_carType; }
    void setCarType(CarType carType) { m_carType = carType; }

    // Initialize resources based on origin and car type
    void initializeResources();
    void initializeResources(OriginType origin, CarType carType);

    // Game state queries
    bool isGameOver() const;
    
    // World map navigation
    int getCurrentNodeId() const { return m_currentNodeId; }
    void setCurrentNodeId(int nodeId) { m_currentNodeId = nodeId; }

    // [MVP] City progression (0-4 для 5 городов)
    int getCurrentCityIndex() const { return m_currentCityIndex; }
    void setCurrentCityIndex(int index) { m_currentCityIndex = index; }
    void advanceToNextCity() { m_currentCityIndex++; }

    // [MVP] Playtime tracking
    float getTotalPlayTime() const { return m_totalPlayTime; }
    void updatePlayTime(float deltaTime) { m_totalPlayTime += deltaTime; }
    void resetPlayTime() { m_totalPlayTime = 0.0f; }

    // Notebook progress tracking
    std::string getCurrentNotebookEntryId() const { return m_currentNotebookEntryId; }
    void setCurrentNotebookEntryId(const std::string& entryId) { m_currentNotebookEntryId = entryId; }

    // Inventory management
    InventorySystem& getInventory() { return m_inventory; }
    const InventorySystem& getInventory() const { return m_inventory; }
    void initializeStartingInventory();

    // Principles and traits system (hidden attributes for conditional choices)
    // Принципы (приобретенные уроки): "understanding_context", "listen_material", "ask_experienced", "resource_efficient"
    // Черты характера: "patient", "impulsive", "analytical", "intuitive"
    // Артефакты: "technical_journal", "burned_finger", "photo_at_machine", "spare_starter"
    void addPrinciple(const std::string& principleId);
    void addTrait(const std::string& traitId);
    void addStoryItem(const std::string& itemId);

    bool hasPrinciple(const std::string& principleId) const;
    bool hasTrait(const std::string& traitId) const;
    bool hasStoryItem(const std::string& itemId) const;

    const std::vector<std::string>& getPrinciples() const { return m_principles; }
    const std::vector<std::string>& getTraits() const { return m_traits; }
    const std::vector<std::string>& getStoryItems() const { return m_storyItems; }

    void clearPrinciples() { m_principles.clear(); }
    void clearTraits() { m_traits.clear(); }
    void clearStoryItems() { m_storyItems.clear(); }

private:
    // Resources and inventory
    float m_energy;      // Player energy (0-100)
    float m_money;       // Player money
    float m_fuel;        // Car fuel (0-100)
    float m_vehicleCondition; // Vehicle condition (0-100)
    float m_mood;        // Player mood (0-100%)
    int m_reputation;    // Player reputation (-50..+50)
    OriginType m_origin; // Character background
    CarType m_carType;   // Vehicle type
    InventorySystem m_inventory; // Player inventory system

    // Game state - ACTIVE
    int m_currentCityIndex;  // Current city (0-4)
    float m_totalPlayTime;   // Total time played in seconds
    int m_currentNodeId;     // Current node on world map
    std::string m_currentNotebookEntryId;  // Current notebook entry for save/load

    // Hidden state for conditional choices (День 0 и далее)
    std::vector<std::string> m_principles;   // Приобретенные принципы/уроки
    std::vector<std::string> m_traits;       // Черты характера
    std::vector<std::string> m_storyItems;   // Сюжетные артефакты (не в инвентаре)
};

#endif // PLAYER_STATE_H
