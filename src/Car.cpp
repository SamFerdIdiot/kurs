#include "Car.h"
#include <algorithm>
#include <sstream>







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


void Car::initializeProperties() {

    switch (m_type) {
        case CarType::SPORTS_COUPE:
            m_fuelCapacity = 200.0f;
            m_baseFuelEfficiency = 10.0f;
            m_cargoCapacity = 160.0f;
            m_baseSpeed = 1.2f;
            m_durability = 0.95f;
            break;

        case CarType::OLD_SEDAN:
            m_fuelCapacity = 100.0f;
            m_baseFuelEfficiency = 12.5f;
            m_cargoCapacity = 320.0f;
            m_baseSpeed = 1.0f;
            m_durability = 1.15f;
            break;

        case CarType::OFFROAD_VAN:
            m_fuelCapacity = 75.0f;
            m_baseFuelEfficiency = 10.0f;
            m_cargoCapacity = 620.0f;
            m_baseSpeed = 1.0f;
            m_durability = 1.1f;
            break;

        default:
            m_fuelCapacity = 100.0f;
            m_baseFuelEfficiency = 10.0f;
            m_cargoCapacity = 320.0f;
            m_baseSpeed = 1.0f;
            m_durability = 1.0f;
            break;
    }


    m_fuel = m_fuelCapacity;
    m_condition = 100.0f;
    m_currentCargoWeight = 0.0f;
}


void Car::setType(CarType carType) {
    m_type = carType;
    initializeProperties();
}


void Car::clampFuel() {
    if (m_fuel < 0.0f) m_fuel = 0.0f;
    if (m_fuel > m_fuelCapacity) m_fuel = m_fuelCapacity;
}


void Car::clampCondition() {
    if (m_condition < 0.0f) m_condition = 0.0f;
    if (m_condition > 100.0f) m_condition = 100.0f;
}


void Car::setFuel(float fuel) {
    m_fuel = fuel;
    clampFuel();
}


float Car::getFuelEfficiency() const {
    return getEffectiveFuelEfficiency();
}


float Car::getEffectiveFuelEfficiency() const {

    float conditionMultiplier = 0.5f + (m_condition / 200.0f);
    return m_baseFuelEfficiency * conditionMultiplier;
}


float Car::addFuel(float amount) {
    if (amount <= 0) return 0;

    float spaceAvailable = m_fuelCapacity - m_fuel;
    float amountToAdd = std::min(amount, spaceAvailable);

    m_fuel += amountToAdd;
    clampFuel();

    return amountToAdd;
}


float Car::calculateFuelConsumption(float distance, float terrainModifier) const {
    if (distance <= 0) return 0;

    float efficiency = getEffectiveFuelEfficiency();
    if (efficiency <= 0) return 0;


    return (distance / efficiency) * terrainModifier;
}


bool Car::consumeFuel(float distance, float terrainModifier) {
    if (distance <= 0) return true;

    float fuelNeeded = calculateFuelConsumption(distance, terrainModifier);

    if (fuelNeeded > m_fuel) {
        return false;
    }

    m_fuel -= fuelNeeded;
    clampFuel();
    return true;
}


bool Car::hasFuelForDistance(float distance, float terrainModifier) const {
    if (distance <= 0) return true;

    float fuelNeeded = calculateFuelConsumption(distance, terrainModifier);
    return fuelNeeded <= m_fuel;
}


void Car::setCondition(float condition) {
    m_condition = condition;
    clampCondition();
}


void Car::repair(float amount) {
    m_condition += amount;
    clampCondition();
}


void Car::applyWear(float distance, float terrainModifier) {

    float wearAmount = (distance / 100.0f) * 0.1f * terrainModifier / m_durability;
    m_condition -= wearAmount;
    clampCondition();
}


void Car::setCurrentCargoWeight(float weight) {
    m_currentCargoWeight = weight;
}


float Car::getRemainingCargoCapacity() const {
    return m_cargoCapacity - m_currentCargoWeight;
}


bool Car::canAddCargo(float additionalWeight) const {
    return (m_currentCargoWeight + additionalWeight) <= m_cargoCapacity;
}


bool Car::isOverloaded() const {
    return m_currentCargoWeight > m_cargoCapacity;
}


float Car::getSpeedModifier() const {

    float conditionMultiplier = 0.5f + (m_condition / 200.0f);


    float cargoMultiplier = 1.0f - (m_currentCargoWeight / m_cargoCapacity) * 0.3f;

    return m_baseSpeed * conditionMultiplier * cargoMultiplier;
}


float Car::getBreakdownChance() const {


    float baseChance = 0.01f;
    float conditionFactor = (100.0f - m_condition) / 200.0f;
    return baseChance + conditionFactor * 0.49f;
}


void Car::syncCargoWeight(const InventorySystem& inventory) {
    m_currentCargoWeight = inventory.getTotalWeight();
}


bool Car::canCarryInventory(const InventorySystem& inventory) const {
    return inventory.getTotalWeight() <= m_cargoCapacity;
}


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


std::string Car::getStatusString() const {
    std::ostringstream oss;
    oss << "Автомобиль: " << getTypeName() << "\n";
    oss << "Состояние: " << static_cast<int>(m_condition) << "%\n";
    oss << "Топливо: " << static_cast<int>(m_fuel) << "/" << static_cast<int>(m_fuelCapacity) << " л\n";
    oss << "Груз: " << static_cast<int>(m_currentCargoWeight) << "/" << static_cast<int>(m_cargoCapacity);
    return oss.str();
}
