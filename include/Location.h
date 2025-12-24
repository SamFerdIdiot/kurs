#ifndef LOCATION_H
#define LOCATION_H

#include <SFML/Graphics.hpp>
#include <string>


enum class LocationType {
    HOME,
    GAS_STATION,
    STORE,
    GARAGE,
    MOTEL,
    DINER,
    MECHANIC,
    UNKNOWN
};


class Location {
public:
    Location();
    Location(LocationType type, const std::string& name, float x, float y);


    LocationType getType() const { return m_type; }
    std::string getName() const { return m_name; }
    sf::Vector2f getPosition() const { return m_position; }
    float getInteractionRadius() const { return m_interactionRadius; }
    bool isActive() const { return m_isActive; }


    void setType(LocationType type) { m_type = type; }
    void setName(const std::string& name) { m_name = name; }
    void setPosition(float x, float y) { m_position = sf::Vector2f(x, y); }
    void setInteractionRadius(float radius) { m_interactionRadius = radius; }
    void setActive(bool active) { m_isActive = active; }


    bool isPlayerNearby(const sf::Vector2f& playerPosition) const;


    void render(sf::RenderWindow& window) const;

private:
    LocationType m_type;
    std::string m_name;
    sf::Vector2f m_position;
    float m_interactionRadius;
    bool m_isActive;


    mutable sf::RectangleShape m_shape;
    mutable sf::CircleShape m_indicator;
};


std::string getLocationTypeName(LocationType type);


sf::Color getLocationColor(LocationType type);

#endif
