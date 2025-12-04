#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <string>
#include <vector>
#include <functional>
#include <memory>

// Event condition structure
struct EventCondition {
    float minFuel = 0.0f;
    float maxFuel = 100.0f;
    float minEnergy = 0.0f;
    float maxEnergy = 100.0f;
    int minMoney = 0;
    int maxMoney = 999999;
    float probability = 1.0f;
    std::string requiredLocation = "";
    std::string requiredRoadType = "";  // "highway", "road", "path", or "" for any

    EventCondition() = default;
};

// Event choice/outcome
struct EventChoice {
    std::string text;
    std::string outcomeText;
    float fuelChange = 0.0f;
    float energyChange = 0.0f;
    int moneyChange = 0;
    float vehicleConditionChange = 0.0f;  // Change in vehicle condition
    bool endsEvent = true;
    std::string perkId = "";  // ID of ability perk if this is a perk choice
    bool isPerkChoice = false;  // True if this choice uses an ability perk

    EventChoice(const std::string& text = "", const std::string& outcomeText = "")
        : text(text), outcomeText(outcomeText) {}
};

// Game event structure
struct GameEvent {
    std::string id;
    std::string title;
    std::string description;
    EventCondition condition;
    std::vector<EventChoice> choices;
    bool triggered = false;
    
    GameEvent(const std::string& id = "", const std::string& title = "", 
              const std::string& description = "")
        : id(id), title(title), description(description) {}
};

// Callback for when an event is triggered
using EventCallback = std::function<void(const GameEvent&)>;

// Manages game events
class EventManager {
public:
    EventManager();
    
    // Add an event to the pool
    void addEvent(const GameEvent& event);
    
    // Remove an event by id
    void removeEvent(const std::string& id);
    
    // Get event by id
    GameEvent* getEvent(const std::string& id);
    
    // Get all events
    const std::vector<GameEvent>& getAllEvents() const;
    
    // Check if condition is met for an event
    bool checkCondition(const EventCondition& condition,
                       float fuel, float energy, int money,
                       const std::string& location = "",
                       const std::string& roadType = "") const;

    // Get random event that meets conditions
    GameEvent* getRandomEvent(float fuel, float energy, int money,
                            const std::string& location = "",
                            const std::string& roadType = "");
    
    // Mark event as triggered
    void triggerEvent(const std::string& id);
    
    // Reset all triggered flags
    void resetTriggeredEvents();
    
    // Apply event choice outcome
    void applyChoice(const EventChoice& choice, 
                    float& fuel, float& energy, int& money);
    
    // Set callback for when event triggers
    void setEventCallback(EventCallback callback);
    
    // Clear all events
    void clear();
    
    // Initialize default events
    void initializeDefaultEvents();
    
    // Add ability perk choices to event
    // Injects perk-based choices into event based on available perks
    void addPerkChoicesToEvent(GameEvent* event, float playerFuel, float playerEnergy, int playerMoney);
    
private:
    std::vector<GameEvent> m_events;
    EventCallback m_eventCallback;
    
    // Random number generation helper
    float randomFloat(float min, float max) const;
    int randomInt(int min, int max) const;
};

#endif // EVENT_MANAGER_H
