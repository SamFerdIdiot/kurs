#include "PlayerState.h"
#include <algorithm>
#include <iostream>

// Constructor with starting resources
PlayerState::PlayerState()
    : m_energy(100.0f),           // Full energy
      m_money(1000.0f),           // Starting money (1000 rubles)
      m_fuel(100.0f),             // Full fuel
      m_vehicleCondition(100.0f), // Perfect condition
      m_mood(70.0f),              // Starting mood (neutral-positive)
      m_reputation(0),            // Neutral reputation
      m_origin(OriginType::NOMAD),
      m_carType(CarType::OLD_SEDAN),
      m_inventory(24, 50.0f),     // 24 slots, 50kg max weight
      m_currentCityIndex(0),      // Start at city 0
      m_totalPlayTime(0.0f),      // No time played yet
      m_currentNodeId(0),         // Keep for compatibility
      m_currentNotebookEntryId("tutorial_start") {  // Start from tutorial

    // Add starting items to inventory
    std::cout << "[PlayerState] Adding starting items to inventory..." << std::endl;

    // Basic food
    Item bread("Bread", "Fresh bread", ItemCategory::CONSUMABLE, ItemRarity::COMMON, 50, 0.5f, true, 10);
    m_inventory.addItem(bread, 3);

    Item water("Water", "Bottle of water", ItemCategory::CONSUMABLE, ItemRarity::COMMON, 30, 1.0f, true, 20);
    m_inventory.addItem(water, 5);

    // Basic tools
    Item map("Map", "Road map", ItemCategory::TOOL, ItemRarity::COMMON, 100, 0.2f, false, 1);
    m_inventory.addItem(map, 1);

    Item flashlight("Flashlight", "LED flashlight", ItemCategory::TOOL, ItemRarity::COMMON, 150, 0.5f, false, 1);
    m_inventory.addItem(flashlight, 1);

    // Some junk to sell
    Item scrap("Scrap Metal", "Valuable scrap", ItemCategory::JUNK, ItemRarity::COMMON, 20, 2.0f, true, 50);
    m_inventory.addItem(scrap, 3);

    std::cout << "[PlayerState] Starting items added. Inventory weight: "
              << m_inventory.getTotalWeight() << "/" << m_inventory.getMaxWeight() << " kg" << std::endl;
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

void PlayerState::modifyVehicleCondition(float amount) {
    setVehicleCondition(m_vehicleCondition + amount);
}

// Mood management (0-100%)
void PlayerState::setMood(float mood) {
    m_mood = std::max(0.0f, std::min(mood, 100.0f));
}

void PlayerState::modifyMood(float amount) {
    setMood(m_mood + amount);
}

// Reputation management (-50..+50)
void PlayerState::setReputation(int reputation) {
    m_reputation = std::max(-50, std::min(reputation, 50));
}

void PlayerState::modifyReputation(int amount) {
    setReputation(m_reputation + amount);
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

// ========== PRINCIPLES AND TRAITS SYSTEM ==========

// Add a principle (acquired lesson/philosophy)
void PlayerState::addPrinciple(const std::string& principleId) {
    // Check if already exists
    if (std::find(m_principles.begin(), m_principles.end(), principleId) == m_principles.end()) {
        m_principles.push_back(principleId);
        std::cout << "[PlayerState] Acquired principle: " << principleId << std::endl;
    }
}

// Add a character trait
void PlayerState::addTrait(const std::string& traitId) {
    // Check if already exists
    if (std::find(m_traits.begin(), m_traits.end(), traitId) == m_traits.end()) {
        m_traits.push_back(traitId);
        std::cout << "[PlayerState] Acquired trait: " << traitId << std::endl;
    }
}

// Add a story item (narrative artifact)
void PlayerState::addStoryItem(const std::string& itemId) {
    // Check if already exists
    if (std::find(m_storyItems.begin(), m_storyItems.end(), itemId) == m_storyItems.end()) {
        m_storyItems.push_back(itemId);
        std::cout << "[PlayerState] Acquired story item: " << itemId << std::endl;
    }
}

// Check if player has a specific principle
bool PlayerState::hasPrinciple(const std::string& principleId) const {
    return std::find(m_principles.begin(), m_principles.end(), principleId) != m_principles.end();
}

// Check if player has a specific trait
bool PlayerState::hasTrait(const std::string& traitId) const {
    return std::find(m_traits.begin(), m_traits.end(), traitId) != m_traits.end();
}

// Check if player has a specific story item
bool PlayerState::hasStoryItem(const std::string& itemId) const {
    return std::find(m_storyItems.begin(), m_storyItems.end(), itemId) != m_storyItems.end();
}
