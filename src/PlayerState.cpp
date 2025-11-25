#include "PlayerState.h"
#include <algorithm>

// Constructor
PlayerState::PlayerState() 
    : m_energy(100.0f),
      m_money(200.0f),
      m_fuel(100.0f),
      m_vehicleCondition(100.0f),
      m_origin(OriginType::UNKNOWN),
      m_carType(CarType::UNKNOWN),
      m_currentNodeId(0) {
}

// Energy management
void PlayerState::setEnergy(float energy) {
    m_energy = std::max(0.0f, std::min(energy, 100.0f));
}

void PlayerState::addEnergy(float amount) {
    setEnergy(m_energy + amount);
}

// Money management
void PlayerState::setMoney(float money) {
    m_money = std::max(0.0f, money);
}

void PlayerState::addMoney(float amount) {
    setMoney(m_money + amount);
}

// Fuel management
void PlayerState::setFuel(float fuel) {
    m_fuel = std::max(0.0f, std::min(fuel, 100.0f));
}

void PlayerState::addFuel(float amount) {
    setFuel(m_fuel + amount);
}

// Vehicle condition management
void PlayerState::setVehicleCondition(float condition) {
    m_vehicleCondition = std::max(0.0f, std::min(condition, 100.0f));
}

// Initialize resources based on origin and car type
void PlayerState::initializeResources() {
    initializeResources(m_origin, m_carType);
}

void PlayerState::initializeResources(OriginType origin, CarType carType) {
    m_origin = origin;
    m_carType = carType;
    
    // Set starting resources based on character choice
    switch (origin) {
        case OriginType::EX_RACER:
            m_money = 150.0f;  // Less money, ex-racer spent it all
            m_fuel = 80.0f;    // Medium fuel
            m_energy = 100.0f; // Full energy
            break;
            
        case OriginType::NOMAD: // Student equivalent
            m_money = 200.0f;  // Standard money
            m_fuel = 100.0f;   // Full fuel
            m_energy = 90.0f;  // Slightly tired from travel
            break;
            
        case OriginType::HITCHHIKER: // Unemployed equivalent
            m_money = 100.0f;  // Less money (hard mode)
            m_fuel = 60.0f;    // Low fuel (hard mode)
            m_energy = 70.0f;  // Low energy (depression)
            break;
            
        default:
            m_money = 200.0f;
            m_fuel = 100.0f;
            m_energy = 100.0f;
            break;
    }
}

// Check if game is over
bool PlayerState::isGameOver() const {
    return (m_energy <= 0.0f || m_fuel <= 0.0f);
}

// Initialize starting inventory
void PlayerState::initializeStartingInventory() {
    // Basic starting items - can be expanded
    // For now, inventory starts empty
}
