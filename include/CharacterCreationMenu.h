#ifndef CHARACTER_CREATION_MENU_H
#define CHARACTER_CREATION_MENU_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "UILayout.h"

// Character Creation Menu
// ASCII/Notebook style menu for character creation with typewriter effect
// Two-step process: 1) Select background, 2) Select car
class CharacterCreationMenu {
public:
    CharacterCreationMenu();
    
    // Initialize menu with font
    bool initialize(const std::string& fontPath);
    
    // Handle input events
    void handleInput(const sf::Event& event);
    
    // Update menu state (typewriter effect, blinking arrow)
    void update(float deltaTime);
    
    // Render the menu
    void render(sf::RenderWindow& window);
    
    // Check if creation is complete
    bool isComplete() const { return m_isComplete; }
    
    // Check if user wants to go back
    bool wantsToGoBack() const { return m_goBack; }
    
    // Get selected background (0: Nomad, 1: Hitchhiker, 2: Ex-Racer)
    int getSelectedBackground() const { return m_selectedBackground; }
    
    // Get selected car (0: Old Sedan, 1: Offroad Van, 2: Sports Coupe)
    int getSelectedCar() const { return m_selectedCar; }
    
    // Get current step (0: background selection, 1: car selection)
    int getCurrentStep() const { return m_currentStep; }
    
private:
    // Update typewriter effect
    void updateTypewriter(float deltaTime);
    
    // Update blinking arrow
    void updateArrow(float deltaTime);
    
    // Отрисовать ASCII рамку/границу
    void drawFrame(sf::RenderWindow& window);
    
    // Draw menu items with typewriter effect
    void drawMenuItems(sf::RenderWindow& window);
    
    // Отрисовать мигающую стрелку
    void drawArrow(sf::RenderWindow& window);
    
    // Draw header text
    void drawHeader(sf::RenderWindow& window);
    
    sf::Font m_font;
    
    // Menu state
    int m_currentStep;              // 0: background, 1: car
    int m_selectedIndex;            // Currently selected menu item index
    int m_selectedBackground;       // Selected background (stored)
    int m_selectedCar;              // Selected car (stored)
    bool m_isComplete;              // Creation complete flag
    bool m_goBack;                  // User wants to go back
    
    // Typewriter effect
    std::vector<std::string> m_menuItems;  // Current menu items
    std::vector<int> m_displayedChars;     // Characters displayed per item
    float m_typewriterTimer;               // Timer for typewriter effect
    bool m_typewriterComplete;             // All text displayed
    
    // Arrow blinking
    float m_arrowTimer;
    bool m_arrowVisible;
    
    // Варианты фона
    const std::vector<std::string> m_backgrounds = {
        "Nomad",
        "Hitchhiker",
        "Ex-Racer"
    };
    
    // Варианты машины
    const std::vector<std::string> m_cars = {
        "Old Sedan",
        "Offroad Van",
        "Sports Coupe"
    };
    
    // Menu dimensions and positioning
    static constexpr float FRAME_WIDTH = 800.0f;
    static constexpr float FRAME_HEIGHT = 600.0f;
    static constexpr float FRAME_X = 283.0f;  // (1366 - 800) / 2
    static constexpr float FRAME_Y = 84.0f;   // (768 - 600) / 2
    
    static constexpr float HEADER_Y = 120.0f;
    static constexpr float MENU_START_Y = 250.0f;
    static constexpr float MENU_ITEM_SPACING = 80.0f;
    
    static constexpr float TYPEWRITER_SPEED = 0.11f;  // Seconds per character
    static constexpr float ARROW_BLINK_SPEED = 0.5f;  // Arrow blink period
};

#endif // CHARACTER_CREATION_MENU_H
