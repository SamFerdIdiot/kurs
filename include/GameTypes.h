#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <string>
#include <SFML/Graphics.hpp>



enum class BiomeType {
    DESERT,
    FOREST,
    COASTAL,
    MOUNTAIN,
    URBAN,
    SUBURBAN
};



enum class OriginType {
    NOMAD,
    HITCHHIKER,
    EX_RACER,
    UNKNOWN
};



enum class CarType {
    OLD_SEDAN,
    OFFROAD_VAN,
    SPORTS_COUPE,
    UNKNOWN
};




std::string getBiomeTypeName(BiomeType type);


sf::Color getBiomeColor(BiomeType type);


OriginType indexToOriginType(int index);


CarType indexToCarType(int index);


std::string getOriginTypeName(OriginType type);


std::string getCarTypeName(CarType type);


BiomeType getPreferredBiome(OriginType origin);


struct StartingResources {
    float money;
    float fuel;
    float energy;


    StartingResources() : money(200.0f), fuel(100.0f), energy(100.0f) {}


    StartingResources(float m, float f, float e) : money(m), fuel(f), energy(e) {}
};


StartingResources getStartingResources(OriginType origin, CarType car);

#endif
