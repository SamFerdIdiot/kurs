# Интеграция EventManager ↔ DialogueManager
## EventManager and DialogueManager Integration Specification

**Дата:** 2025-11-15  
**Статус:** Для утверждения  
**Контекст:** TravelCinemaScene

---

## 🎯 Обзор

В **TravelCinemaScene** игрок встречает два типа всплывающих окон:
1. **События (Events)** - выборы с последствиями
2. **Диалоги (Dialogues)** - короткие воспоминания/истории

Оба управляются разными менеджерами, но используют **одинаковый UI механизм** (всплывающие окна).

---

## 🏗️ Архитектура интеграции

### Две отдельные всплывающие сцены

```
TravelCinemaScene
    │
    ├─ EventPopup (окно выбора события)
    │   └─ EventManager (логика событий)
    │
    └─ DialoguePopup (окно диалога)
        └─ DialogueManager (логика диалогов)
```

**Принцип:** Одновременно может быть активно только ОДНО всплывающее окно.

---

## 📋 EventManager - Система событий

### Назначение

Управляет **случайными событиями на дороге**, которые требуют выбора игрока.

### Класс EventManager

```cpp
class EventManager {
public:
    static EventManager& getInstance();
    
    // Регистрация событий
    void registerEvent(const GameEvent& event);
    void registerEventsFromFile(const std::string& jsonPath);
    
    // Получение случайного события
    GameEvent getRandomEvent();
    GameEvent getEventById(const std::string& id);
    
    // Проверка условий
    bool checkCondition(const EventCondition& condition);
    
    // Применение последствий
    void applyOutcome(const EventOutcome& outcome);
    
private:
    std::map<std::string, GameEvent> m_events;
    std::vector<std::string> m_eventPool;  // ID доступных событий
};
```

### Структура GameEvent

```cpp
struct GameEvent {
    std::string id;
    EventType type;                     // EVENT, DIALOGUE, NPC_ENCOUNTER
    std::string title;                  // Заголовок окна
    std::string description;            // Описание ситуации
    std::vector<EventChoice> choices;   // Варианты выбора
    EventCondition condition;           // Условия показа
};

struct EventChoice {
    std::string text;                   // Текст выбора
    std::vector<ResourceCost> costs;    // Расход ресурсов
    std::vector<Requirement> requirements; // Требования (предметы, NPC)
    EventOutcome outcome;               // Последствия
};

struct ResourceCost {
    ResourceType type;                  // FUEL, MONEY, ENERGY, TIME
    float amount;
};

struct Requirement {
    RequirementType type;               // ITEM, NPC_ABILITY, PLAYER_SKILL
    std::string id;                     // ID предмета или способности
};

struct EventOutcome {
    std::vector<ResourceCost> rewards;  // Награды
    std::string message;                // Сообщение результата
    float moraleChange;                 // Изменение морали
};
```

### Пример события JSON

```json
{
  "id": "event_fallen_tree",
  "type": "EVENT",
  "title": "Дорожное событие",
  "description": "На дороге лежит упавшее дерево, блокирующее проезд! Что делаете?",
  "choices": [
    {
      "text": "🔧 Убрать дерево инструментами",
      "costs": [
        {"type": "ENERGY", "amount": 20},
        {"type": "TIME", "amount": 10}
      ],
      "requirements": [
        {"type": "ITEM", "id": "tools"}
      ],
      "outcome": {
        "message": "Вы убрали дерево и продолжили путь.",
        "moraleChange": 5
      }
    },
    {
      "text": "🚗 Объехать через поле",
      "costs": [
        {"type": "FUEL", "amount": 15},
        {"type": "TIME", "amount": 5}
      ],
      "outcome": {
        "message": "Вы объехали препятствие по бездорожью."
      }
    },
    {
      "text": "⏳ Дождаться помощи",
      "costs": [
        {"type": "TIME", "amount": 30}
      ],
      "outcome": {
        "message": "Через полчаса приехали дорожные службы и убрали дерево."
      }
    }
  ]
}
```

---

## 💬 DialogueManager - Система диалогов

### Назначение

Управляет **диалогами-воспоминаниями** и **диалогами встречи NPC**.

### Класс DialogueManager (уже реализован в PR #119)

```cpp
class DialogueManager {
public:
    static DialogueManager& getInstance();
    
    // Запуск диалога
    void startDialogue(const std::string& dialogueId);
    
    // Выбор варианта (если есть)
    void selectChoice(int choiceIndex);
    
    // Получение текущего узла
    const DialogueNode* getCurrentNode() const;
    
    // Проверка активности
    bool isActive() const;
    
    // Завершение
    void endDialogue();
    
    // Регистрация диалогов
    void registerDialogue(const std::string& id, const DialogueTree& tree);
    
private:
    std::map<std::string, DialogueTree> m_dialogues;
    DialogueNode* m_currentNode;
    bool m_active;
};
```

