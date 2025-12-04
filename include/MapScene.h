#ifndef MAP_SCENE_H
#define MAP_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <optional>

// MapScene - Static world map viewer
// Purpose: Display a large static world map image that can be panned with mouse
// Features:
//   - Displays static map image (or placeholder for MVP)
//   - Mouse drag to pan the map
//   - ESC to close and return to node
//   - Reference-only view (no gameplay interaction)
class MapScene : public Scene {
public:
    MapScene();
    ~MapScene() override = default;

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    SceneType getNextScene() const override;
    bool isFinished() const override;

private:
    // UI elements
    sf::RectangleShape m_background;        // Dark background
    sf::RectangleShape m_mapPlaceholder;    // [MVP] Placeholder for actual map image
    sf::Font m_font;
    std::optional<sf::Text> m_titleText;
    std::optional<sf::Text> m_instructionText;
    std::optional<sf::Text> m_placeholderText;  // [MVP] Indicates where image goes
    bool m_fontLoaded;

    // Map pan/view state
    sf::Vector2f m_mapOffset;               // Current pan offset
    bool m_isDragging;                      // Is user dragging?
    sf::Vector2i m_lastMousePos;            // Last mouse position for drag

    // Scene state
    bool m_finished;
    SceneType m_nextScene;
    SceneType m_returnToScene;              // Scene to return to when closing
};

#endif // MAP_SCENE_H
