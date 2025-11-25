#ifndef RESOURCEBAR_H
#define RESOURCEBAR_H

#include <SFML/Graphics.hpp>

// ResourceBar - Animated progress bar component for resource display
// 
// This class implements a smooth animated progress bar with:
// - Dynamic color gradient based on percentage (Red → Yellow → Green)
// - Smooth transitions using linear interpolation
// - Customizable position and size
// - Border with white outline
//
// Used by HUDPanel to display fuel, energy, and other resource levels
// in the top-right corner of the screen.

class ResourceBar {
public:
    // Constructor
    // Parameters:
    //   pos - Position of the bar (top-left corner)
    //   size - Size of the bar (width x height)
    ResourceBar(sf::Vector2f pos, sf::Vector2f size);
    
    // Set the current and maximum values for the bar
    // Parameters:
    //   current - Current resource value
    //   max - Maximum resource value
    // The bar will animate smoothly to the new percentage
    void setValue(float current, float max);
    
    // Set the position of the bar
    // Parameters:
    //   pos - New position (top-left corner)
    void setPosition(sf::Vector2f pos);
    
    // Update the bar animation
    // Parameters:
    //   deltaTime - Time since last update in seconds
    // Smoothly transitions the displayed value using lerp
    void update(float deltaTime);
    
    // Draw the bar to the render window
    // Parameters:
    //   window - SFML render window to draw to
    void draw(sf::RenderWindow& window);
    
    // Get the current percentage (0-100)
    // Returns: Current percentage of the bar
    float getPercentage() const;
    
private:
    // Visual components
    sf::RectangleShape m_border;      // Border rectangle (white outline)
    sf::RectangleShape m_background;  // Background rectangle (dark gray)
    sf::RectangleShape m_fill;        // Fill rectangle (colored based on percentage)
    
    // Position and size
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    
    // Resource values
    float m_currentValue;   // Current resource value
    float m_maxValue;       // Maximum resource value
    float m_displayValue;   // Animated display value (for smooth transitions)
    
    // Animation parameters
    static constexpr float TRANSITION_SPEED = 5.0f;  // Lerp speed multiplier
    static constexpr float BORDER_THICKNESS = 2.0f;  // Border thickness in pixels
    
    // Helper functions
    
    // Get the border color (always white)
    sf::Color getBorderColor() const;
    
    // Get the fill color based on percentage
    // Parameters:
    //   percentage - Resource percentage (0-100)
    // Returns: Color gradient (Red < 40%, Yellow 40-70%, Green > 70%)
    sf::Color getFillColor(float percentage) const;
    
    // Update the visual appearance of the bar
    void updateVisuals();
};

#endif // RESOURCEBAR_H
