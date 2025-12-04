#include "Structure.h"
#include <cmath>

Structure::Structure(LocationType type, const std::string& name, const sf::Vector2f& position)
    : m_type(type),
      m_name(name),
      m_position(position),
      m_interactionDistance(60.0f) {
    
    setupVisuals();
}

void Structure::setupVisuals() {
    // Set size and position
    m_shape.setSize(sf::Vector2f(80.0f, 80.0f));
    m_shape.setPosition(m_position);
    m_shape.setOrigin(sf::Vector2f(40.0f, 40.0f));  // Fixed: Vector2f wrapper

    // Set color based on type
    m_color = getColorForType();
    m_shape.setFillColor(m_color);
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(2.0f);

    // Get icon for this type
    m_icon = getIconForType();
}

sf::Color Structure::getColorForType() const {
    switch (m_type) {
        case LocationType::GAS_STATION:
            return sf::Color(255, 165, 0);  // Orange for gas station
        case LocationType::STORE:
            return sf::Color(100, 200, 100); // Green for store
        case LocationType::MOTEL:
            return sf::Color(150, 150, 255); // Blue for motel
        case LocationType::GARAGE:
            return sf::Color(200, 100, 100); // Red for garage
        case LocationType::DINER:
            return sf::Color(200, 200, 100); // Yellow for diner
        default:
            return sf::Color(150, 150, 150); // Gray for unknown
    }
}

std::string Structure::getIconForType() const {
    switch (m_type) {
        case LocationType::GAS_STATION:
            return "⛽";  // Gas pump
        case LocationType::STORE:
            return "🏪";  // Store
        case LocationType::MOTEL:
            return "🏨";  // Hotel/Motel
        case LocationType::GARAGE:
            return "🔧";  // Wrench for garage
        case LocationType::DINER:
            return "🍔";  // Burger for diner
        default:
            return "🏢";  // Generic building
    }
}

bool Structure::isPlayerNear(const sf::Vector2f& playerPos) const {
    float dx = playerPos.x - m_position.x;
    float dy = playerPos.y - m_position.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= m_interactionDistance;
}

void Structure::render(sf::RenderWindow& window) {
    // Draw the structure shape
    window.draw(m_shape);

    // Note: Sprite/icon system not yet implemented
    // Currently using colored rectangle to represent the structure
}

void Structure::renderPrompt(sf::RenderWindow& window, const sf::Font& font) {
    // SFML 3.x: Text requires font in constructor
    sf::Text promptText(font, "[E] " + m_name, 14);
    promptText.setFillColor(sf::Color::White);
    promptText.setOutlineColor(sf::Color::Black);
    promptText.setOutlineThickness(1.0f);

    // Position above the structure (Fixed: Vector2f wrapper)
    promptText.setPosition(sf::Vector2f(m_position.x - 40.0f, m_position.y - 60.0f));

    window.draw(promptText);
}
