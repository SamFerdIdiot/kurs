#ifndef PAUSE_MENU_SCENE_H
#define PAUSE_MENU_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>

// Pause menu scene
// Purpose: Pause the game and access various options
// Features:
//   - Pause current gameplay
//   - Display pause menu overlay
//   - Resume game
//   - Access settings/options
//   - Save game
//   - Load game
//   - View controls
//   - Return to main menu
//   - Quit game
class PauseMenuScene : public Scene {
public:
    PauseMenuScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Placeholder rectangles for pause menu elements
    sf::RectangleShape m_overlayBox;        // Semi-transparent overlay
    sf::RectangleShape m_menuBox;           // Pause menu container
    sf::RectangleShape m_titleBox;          // "PAUSED" title
    sf::RectangleShape m_resumeBox;         // Resume game option
    sf::RectangleShape m_settingsBox;       // Settings option
    sf::RectangleShape m_saveBox;           // Save game option
    sf::RectangleShape m_loadBox;           // Load game option
    sf::RectangleShape m_controlsBox;       // Controls/help option
    sf::RectangleShape m_mainMenuBox;       // Return to main menu option
    sf::RectangleShape m_quitBox;           // Quit game option
    sf::RectangleShape m_selectorBox;       // Selection indicator
    
    // Font and text elements
    sf::Font m_font;                        // Font for text rendering
    sf::Text m_titleText;                   // "PAUSED" title text
    sf::Text m_resumeText;                  // Resume option text
    sf::Text m_settingsText;                // Settings option text
    sf::Text m_saveText;                    // Save option text
    sf::Text m_loadText;                    // Load option text
    sf::Text m_controlsText;                // Controls option text
    sf::Text m_mainMenuText;                // Main menu option text
    sf::Text m_quitText;                    // Quit option text
    bool m_fontLoaded;                      // Font loading status
    
    int m_selectedOption;                   // Currently selected menu option
    bool m_isFinished;                      // Scene completion flag
    SceneType m_nextScene;                  // Next scene to transition to
};

#endif // PAUSE_MENU_SCENE_H
