#ifndef TRAVEL_CINEMATIC_SCENE_H
#define TRAVEL_CINEMATIC_SCENE_H

#include "Scene.h"
#include "EventManager.h"
#include "PlayerState.h"
#include "CinematicEventScene.h"
#include "UI/HUDPanel.hpp"
#include "UILayout.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

// Road data structure
struct RoadData {
    std::string fromNodeId;
    std::string toNodeId;
    std::string roadType;       // "highway", "road", "path"
    float distance;             // Distance in km
    float baseEventChance;      // Base probability of event (0.0-1.0)
    
    RoadData()
        : distance(100.0f), baseEventChance(0.3f) {}
    
    RoadData(const std::string& from, const std::string& to, 
             const std::string& type, float dist, float eventChance = 0.3f)
        : fromNodeId(from), toNodeId(to), roadType(type),
          distance(dist), baseEventChance(eventChance) {}
};

// TravelCinematicScene - Scene for traveling between nodes
// Shows side-view of the car driving with parallax background
// Triggers random events during travel
// Consumes fuel and time
class TravelCinematicScene : public Scene {
public:
    TravelCinematicScene(const RoadData& roadData, PlayerState* playerState);
    ~TravelCinematicScene() override = default;
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    void updateTravel(float deltaTime);
    void tryTriggerEvent();
    void consumeResources(float deltaTime);
    void checkArrival();
    void setupUI();
    float calculateFuelConsumption() const;
    
    // Road and travel data
    RoadData m_roadData;
    PlayerState* m_playerState;
    
    // Travel state
    float m_progress;               // 0.0 to 1.0
    float m_travelSpeed;            // km per second
    float m_distanceTraveled;       // Current distance in km
    bool m_isFinished;
    bool m_arrived;
    
    // Event system
    EventManager m_eventManager;
    std::unique_ptr<CinematicEventScene> m_activeEvent;
    float m_timeSinceLastEventCheck;
    float m_eventCheckInterval;     // Check for events every N seconds
    
    // UI elements
    sf::Font m_font;
    bool m_fontLoaded;
    
    // Background layers for parallax
    sf::RectangleShape m_sky;
    sf::RectangleShape m_farBackground;
    sf::RectangleShape m_midBackground;
    sf::RectangleShape m_road;
    
    // Car sprite
    sf::RectangleShape m_car;
    
    // Progress bar
    sf::RectangleShape m_progressBarBg;
    sf::RectangleShape m_progressBar;
    sf::Text m_progressText;
    
    // Resource display
    sf::Text m_fuelText;
    sf::Text m_distanceText;
    
    // Scene transition
    SceneType m_nextScene;
    
    // Parallax offsets
    float m_farOffset;
    float m_midOffset;
    float m_roadOffset;
    
    // HUD Panel for displaying resources
    HUDPanel m_hudPanel;
};

#endif // TRAVEL_CINEMATIC_SCENE_H
