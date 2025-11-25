#ifndef CINEMATIC_EVENT_SCENE_H
#define CINEMATIC_EVENT_SCENE_H

#include "Scene.h"
#include "EventManager.h"
#include "UI/HUDPanel.hpp"
#include "UILayout.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// Forward declaration
class PlayerState;

// Scene for displaying cinematic events and dialogues
// This is a proof-of-principle implementation with simple UI
class CinematicEventScene : public Scene {
public:
    CinematicEventScene();
    explicit CinematicEventScene(GameEvent* event, PlayerState* playerState = nullptr);
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
    // Set the event to display
    void setEvent(GameEvent* event, PlayerState* playerState = nullptr);
    
    // Check if an outcome was selected
    bool hasOutcome() const;
    
    // Get the selected choice
    const EventChoice* getSelectedChoice() const;
    
private:
    GameEvent* m_event;
    PlayerState* m_playerState;
    bool m_isFinished;
    SceneType m_nextScene;
    int m_selectedChoice;
    int m_hoveredChoice;
    bool m_outcomeShown;
    
    // UI elements
    sf::Font m_font;
    bool m_fontLoaded;
    
    sf::RectangleShape m_overlay;
    sf::RectangleShape m_dialogBox;
    sf::Text m_titleText;
    sf::Text m_descriptionText;
    std::vector<sf::RectangleShape> m_choiceBoxes;
    std::vector<sf::Text> m_choiceTexts;
    sf::Text m_outcomeText;
    sf::Text m_continuePrompt;
    
    // HUD Panel for displaying resources
    HUDPanel m_hudPanel;
    
    // Helper methods
    void setupUI();
    void updateChoiceHover(const sf::Vector2i& mousePos);
    std::string wrapText(const std::string& text, int maxWidth, const sf::Font& font, unsigned int charSize);
};

#endif // CINEMATIC_EVENT_SCENE_H
