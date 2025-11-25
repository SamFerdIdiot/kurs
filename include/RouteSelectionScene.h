#ifndef ROUTE_SELECTION_SCENE_H
#define ROUTE_SELECTION_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Route data structure for future expansion
struct Route {
    std::string name;
    std::string description;
    int distance;           // Distance in miles/km
    int difficulty;         // 1-5 scale
    std::string terrain;    // "Highway", "Backroad", "Mountain", etc.
    sf::Color color;        // Visual representation color
    
    Route(const std::string& n, const std::string& desc, int dist, int diff, 
          const std::string& terr, const sf::Color& col)
        : name(n), description(desc), distance(dist), difficulty(diff), 
          terrain(terr), color(col) {}
};

// Route Selection scene
// Purpose: Allow player to choose their journey route before starting gameplay
// Features:
//   - Display map with multiple route options
//   - Visual representation as colored rectangles/lines
//   - Keyboard navigation to select routes
//   - Route information display (name, distance, difficulty, terrain)
//   - Stub route properties for future expansion
class RouteSelectionScene : public Scene {
public:
    RouteSelectionScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    void initializeRoutes();
    void updateSelectionIndicator();
    
    // Font for text display
    sf::Font m_font;
    bool m_fontLoaded;
    
    // Route data
    std::vector<Route> m_routes;
    int m_selectedRouteIndex;
    
    // Visual elements
    sf::RectangleShape m_mapBackground;         // Map background
    std::vector<sf::RectangleShape> m_routeShapes;  // Visual route representations
    sf::RectangleShape m_selectionIndicator;    // Highlight selected route
    
    // Text elements
    sf::Text m_titleText;
    sf::Text m_routeNameText;
    sf::Text m_routeDescText;
    sf::Text m_routeStatsText;
    sf::Text m_instructionText;
    
    // Scene state
    bool m_isFinished;
    SceneType m_nextScene;
    
    // Анимация
    float m_indicatorBlinkTimer;
    bool m_indicatorVisible;
};

#endif // ROUTE_SELECTION_SCENE_H
