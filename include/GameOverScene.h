#ifndef GAME_OVER_SCENE_H
#define GAME_OVER_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>

// Game over scene
// Purpose: Display game over screen and final stats
// Features:
//   - Display game over message
//   - Show reason for game over (ran out of fuel, no money, etc.)
//   - Display final statistics (distance traveled, money earned, etc.)
//   - Show achievement unlocks
//   - Display high score
//   - Offer to restart or return to main menu
//   - Allow sharing of results
class GameOverScene : public Scene {
public:
    GameOverScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Placeholder rectangles for game over elements
    sf::RectangleShape m_backgroundBox;     // Game over background
    sf::RectangleShape m_gameOverBox;       // "GAME OVER" title
    sf::RectangleShape m_reasonBox;         // Game over reason text
    sf::RectangleShape m_statsBox;          // Final statistics panel
    sf::RectangleShape m_distanceBox;       // Distance traveled stat
    sf::RectangleShape m_moneyBox;          // Money earned stat
    sf::RectangleShape m_timeBox;           // Time played stat
    sf::RectangleShape m_achievementsBox;   // Achievements display
    sf::RectangleShape m_highScoreBox;      // High score display
    sf::RectangleShape m_restartBox;        // Restart game button
    sf::RectangleShape m_mainMenuBox;       // Return to main menu button
    sf::RectangleShape m_selectorBox;       // Selection indicator
    
    // Font and text elements
    sf::Font m_font;                        // Font for text rendering
    sf::Text m_gameOverText;                // "GAME OVER" title text
    sf::Text m_reasonText;                  // Game over reason text
    sf::Text m_distanceText;                // Distance stat text
    sf::Text m_moneyText;                   // Money stat text
    sf::Text m_timeText;                    // Time stat text
    sf::Text m_achievementsText;            // Achievements text
    sf::Text m_highScoreText;               // High score text
    sf::Text m_restartText;                 // Restart button text
    sf::Text m_mainMenuText;                // Main menu button text
    bool m_fontLoaded;                      // Font loading status
    
    int m_selectedOption;                   // Currently selected option (0: restart, 1: main menu)
    bool m_isFinished;                      // Scene completion flag
    SceneType m_nextScene;                  // Next scene to transition to
};

#endif // GAME_OVER_SCENE_H
