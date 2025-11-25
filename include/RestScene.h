#ifndef REST_SCENE_H
#define REST_SCENE_H

#include "Scene.h"
#include "PlayerState.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// Scene for resting/sleeping at motels and rest areas
// This scene provides options for the player to rest and recover energy
class RestScene : public Scene {
public:
    RestScene();
    explicit RestScene(PlayerState* playerState);
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
    void setPlayerState(PlayerState* playerState);
    
private:
    PlayerState* m_playerState;
    bool m_isFinished;
    SceneType m_nextScene;
    int m_selectedOption;
    int m_hoveredOption;
    
    // UI elements
    sf::Font m_font;
    bool m_fontLoaded;
    
    sf::RectangleShape m_background;
    sf::RectangleShape m_menuBox;
    sf::Text m_titleText;
    sf::Text m_statusText;
    std::vector<sf::RectangleShape> m_optionBoxes;
    std::vector<sf::Text> m_optionTexts;
    
    struct RestOption {
        std::wstring name;
        std::wstring description;
        int cost;
        float energyRestore;
        float timePass;
    };
    
    std::vector<RestOption> m_restOptions;
    
    // Helper methods
    void setupUI();
    void initializeOptions();
    void updateOptionHover(const sf::Vector2i& mousePos);
    void applyRestOption(const RestOption& option);
};

#endif // REST_SCENE_H
