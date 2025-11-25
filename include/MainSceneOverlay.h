#ifndef MAIN_SCENE_OVERLAY_H
#define MAIN_SCENE_OVERLAY_H

#include <SFML/Graphics.hpp>

// MainSceneOverlay class
// Purpose: Manages the overlay that appears during driving scene when Space is pressed
// Responsibilities:
//   - Display semi-transparent overlay over the driving scene
//   - Show placeholder UI for future game events (rest stops, random encounters, dialogue)
//   - Handle input when overlay is active
//   - Provide extensible structure for adding interactive elements
//   - Clean interface for showing/hiding overlay
class MainSceneOverlay {
public:
    // Конструктор
    // Parameters:
    //   - windowWidth: Width of the game window
    //   - windowHeight: Height of the game window
    MainSceneOverlay(float windowWidth, float windowHeight);
    
    // Обновить состояние оверлея
    // Parameters:
    //   - deltaTime: Time elapsed since last frame (in seconds)
    // For future animations, transitions, or timed events
    void update(float deltaTime);
    
    // Process keyboard input events when overlay is active
    // Parameters:
    //   - event: SFML event to process
    // Returns: true if event was handled by overlay, false otherwise
    bool handleInput(const sf::Event& event);
    
    // Render the overlay to the window
    // Parameters:
    //   - window: SFML render window to draw to
    void render(sf::RenderWindow& window);
    
    // Check if overlay is currently visible
    // Returns: true if visible, false otherwise
    bool isVisible() const { return m_visible; }
    
    // Show the overlay
    void show() { m_visible = true; }
    
    // Hide the overlay
    void hide() { m_visible = false; }
    
    // Toggle overlay visibility
    void toggle() { m_visible = !m_visible; }
    
    // TODO: Future extensions
    // - Add event type system (rest stop, random encounter, dialogue, etc.)
    // - Implement menu options for different actions
    // - Add text display for event descriptions
    // - Support for interactive choices that affect game state
    // - Animation for overlay appearance/disappearance
    // - Connect to quest/event system
    // - Add icons/images for different event types
    // - Support for rewards/consequences display
    
private:
    bool m_visible;                      // Is overlay currently shown
    sf::RectangleShape m_background;     // Semi-transparent background
    sf::RectangleShape m_contentPanel;   // Main content area
    sf::RectangleShape m_titleBar;       // Title/header area
    sf::RectangleShape m_option1Box;     // Option 1 placeholder
    sf::RectangleShape m_option2Box;     // Option 2 placeholder
    sf::RectangleShape m_option3Box;     // Option 3 placeholder
    sf::RectangleShape m_closePrompt;    // Close/continue prompt
};

#endif // MAIN_SCENE_OVERLAY_H
