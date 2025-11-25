#ifndef LOCATION_GENERATOR_H
#define LOCATION_GENERATOR_H

#include "Location.h"
#include "GameTypes.h"
#include <vector>
#include <memory>
#include <random>

// LocationGenerator - Procedural generation of home location and nearby structures
// Purpose: Generate starting area based on player's origin and biome type
// Features:
//   - Biome-based generation (affects structure types and placement)
//   - Origin-based customization (different starting scenarios)
//   - Structure placement algorithms (3-4 buildings around home)
//   - Neighbor structure logic (related buildings grouped together)
//   - Visual variety through randomization
class LocationGenerator {
public:
    LocationGenerator();
    
    // Generate home location with nearby structures
    // origin: Player's character background
    // screenWidth: Screen width for positioning
    // screenHeight: Screen height for positioning
    // Returns: Vector of locations (first is always home)
    std::vector<std::shared_ptr<Location>> generateHomeArea(
        OriginType origin,
        float screenWidth,
        float screenHeight
    );
    
    // Get the biome type for the generated area
    BiomeType getCurrentBiome() const { return m_currentBiome; }
    
private:
    BiomeType m_currentBiome;
    std::mt19937 m_randomEngine;
    
    // Get structure types appropriate for biome
    std::vector<LocationType> getStructureTypesForBiome(BiomeType biome);
    
    // Get random structure type from biome-appropriate list
    LocationType getRandomStructureType(BiomeType biome);
    
    // Calculate position for structure around home
    sf::Vector2f calculateStructurePosition(
        const sf::Vector2f& homePosition,
        int structureIndex,
        int totalStructures,
        float screenWidth,
        float screenHeight
    );
    
    // Get complementary structure for neighboring placement
    // e.g., gas station near diner, motel near store
    LocationType getNeighborStructure(LocationType primaryType);
    
    // Check if two structures should be placed near each other
    bool areNeighbors(LocationType type1, LocationType type2);
};

#endif // LOCATION_GENERATOR_H
