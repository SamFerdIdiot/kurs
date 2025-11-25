#ifndef LOCATION_H
#define LOCATION_H

#include <SFML/Graphics.hpp>
#include <string>

// Enum for different types of locations/structures
enum class LocationType {
    HOME,           // Player's starting home
    GAS_STATION,    // Refuel location
    STORE,          // Buy supplies
    GARAGE,         // Repair car
    MOTEL,          // Rest location
    DINER,          // Eat and recover energy
    MECHANIC,       // Advanced repairs
    UNKNOWN         // Default/uninitialized
};

// Structure represents a location that the player can interact with
class Location {
public:
    Location();
    Location(LocationType type, const std::string& name, float x, float y);
    
    // Getters
    LocationType getType() const { return m_type; }
    std::string getName() const { return m_name; }
    sf::Vector2f getPosition() const { return m_position; }
    float getInteractionRadius() const { return m_interactionRadius; }
    bool isActive() const { return m_isActive; }
    
    // Setters
    void setType(LocationType type) { m_type = type; }
    void setName(const std::string& name) { m_name = name; }
    void setPosition(float x, float y) { m_position = sf::Vector2f(x, y); }
    void setInteractionRadius(float radius) { m_interactionRadius = radius; }
    void setActive(bool active) { m_isActive = active; }
    
    // Check if player is within interaction distance
    bool isPlayerNearby(const sf::Vector2f& playerPosition) const;
    
    // Render the location on screen
    void render(sf::RenderWindow& window) const;
    
private:
    LocationType m_type;
    std::string m_name;
    sf::Vector2f m_position;
    float m_interactionRadius;  // Distance within which player can interact
    bool m_isActive;            // Whether this location is currently available
    
    // Visual representation
    mutable sf::RectangleShape m_shape;
    mutable sf::CircleShape m_indicator;
};

// Helper function to get display name for location type
std::string getLocationTypeName(LocationType type);

// Helper function to get color for location type
sf::Color getLocationColor(LocationType type);

#endif // LOCATION_H
