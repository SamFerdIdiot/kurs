#include "FuelSystem.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>

// ============================================================================
// FUEL CONSUMPTION RATES BY CAR TYPE
// ============================================================================

FuelSystem::FuelConsumptionRates FuelSystem::getConsumptionRates(CarType carType) {
    FuelConsumptionRates rates;

    switch (carType) {
        case CarType::OLD_SEDAN:
            // Reliable but not efficient
            rates.highway = 7.0f;      // 7L/100km on highway
            rates.road = 9.0f;         // 9L/100km on normal road
            rates.roughRoad = 12.0f;   // 12L/100km on rough road
            break;

        case CarType::OFFROAD_VAN:
            // Heavy and thirsty
            rates.highway = 11.0f;     // 11L/100km on highway
            rates.road = 13.0f;        // 13L/100km on normal road
            rates.roughRoad = 15.0f;   // 15L/100km on rough road (but handles it better)
            break;

        case CarType::SPORTS_COUPE:
            // Fast but fuel-hungry
            rates.highway = 9.0f;      // 9L/100km on highway
            rates.road = 11.0f;        // 11L/100km on normal road
            rates.roughRoad = 16.0f;   // 16L/100km on rough road (not designed for this)
            break;

        default:
            // Default to OLD_SEDAN
            rates.highway = 7.0f;
            rates.road = 9.0f;
            rates.roughRoad = 12.0f;
            break;
    }

    return rates;
}

// ============================================================================
// FUEL CONSUMPTION CALCULATION
// ============================================================================

float FuelSystem::getConsumptionForRoadType(
    const FuelConsumptionRates& rates,
    const std::string& roadType
) {
    if (roadType == "highway") {
        return rates.highway;
    } else if (roadType == "road") {
        return rates.road;
    } else if (roadType == "rough" || roadType == "path") {
        return rates.roughRoad;
    } else {
        // Default to normal road
        return rates.road;
    }
}

float FuelSystem::calculateFuelConsumption(
    CarType carType,
    float distance,
    const std::string& roadType
) {
    FuelConsumptionRates rates = getConsumptionRates(carType);
    float consumption = getConsumptionForRoadType(rates, roadType);

    // Calculate fuel consumed
    // consumption is in L/100km, so:
    // fuel = (distance / 100) * consumption
    float fuelConsumed = (distance / 100.0f) * consumption;

    return fuelConsumed;
}

// ============================================================================
// RANGE CALCULATION
// ============================================================================

float FuelSystem::calculateRange(
    CarType carType,
    float currentFuel,
    const std::string& roadType
) {
    FuelConsumptionRates rates = getConsumptionRates(carType);
    float consumption = getConsumptionForRoadType(rates, roadType);

    if (consumption <= 0.0f) {
        return 0.0f;
    }

    // Range = (fuel / consumption) * 100
    // If we have X liters and consume Y L/100km,
    // we can go (X / Y) * 100 km
    float range = (currentFuel / consumption) * 100.0f;

    // Subtract reserve fuel
    float reserveFuel = FUEL_TANK_CAPACITY * (FUEL_RESERVE / 100.0f);
    float usableFuel = std::max(0.0f, currentFuel - reserveFuel);
    float safeRange = (usableFuel / consumption) * 100.0f;

    return safeRange;
}

// ============================================================================
// REFUELING COST CALCULATION
// ============================================================================

FuelSystem::RefuelPricing FuelSystem::getDefaultPricing() {
    RefuelPricing pricing;
    pricing.pricePerLiter = 50.0f;          // 50₽ per liter (realistic for Russia)
    pricing.fullTankDiscount = 0.05f;       // 5% discount for full tank
    pricing.serviceFee = 0.0f;              // No service fee
    return pricing;
}

float FuelSystem::calculateRefuelCost(
    float litersToRefuel,
    const RefuelPricing& pricing
) {
    float baseCost = litersToRefuel * pricing.pricePerLiter;

    // Apply full tank discount if refueling more than 90% of tank
    float fullTankThreshold = FUEL_TANK_CAPACITY * 0.9f;
    if (litersToRefuel >= fullTankThreshold) {
        baseCost *= (1.0f - pricing.fullTankDiscount);
    }

    // Add service fee
    float totalCost = baseCost + pricing.serviceFee;

    return totalCost;
}

