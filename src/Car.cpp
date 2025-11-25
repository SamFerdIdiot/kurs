#include "Car.h"
#include <algorithm>
#include <sstream>

// Russian 90s car types mapping (updated to match CAR_SPECIFICATIONS.md):
// SPORTS_COUPE = BMW E30 Coupe - Sport coupe, fast but high fuel consumption
// OLD_SEDAN = Volvo 940 Sedan - Reliable sedan, balanced characteristics
// OFFROAD_VAN = Mercedes-Benz Sprinter - Cargo van, huge inventory space

// Constructor
Car::Car(CarType carType) 
    : m_type(carType), 
      m_fuel(50.0f), 
      m_fuelCapacity(50.0f),
      m_baseFuelEfficiency(10.0f),
      m_cargoCapacity(400.0f),
      m_currentCargoWeight(0.0f),
      m_condition(100.0f),
      m_baseSpeed(1.0f),
      m_durability(1.0f) {
    initializeProperties();
}

// Initialize properties based on car type
void Car::initializeProperties() {
    // Initialize properties based on car type (from CAR_SPECIFICATIONS.md)
    switch (m_type) {
        case CarType::SPORTS_COUPE: // BMW E30 Coupe
            m_fuelCapacity = 200.0f;         // 200 liters (large tank)
            m_baseFuelEfficiency = 10.0f;    // ~10 km/liter (3.0L per 100km on road)
            m_cargoCapacity = 160.0f;        // 32 slots (small trunk)
            m_baseSpeed = 1.2f;              // +20% speed bonus
            m_durability = 0.95f;            // Slightly less durable (sport car)
            break;
            
        case CarType::OLD_SEDAN: // Volvo 940 Sedan
            m_fuelCapacity = 100.0f;         // 100 liters (medium tank)
            m_baseFuelEfficiency = 12.5f;    // ~12.5 km/liter (2.0L per 100km on road)
            m_cargoCapacity = 320.0f;        // 64 slots (medium trunk)
            m_baseSpeed = 1.0f;              // Standard speed (100%)
            m_durability = 1.15f;            // Very reliable
            break;
            
        case CarType::OFFROAD_VAN: // Mercedes-Benz Sprinter
            m_fuelCapacity = 75.0f;          // 75 liters (smaller tank)
            m_baseFuelEfficiency = 10.0f;    // ~10 km/liter (2.5L per 100km on road)
            m_cargoCapacity = 620.0f;        // 124 slots (huge cargo space)
            m_baseSpeed = 1.0f;              // Standard speed
            m_durability = 1.1f;             // Durable utility vehicle
            break;
            
        default: // Unknown
            m_fuelCapacity = 100.0f;
            m_baseFuelEfficiency = 10.0f;
            m_cargoCapacity = 320.0f;
            m_baseSpeed = 1.0f;
            m_durability = 1.0f;
            break;
    }
    
    // Start with full fuel and perfect condition
    m_fuel = m_fuelCapacity;
    m_condition = 100.0f;
    m_currentCargoWeight = 0.0f;
}

// Set car type
void Car::setType(CarType carType) {
    m_type = carType;
    initializeProperties();
}

// Clamp fuel to valid range
void Car::clampFuel() {
    if (m_fuel < 0.0f) m_fuel = 0.0f;
    if (m_fuel > m_fuelCapacity) m_fuel = m_fuelCapacity;
}

// Clamp condition to valid range
void Car::clampCondition() {
    if (m_condition < 0.0f) m_condition = 0.0f;
    if (m_condition > 100.0f) m_condition = 100.0f;
}

// Set fuel level (clamped)
void Car::setFuel(float fuel) {
    m_fuel = fuel;
    clampFuel();
}

// Get fuel efficiency adjusted by condition
float Car::getFuelEfficiency() const {
    return getEffectiveFuelEfficiency();
}

// Get effective fuel efficiency considering condition
float Car::getEffectiveFuelEfficiency() const {
    // Condition affects efficiency (50% condition = 75% efficiency)
    float conditionMultiplier = 0.5f + (m_condition / 200.0f);
    return m_baseFuelEfficiency * conditionMultiplier;
}

// Add fuel to tank
float Car::addFuel(float amount) {
    if (amount <= 0) return 0;
    
    float spaceAvailable = m_fuelCapacity - m_fuel;
    float amountToAdd = std::min(amount, spaceAvailable);
    
    m_fuel += amountToAdd;
    clampFuel();
    
    return amountToAdd;
}

// Calculate fuel consumption for a given distance
float Car::calculateFuelConsumption(float distance, float terrainModifier) const {
    if (distance <= 0) return 0;
    
    float efficiency = getEffectiveFuelEfficiency();
    if (efficiency <= 0) return 0;
    
    // Terrain modifier affects fuel consumption (harder terrain = more fuel)
    return (distance / efficiency) * terrainModifier;
}

