#ifndef CINEMATIC_EVENT_SCENE_H
#define CINEMATIC_EVENT_SCENE_H

#include "Scene.h"
#include "EventManager.h"
#include "UI/HUDPanel.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <optional>

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
    std::optional<sf::Text> m_titleText;  // SFML 3.x: Text requires font in constructor
    std::optional<sf::Text> m_descriptionText;  // SFML 3.x: Text requires font in constructor
    std::vector<sf::RectangleShape> m_choiceBoxes;
    std::vector<sf::Text> m_choiceTexts;
    std::optional<sf::Text> m_outcomeText;  // SFML 3.x: Text requires font in constructor
    std::optional<sf::Text> m_continuePrompt;  // SFML 3.x: Text requires font in constructor
    
    // HUD Panel for displaying resources
    HUDPanel m_hudPanel;

    // Visual effects
    float m_fadeAlpha;
    float m_animationTime;
    sf::RectangleShape m_eventIcon;  // Icon indicator for event type
    std::optional<sf::Text> m_eventTypeText;  // Event type label

    // Helper methods
    void setupUI();
    void updateChoiceHover(const sf::Vector2i& mousePos);
    std::string wrapText(const std::string& text, int maxWidth, const sf::Font& font, unsigned int charSize);
    sf::Color getEventTypeColor() const;
    std::string getEventTypeLabel() const;
};

#endif // CINEMATIC_EVENT_SCENE_H
