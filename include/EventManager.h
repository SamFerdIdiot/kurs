#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>


enum class EventType {
    ROAD,
    COMPANION,
    RESOURCE,
    SHOP,
    ENCOUNTER,
    CONFLICT
};


struct EventCondition {

    float minFuel = 0.0f;
    float maxFuel = 100.0f;
    float minEnergy = 0.0f;
    float maxEnergy = 100.0f;
    int minMoney = 0;
    int maxMoney = 999999;
    float probability = 1.0f;
    std::string requiredLocation = "";
    std::string requiredRoadType = "";


    std::vector<std::string> requiredNPCsInParty;
    std::map<std::string, int> minRelationships;
    std::vector<std::string> requiredItems;
    std::vector<std::string> blockedIfTriggered;
    int minPartySize = 0;
    int maxPartySize = 999;

    EventCondition() = default;
};


struct RandomOutcome {
    std::string outcomeText;
    float fuelChange = 0.0f;
    float energyChange = 0.0f;
    int moneyChange = 0;
    float probability = 1.0f;

    RandomOutcome(const std::string& text, float fuel, float energy, int money, float prob)
        : outcomeText(text), fuelChange(fuel), energyChange(energy), moneyChange(money), probability(prob) {}
};


struct EventChoice {
    std::string text;
    std::string outcomeText;
    float fuelChange = 0.0f;
    float energyChange = 0.0f;
    int moneyChange = 0;
    float vehicleConditionChange = 0.0f;
    bool endsEvent = true;
    std::string perkId = "";
    bool isPerkChoice = false;


    std::map<std::string, int> relationshipChanges;
    std::vector<std::string> addItems;
    std::vector<std::string> removeItems;
    std::string recruitNPC = "";
    std::string removeNPC = "";
    std::string triggerEvent = "";


    std::vector<RandomOutcome> randomOutcomes;

    EventChoice(const std::string& text = "", const std::string& outcomeText = "")
        : text(text), outcomeText(outcomeText) {}
};


struct GameEvent {
    std::string id;
    std::string title;
    std::string description;
    EventCondition condition;
    std::vector<EventChoice> choices;
    bool triggered = false;


    EventType type = EventType::ROAD;
    float weight = 1.0f;
    bool oneTimeOnly = false;
    std::vector<std::string> blocksEvents;
    std::string associatedNPC = "";

    GameEvent(const std::string& id = "", const std::string& title = "",
              const std::string& description = "")
        : id(id), title(title), description(description) {}
};


using EventCallback = std::function<void(const GameEvent&)>;


class EventManager {
public:
    EventManager();


    void addEvent(const GameEvent& event);


    void removeEvent(const std::string& id);


    GameEvent* getEvent(const std::string& id);


    const std::vector<GameEvent>& getAllEvents() const;


    bool checkCondition(const EventCondition& condition,
                       float fuel, float energy, int money,
                       const std::string& location = "",
                       const std::string& roadType = "") const;


    GameEvent* getRandomEvent(float fuel, float energy, int money,
                            const std::string& location = "",
                            const std::string& roadType = "");


    void triggerEvent(const std::string& id);


    void resetTriggeredEvents();


    void applyChoice(const EventChoice& choice,
                    float& fuel, float& energy, int& money);


    void setEventCallback(EventCallback callback);


    void clear();


    void initializeDefaultEvents();



    void addPerkChoicesToEvent(GameEvent* event, float playerFuel, float playerEnergy, int playerMoney);

private:
    std::vector<GameEvent> m_events;
    EventCallback m_eventCallback;


    float randomFloat(float min, float max) const;
    int randomInt(int min, int max) const;
};

#endif
