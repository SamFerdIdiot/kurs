#ifndef POI_MANAGER_H
#define POI_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

// Point of Interest types
enum class POIType {
    GAS_STATION,
    MOTEL,
    DINER,
    REST_AREA,
    JUNCTION,
    TOWN,
    MECHANIC,
    STORE
};

// Point of Interest structure
struct POI {
    std::string id;
    std::string name;
    POIType type;
    sf::Vector2f position;
    bool canStop;
    bool discovered;
    
    POI(const std::string& id, const std::string& name, POIType type, 
        const sf::Vector2f& position, bool canStop = true)
        : id(id), name(name), type(type), position(position), 
          canStop(canStop), discovered(false) {}
};

// Manages all points of interest in the game
class POIManager {
public:
    POIManager();
    
    // Add a POI
    void addPOI(const POI& poi);
    
    // Remove a POI by id
    void removePOI(const std::string& id);
    
    // Get POI by id
    POI* getPOI(const std::string& id);
    
    // Get all POIs
    const std::vector<POI>& getAllPOIs() const;
    
    // Check if position is near a POI that allows stopping
    bool canStopAtPosition(const sf::Vector2f& position, float radius = 50.0f) const;
    
    // Get nearest POI to position
    POI* getNearestPOI(const sf::Vector2f& position);
    
    // Get nearest POI within radius
    POI* getNearestPOIInRadius(const sf::Vector2f& position, float radius);
    
    // Mark POI as discovered
    void discoverPOI(const std::string& id);
    
    // Get all discovered POIs
    std::vector<POI*> getDiscoveredPOIs();
    
    // Clear all POIs
    void clear();
    
private:
    std::vector<POI> m_pois;
    
    // Helper to calculate distance
    float distance(const sf::Vector2f& a, const sf::Vector2f& b) const;
};

#endif // POI_MANAGER_H