### Структура DialogueNode (из PR #119)

```cpp
struct DialogueNode {
    std::string text;                   // Текст диалога (1-6 фраз)
    std::string speaker;                // Кто говорит
    std::vector<DialogueChoice> choices; // Варианты ответа (если есть)
    std::string nextNodeId;             // Следующий узел (если нет выборов)
    DialogueOutcome outcome;            // Последствия
};

struct DialogueChoice {
    std::string text;
    std::string nextNodeId;
    EventCondition condition;           // Условие доступности
};

struct DialogueOutcome {
    float moraleChange;                 // Изменение морали
    std::vector<std::string> itemsGained; // Полученные предметы
    std::string message;                // Сообщение результата
};
```

### Пример диалога JSON (воспоминание)

```json
{
  "id": "dialogue_first_car",
  "type": "DIALOGUE",
  "nodes": [
    {
      "id": "node_1",
      "text": "Вы вспоминаете, как впервые сели за руль этой машины...",
      "speaker": "narrator",
      "nextNodeId": "node_2"
    },
    {
      "id": "node_2",
      "text": "Отец сказал: 'Береги её. Она провезёт тебя через всё, если ты будешь заботиться о ней.'",
      "speaker": "father",
      "nextNodeId": "node_3"
    },
    {
      "id": "node_3",
      "text": "Воспоминание согревает душу.",
      "speaker": "narrator",
      "outcome": {
        "moraleChange": 5,
        "message": "Мораль команды +5%"
      }
    }
  ]
}
```

### Пример диалога JSON (встреча NPC)

```json
{
  "id": "dialogue_npc_mechanic",
  "type": "DIALOGUE",
  "nodes": [
    {
      "id": "node_1",
      "text": "На обочине стоит мужчина средних лет с рюкзаком. Он голосует, пытаясь поймать попутку.",
      "speaker": "narrator",
      "nextNodeId": "node_2"
    },
    {
      "id": "node_2",
      "text": "Эй! Подвезёте до города? Я механик, могу помочь с машиной в дороге!",
      "speaker": "mechanic",
      "choices": [
        {
          "text": "👍 Взять попутчика",
          "nextNodeId": "node_accept"
        },
        {
          "text": "👋 Отказать вежливо",
          "nextNodeId": "node_refuse"
        }
      ]
    },
    {
      "id": "node_accept",
      "text": "Спасибо! Меня зовут Михаил. Вы не пожалеете!",
      "speaker": "mechanic",
      "outcome": {
        "npcJoined": "npc_mikhail_mechanic",
        "message": "Михаил присоединился к команде!"
      }
    },
    {
      "id": "node_refuse",
      "text": "Ну ладно... Счастливого пути.",
      "speaker": "mechanic"
    }
  ]
}
```

---

## 🎨 UI Интеграция

### EventPopup - Всплывающее окно события

```cpp
class EventPopup {
public:
    EventPopup(const GameEvent& event);
    
    void handleInput(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    bool isFinished() const { return m_finished; }
    int getSelectedChoice() const { return m_selectedChoice; }
    
private:
    GameEvent m_event;
    int m_selectedIndex = 0;
    int m_selectedChoice = -1;
    bool m_finished = false;
    
    void renderTitle(sf::RenderWindow& window);
    void renderDescription(sf::RenderWindow& window);
    void renderChoices(sf::RenderWindow& window);
    void renderRequirements(sf::RenderWindow& window, const EventChoice& choice);
};
```

**Wireframe:**

```
┌────────────────────────────────────────────────────────────┐
│  ⚠️ ДОРОЖНОЕ СОБЫТИЕ                                       │
├────────────────────────────────────────────────────────────┤
│                                                              │
│  [Описание события из GameEvent.description]               │
│                                                              │
│  ┌────────────────────────────────────────────────────┐   │
│  │ > [Выбор 1 текст]                                  │   │
│  │   [Расходы: -20% энергии, -10 минут]              │   │
│  │   [Требуется: Инструменты]                        │   │
│  └────────────────────────────────────────────────────┘   │
│                                                              │
│  ┌────────────────────────────────────────────────────┐   │
│  │   [Выбор 2 текст]                                  │   │
│  │   [Расходы: -15L топлива]                          │   │
│  └────────────────────────────────────────────────────┘   │
│                                                              │
│  [↑/↓] Выбор  [Enter] Подтвердить                          │
└────────────────────────────────────────────────────────────┘
```

### DialoguePopup - Всплывающее окно диалога