// Consume fuel for distance traveled
bool Car::consumeFuel(float distance, float terrainModifier) {
    if (distance <= 0) return true;
    
    float fuelNeeded = calculateFuelConsumption(distance, terrainModifier);
    
    if (fuelNeeded > m_fuel) {
        return false; // Not enough fuel
    }
    
    m_fuel -= fuelNeeded;
    clampFuel();
    return true;
}

// Check if car has enough fuel for distance
bool Car::hasFuelForDistance(float distance, float terrainModifier) const {
    if (distance <= 0) return true;
    
    float fuelNeeded = calculateFuelConsumption(distance, terrainModifier);
    return fuelNeeded <= m_fuel;
}

// Set car condition (clamped 0-100)
void Car::setCondition(float condition) {
    m_condition = condition;
    clampCondition();
}

// Repair car (increase condition)
void Car::repair(float amount) {
    m_condition += amount;
    clampCondition();
}

// Apply wear and damage over distance
void Car::applyWear(float distance, float terrainModifier) {
    // Base wear rate: 0.1 per 100km
    float wearAmount = (distance / 100.0f) * 0.1f * terrainModifier / m_durability;
    m_condition -= wearAmount;
    clampCondition();
}

// Set current cargo weight
void Car::setCurrentCargoWeight(float weight) {
    m_currentCargoWeight = weight;
}

// Get remaining cargo capacity
float Car::getRemainingCargoCapacity() const {
    return m_cargoCapacity - m_currentCargoWeight;
}

// Check if can add cargo
bool Car::canAddCargo(float additionalWeight) const {
    return (m_currentCargoWeight + additionalWeight) <= m_cargoCapacity;
}

// Check if overloaded
bool Car::isOverloaded() const {
    return m_currentCargoWeight > m_cargoCapacity;
}

// Get speed modifier based on condition and cargo
float Car::getSpeedModifier() const {
    // Condition affects speed (50% condition = 75% speed)
    float conditionMultiplier = 0.5f + (m_condition / 200.0f);
    
    // Cargo weight affects speed (max cargo = 70% speed)
    float cargoMultiplier = 1.0f - (m_currentCargoWeight / m_cargoCapacity) * 0.3f;
    
    return m_baseSpeed * conditionMultiplier * cargoMultiplier;
}

// Get breakdown chance per 100 km
float Car::getBreakdownChance() const {
    // Lower condition = higher breakdown chance
    // 100% condition = 1% chance, 0% condition = 50% chance
    float baseChance = 0.01f;
    float conditionFactor = (100.0f - m_condition) / 200.0f;
    return baseChance + conditionFactor * 0.49f;
}

// Sync cargo weight from inventory system
void Car::syncCargoWeight(const InventorySystem& inventory) {
    m_currentCargoWeight = inventory.getTotalWeight();
}

// Check if inventory weight is within cargo capacity
bool Car::canCarryInventory(const InventorySystem& inventory) const {
    return inventory.getTotalWeight() <= m_cargoCapacity;
}

// Get car type name (from CAR_SPECIFICATIONS.md)
std::string Car::getTypeName() const {
    switch (m_type) {
        case CarType::SPORTS_COUPE:
            return "BMW E30 Coupe";
        case CarType::OLD_SEDAN:
            return "Volvo 940 Sedan";
        case CarType::OFFROAD_VAN:
            return "Mercedes-Benz Sprinter";
        default:
            return "Неизвестно";
    }
}

// Get car description (from CAR_SPECIFICATIONS.md)
std::string Car::getDescription() const {
    switch (m_type) {
        case CarType::SPORTS_COUPE:
            return "Спортивное купе. Быстрое, но прожорливое. 3 места для NPC, 32 слота инвентаря.";
        case CarType::OLD_SEDAN:
            return "Надежный седан. Сбалансированный. 4 места для NPC, 64 слота инвентаря.";
        case CarType::OFFROAD_VAN:
            return "Грузовой фургон. Огромный багажник. 2 места для NPC, 124 слота инвентаря.";
        default:
            return "";
    }
}

// Get detailed status string
std::string Car::getStatusString() const {
    std::ostringstream oss;
    oss << "Автомобиль: " << getTypeName() << "\n";
    oss << "Состояние: " << static_cast<int>(m_condition) << "%\n";
    oss << "Топливо: " << static_cast<int>(m_fuel) << "/" << static_cast<int>(m_fuelCapacity) << " л\n";
    oss << "Груз: " << static_cast<int>(m_currentCargoWeight) << "/" << static_cast<int>(m_cargoCapacity);
    return oss.str();
}
