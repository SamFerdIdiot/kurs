#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

// Event types for categorization
enum class EventType {
    ROAD,           // Дорожное событие
    COMPANION,      // Событие с компаньоном
    RESOURCE,       // Ресурсное событие
    SHOP,           // Магазин
    ENCOUNTER,      // Встреча с NPC
    CONFLICT        // Конфликт в команде
};

// Event condition structure
struct EventCondition {
    // Базовые условия ресурсов
    float minFuel = 0.0f;
    float maxFuel = 100.0f;
    float minEnergy = 0.0f;
    float maxEnergy = 100.0f;
    int minMoney = 0;
    int maxMoney = 999999;
    float probability = 1.0f;
    std::string requiredLocation = "";
    std::string requiredRoadType = "";  // "highway", "road", "path", or "" for any

    // Новые условия для компаньонов
    std::vector<std::string> requiredNPCsInParty;    // NPC должны быть в команде
    std::map<std::string, int> minRelationships;      // Минимальные отношения с NPC (npcId -> минимум)
    std::vector<std::string> requiredItems;           // Требуемые предметы в инвентаре
    std::vector<std::string> blockedIfTriggered;      // Блокируется если эти события уже были
    int minPartySize = 0;                             // Минимальный размер команды
    int maxPartySize = 999;                           // Максимальный размер команды

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

    // Новые поля для системы событий с компаньонами
    std::map<std::string, int> relationshipChanges;  // Изменения отношений с NPC (npcId -> дельта)
    std::vector<std::string> addItems;                // Добавить предметы в инвентарь
    std::vector<std::string> removeItems;             // Убрать предметы из инвентаря
    std::string recruitNPC = "";                      // ID NPC для рекрутинга (добавить в команду)
    std::string removeNPC = "";                       // ID NPC для удаления из команды
    std::string triggerEvent = "";                    // ID события для немедленного триггера

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

    // Новые поля для системы событий
    EventType type = EventType::ROAD;             // Тип события
    float weight = 1.0f;                          // Вес для случайного выбора (по умолчанию 1.0)
    bool oneTimeOnly = false;                     // Может произойти только раз
    std::vector<std::string> blocksEvents;        // Блокирует другие события
    std::string associatedNPC = "";               // Связанный NPC (для COMPANION событий)

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
