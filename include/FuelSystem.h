#ifndef FUEL_SYSTEM_H
#define FUEL_SYSTEM_H

#include "GameTypes.h"
#include <string>

// FuelSystem - Comprehensive fuel consumption and refueling calculations
// Purpose: Calculate fuel consumption, costs, and range for different car types
// Features:
//   - Fuel consumption rates per car type
//   - Road type modifiers (highway vs rough road)
//   - Range calculations (how far can you go?)
//   - Refueling cost calculations
//   - Low fuel warnings
//   - Optimal refueling strategy
class FuelSystem {
public:
    // Fuel consumption rates (liters per 100 km)
    struct FuelConsumptionRates {
        float highway;      // Best consumption (smooth road)
        float road;         // Normal consumption
        float roughRoad;    // Worst consumption (bad road)
    };

    // Refuel pricing
    struct RefuelPricing {
        float pricePerLiter;        // Base price per liter
        float fullTankDiscount;     // Discount % for full tank (0.0-1.0)
        float serviceFee;           // Fixed service fee
    };

    // Trip fuel requirements
    struct TripFuelData {
        float distance;             // Trip distance in km
        float fuelRequired;         // Fuel needed in liters
        float fuelCost;             // Total cost in rubles
        bool canReach;              // Can reach with current fuel
        float fuelRemaining;        // Fuel left after trip
        std::string warningMessage; // Warning if low fuel
    };

    // Get fuel consumption rates for a car type
    static FuelConsumptionRates getConsumptionRates(CarType carType);

    // Calculate fuel consumption for a trip
    // Returns liters consumed
    static float calculateFuelConsumption(
        CarType carType,
        float distance,                  // km
        const std::string& roadType      // "highway", "road", "rough"
    );

    // Calculate how far you can go with current fuel
    static float calculateRange(
        CarType carType,
        float currentFuel,               // liters
        const std::string& roadType      // "highway", "road", "rough"
    );

    // Calculate refueling cost
    static float calculateRefuelCost(
        float litersToRefuel,
        const RefuelPricing& pricing
    );

    // Calculate optimal refuel amount
    // Returns how many liters to buy
    static float calculateOptimalRefuel(
        float currentFuel,               // Current fuel in tank
        float tankCapacity,              // Max tank size
        float nextStationDistance,       // Distance to next gas station
        CarType carType,
        const std::string& roadType
    );

    // Check if trip is possible with current fuel
    static TripFuelData analyzeTripFuel(
        float currentFuel,
        float tankCapacity,
        float distance,
        CarType carType,
        const std::string& roadType,
        const RefuelPricing& pricing
    );

    // Get fuel warning level
    // Returns: 0 = OK, 1 = Low, 2 = Critical, 3 = Empty
    static int getFuelWarningLevel(float fuelPercent);

    // Get fuel warning message
    static std::string getFuelWarningMessage(int warningLevel);

    // Default pricing (can be overridden per location)
    static RefuelPricing getDefaultPricing();

    // Constants
    static constexpr float FUEL_TANK_CAPACITY = 100.0f;  // liters
    static constexpr float FUEL_WARNING_LOW = 25.0f;     // % when to warn
    static constexpr float FUEL_WARNING_CRITICAL = 10.0f; // % critical warning
    static constexpr float FUEL_RESERVE = 5.0f;          // % reserve fuel

private:
    // Helper: Get consumption rate for road type
    static float getConsumptionForRoadType(
        const FuelConsumptionRates& rates,
        const std::string& roadType
    );
};

// Helper struct for LocationScene refueling UI
struct RefuelOption {
    std::string name;           // "Fill Tank", "50L", "25L", etc.
    float liters;               // Amount of fuel
    float cost;                 // Total cost
    float finalFuelLevel;       // Final fuel % after refuel
    bool isRecommended;         // Is this the recommended option?
    std::string description;    // Additional info
};

// Create refuel options for UI
class RefuelOptionsGenerator {
public:
    // Generate refuel options for player
    static std::vector<RefuelOption> generateOptions(
        float currentFuel,          // Current fuel in liters
        float tankCapacity,         // Max tank capacity
        float playerMoney,          // Available money
        const FuelSystem::RefuelPricing& pricing,
        float nextStationDistance = 0.0f,  // Optional: distance to next station
        CarType carType = CarType::OLD_SEDAN  // For consumption calc
    );
};

#endif // FUEL_SYSTEM_H
