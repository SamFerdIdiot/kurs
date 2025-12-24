#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <optional>







class MainMenuScene : public Scene {
public:
    MainMenuScene();

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;

private:

    sf::RectangleShape m_titleBox;
    sf::RectangleShape m_newGameBox;
    sf::RectangleShape m_continueBox;
    sf::RectangleShape m_exitBox;
    sf::RectangleShape m_selectorBox;


    sf::Font m_font;
    std::optional<sf::Text> m_titleText;
    std::optional<sf::Text> m_newGameText;
    std::optional<sf::Text> m_continueText;
    std::optional<sf::Text> m_journalText;
    std::optional<sf::Text> m_exitText;
    bool m_fontLoaded;

    int m_selectedIndex;
    bool m_isFinished;
    SceneType m_nextScene;
};

#endif
