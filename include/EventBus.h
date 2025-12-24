#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <any>
































using EventCallback = std::function<void(const std::any&)>;


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



    static EventBus& getInstance();







    int subscribe(const std::string& eventType, EventCallback callback);







    bool unsubscribe(const std::string& eventType, int listenerId);






    void publish(const std::string& eventType, const std::any& data);






    void queueEvent(const std::string& eventType, const std::any& data);





    void processQueue();





    void clearSubscribers(const std::string& eventType);




    void clearAllSubscribers();




    size_t getSubscriberCount(const std::string& eventType) const;


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


    std::map<std::string, std::vector<Listener>> m_listeners;


    std::vector<QueuedEvent> m_eventQueue;


    mutable std::mutex m_mutex;


    int m_nextListenerId;
};


#define EVENT_RESOURCE_CHANGED "ResourceChanged"
#define EVENT_INVENTORY_CHANGED "InventoryChanged"
#define EVENT_NPC_CHANGED "NPCChanged"
#define EVENT_SCENE_TRANSITION "SceneTransition"
#define EVENT_QUEST_UPDATE "QuestUpdate"
#define EVENT_GAME_EVENT "GameEvent"
#define EVENT_UI_UPDATE "UIUpdate"
#define EVENT_SAVE_GAME "SaveGame"
#define EVENT_LOAD_GAME "LoadGame"

#endif
