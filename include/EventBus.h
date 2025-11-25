#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <any>

/**
 * @brief Unified Event Bus for inter-component communication
 * 
 * This class implements the Observer pattern and provides a centralized
 * event system for communication between scenes, systems, and components.
 * 
 * Key Features:
 * - Type-safe event handling with std::any
 * - Subscribe/Unsubscribe mechanism
 * - Thread-safe operations
 * - Multiple subscribers per event type
 * - Immediate and queued event dispatch
 * 
 * Usage Example:
 * 
 *   // Subscribe to an event
 *   int listenerId = EventBus::getInstance().subscribe("ResourceChanged", 
 *       [](const std::any& data) {
 *           auto resourceData = std::any_cast<ResourceChangeData>(data);
 *           // Handle resource change
 *       });
 *   
 *   // Publish an event
 *   ResourceChangeData data{ResourceType::FUEL, -10.0f};
 *   EventBus::getInstance().publish("ResourceChanged", data);
 *   
 *   // Unsubscribe
 *   EventBus::getInstance().unsubscribe("ResourceChanged", listenerId);
 */

// Event callback type - receives event data as std::any
using EventCallback = std::function<void(const std::any&)>;

// Common event data structures
struct ResourceChangeEvent {
    enum class ResourceType { FUEL, MONEY, ENERGY, MORALE };
    ResourceType type;
    float oldValue;
    float newValue;
    float delta;
};

struct InventoryChangeEvent {
    enum class ChangeType { ITEM_ADDED, ITEM_REMOVED, ITEM_MOVED };
    ChangeType type;
    std::string itemId;
    int quantity;
    int gridX, gridY;
};

struct NPCChangeEvent {
    enum class ChangeType { ADDED, REMOVED, MORALE_CHANGED };
    ChangeType type;
    std::string npcId;
    std::string npcName;
    float moraleValue;
};

struct SceneTransitionEvent {
    std::string fromScene;
    std::string toScene;
    std::map<std::string, std::any> transferData;
};

struct QuestEvent {
    enum class EventType { STARTED, COMPLETED, FAILED, UPDATED };
    EventType type;
    std::string questId;
    std::string questName;
    float progress;
};

struct GameEvent {
    std::string eventId;
    std::string eventType;
    std::map<std::string, std::any> eventData;
};

class EventBus {
public:
    /**
     * @brief Get the singleton instance
     */
    static EventBus& getInstance();
    
    /**
     * @brief Subscribe to an event type
     * @param eventType The name of the event to subscribe to
     * @param callback Function to call when event is published
     * @return Unique listener ID for unsubscribing
     */
    int subscribe(const std::string& eventType, EventCallback callback);
    
    /**
     * @brief Unsubscribe from an event type
     * @param eventType The event type to unsubscribe from
     * @param listenerId The ID returned from subscribe()
     * @return true if successfully unsubscribed
     */
    bool unsubscribe(const std::string& eventType, int listenerId);
    
    /**
     * @brief Publish an event immediately
     * @param eventType The type of event
     * @param data Event data (can be any type wrapped in std::any)
     */
    void publish(const std::string& eventType, const std::any& data);
    
    /**
     * @brief Queue an event for later dispatch
     * @param eventType The type of event
     * @param data Event data
     */
    void queueEvent(const std::string& eventType, const std::any& data);
    
    /**
     * @brief Process all queued events
     * Should be called once per frame in the main game loop
     */
    void processQueue();
    
    /**
     * @brief Clear all subscribers for an event type
     * @param eventType The event type to clear
     */
    void clearSubscribers(const std::string& eventType);
    
    /**
     * @brief Clear all subscribers for all event types
     */
    void clearAllSubscribers();
    
    /**
     * @brief Get number of subscribers for an event type
     */
    size_t getSubscriberCount(const std::string& eventType) const;
    
    // Prevent copying
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    
private:
    EventBus() : m_nextListenerId(1) {}
    
    struct Listener {
        int id;
        EventCallback callback;
    };
    
    struct QueuedEvent {
        std::string eventType;
        std::any data;
    };
    
    // Map of event types to their listeners
    std::map<std::string, std::vector<Listener>> m_listeners;
    
    // Queue for deferred event processing
    std::vector<QueuedEvent> m_eventQueue;
    
    // Thread safety
    mutable std::mutex m_mutex;
    
    // Next unique listener ID
    int m_nextListenerId;
};

// Convenience macros for common events
#define EVENT_RESOURCE_CHANGED "ResourceChanged"
#define EVENT_INVENTORY_CHANGED "InventoryChanged"
#define EVENT_NPC_CHANGED "NPCChanged"
#define EVENT_SCENE_TRANSITION "SceneTransition"
#define EVENT_QUEST_UPDATE "QuestUpdate"
#define EVENT_GAME_EVENT "GameEvent"
#define EVENT_UI_UPDATE "UIUpdate"
#define EVENT_SAVE_GAME "SaveGame"
#define EVENT_LOAD_GAME "LoadGame"

#endif // EVENT_BUS_H
