#include "PlayerState.h"
#include <algorithm>
#include <iostream>


PlayerState::PlayerState()
    : m_energy(100.0f),
      m_money(1000.0f),
      m_fuel(100.0f),
      m_vehicleCondition(100.0f),
      m_mood(70.0f),
      m_reputation(0),
      m_origin(OriginType::NOMAD),
      m_carType(CarType::OLD_SEDAN),
      m_inventory(24, 50.0f),
      m_currentCityIndex(0),
      m_totalPlayTime(0.0f),
      m_currentNodeId(0),
      m_currentNotebookEntryId("tutorial_start") {


    std::cout << "[PlayerState] Adding starting items to inventory..." << std::endl;


    Item bread("Bread", "Fresh bread", ItemCategory::CONSUMABLE, ItemRarity::COMMON, 50, 0.5f, true, 10);
    m_inventory.addItem(bread, 3);

    Item water("Water", "Bottle of water", ItemCategory::CONSUMABLE, ItemRarity::COMMON, 30, 1.0f, true, 20);
    m_inventory.addItem(water, 5);


    Item map("Map", "Road map", ItemCategory::TOOL, ItemRarity::COMMON, 100, 0.2f, false, 1);
    m_inventory.addItem(map, 1);

    Item flashlight("Flashlight", "LED flashlight", ItemCategory::TOOL, ItemRarity::COMMON, 150, 0.5f, false, 1);
    m_inventory.addItem(flashlight, 1);


    Item scrap("Scrap Metal", "Valuable scrap", ItemCategory::JUNK, ItemRarity::COMMON, 20, 2.0f, true, 50);
    m_inventory.addItem(scrap, 3);

    std::cout << "[PlayerState] Starting items added. Inventory weight: "
              << m_inventory.getTotalWeight() << "/" << m_inventory.getMaxWeight() << " kg" << std::endl;
}


void PlayerState::setEnergy(float energy) {
    m_energy = std::max(0.0f, std::min(energy, 100.0f));
}

void PlayerState::addEnergy(float amount) {
    setEnergy(m_energy + amount);
}


void PlayerState::setMoney(float money) {
    m_money = std::max(0.0f, money);
}

void PlayerState::addMoney(float amount) {
    setMoney(m_money + amount);
}


void PlayerState::setFuel(float fuel) {
    m_fuel = std::max(0.0f, std::min(fuel, 100.0f));
}

void PlayerState::addFuel(float amount) {
    setFuel(m_fuel + amount);
}


void PlayerState::setVehicleCondition(float condition) {
    m_vehicleCondition = std::max(0.0f, std::min(condition, 100.0f));
}

void PlayerState::modifyVehicleCondition(float amount) {
    setVehicleCondition(m_vehicleCondition + amount);
}


void PlayerState::setMood(float mood) {
    m_mood = std::max(0.0f, std::min(mood, 100.0f));
}

void PlayerState::modifyMood(float amount) {
    setMood(m_mood + amount);
}


void PlayerState::setReputation(int reputation) {
    m_reputation = std::max(-50, std::min(reputation, 50));
}

void PlayerState::modifyReputation(int amount) {
    setReputation(m_reputation + amount);
}


void PlayerState::initializeResources() {
    initializeResources(m_origin, m_carType);
}

void PlayerState::initializeResources(OriginType origin, CarType carType) {
    m_origin = origin;
    m_carType = carType;


    switch (origin) {
        case OriginType::EX_RACER:
            m_money = 150.0f;
            m_fuel = 80.0f;
            m_energy = 100.0f;
            break;

        case OriginType::NOMAD:
            m_money = 200.0f;
            m_fuel = 100.0f;
            m_energy = 90.0f;
            break;

        case OriginType::HITCHHIKER:
            m_money = 100.0f;
            m_fuel = 60.0f;
            m_energy = 70.0f;
            break;

        default:
            m_money = 200.0f;
            m_fuel = 100.0f;
            m_energy = 100.0f;
            break;
    }
}


bool PlayerState::isGameOver() const {
    return (m_energy <= 0.0f || m_fuel <= 0.0f);
}


void PlayerState::initializeStartingInventory() {


}




void PlayerState::addPrinciple(const std::string& principleId) {

    if (std::find(m_principles.begin(), m_principles.end(), principleId) == m_principles.end()) {
        m_principles.push_back(principleId);
        std::cout << "[PlayerState] Acquired principle: " << principleId << std::endl;
    }
}


void PlayerState::addTrait(const std::string& traitId) {

    if (std::find(m_traits.begin(), m_traits.end(), traitId) == m_traits.end()) {
        m_traits.push_back(traitId);
        std::cout << "[PlayerState] Acquired trait: " << traitId << std::endl;
    }
}


void PlayerState::addStoryItem(const std::string& itemId) {

    if (std::find(m_storyItems.begin(), m_storyItems.end(), itemId) == m_storyItems.end()) {
        m_storyItems.push_back(itemId);
        std::cout << "[PlayerState] Acquired story item: " << itemId << std::endl;
    }
}


bool PlayerState::hasPrinciple(const std::string& principleId) const {
    return std::find(m_principles.begin(), m_principles.end(), principleId) != m_principles.end();
}


bool PlayerState::hasTrait(const std::string& traitId) const {
    return std::find(m_traits.begin(), m_traits.end(), traitId) != m_traits.end();
}


bool PlayerState::hasStoryItem(const std::string& itemId) const {
    return std::find(m_storyItems.begin(), m_storyItems.end(), itemId) != m_storyItems.end();
}
