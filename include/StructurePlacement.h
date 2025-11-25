#ifndef STRUCTURE_PLACEMENT_H
#define STRUCTURE_PLACEMENT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>

// StructurePlacement - Manages automatic generation and rendering of structures
// Purpose: Generate and place structures (buildings, gas stations, stores, etc.) along the road
// Features:
//   - Procedural generation of structures at intervals
//   - Different structure types with varying properties
//   - Collision detection for player proximity
//   - Automatic cleanup of off-screen structures
class StructurePlacement {
public:
    // Structure type enum
    enum class StructureType {
        GAS_STATION,
        STORE,
        GARAGE,
        MOTEL,
        DINER,
        MECHANIC,
        REST_STOP,
        BILLBOARD
    };
    
    // Structure definition
    struct Structure {
        StructureType type;
        float x;                  // X position in world space
        float y;                  // Y position in screen space
        float width;
        float height;
        sf::RectangleShape shape;
        std::string name;
        bool isInteractive;       // Can player interact with this structure?
        
        Structure(StructureType t, float posX, float posY, float w, float h);
    };
    
    StructurePlacement();
    
    // Generate initial structures
    void generateInitialStructures(float startX, float endX);
    
    // Update structures (generate new ones if needed, cleanup old ones)
    void update(float cameraX, float deltaTime);
    
    // Render all visible structures
    void render(sf::RenderWindow& window, float cameraX, float screenWidth);
    
    // Проверить находится ли игрок рядом с каким-либо сооружением
    Structure* getNearbyStructure(float playerX, float playerY, float proximityRange);
    
    // Get all structures (for debugging/testing)
    const std::vector<Structure>& getStructures() const { return m_structures; }
    
private:
    std::vector<Structure> m_structures;
    std::mt19937 m_randomEngine;
    
    // Generation parameters
    float m_minStructureSpacing;
    float m_maxStructureSpacing;
    float m_lastGeneratedX;
    
    // Generate a single structure at position
    void generateStructure(float x);
    
    // Get random structure type
    StructureType getRandomStructureType();
    
    // Get structure properties based on type
    void getStructureProperties(StructureType type, float& width, float& height, 
                                sf::Color& color, std::string& name, bool& interactive);
    
    // Cleanup structures that are too far behind camera
    void cleanupStructures(float cameraX);
};

#endif // STRUCTURE_PLACEMENT_H
