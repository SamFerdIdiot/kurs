#ifndef END_GAME_SCENE_H
#define END_GAME_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <optional>

// Final scene after completing the journey
// Shows congratulations and statistics
class EndGameScene : public Scene {
public:
    EndGameScene(float totalTime, int citiesVisited);

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;

private:
    // Font
    sf::Font m_font;
    bool m_fontLoaded;

    // Background
    sf::RectangleShape m_background;

    // Title
    std::optional<sf::Text> m_titleText;

    // Stats
    std::optional<sf::Text> m_statsText;
    float m_totalTime;
    int m_citiesVisited;

    // Menu button
    sf::RectangleShape m_menuButton;
    std::optional<sf::Text> m_menuButtonText;
    bool m_menuButtonHovered;

    // State
    bool m_isFinished;
    SceneType m_nextScene;

    // Animation
    float m_fadeAlpha;
    float m_animationTime;

    // Helper methods
    void setupUI();
    std::string formatTime(float seconds) const;
};

#endif // END_GAME_SCENE_H
