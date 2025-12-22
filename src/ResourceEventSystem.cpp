#include "ResourceEventSystem.h"
#include <iostream>
#include <sstream>

ResourceEventSystem::ResourceEventSystem(PlayerState* playerState, EventManager* eventManager)
    : m_playerState(playerState), m_eventManager(eventManager) {
}

std::string ResourceEventSystem::checkResourcesAndTriggerEvents() {
    if (!m_playerState || !m_eventManager) {
        return "";
    }

    // Проверяем ресурсы в порядке критичности:
    // 1. Топливо (самое критичное - без него не доедешь)
    std::string eventId = checkFuel();
    if (!eventId.empty()) {
        return eventId;
    }

    // 2. Энергия (усталость опасна на дороге)
    eventId = checkEnergy();
    if (!eventId.empty()) {
        return eventId;
    }

    // 3. Состояние машины (поломка опасна)
    eventId = checkVehicle();
    if (!eventId.empty()) {
        return eventId;
    }

    // 4. Деньги (важно, но не критично прямо сейчас)
    eventId = checkMoney();
    if (!eventId.empty()) {
        return eventId;
    }

    // 5. Настроение (влияет на решения)
    eventId = checkMood();
    if (!eventId.empty()) {
        return eventId;
    }

    return "";
}

std::string ResourceEventSystem::checkSpecificResource(const std::string& resourceType) {
    if (resourceType == "fuel") {
        return checkFuel();
    } else if (resourceType == "energy") {
        return checkEnergy();
    } else if (resourceType == "money") {
        return checkMoney();
    } else if (resourceType == "vehicle") {
        return checkVehicle();
    } else if (resourceType == "mood") {
        return checkMood();
    }
    return "";
}

std::string ResourceEventSystem::checkFuel() {
    float fuel = m_playerState->getFuel();

    // Очень мало топлива (<10L) - КРИТИЧНО
    if (fuel < m_thresholds.fuel_very_low && !m_fuelWarningShown) {
        m_fuelWarningShown = true;
        std::cout << "[ResourceEvent] CRITICAL: Fuel very low (" << fuel << "L)" << std::endl;
        return "resource_fuel_critical";  // ID события
    }

    // Мало топлива (<20L)
    if (fuel < m_thresholds.fuel_critical && !m_fuelWarningShown) {
        m_fuelWarningShown = true;
        std::cout << "[ResourceEvent] WARNING: Fuel low (" << fuel << "L)" << std::endl;
        return "resource_fuel_low";
    }

    // Топливо восстановлено
    if (fuel > m_thresholds.fuel_low && m_fuelWarningShown) {
        m_fuelWarningShown = false;
    }

    return "";
}

std::string ResourceEventSystem::checkEnergy() {
    float energy = m_playerState->getEnergy();

    // Истощение (<10%)
    if (energy < m_thresholds.energy_very_tired && !m_energyWarningShown) {
        m_energyWarningShown = true;
        std::cout << "[ResourceEvent] CRITICAL: Energy exhausted (" << energy << "%)" << std::endl;
        return "resource_energy_exhausted";
    }

    // Усталость (<30%)
    if (energy < m_thresholds.energy_tired && !m_energyWarningShown) {
        m_energyWarningShown = true;
        std::cout << "[ResourceEvent] WARNING: Energy low (" << energy << "%)" << std::endl;
        return "resource_energy_tired";
    }

    // Энергия восстановлена
    if (energy > m_thresholds.energy_tired + 10.0f && m_energyWarningShown) {
        m_energyWarningShown = false;
    }

    return "";
}

