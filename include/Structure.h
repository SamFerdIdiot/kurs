#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <optional>
#include "Location.h"

// Structure - Represents a building/location in the city that player can interact with
// Examples: Gas Station, Store, Motel, Garage
class Structure {
public:
    Structure(LocationType type, const std::string& name, const sf::Vector2f& position);
    
    // Getters
    LocationType getType() const { return m_type; }
    std::string getName() const { return m_name; }
    sf::Vector2f getPosition() const { return m_position; }
    float getInteractionDistance() const { return m_interactionDistance; }
    
    // Check if player is near enough to interact
    bool isPlayerNear(const sf::Vector2f& playerPos) const;
    
    // Render the structure icon/sprite
    void render(sf::RenderWindow& window);
    
    // Render interaction prompt when player is nearby
    void renderPrompt(sf::RenderWindow& window, const sf::Font& font);
    
private:
    LocationType m_type;              // Type of structure (gas station, store, etc.)
    std::string m_name;               // Display name (e.g., "Joe's Gas Station")
    sf::Vector2f m_position;          // Position in the city
    float m_interactionDistance;      // How close player needs to be
    
    // Visual representation
    sf::RectangleShape m_shape;       // Temporary rectangle representation
    std::optional<sf::Text> m_label;  // SFML 3.x: Text requires font in constructor
    sf::Color m_color;                // Color based on type
    std::string m_icon;               // Unicode icon (⛽, 🏪, 🏨)
    
    // Initialize visual elements based on type
    void setupVisuals();
    sf::Color getColorForType() const;
    std::string getIconForType() const;
};

#endif // STRUCTURE_H