```cpp
class DialoguePopup {
public:
    DialoguePopup(const std::string& dialogueId);
    
    void handleInput(const sf::Event& event);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
    bool isFinished() const { return m_finished; }
    
private:
    std::string m_dialogueId;
    const DialogueNode* m_currentNode;
    bool m_finished = false;
    
    void renderSpeaker(sf::RenderWindow& window);
    void renderText(sf::RenderWindow& window);
    void renderChoices(sf::RenderWindow& window);
    void advance();
};
```

**Wireframe:**

```
┌────────────────────────────────────────────────────────────┐
│  💭 ВОСПОМИНАНИЕ                                            │
├────────────────────────────────────────────────────────────┤
│                                                              │
│  [Текст из DialogueNode.text]                              │
│  [1-6 фраз максимум]                                       │
│                                                              │
│  [Если есть choices:]                                      │
│  ┌────────────────────────────────────────────────────┐   │
│  │ > [Вариант 1]                                      │   │
│  └────────────────────────────────────────────────────┘   │
│  ┌────────────────────────────────────────────────────┐   │
│  │   [Вариант 2]                                      │   │
│  └────────────────────────────────────────────────────┘   │
│                                                              │
│  [Enter] Продолжить                                         │
└────────────────────────────────────────────────────────────┘
```

---

## 🔄 Интеграция в TravelCinemaScene

### Логика переключения

```cpp
class TravelCinemaScene : public Scene {
private:
    enum PopupState {
        NONE,           // Нет активного popup
        EVENT_ACTIVE,   // Активно окно события
        DIALOGUE_ACTIVE // Активен диалог
    };
    
    PopupState m_popupState = PopupState::NONE;
    std::unique_ptr<EventPopup> m_eventPopup;
    std::unique_ptr<DialoguePopup> m_dialoguePopup;
    
public:
    void triggerEvent() {
        if (m_popupState != PopupState::NONE) {
            return; // Уже есть активное окно
        }
        
        // Получить случайное событие
        GameEvent event = EventManager::getInstance().getRandomEvent();
        
        // Создать popup
        m_eventPopup = std::make_unique<EventPopup>(event);
        m_popupState = PopupState::EVENT_ACTIVE;
        
        // Пауза фона
        m_parallax.pause();
    }
    
    void triggerDialogue() {
        if (m_popupState != PopupState::NONE) {
            return; // Уже есть активное окно
        }
        
        // Получить случайный диалог
        std::string dialogueId = pickRandomDialogue();
        
        // Запустить диалог
        DialogueManager::getInstance().startDialogue(dialogueId);
        
        // Создать popup
        m_dialoguePopup = std::make_unique<DialoguePopup>(dialogueId);
        m_popupState = PopupState::DIALOGUE_ACTIVE;
        
        // Пауза фона
        m_parallax.pause();
    }
    
    void update(float deltaTime) override {
        if (m_popupState == PopupState::EVENT_ACTIVE) {
            m_eventPopup->update(deltaTime);
            
            if (m_eventPopup->isFinished()) {
                // Применить последствия выбора
                int choice = m_eventPopup->getSelectedChoice();
                applyEventOutcome(choice);
                
                // Закрыть popup
                m_eventPopup.reset();
                m_popupState = PopupState::NONE;
                
                // Возобновить фон
                m_parallax.resume();
            }
        }
        else if (m_popupState == PopupState::DIALOGUE_ACTIVE) {
            m_dialoguePopup->update(deltaTime);
            
            if (m_dialoguePopup->isFinished()) {
                // Применить последствия диалога
                applyDialogueOutcome();
                
                // Закрыть popup
                m_dialoguePopup.reset();
                m_popupState = PopupState::NONE;
                
                // Возобновить фон
                m_parallax.resume();
            }
        }
        else {
            // Обычное обновление фона
            m_parallax.update(deltaTime);
            m_progress += deltaTime / m_totalTravelTime;
            
            // Проверка триггеров событий
            checkEventTriggers();
        }
    }
};
```

---

## 📅 Планирование событий и диалогов

### EventScheduler (в TravelCinemaScene)

