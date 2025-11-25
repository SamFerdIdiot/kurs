#ifndef START_ZONE_H
#define START_ZONE_H

#include "Location.h"
#include "LocationGenerator.h"
#include "GameTypes.h"
#include <vector>
#include <memory>
#include <random>

// StartZone manages the initial play area with home and nearby structures
class StartZone {
public:
    StartZone();
    
    // Generate the starting zone with home and 3-4 random structures
    // Uses player's origin to determine biome and structure types
    void generate(OriginType origin = OriginType::HITCHHIKER);
    
    // Get all locations in the start zone
    const std::vector<std::shared_ptr<Location>>& getLocations() const { return m_locations; }
    
    // Get home location specifically
    std::shared_ptr<Location> getHomeLocation() const;
    
    // Проверить находится ли игрок рядом с какой-либо локацией и вернуть ее
    std::shared_ptr<Location> getNearestLocation(const sf::Vector2f& playerPosition) const;
    
    // Get the current biome type
    BiomeType getCurrentBiome() const { return m_generator.getCurrentBiome(); }
    
    // Render all locations in the zone
    void render(sf::RenderWindow& window) const;
    
private:
    std::vector<std::shared_ptr<Location>> m_locations;
    LocationGenerator m_generator;
    std::mt19937 m_randomEngine;
    
    // Helper functions (kept for backward compatibility if needed)
    LocationType getRandomStructureType();
    sf::Vector2f getRandomPositionAroundHome(const sf::Vector2f& homePos, int index, int total);
};

#endif // START_ZONE_H