// ============================================================================
// OPTIMAL REFUEL CALCULATION
// ============================================================================

float FuelSystem::calculateOptimalRefuel(
    float currentFuel,
    float tankCapacity,
    float nextStationDistance,
    CarType carType,
    const std::string& roadType
) {
    // Calculate fuel needed to reach next station
    float fuelNeeded = calculateFuelConsumption(carType, nextStationDistance, roadType);

    // Add safety margin (20%)
    float safetyMargin = 1.2f;
    float fuelWithMargin = fuelNeeded * safetyMargin;

    // Calculate how much we need to refuel
    float optimalRefuel = fuelWithMargin - currentFuel;

    // Clamp to tank capacity
    optimalRefuel = std::max(0.0f, optimalRefuel);
    optimalRefuel = std::min(optimalRefuel, tankCapacity - currentFuel);

    // If we're close to full tank threshold, recommend full tank for discount
    float fullTankThreshold = tankCapacity * 0.9f;
    float refuelToFull = tankCapacity - currentFuel;

    if (refuelToFull - optimalRefuel < tankCapacity * 0.2f) {
        // If we're within 20% of full tank, just fill it
        optimalRefuel = refuelToFull;
    }

    return optimalRefuel;
}

// ============================================================================
// TRIP FUEL ANALYSIS
// ============================================================================

FuelSystem::TripFuelData FuelSystem::analyzeTripFuel(
    float currentFuel,
    float tankCapacity,
    float distance,
    CarType carType,
    const std::string& roadType,
    const RefuelPricing& pricing
) {
    TripFuelData data;
    data.distance = distance;
    data.fuelRequired = calculateFuelConsumption(carType, distance, roadType);
    data.fuelCost = calculateRefuelCost(data.fuelRequired, pricing);
    data.canReach = (currentFuel >= data.fuelRequired);
    data.fuelRemaining = currentFuel - data.fuelRequired;

    // Generate warning message
    if (!data.canReach) {
        float shortage = data.fuelRequired - currentFuel;
        std::ostringstream oss;
        oss << "Not enough fuel! Need " << std::fixed << std::setprecision(1)
            << shortage << "L more";
        data.warningMessage = oss.str();
    } else if (data.fuelRemaining < tankCapacity * (FUEL_WARNING_CRITICAL / 100.0f)) {
        std::ostringstream oss;
        oss << "Warning: Only " << std::fixed << std::setprecision(1)
            << data.fuelRemaining << "L remaining after trip";
        data.warningMessage = oss.str();
    } else if (data.fuelRemaining < tankCapacity * (FUEL_WARNING_LOW / 100.0f)) {
        data.warningMessage = "Fuel will be low after this trip";
    } else {
        data.warningMessage = "Sufficient fuel for this trip";
    }

    return data;
}

// ============================================================================
// FUEL WARNING SYSTEM
// ============================================================================

int FuelSystem::getFuelWarningLevel(float fuelPercent) {
    if (fuelPercent <= 0.0f) {
        return 3;  // Empty
    } else if (fuelPercent <= FUEL_WARNING_CRITICAL) {
        return 2;  // Critical
    } else if (fuelPercent <= FUEL_WARNING_LOW) {
        return 1;  // Low
    } else {
        return 0;  // OK
    }
}

std::string FuelSystem::getFuelWarningMessage(int warningLevel) {
    switch (warningLevel) {
        case 3:
            return "OUT OF FUEL! Find a gas station immediately!";
        case 2:
            return "CRITICAL: Fuel very low! Refuel as soon as possible!";
        case 1:
            return "Warning: Fuel is running low";
        case 0:
        default:
            return "Fuel level OK";
    }
}

// ============================================================================
// REFUEL OPTIONS GENERATOR
// ============================================================================