std::string ResourceEventSystem::checkMoney() {
    float money = m_playerState->getMoney();

    // Почти без денег (<50₽)
    if (money < m_thresholds.money_broke && !m_moneyWarningShown) {
        m_moneyWarningShown = true;
        std::cout << "[ResourceEvent] WARNING: Almost broke (" << money << "₽)" << std::endl;
        return "resource_money_broke";
    }

    // Мало денег (<200₽)
    if (money < m_thresholds.money_low && !m_moneyWarningShown) {
        m_moneyWarningShown = true;
        std::cout << "[ResourceEvent] INFO: Money low (" << money << "₽)" << std::endl;
        return "resource_money_low";
    }

    // Деньги восстановлены
    if (money > m_thresholds.money_low + 100.0f && m_moneyWarningShown) {
        m_moneyWarningShown = false;
    }

    return "";
}

std::string ResourceEventSystem::checkVehicle() {
    float condition = m_playerState->getVehicleCondition();

    // Критическое состояние (<25%)
    if (condition < m_thresholds.vehicle_broken && !m_vehicleWarningShown) {
        m_vehicleWarningShown = true;
        std::cout << "[ResourceEvent] CRITICAL: Vehicle broken (" << condition << "%)" << std::endl;
        return "resource_vehicle_broken";
    }

    // Повреждена (<50%)
    if (condition < m_thresholds.vehicle_damaged && !m_vehicleWarningShown) {
        m_vehicleWarningShown = true;
        std::cout << "[ResourceEvent] WARNING: Vehicle damaged (" << condition << "%)" << std::endl;
        return "resource_vehicle_damaged";
    }

    // Машина отремонтирована
    if (condition > m_thresholds.vehicle_damaged + 15.0f && m_vehicleWarningShown) {
        m_vehicleWarningShown = false;
    }

    return "";
}

std::string ResourceEventSystem::checkMood() {
    float mood = m_playerState->getMood();

    // Депрессия (<20%)
    if (mood < m_thresholds.mood_depressed) {
        std::cout << "[ResourceEvent] INFO: Mood depressed (" << mood << "%)" << std::endl;
        return "resource_mood_depressed";
    }

    // Плохое настроение (<40%)
    if (mood < m_thresholds.mood_low) {
        std::cout << "[ResourceEvent] INFO: Mood low (" << mood << "%)" << std::endl;
        return "resource_mood_low";
    }

    return "";
}

std::string ResourceEventSystem::getResourceWarning() const {
    if (!m_playerState) {
        return "";
    }

    std::stringstream warning;
    bool hasWarning = false;

    float fuel = m_playerState->getFuel();
    float energy = m_playerState->getEnergy();
    float money = m_playerState->getMoney();
    float vehicle = m_playerState->getVehicleCondition();

    if (fuel < m_thresholds.fuel_critical) {
        warning << "⚠ ТОПЛИВО: " << fuel << "L (критично!)\n";
        hasWarning = true;
    }

    if (energy < m_thresholds.energy_tired) {
        warning << "⚠ ЭНЕРГИЯ: " << energy << "% (усталость)\n";
        hasWarning = true;
    }

    if (money < m_thresholds.money_broke) {
        warning << "⚠ ДЕНЬГИ: " << money << "₽ (почти нет)\n";
        hasWarning = true;
    }

    if (vehicle < m_thresholds.vehicle_damaged) {
        warning << "⚠ МАШИНА: " << vehicle << "% (повреждена)\n";
        hasWarning = true;
    }

    return hasWarning ? warning.str() : "";
}

bool ResourceEventSystem::hasCriticalResources() const {
    if (!m_playerState) {
        return false;
    }

    return m_playerState->getFuel() < m_thresholds.fuel_very_low ||
           m_playerState->getEnergy() < m_thresholds.energy_very_tired ||
           m_playerState->getVehicleCondition() < m_thresholds.vehicle_broken;
}

void ResourceEventSystem::setThresholds(const ResourceThresholds& thresholds) {
    m_thresholds = thresholds;
}

void ResourceEventSystem::resetWarningFlags() {
    m_fuelWarningShown = false;
    m_energyWarningShown = false;
    m_moneyWarningShown = false;
    m_vehicleWarningShown = false;
}
