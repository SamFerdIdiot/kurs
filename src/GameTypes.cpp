#include "GameTypes.h"





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
            return sf::Color(218, 165, 32);
        case BiomeType::FOREST:
            return sf::Color(34, 139, 34);
        case BiomeType::COASTAL:
            return sf::Color(0, 191, 255);
        case BiomeType::MOUNTAIN:
            return sf::Color(112, 128, 144);
        case BiomeType::URBAN:
            return sf::Color(169, 169, 169);
        case BiomeType::SUBURBAN:
            return sf::Color(154, 205, 50);
        default:
            return sf::Color::White;
    }
}





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





CarType indexToCarType(int index) {
    switch (index) {
        case 0:
            return CarType::SPORTS_COUPE;
        case 1:
            return CarType::OLD_SEDAN;
        case 2:
            return CarType::OFFROAD_VAN;
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





StartingResources getStartingResources(OriginType origin, CarType car) {

    float baseMoney = 200.0f;
    float baseFuel = 100.0f;
    float baseEnergy = 100.0f;


    switch (origin) {
        case OriginType::NOMAD:

            baseMoney *= 0.75f;
            baseEnergy *= 1.1f;
            break;

        case OriginType::HITCHHIKER:

            baseMoney *= 1.0f;
            baseEnergy *= 1.0f;
            break;

        case OriginType::EX_RACER:

            baseMoney *= 1.25f;
            baseFuel *= 1.1f;
            break;

        default:
            break;
    }


    switch (car) {
        case CarType::SPORTS_COUPE:

            baseFuel *= 1.0f;
            baseMoney -= 50.0f;
            break;

        case CarType::OLD_SEDAN:

            baseFuel *= 1.0f;
            baseMoney += 0.0f;
            break;

        case CarType::OFFROAD_VAN:

            baseFuel *= 0.75f;
            baseMoney -= 20.0f;
            break;

        default:
            break;
    }


    baseMoney = std::max(50.0f, baseMoney);
    baseFuel = std::max(50.0f, std::min(100.0f, baseFuel));
    baseEnergy = std::max(80.0f, std::min(100.0f, baseEnergy));

    return StartingResources(baseMoney, baseFuel, baseEnergy);
}