std::vector<RefuelOption> RefuelOptionsGenerator::generateOptions(
    float currentFuel,
    float tankCapacity,
    float playerMoney,
    const FuelSystem::RefuelPricing& pricing,
    float nextStationDistance,
    CarType carType
) {
    std::vector<RefuelOption> options;

    // Calculate amounts to offer
    float availableSpace = tankCapacity - currentFuel;

    // Option 1: Fill Tank (full refuel)
    if (availableSpace > 0.1f) {  // If tank not already full
        RefuelOption fillTank;
        fillTank.name = "Fill Tank";
        fillTank.liters = availableSpace;
        fillTank.cost = FuelSystem::calculateRefuelCost(availableSpace, pricing);
        fillTank.finalFuelLevel = 100.0f;
        fillTank.isRecommended = (fillTank.liters >= tankCapacity * 0.7f);  // Recommend if > 70L needed
        fillTank.description = "Full tank";

        // Add discount note if applicable
        if (fillTank.liters >= tankCapacity * 0.9f && pricing.fullTankDiscount > 0.0f) {
            std::ostringstream oss;
            oss << "Full tank (5% discount!)";
            fillTank.description = oss.str();
        }

        if (fillTank.cost <= playerMoney) {
            options.push_back(fillTank);
        }
    }

    // Option 2: Optimal amount (if next station distance known)
    if (nextStationDistance > 0.0f) {
        float optimalAmount = FuelSystem::calculateOptimalRefuel(
            currentFuel, tankCapacity, nextStationDistance, carType, "road"
        );

        if (optimalAmount > 0.1f && optimalAmount < availableSpace * 0.95f) {
            RefuelOption optimal;
            optimal.name = "Optimal";
            optimal.liters = optimalAmount;
            optimal.cost = FuelSystem::calculateRefuelCost(optimalAmount, pricing);
            optimal.finalFuelLevel = ((currentFuel + optimalAmount) / tankCapacity) * 100.0f;
            optimal.isRecommended = true;

            std::ostringstream oss;
            oss << "Enough to reach next station + 20%";
            optimal.description = oss.str();

            if (optimal.cost <= playerMoney) {
                options.push_back(optimal);
            }
        }
    }

    // Option 3-5: Fixed amounts (50L, 25L, 10L)
    float fixedAmounts[] = {50.0f, 25.0f, 10.0f};
    const char* fixedNames[] = {"50 Liters", "25 Liters", "10 Liters"};

    for (int i = 0; i < 3; ++i) {
        float amount = fixedAmounts[i];
        if (amount <= availableSpace && amount > 0.1f) {
            RefuelOption fixed;
            fixed.name = fixedNames[i];
            fixed.liters = amount;
            fixed.cost = FuelSystem::calculateRefuelCost(amount, pricing);
            fixed.finalFuelLevel = ((currentFuel + amount) / tankCapacity) * 100.0f;
            fixed.isRecommended = false;

            std::ostringstream oss;
            oss << std::fixed << std::setprecision(0) << fixed.finalFuelLevel << "% full";
            fixed.description = oss.str();

            if (fixed.cost <= playerMoney) {
                options.push_back(fixed);
            }
        }
    }

    // Option 6: Minimum (just enough to reach next station, if known)
    if (nextStationDistance > 0.0f) {
        float minAmount = FuelSystem::calculateFuelConsumption(carType, nextStationDistance, "road");
        float shortage = minAmount - currentFuel;

        if (shortage > 0.1f && shortage < availableSpace) {
            RefuelOption minimum;
            minimum.name = "Minimum";
            minimum.liters = shortage;
            minimum.cost = FuelSystem::calculateRefuelCost(shortage, pricing);
            minimum.finalFuelLevel = ((currentFuel + shortage) / tankCapacity) * 100.0f;
            minimum.isRecommended = false;
            minimum.description = "Just enough to reach next station (risky!)";

            if (minimum.cost <= playerMoney) {
                options.push_back(minimum);
            }
        }
    }

    // Sort options: recommended first, then by amount descending
    std::sort(options.begin(), options.end(), [](const RefuelOption& a, const RefuelOption& b) {
        if (a.isRecommended != b.isRecommended) {
            return a.isRecommended;  // Recommended first
        }
        return a.liters > b.liters;  // Then by amount descending
    });

    return options;
}
