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

    // Inventory management
    InventorySystem& getInventory() { return m_inventory; }
    const InventorySystem& getInventory() const { return m_inventory; }
    void initializeStartingInventory();

private:
    // Resources and inventory
    float m_energy;      // Player energy (0-100)
    float m_money;       // Player money
    float m_fuel;        // Car fuel (0-100)
    float m_vehicleCondition; // Vehicle condition (0-100)
    OriginType m_origin; // Character background
    CarType m_carType;   // Vehicle type
    InventorySystem m_inventory; // Player inventory system

    // Game state - ACTIVE
    int m_currentCityIndex;  // Current city (0-4)
    float m_totalPlayTime;   // Total time played in seconds
    int m_currentNodeId;     // Current node on world map
};

#endif // PLAYER_STATE_H
