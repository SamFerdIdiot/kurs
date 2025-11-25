#include "GameTypes.h"

// ============================================================================
// BiomeType Functions
// ============================================================================

std::string getBiomeTypeName(BiomeType type) {
    switch (type) {
        case BiomeType::DESERT:
            return "Пустыня / Desert";
        case BiomeType::FOREST:
            return "Лес / Forest";
        case BiomeType::COASTAL:
            return "Побережье / Coastal";
        case BiomeType::MOUNTAIN:
            return "Горы / Mountain";
        case BiomeType::URBAN:
            return "Город / Urban";
        case BiomeType::SUBURBAN:
            return "Пригород / Suburban";
        default:
            return "Неизвестно / Unknown";
    }
}

sf::Color getBiomeColor(BiomeType type) {
    switch (type) {
        case BiomeType::DESERT:
            return sf::Color(218, 165, 32);  // Goldenrod
        case BiomeType::FOREST:
            return sf::Color(34, 139, 34);   // ForestGreen
        case BiomeType::COASTAL:
            return sf::Color(0, 191, 255);   // DeepSkyBlue
        case BiomeType::MOUNTAIN:
            return sf::Color(112, 128, 144); // SlateGray
        case BiomeType::URBAN:
            return sf::Color(169, 169, 169); // DarkGray
        case BiomeType::SUBURBAN:
            return sf::Color(154, 205, 50);  // YellowGreen
        default:
            return sf::Color::White;
    }
}

// ============================================================================
// OriginType Functions
// ============================================================================

OriginType indexToOriginType(int index) {
    switch (index) {
        case 0:
            return OriginType::NOMAD;
        case 1:
            return OriginType::HITCHHIKER;
        case 2:
            return OriginType::EX_RACER;
        default:
            return OriginType::UNKNOWN;
    }
}

std::string getOriginTypeName(OriginType type) {
    switch (type) {
        case OriginType::NOMAD:
            return "Кочевник / Nomad";
        case OriginType::HITCHHIKER:
            return "Попутчик / Hitchhiker";
        case OriginType::EX_RACER:
            return "Экс-Гонщик / Ex-Racer";
        case OriginType::UNKNOWN:
        default:
            return "Неизвестно / Unknown";
    }
}

BiomeType getPreferredBiome(OriginType origin) {
    switch (origin) {
        case OriginType::NOMAD:
            return BiomeType::DESERT;
        case OriginType::HITCHHIKER:
            return BiomeType::SUBURBAN;
        case OriginType::EX_RACER:
            return BiomeType::URBAN;
        default:
            return BiomeType::FOREST;
    }
}

// ============================================================================
// CarType Functions
// ============================================================================

CarType indexToCarType(int index) {
    switch (index) {
        case 0:
            return CarType::SPORTS_COUPE;  // BMW E30 Coupe
        case 1:
            return CarType::OLD_SEDAN;     // Volvo 940 Sedan
        case 2:
            return CarType::OFFROAD_VAN;   // Mercedes-Benz Sprinter
        default:
            return CarType::UNKNOWN;
    }
}

std::string getCarTypeName(CarType type) {
    switch (type) {
        case CarType::SPORTS_COUPE:
            return "BMW E30 Coupe";
        case CarType::OLD_SEDAN:
            return "Volvo 940 Sedan";
        case CarType::OFFROAD_VAN:
            return "Mercedes-Benz Sprinter";
        case CarType::UNKNOWN:
        default:
            return "Неизвестно / Unknown";
    }
}

// ============================================================================
// Starting Resources Functions
// ============================================================================

StartingResources getStartingResources(OriginType origin, CarType car) {
    // Base resources
    float baseMoney = 200.0f;
    float baseFuel = 100.0f;
    float baseEnergy = 100.0f;
    
    // Origin modifiers
    switch (origin) {
        case OriginType::NOMAD:
            // Nomad: Less money, but more resourceful
            baseMoney *= 0.75f;  // $150
            baseEnergy *= 1.1f;  // 110% energy
            break;
            
        case OriginType::HITCHHIKER:
            // Hitchhiker: Average resources, social skills
            baseMoney *= 1.0f;   // $200
            baseEnergy *= 1.0f;  // 100% energy
            break;
            
        case OriginType::EX_RACER:
            // Ex-Racer: More money from racing, good condition
            baseMoney *= 1.25f;  // $250
            baseFuel *= 1.1f;    // 110% fuel
            break;
            
        default:
            break;
    }
    
    // Car modifiers (updated for BMW, Volvo, Mercedes)
    switch (car) {
        case CarType::SPORTS_COUPE: // BMW E30
            // Fast but expensive and fuel-hungry
            baseFuel *= 1.0f;    // Full tank (200L)
            baseMoney -= 50.0f;  // Expensive to maintain
            break;
            
        case CarType::OLD_SEDAN: // Volvo 940
            // Reliable, balanced, economical
            baseFuel *= 1.0f;    // Full tank (100L)
            baseMoney += 0.0f;   // No extra cost
            break;
            
        case CarType::OFFROAD_VAN: // Mercedes Sprinter
            // Utility vehicle, smaller tank
            baseFuel *= 0.75f;   // 75L tank
            baseMoney -= 20.0f;  // Commercial vehicle maintenance
            break;
            
        default:
            break;
    }
    
    // Ensure values are within reasonable bounds
    baseMoney = std::max(50.0f, baseMoney);      // Minimum $50
    baseFuel = std::max(50.0f, std::min(100.0f, baseFuel));    // 50-100%
    baseEnergy = std::max(80.0f, std::min(100.0f, baseEnergy)); // 80-100%
    
    return StartingResources(baseMoney, baseFuel, baseEnergy);
}
