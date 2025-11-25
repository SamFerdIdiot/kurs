#ifndef WORLD_MAP_SCENE_H
#define WORLD_MAP_SCENE_H

#include "Scene.h"
#include "PlayerState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>

// Node types representing different location categories
enum class NodeType {
    HOME_BASE,      // Starting location
    CITY,           // Large location with multiple services
    TOWN,           // Medium location with basic services
    JUNCTION,       // Small rest stop with 2-4 structures
    GAS_STATION,    // Gas station with shop
    WORKSHOP,       // Repair workshop
    UNIQUE          // Unique location (lighthouse, factory, etc.)
};

// Node status representing accessibility
enum class NodeStatus {
    LOCKED,         // Not visible on map
    VISIBLE,        // Visible but not accessible
    AVAILABLE,      // Available for travel
    CURRENT,        // Current player position
    VISITED         // Already visited by player
};

// Structure representing a building/service at a node
struct Structure {
    std::string type;       // "GAS_PUMP", "STORE", "GARAGE", "MOTEL", etc.
    std::string name;       // Display name
    bool available;         // Is structure open
};

// Node representing a location on the world map
struct MapNode {
    int id;                             // Unique identifier
    NodeType type;                      // Type of location
    std::string name;                   // Display name
    sf::Vector2f position;              // Position on map (x, y)
    NodeStatus status;                  // Current status
    std::vector<int> connections;       // IDs of connected nodes
    std::map<int, float> distances;     // Distances to connected nodes (km)
    std::vector<Structure> structures;  // Buildings at this location
    
    // Constructor
    MapNode(int id_, NodeType type_, const std::string& name_, 
            sf::Vector2f pos, NodeStatus status_)
        : id(id_), type(type_), name(name_), position(pos), status(status_) {}
};

// World Map Scene - main navigation interface
// Purpose: Display world map with nodes and roads, allow player to select destination
// Features:
//   - Procedurally generated or predefined nodes
//   - Visual roads connecting nodes
//   - Player pin marker on current location
//   - Mouse-based selection of available destinations
//   - Fuel consumption calculation
//   - Confirmation dialog before travel
class WorldMapScene : public Scene {
public:
    WorldMapScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Initialization methods
    void initializeNodes();
    void generateRoads();
    void updateNodeStatus();
    
    // Input handling
    void handleMouseClick(int mouseX, int mouseY);
    MapNode* getNodeAtPosition(int mouseX, int mouseY);
    
    // Calculation methods
    float calculateFuelConsumption(int fromNodeId, int toNodeId);
    std::vector<MapNode*> getAvailableNodes();
    
    // Rendering methods
    void renderRoads(sf::RenderWindow& window);
    void renderNodes(sf::RenderWindow& window);
    void renderPlayerMarker(sf::RenderWindow& window);
    void renderUI(sf::RenderWindow& window);
    void renderConfirmationDialog(sf::RenderWindow& window);
    
    // Helper methods
    sf::Color getNodeColor(const MapNode& node) const;
    std::string getNodeTypeName(NodeType type) const;
    
    // Font
    sf::Font m_font;
    bool m_fontLoaded;
    
    // Map data
    std::vector<MapNode> m_nodes;
    int m_currentNodeId;        // Player's current location
    int m_selectedNodeId;       // Node selected for travel (-1 if none)
    
    // UI elements
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    sf::Text m_infoText;
    sf::Text m_instructionText;
    
    // Confirmation dialog
    bool m_showConfirmation;
    sf::RectangleShape m_confirmationPanel;
    sf::Text m_confirmationText;
    sf::Text m_confirmYesText;
    sf::Text m_confirmNoText;
    bool m_yesSelected;         // True if "Yes" is selected, false if "No"
    
    // Hovering
    MapNode* m_hoveredNode;     // Node under mouse cursor
    
    // Player state
    PlayerState m_playerState;  // Player resources and state
    
    // Scene state
    bool m_isFinished;
    SceneType m_nextScene;
    
    // Constants for rendering
    static constexpr float NODE_RADIUS = 20.0f;
    static constexpr float HOVER_RADIUS = 25.0f;
    static constexpr float ROAD_THICKNESS = 3.0f;
};

#endif // WORLD_MAP_SCENE_H
