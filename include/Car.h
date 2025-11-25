#ifndef CAR_H
#define CAR_H

#include "GameTypes.h"
#include "InventorySystem.h"
#include <string>

// Car class
// Purpose: Manages vehicle properties, fuel consumption, and cargo capacity
// Features:
//   - Vehicle type-specific attributes (fuel efficiency, capacity, speed)
//   - Fuel consumption calculation based on distance and conditions
//   - Weight and cargo capacity management
//   - Vehicle condition tracking (wear and damage)
//   - Integration with inventory system for cargo weight
//   - Restriction algorithms for valid travel and loading
//
// Design notes:
//   - Car properties are determined by CarType enum
//   - Fuel efficiency affects consumption rate per distance unit
//   - Cargo capacity limits inventory weight
//   - Condition affects fuel efficiency and reliability
//   - Can be extended with upgrades and customization
class Car {
public:
    // Constructor
    // Parameters:
    //   - carType: The type of vehicle (from GameTypes)
    Car(CarType carType = CarType::UNKNOWN);
    
    // Get the car type
    CarType getType() const { return m_type; }
    
    // Set the car type and reinitialize properties
    void setType(CarType carType);
    
    // === Fuel Management ===
    
    // Get current fuel level (0-100)
    float getFuel() const { return m_fuel; }
    
    // Get maximum fuel tank capacity in liters
    float getFuelCapacity() const { return m_fuelCapacity; }
    
    // Get fuel efficiency (km per liter)
    float getFuelEfficiency() const;
    
    // Set fuel level (clamped to 0-capacity)
    void setFuel(float fuel);
    
    // Add fuel to tank (clamped to capacity)
    // Returns: Amount actually added
    float addFuel(float amount);
    
    // Calculate fuel consumption for a given distance
    // Parameters:
    //   - distance: Distance in km
    //   - terrainModifier: Multiplier for difficult terrain (1.0 = normal, >1.0 = harder)
    // Returns: Fuel consumed in liters
    float calculateFuelConsumption(float distance, float terrainModifier = 1.0f) const;
    
    // Consume fuel for travel
    // Parameters:
    //   - distance: Distance in km
    //   - terrainModifier: Multiplier for difficult terrain
    // Returns: True if had enough fuel, false otherwise
    bool consumeFuel(float distance, float terrainModifier = 1.0f);
    
    // Check if car has enough fuel for a given distance
    // Parameters:
    //   - distance: Distance in km
    //   - terrainModifier: Multiplier for difficult terrain
    // Returns: True if has enough fuel
    bool hasFuelForDistance(float distance, float terrainModifier = 1.0f) const;
    
    // === Cargo Capacity Management ===
    
    // Get maximum cargo capacity
    float getCargoCapacity() const { return m_cargoCapacity; }
    
    // Get current cargo weight
    float getCurrentCargoWeight() const { return m_currentCargoWeight; }
    
    // Set current cargo weight (used when syncing with inventory)
    void setCurrentCargoWeight(float weight);
    
    // Get remaining cargo capacity
    float getRemainingCargoCapacity() const;
    
    // Check if adding weight would exceed capacity
    // Parameters:
    //   - additionalWeight: Weight to add
    // Returns: True if within capacity
    bool canAddCargo(float additionalWeight) const;
    
    // Check if cargo weight exceeds capacity (overloaded)
    // Returns: True if overloaded
    bool isOverloaded() const;
    
    // === Vehicle Condition Management ===
    
    // Get vehicle condition (0-100, 100 = perfect)
    float getCondition() const { return m_condition; }
    
    // Set vehicle condition (clamped to 0-100)
    void setCondition(float condition);
    
    // Repair vehicle
    // Parameters:
    //   - amount: Amount to repair (0-100)
    void repair(float amount);
    
    // Apply wear and damage over distance
    // Parameters:
    //   - distance: Distance traveled in km
    //   - terrainModifier: Multiplier for harsh conditions
    void applyWear(float distance, float terrainModifier = 1.0f);
    
    // === Performance Calculations ===
    
    // Get effective fuel efficiency considering condition
    // Returns: Fuel efficiency in km/liter
    float getEffectiveFuelEfficiency() const;
    
    // Get speed modifier based on condition and cargo
    // Returns: Speed multiplier (0.5 to 1.0)
    float getSpeedModifier() const;
    
    // Get breakdown chance per 100 km
    // Returns: Probability (0.0 to 1.0)
    float getBreakdownChance() const;
    
    // === Inventory Integration ===
    
    // Sync cargo weight from inventory system
    // Parameters:
    //   - inventory: Reference to inventory system
    void syncCargoWeight(const InventorySystem& inventory);
    
    // Check if inventory weight is within cargo capacity
    // Parameters:
    //   - inventory: Reference to inventory system
    // Returns: True if within capacity
    bool canCarryInventory(const InventorySystem& inventory) const;
    
    // === Information Methods ===
    
    // Get car type name
    std::string getTypeName() const;
    
    // Get car description
    std::string getDescription() const;
    
    // Get detailed status string
    std::string getStatusString() const;
    
private:
    CarType m_type;                // Vehicle type
    
    // Fuel properties
    float m_fuel;                  // Current fuel level (0-100 liters)
    float m_fuelCapacity;          // Maximum fuel capacity (liters)
    float m_baseFuelEfficiency;    // Base km per liter
    
    // Cargo properties
    float m_cargoCapacity;         // Maximum cargo capacity
    float m_currentCargoWeight;    // Current cargo weight
    
    // Condition
    float m_condition;             // Vehicle condition (0-100)
    
    // Performance characteristics
    float m_baseSpeed;             // Base speed multiplier
    float m_durability;            // Resistance to wear (higher = more durable)
    
    // Initialize car properties based on type
    void initializeProperties();
    
    // Helper to clamp fuel to valid range
    void clampFuel();
    
    // Helper to clamp condition to valid range
    void clampCondition();
};

#endif // CAR_H
