#ifndef ROAD_EVENT_MANAGER_H
#define ROAD_EVENT_MANAGER_H

#include "EventManager.h"
#include "PlayerState.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>

// RoadEventManager class
// Purpose: Specialized manager for road-specific events during travel
// Responsibilities:
//   - Generate context-aware road events based on player state
//   - Handle UI wireframe for event presentation
//   - Process event triggers and outcomes
//   - Manage event frequency and cooldowns
//   - Coordinate with EventManager for event data
class RoadEventManager {
public:
    // Constructor
    // Parameters:
    //   - eventManager: Reference to the global event manager
    RoadEventManager(EventManager& eventManager);
    
    // Update the road event system
    // Parameters:
    //   - deltaTime: Time elapsed since last frame
    //   - travelDistance: Total distance traveled
    //   - playerState: Current player state (fuel, energy, money)
    void update(float deltaTime, float travelDistance, const PlayerState& playerState);
    
    // Check if an event should be triggered
    // Parameters:
    //   - distanceTraveled: Distance traveled since last check
    //   - playerState: Current player state
    // Returns: Pointer to triggered event or nullptr
    GameEvent* checkForEvent(float distanceTraveled, const PlayerState& playerState);
    
    // Trigger a specific road event
    // Parameters:
    //   - event: Event to trigger
    void triggerEvent(GameEvent* event);
    
    // Handle event choice selection
    // Parameters:
    //   - event: Current event
    //   - choiceIndex: Index of selected choice
    //   - playerState: Player state to modify based on choice
    void handleChoice(const GameEvent& event, int choiceIndex, PlayerState& playerState);
    
    // Draw event UI wireframe
    // Parameters:
    //   - window: Render window
    //   - event: Event to display
    void renderEventUI(sf::RenderWindow& window, const GameEvent& event);
    
    // Check if an event is currently active
    bool hasActiveEvent() const { return m_activeEvent != nullptr; }
    
    // Get the current active event
    GameEvent* getActiveEvent() const { return m_activeEvent; }
    
    // Clear the active event
    void clearActiveEvent();
    
    // Set minimum distance between events
    void setMinEventDistance(float distance) { m_minEventDistance = distance; }
    
    // Get distance since last event
    float getDistanceSinceLastEvent() const { return m_distanceSinceLastEvent; }
    
    // Reset event cooldowns and state
    void reset();
    
    // Set callback for when event is triggered
    using EventTriggerCallback = std::function<void(GameEvent*)>;
    void setEventTriggerCallback(EventTriggerCallback callback) { m_eventTriggerCallback = callback; }

private:
    EventManager& m_eventManager;           // Reference to global event manager
    GameEvent* m_activeEvent;               // Currently active event
    float m_distanceSinceLastEvent;         // Distance traveled since last event
    float m_minEventDistance;               // Minimum distance between events
    float m_eventCheckInterval;             // Time interval for event checks
    float m_timeSinceLastCheck;             // Time since last event check
    EventTriggerCallback m_eventTriggerCallback; // Callback for event triggers
    
    // UI elements for event wireframe
    sf::Font m_font;                        // Font for text
    sf::RectangleShape m_eventPanel;        // Background panel for event
    sf::Text m_titleText;                   // Event title
    sf::Text m_descriptionText;             // Event description
    std::vector<sf::RectangleShape> m_choiceBoxes;  // Choice option boxes
    std::vector<sf::Text> m_choiceTexts;    // Choice text labels
    bool m_fontLoaded;                      // Font loading status
    
    // Helper methods
    void initializeUI();
    void updateUILayout(const GameEvent& event);
    bool shouldCheckForEvent() const;
    float calculateEventProbability(const PlayerState& playerState) const;
};

#endif // ROAD_EVENT_MANAGER_H