```cpp
void TravelCinemaScene::scheduleEventsAndDialogues() {
    float totalTime = m_route.travelTime;  // часы
    
    // 3-4 события
    int numEvents = randomInt(3, 4);
    
    // 2-3 диалога
    int numDialogues = randomInt(2, 3);
    
    // 1 NPC каждые 3 дороги
    bool shouldSpawnNPC = (m_roadCount % 3 == 0);
    
    // Создать очередь событий
    std::vector<ScheduledItem> schedule;
    
    // Добавить события
    for (int i = 0; i < numEvents; ++i) {
        float time = (totalTime / (numEvents + 1)) * (i + 1);
        schedule.push_back({time, ItemType::EVENT, ""});
    }
    
    // Добавить диалоги (убедиться, что не пересекаются)
    for (int i = 0; i < numDialogues; ++i) {
        float time = (totalTime / (numDialogues + 1)) * (i + 1);
        // Сдвинуть, если пересекается с событием
        time += 5.0f; // +5 минут сдвига
        schedule.push_back({time, ItemType::DIALOGUE, pickRandomDialogueId()});
    }
    
    // Добавить встречу NPC
    if (shouldSpawnNPC && NPCManager::getInstance().hasSpace()) {
        float time = totalTime * 0.6f;
        schedule.push_back({time, ItemType::NPC_ENCOUNTER, "npc_random"});
    }
    
    // Сортировать по времени
    std::sort(schedule.begin(), schedule.end(), 
              [](const ScheduledItem& a, const ScheduledItem& b) {
                  return a.time < b.time;
              });
    
    m_schedule = schedule;
}

void TravelCinemaScene::checkEventTriggers() {
    if (m_schedule.empty()) return;
    
    float currentTime = m_progress * m_totalTravelTime;
    
    // Проверить, не пора ли триггерить следующее событие
    if (currentTime >= m_schedule.front().time) {
        ScheduledItem item = m_schedule.front();
        m_schedule.erase(m_schedule.begin());
        
        switch (item.type) {
            case ItemType::EVENT:
                triggerEvent();
                break;
            case ItemType::DIALOGUE:
                triggerDialogue();
                break;
            case ItemType::NPC_ENCOUNTER:
                triggerNPCEncounter();
                break;
        }
    }
}
```

---

## 🎲 Система замещения событий

**Правило:** События могут быть **заменены** диалогами или встречами NPC.

**Механика:**
1. EventScheduler планирует 3-4 события
2. EventScheduler планирует 2-3 диалога
3. При планировании диалоги **могут заменить** события

**Пример:**
```
Запланировано: EVENT1 (10 мин), DIALOGUE1 (15 мин), EVENT2 (20 мин), EVENT3 (25 мин)

Замещение:
- DIALOGUE1 заменяет EVENT1 → показывается DIALOGUE1
- EVENT2 остаётся
- EVENT3 остаётся
```

**Код:**
```cpp
void TravelCinemaScene::scheduleWithSubstitution() {
    // План событий
    std::vector<float> eventTimes = {10, 20, 30, 40}; // 4 события
    std::vector<float> dialogueTimes = {12, 25}; // 2 диалога
    
    // Замещение
    for (float dialogueTime : dialogueTimes) {
        // Найти ближайшее событие
        auto closest = std::min_element(eventTimes.begin(), eventTimes.end(),
            [dialogueTime](float a, float b) {
                return std::abs(a - dialogueTime) < std::abs(b - dialogueTime);
            });
        
        // Если близко (в пределах 5 минут), заменить
        if (std::abs(*closest - dialogueTime) < 5.0f) {
            eventTimes.erase(closest);
        }
    }
    
    // Теперь eventTimes содержит только не замещённые события
}
```

---

## ✅ Критерии приёмки

Интеграция готова, когда:

- [ ] EventManager загружает события из JSON
- [ ] DialogueManager загружает диалоги из JSON (из PR #119)
- [ ] TravelCinemaScene может триггерить EventPopup
- [ ] TravelCinemaScene может триггерить DialoguePopup
- [ ] Только одно popup активно одновременно
- [ ] Parallax фон паузится при показе popup
- [ ] Последствия событий применяются корректно
- [ ] Последствия диалогов применяются корректно
- [ ] Система замещения работает (диалоги могут заменить события)
- [ ] 3-4 события на дорогу
- [ ] 2-3 диалога на дорогу
- [ ] 1 встреча NPC на 3 дороги

---

## 📝 Примечания

### Различия между Events и Dialogues:

| Аспект | Events | Dialogues |
|--------|--------|-----------|
| **Выборы** | Всегда есть (2-4) | Опционально (0-2) |
| **Требования** | Могут быть (предметы, NPC) | Редко |
| **Расходы** | Обязательно (ресурсы) | Редко |
| **Длительность** | Средняя (1-2 мин чтения) | Короткая (10-30 сек) |
| **Частота** | 3-4 на дорогу | 2-3 на дорогу |
| **UI** | Детальное (требования, расходы) | Простое (текст) |

### Рекомендации:

1. **События** - для геймплейных моментов (выборы, ресурсы)
2. **Диалоги** - для атмосферы и сторителлинга
3. **NPC встречи** - отдельный тип диалога с возможностью взять NPC

---

**Статус:** ✅ Готово к утверждению  
**Зависимости:** PR #119 (DialogueManager)  
**Готово к реализации после утверждения**
