#ifndef ROAD_SCENE_H
#define ROAD_SCENE_H

#include "Scene.h"
#include "PlayerState.h"
#include "UI/HUDPanel.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// Forward declarations
class StructurePlacement;
class EventManager;

// RoadScene - Cinematic travel mode with automatic scrolling
// Purpose: Non-interactive travel scene between locations
// Features:
//   - Cinematic side-view with automatic scrolling
//   - Multi-layer parallax background for depth
//   - Animated car movement (auto-drive)
//   - Progress bar showing journey completion
//   - HUD integration for resources
//   - Random event system integration
//   - NO player control (A/D disabled) - pure cinema mode
//   - Cannot skip - player must watch/experience events
class RoadScene : public Scene {
public:
    RoadScene();
    ~RoadScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
    // Set journey parameters
    void setJourneyDistance(float distance);
    void setDestination(SceneType destination);
    
private:
    // Scene state
    bool m_isFinished;
    SceneType m_nextScene;
    
    // Journey progress
    float m_journeyDistance;      // Total distance to travel
    float m_currentDistance;      // Distance traveled so far
    float m_travelSpeed;          // Auto-scroll speed
    
    // Camera position (auto-scrolling)
    float m_cameraX;
    
    // Player state reference
    PlayerState* m_playerState;
    
    // Player car properties (auto-moving)
    float m_playerCarX;           // Car X position in world space
    float m_playerCarY;           // Car Y position (screen space, fixed)
    
    // === Layer 1: Background (furthest, slowest parallax) ===
    sf::RectangleShape m_backgroundSky;
    sf::RectangleShape m_backgroundMountains;
    float m_backgroundParallaxSpeed;
    
    // === Layer 2: Second Background (medium parallax) ===
    sf::RectangleShape m_secondBackgroundHills;
    sf::RectangleShape m_secondBackgroundTrees;
    float m_secondBackgroundParallaxSpeed;
    
    // === Layer 3: Structures (generated via StructurePlacement) ===
    std::unique_ptr<StructurePlacement> m_structurePlacement;
    
    // === Layer 4: Traffic (oncoming and passing vehicles) ===
    struct Vehicle {
        float x;                  // X position in world space
        float y;                  // Y position in screen space
        float speed;              // Relative speed
        bool isOncoming;          // Direction
        sf::RectangleShape shape;
    };
    std::vector<Vehicle> m_vehicles;
    float m_vehicleSpawnTimer;
    
    // === Layer 5: Road and Player Car ===
    sf::RectangleShape m_road;
    sf::RectangleShape m_roadLine1;
    sf::RectangleShape m_roadLine2;
    float m_roadLineOffset;       // For animated road lines
    sf::RectangleShape m_playerCar;
    
    // === Layer 6: Progress Bar ===
    sf::RectangleShape m_progressBarBackground;
    sf::RectangleShape m_progressBarFill;
    sf::Text m_progressText;
    
    // === Layer 7: HUD ===
    std::unique_ptr<HUDPanel> m_hudPanel;
    
    // Font
    sf::Font m_font;
    bool m_fontLoaded;
    
    // Event system
    float m_eventTimer;
    float m_eventCheckInterval;
    
    // Helper methods
    void initializeLayers();
    void initializePlayerCar();
    void initializeVehicles();
    void initializeProgressBar();
    
    void updateCamera(float deltaTime);
    void updateAutoScroll(float deltaTime);
    void updateVehicles(float deltaTime);
    void updateRoadAnimation(float deltaTime);
    void updateProgress();
    void updateHUD();
    void updateEventSystem(float deltaTime);
    
    void renderBackground(sf::RenderWindow& window);
    void renderSecondBackground(sf::RenderWindow& window);
    void renderStructures(sf::RenderWindow& window);
    void renderVehicles(sf::RenderWindow& window);
    void renderRoadAndPlayer(sf::RenderWindow& window);
    void renderProgressBar(sf::RenderWindow& window);
    void renderHUD(sf::RenderWindow& window);
    
    // Calculate parallax offset for a layer
    float calculateParallaxOffset(float parallaxSpeed) const;
    
    // Spawn traffic vehicle
    void spawnVehicle();
    
    // Check if journey is complete
    bool isJourneyComplete() const;
    
    // Consume resources during travel
    void consumeResources(float deltaTime);
};

#endif // ROAD_SCENE_H
