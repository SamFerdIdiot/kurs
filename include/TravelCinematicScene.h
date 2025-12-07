#ifndef TRAVEL_CINEMATIC_SCENE_H
#define TRAVEL_CINEMATIC_SCENE_H

#include "Scene.h"
#include "EventManager.h"
#include "PlayerState.h"
#include "CinematicEventScene.h"
#include "FuelSystem.h"
#include "UI/HUDPanel.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <optional>
#include <vector>

// Simple travel dialogue structure (for roadmovie)
struct TravelDialogue {
    std::string npcName;           // Name of speaking NPC
    std::string text;              // Dialogue text
    float displayDuration;         // How long to show (seconds)

    TravelDialogue(const std::string& name, const std::string& txt, float duration = 7.0f)
        : npcName(name), text(txt), displayDuration(duration) {}
};

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
    void tryTriggerDialogue();
    void consumeResources(float deltaTime);
    void checkArrival();
    void setupUI();
    float calculateFuelConsumption() const;
    void initializeTravelDialogues();
    
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

    // Dialogue system (simple roadmovie dialogues)
    std::vector<TravelDialogue> m_availableDialogues;
    std::optional<TravelDialogue> m_currentDialogue;
    float m_dialogueTimer;
    float m_timeSinceLastDialogue;
    float m_dialogueCheckInterval;  // Check for dialogues every N seconds
    
    // UI elements
    sf::Font m_font;
    bool m_fontLoaded;
    
    // Background layers for parallax (7 layers like Keep Driving)
    sf::RectangleShape m_sky;              // Layer 0: Static sky
    sf::RectangleShape m_layer1_clouds;    // Layer 1: Very slow clouds
    sf::RectangleShape m_layer2_mountains; // Layer 2: Slow mountains
    sf::RectangleShape m_layer3_hills;     // Layer 3: Medium hills
    sf::RectangleShape m_layer4_trees;     // Layer 4: Medium-fast trees
    sf::RectangleShape m_layer5_fence;     // Layer 5: Fast fence/poles
    sf::RectangleShape m_road;             // Layer 6: Fastest road
    std::vector<sf::RectangleShape> m_roadLines;  // Road lane markings
    std::vector<sf::RectangleShape> m_roadPosts;  // White posts along road edges

    // Objects/squares for each parallax layer
    std::vector<sf::RectangleShape> m_layer1Objects; // Clouds layer objects
    std::vector<sf::RectangleShape> m_layer2Objects; // Mountains layer objects
    std::vector<sf::RectangleShape> m_layer3Objects; // Hills layer objects
    std::vector<sf::RectangleShape> m_layer4Objects; // Trees layer objects
    std::vector<sf::RectangleShape> m_layer5Objects; // Fence layer objects
    std::vector<sf::RectangleShape> m_speedSquares;  // Road layer speed indicators

    // Car sprite (player's car)
    sf::RectangleShape m_car;

    // Oncoming traffic (встречные машины)
    std::vector<sf::RectangleShape> m_oncomingCars;
    std::vector<float> m_oncomingCarOffsets;  // X offsets for each oncoming car
    
    // Progress bar
    sf::RectangleShape m_progressBarBg;
    sf::RectangleShape m_progressBar;
    std::optional<sf::Text> m_progressText;  // SFML 3.x: Text requires font in constructor

    // Resource display
    std::optional<sf::Text> m_fuelText;  // SFML 3.x: Text requires font in constructor
    std::optional<sf::Text> m_distanceText;  // SFML 3.x: Text requires font in constructor

    // Dialogue UI
    sf::RectangleShape m_dialogueBox;
    sf::RectangleShape m_npcPortrait;
    std::optional<sf::Text> m_dialogueNameText;
    std::optional<sf::Text> m_dialogueText;
    
    // Scene transition
    SceneType m_nextScene;
    
    // Parallax offsets for 7 layers
    float m_layer1Offset;     // Clouds
    float m_layer2Offset;     // Mountains
    float m_layer3Offset;     // Hills
    float m_layer4Offset;     // Trees
    float m_layer5Offset;     // Fence
    float m_roadOffset;       // Road

    // Animation state
    float m_carBobTime;          // Time for car bobbing animation
    float m_carBobAmount;        // Vertical offset for car animation

    // HUD Panel for displaying resources
    HUDPanel m_hudPanel;

    // UI Improvements: Warning indicators
    sf::CircleShape m_fuelWarningIcon;
    sf::CircleShape m_energyWarningIcon;
    std::optional<sf::Text> m_fuelWarningText;
    std::optional<sf::Text> m_energyWarningText;
    float m_warningFlashTimer;
    bool m_warningFlashVisible;

    // UI Improvements: Speedometer
    sf::RectangleShape m_speedometerBg;
    std::optional<sf::Text> m_speedText;
    sf::RectangleShape m_speedNeedle;

    // UI Improvements: Arrival notification
    sf::RectangleShape m_arrivalNotificationBg;
    std::optional<sf::Text> m_arrivalNotificationText;
    float m_arrivalNotificationTimer;
    bool m_showArrivalNotification;

    // [Decorative] Keep Driving style HUD panel (bottom panel)
    sf::RectangleShape m_hudBackground;           // Main HUD background panel
    sf::RectangleShape m_hudLeftPanel;            // Left section (ENERGY/STATUS)
    sf::RectangleShape m_hudCenterPanel;          // Center section (date/time/weather)
    sf::RectangleShape m_hudRightPanel;           // Right section (CAR/GAS)
    std::optional<sf::Text> m_hudEnergyLabel;     // "ENERGY" text
    std::optional<sf::Text> m_hudStatusLabel;     // "STATUS" text
    std::optional<sf::Text> m_hudCarLabel;        // "CAR" text
    std::optional<sf::Text> m_hudGasLabel;        // "GAS" text
    std::optional<sf::Text> m_hudDateTimeText;    // Center date/time display
    std::vector<sf::RectangleShape> m_hudEnergyBars;  // Energy indicator bars
    std::vector<sf::RectangleShape> m_hudCarBars;     // Car fuel bars
    sf::RectangleShape m_hudCharacterSprite;      // Character placeholder
    sf::RectangleShape m_hudCarTopView;           // Car top-view placeholder
};

#endif // TRAVEL_CINEMATIC_SCENE_H
