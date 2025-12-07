#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <optional>

// Main menu scene
// Purpose: Display game title and menu options (New Game, Continue, Exit)
// Features:
//   - Display game title/logo
//   - Show menu options with keyboard navigation
//   - Handle selection and transition to character creation or exit
class MainMenuScene : public Scene {
public:
    MainMenuScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Placeholder rectangles for menu elements
    sf::RectangleShape m_titleBox;      // Game title area
    sf::RectangleShape m_newGameBox;    // "New Game" button
    sf::RectangleShape m_continueBox;   // "Continue" button  
    sf::RectangleShape m_exitBox;       // "Exit" button
    sf::RectangleShape m_selectorBox;   // Selection indicator
    
    // Font and text elements (using optional for SFML 3.x compatibility)
    sf::Font m_font;                              // Font for text rendering
    std::optional<sf::Text> m_titleText;          // Game title text
    std::optional<sf::Text> m_newGameText;        // "New Game" text
    std::optional<sf::Text> m_continueText;       // "Continue" text
    std::optional<sf::Text> m_journalText;        // "Journal (Test)" text
    std::optional<sf::Text> m_exitText;           // "Exit" text
    bool m_fontLoaded;                            // Font loading status
    
    int m_selectedIndex;                // Currently selected menu item (0-3)
    bool m_isFinished;                  // Scene completion flag
    SceneType m_nextScene;              // Next scene to transition to
};

#endif // MAIN_MENU_SCENE_H
