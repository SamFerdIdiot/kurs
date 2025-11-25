#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <string>
#include <SFML/Graphics.hpp>

// BiomeType - Different environmental regions for location generation
// Affects structure types, visual style, and available resources
enum class BiomeType {
    DESERT,      // Hot, sandy areas with limited water
    FOREST,      // Wooded areas with nature
    COASTAL,     // Beach/ocean areas
    MOUNTAIN,    // Elevated terrain
    URBAN,       // City/town areas
    SUBURBAN     // Mixed residential areas
};

// OriginType - Player's character background from creation
// Determines starting resources, biome preference, and initial story
enum class OriginType {
    NOMAD,       // Wanderer, used to rough conditions
    HITCHHIKER,  // Traveler, social and resourceful
    EX_RACER,    // Former racer, knows vehicles well
    UNKNOWN      // Not yet selected
};

// CarType - Vehicle type selected during character creation
// Affects fuel efficiency, capacity, and starting condition
enum class CarType {
    OLD_SEDAN,     // Reliable but slow, good fuel economy
    OFFROAD_VAN,   // Spacious, handles rough terrain
    SPORTS_COUPE,  // Fast but fuel-hungry, fragile
    UNKNOWN        // Not yet selected
};

// Helper functions for type conversions and display

// Get display name for biome type
std::string getBiomeTypeName(BiomeType type);

// Get color representing biome type
sf::Color getBiomeColor(BiomeType type);

// Convert character creation background index to OriginType
OriginType indexToOriginType(int index);

// Convert character creation car index to CarType
CarType indexToCarType(int index);

// Get display name for origin type
std::string getOriginTypeName(OriginType type);

// Get display name for car type
std::string getCarTypeName(CarType type);

// Get biome type preference based on origin
BiomeType getPreferredBiome(OriginType origin);

// Starting resource modifiers based on origin and car
struct StartingResources {
    float money;      // Starting cash
    float fuel;       // Starting fuel percentage (0-100)
    float energy;     // Starting energy percentage (0-100)
    
    // Default constructor
    StartingResources() : money(200.0f), fuel(100.0f), energy(100.0f) {}
    
    // Constructor with values
    StartingResources(float m, float f, float e) : money(m), fuel(f), energy(e) {}
};

// Get starting resources based on origin and car combination
StartingResources getStartingResources(OriginType origin, CarType car);

#endif // GAME_TYPES_H
