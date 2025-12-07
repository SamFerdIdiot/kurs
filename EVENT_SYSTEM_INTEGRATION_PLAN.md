# План интеграции системы событий и компаньонов

## Текущее состояние архитектуры

### ✅ Уже реализовано:
1. **EventManager** - базовая система событий с условиями
   - EventCondition: minFuel, maxFuel, minEnergy, maxEnergy, minMoney, location, roadType
   - GameEvent: id, title, description, choices
   - EventChoice: text, outcomeText, resource changes

2. **NPC система**
   - NPC класс: id, name, bio, level (1-3), experience
   - PassiveAbility и ActiveAbility (структуры готовы)
   - NPCManager: синглтон для управления командой (maxTeamSize)
   - NPCFactory: предопределённые NPC (mechanic, trader, medic, navigator, companion)

3. **DialogueManager** - система диалогов
   - DialogueTree с узлами и выборами
   - DialogueCondition и DialogueConsequence
   - История выборов
   - Фабрика диалогов рекрутинга

4. **PlayerState** - состояние игрока
   - Ресурсы: energy, money, fuel, vehicleCondition
   - InventorySystem
   - Origin и CarType
   - currentCityIndex (0-4)

5. **Сцены**
   - TravelCinematicScene - путешествия между городами
   - LocationScene - локации с магазином/заправкой
   - CinematicEventScene - показ событий
   - CompanionsScene - экран команды (уже есть!)

---

## ⚠️ Что нужно добавить

### 1. Система отношений (Relationship System)

#### 1.1 Расширить класс NPC
**Файл**: `include/NPC.h`, `src/NPC.cpp`

```cpp
class NPC {
    // ... существующие поля ...

    // ДОБАВИТЬ:
    int m_relationship;              // -100 до +100
    bool m_metBefore;                // Встречался ли ранее
    bool m_inParty;                  // В команде ли сейчас
    std::string m_currentLocation;   // Где находится (если не в команде)

    // Новые методы:
    int getRelationship() const { return m_relationship; }
    void setRelationship(int value);
    void modifyRelationship(int delta);

    bool hasMetBefore() const { return m_metBefore; }
    void setMetBefore(bool met) { m_metBefore = met; }

    bool isInParty() const { return m_inParty; }
    void setInParty(bool inParty) { m_inParty = inParty; }

    std::string getCurrentLocation() const { return m_currentLocation; }
    void setCurrentLocation(const std::string& loc) { m_currentLocation = loc; }
};
```

#### 1.2 Добавить персонажей для российской тематики
**Файл**: `include/NPC.h` (NPCFactory namespace)

```cpp
namespace NPCFactory {
    // Существующие:
    std::unique_ptr<NPC> createMechanic(const std::string& id);
    std::unique_ptr<NPC> createTrader(const std::string& id);

    // ДОБАВИТЬ русских персонажей:
    std::unique_ptr<NPC> createUnemployed(const std::string& id);  // Безработный (зануда)
    std::unique_ptr<NPC> createPunk(const std::string& id);         // Панк с гитарой
    std::unique_ptr<NPC> createGranny(const std::string& id);       // Бабуля с пирожками
    std::unique_ptr<NPC> createTrucker(const std::string& id);      // Дальнобойщик
    std::unique_ptr<NPC> createStudent(const std::string& id);      // Студент-автостопщик
}
```

---

### 2. Расширить систему событий

#### 2.1 Улучшить EventCondition
**Файл**: `include/EventManager.h`

```cpp
struct EventCondition {
    // Существующие:
    float minFuel, maxFuel;
    float minEnergy, maxEnergy;
    int minMoney, maxMoney;
    float probability;
    std::string requiredLocation;
    std::string requiredRoadType;

    // ДОБАВИТЬ:
    std::vector<std::string> requiredNPCsInParty;    // NPC должны быть в команде
    std::map<std::string, int> minRelationships;      // Минимальные отношения с NPC
    std::vector<std::string> requiredItems;           // Требуемые предметы
    std::vector<std::string> blockedIfTriggered;      // Блокируется если эти события уже были
    int minPartySize;                                  // Минимальный размер команды
    int maxPartySize;                                  // Максимальный размер команды
};
```

#### 2.2 Добавить метаданные к GameEvent
**Файл**: `include/EventManager.h`

```cpp
enum class EventType {
    ROAD,           // Дорожное событие
    COMPANION,      // Событие с компаньоном
    RESOURCE,       // Ресурсное событие
    SHOP,           // Магазин
    ENCOUNTER,      // Встреча с NPC
    CONFLICT        // Конфликт в команде
};

struct GameEvent {
    // Существующие:
    std::string id;
    std::string title;
    std::string description;
    EventCondition condition;
    std::vector<EventChoice> choices;
    bool triggered;

    // ДОБАВИТЬ:
    EventType type;                    // Тип события
    float weight;                      // Вес для случайного выбора (1.0 по умолчанию)
    bool oneTimeOnly;                  // Может произойти только раз
    std::vector<std::string> blocksEvents;  // Блокирует другие события
    std::string associatedNPC;         // Связанный NPC (для COMPANION событий)
};
```

#### 2.3 Улучшить EventChoice
**Файл**: `include/EventManager.h`

```cpp
struct EventChoice {
    // Существующие:
    std::string text;
    std::string outcomeText;
    float fuelChange, energyChange;
    int moneyChange;
    float vehicleConditionChange;
    bool endsEvent;
    std::string perkId;
    bool isPerkChoice;

    // ДОБАВИТЬ:
    std::map<std::string, int> relationshipChanges;  // Изменения отношений с NPC
    std::vector<std::string> addItems;                // Добавить предметы
    std::vector<std::string> removeItems;             // Убрать предметы
    std::string recruitNPC;                           // ID NPC для рекрутинга
    std::string removeNPC;                            // ID NPC для удаления из команды
    std::string triggerEvent;                         // ID события для триггера
};
```

---

### 3. Система пассивных способностей

#### 3.1 Создать AbilityEffectManager
**Новый файл**: `include/AbilityEffectManager.h`

```cpp
#ifndef ABILITY_EFFECT_MANAGER_H
#define ABILITY_EFFECT_MANAGER_H

#include "NPC.h"
#include "PlayerState.h"
#include <string>

// Система применения пассивных способностей компаньонов
class AbilityEffectManager {
public:
    static AbilityEffectManager& getInstance();

    // Применить пассивные способности всех компаньонов
    void applyDailyPassiveEffects(PlayerState& playerState);

    // Применить способности во время определённого события
    void applyEventPassiveEffects(PlayerState& playerState, const std::string& eventType);

    // Проверить активную способность компаньона
    bool canUseActiveAbility(const std::string& npcId) const;
    void useActiveAbility(const std::string& npcId, PlayerState& playerState);

    // Кулдауны способностей
    void updateCooldowns(float deltaTime);
    void resetAllCooldowns();

private:
    AbilityEffectManager();

    // Конкретные эффекты:
    void applyMechanicPassive(PlayerState& playerState);      // +5% к машине
    void applyUnemployedPassive(PlayerState& playerState);    // +100 денег раз в неделю
    void applyPunkPassive(PlayerState& playerState);          // Шанс воровства

    std::map<std::string, float> m_cooldowns;  // npcId -> remaining cooldown
    float m_daysSinceLastWeeklyPayout;         // Для безработного

    static AbilityEffectManager* s_instance;
};

#endif // ABILITY_EFFECT_MANAGER_H
```

---

### 4. Интеграция с существующими сценами

#### 4.1 TravelCinematicScene
**Файл**: `src/TravelCinematicScene.cpp`

**Изменения**:
1. После прибытия в город → проверить случайную встречу с NPC:
```cpp
void TravelCinematicScene::checkArrival() {
    if (m_progress >= 1.0f) {
        m_arrived = true;

        // ДОБАВИТЬ: Шанс встретить NPC на дороге
        if (!m_arrivedEventTriggered) {
            tryTriggerNPCEncounter();
            m_arrivedEventTriggered = true;
        }

        // ... existing code ...
    }
}

void TravelCinematicScene::tryTriggerNPCEncounter() {
    // 20% шанс встретить NPC
    if (randomFloat(0.0f, 1.0f) < 0.2f) {
        // Получить случайного NPC который ещё не встречался
        auto& npcManager = NPCManager::getInstance();
        // ... логика выбора NPC ...

        // Показать событие встречи
        GameEvent encounter = EventFactory::createNPCEncounterEvent(npcId);
        m_activeEvent = std::make_unique<CinematicEventScene>(encounter, m_playerState);
    }
}
```

2. Применить пассивные способности компаньонов во время поездки:
```cpp
void TravelCinematicScene::consumeResources(float deltaTime) {
    // ... existing fuel consumption ...

    // ДОБАВИТЬ: Применить модификаторы компаньонов
    float fuelMultiplier = 1.0f;

    // Механик: +10% расход топлива (негатив)
    if (npcManager.getNPC("mechanic") && npcManager.getNPC("mechanic")->isInParty()) {
        fuelMultiplier *= 1.1f;
    }

    fuelConsumed *= fuelMultiplier;
    m_playerState->addFuel(-fuelConsumed);
}
```

#### 4.2 LocationScene - добавить применение дневных эффектов
**Файл**: `src/LocationScene.cpp`

```cpp
void LocationScene::enter() {
    // ... existing code ...

    // ДОБАВИТЬ: Применить дневные пассивные эффекты
    AbilityEffectManager::getInstance().applyDailyPassiveEffects(playerState);

    // Показать сообщения о способностях:
    // "Механик починил машину (+5%)"
    // "Безработный получил пособие (+100 руб)"
}
```

#### 4.3 Расширить EventManager::getRandomEvent()
**Файл**: `src/EventManager.cpp`

```cpp
GameEvent* EventManager::getRandomEvent(float fuel, float energy, int money,
                                       const std::string& location,
                                       const std::string& roadType) {
    // Собрать все доступные события
    std::vector<GameEvent*> availableEvents;
    std::vector<float> weights;

    auto& npcManager = NPCManager::getInstance();

    for (auto& event : m_events) {
        if (event.triggered && event.oneTimeOnly) continue;

        // Базовая проверка
        if (!checkCondition(event.condition, fuel, energy, money, location, roadType)) {
            continue;
        }

        // ДОБАВИТЬ: Проверка требований к компаньонам
        bool companionReqsMet = true;
        for (const auto& npcId : event.condition.requiredNPCsInParty) {
            auto* npc = npcManager.getNPC(npcId);
            if (!npc || !npc->isInParty()) {
                companionReqsMet = false;
                break;
            }
        }
        if (!companionReqsMet) continue;

        // ДОБАВИТЬ: Проверка отношений
        bool relationshipReqsMet = true;
        for (const auto& [npcId, minRel] : event.condition.minRelationships) {
            auto* npc = npcManager.getNPC(npcId);
            if (!npc || npc->getRelationship() < minRel) {
                relationshipReqsMet = false;
                break;
            }
        }
        if (!relationshipReqsMet) continue;

        // Добавить в пул с весом
        availableEvents.push_back(&event);
        weights.push_back(event.weight);
    }

    // Выбрать случайное событие с учётом весов
    if (availableEvents.empty()) return nullptr;

    return weightedRandomChoice(availableEvents, weights);
}
```

---

## 5. Фабрика событий

#### 5.1 Создать EventFactory
**Новый файл**: `include/EventFactory.h`

```cpp
#ifndef EVENT_FACTORY_H
#define EVENT_FACTORY_H

#include "EventManager.h"
#include <memory>

namespace EventFactory {
    // Встреча с NPC на дороге
    GameEvent createNPCEncounterEvent(const std::string& npcId);

    // События с механиком
    GameEvent createMechanicRepairEvent();          // Просит запчасти
    GameEvent createMechanicStealsCigarettesEvent(); // Ворует сигареты

    // События с безработным
    GameEvent createUnemployedDocumentsLostEvent();  // Потерял документы
    GameEvent createUnemployedCheapStoreEvent();     // Знает дешёвый магазин

    // События с панком
    GameEvent createPunkConcertEvent();              // Хочет устроить концерт
    GameEvent createPunkShopliftingEvent();          // Ворует в магазине
    GameEvent createPunkConflictEvent();             // Провоцирует драку

    // Конфликты между компаньонами
    GameEvent createCompanionConflictEvent(const std::string& npc1, const std::string& npc2);

    // Критические события
    GameEvent createOutOfFuelEvent();
    GameEvent createOutOfMoneyEvent();
    GameEvent createCarBrokenEvent();
}

#endif // EVENT_FACTORY_H
```

---

## 6. План реализации (по порядку)

### Фаза 1: Базовые расширения (1-2 дня)
1. ✅ Расширить NPC класс (relationship, metBefore, inParty, currentLocation)
2. ✅ Расширить EventCondition (requiredNPCs, minRelationships, requiredItems)
3. ✅ Расширить GameEvent (type, weight, oneTimeOnly, associatedNPC)
4. ✅ Расширить EventChoice (relationshipChanges, addItems, recruitNPC)
5. ✅ Создать новых NPC в NPCFactory (Unemployed, Punk, Granny, Trucker, Student)

### Фаза 2: Система способностей (1 день)
1. ✅ Создать AbilityEffectManager
2. ✅ Реализовать пассивные эффекты (Mechanic, Unemployed, Punk)
3. ✅ Интегрировать с LocationScene (applyDailyPassiveEffects)

### Фаза 3: Улучшение EventManager (1 день)
1. ✅ Обновить checkCondition() для новых условий
2. ✅ Обновить getRandomEvent() для weighted selection
3. ✅ Добавить блокировку событий (blockedIfTriggered)
4. ✅ Реализовать применение relationshipChanges в applyChoice()

### Фаза 4: EventFactory и контент (2 дня)
1. ✅ Создать EventFactory
2. ✅ Добавить 10-15 базовых событий:
   - 3 встречи с NPC
   - 5 событий с компаньонами
   - 2 конфликта
   - 3 критических события
3. ✅ Добавить диалоги рекрутинга в DialogueFactory

### Фаза 5: Интеграция со сценами (1 день)
1. ✅ TravelCinematicScene: tryTriggerNPCEncounter()
2. ✅ TravelCinematicScene: модификаторы топлива от компаньонов
3. ✅ LocationScene: применение дневных эффектов
4. ✅ CompanionsScene: отображение отношений

### Фаза 6: Тестирование и балансировка (1 день)
1. ✅ Проверить все события
2. ✅ Балансировка весов и вероятностей
3. ✅ Проверить систему отношений
4. ✅ Протестировать пассивные способности

---

## 7. Пример JSON для будущего data-driven подхода

```json
{
  "events": [
    {
      "id": "punk_concert",
      "type": "COMPANION",
      "title": "Концерт на заправке",
      "description": "Панк хочет устроить концерт прямо на заправке...",
      "weight": 2.5,
      "oneTimeOnly": false,
      "associatedNPC": "punk_vasya",
      "condition": {
        "requiredNPCsInParty": ["punk_vasya"],
        "minRelationships": {"punk_vasya": 20},
        "minMoney": 50,
        "probability": 0.7
      },
      "choices": [
        {
          "text": "Разрешить концерт",
          "outcomeText": "Концерт прошёл на ура! Заработали 150 рублей, Панк доволен.",
          "moneyChange": 150,
          "relationshipChanges": {"punk_vasya": 15}
        },
        {
          "text": "Запретить",
          "outcomeText": "Панк недоволен вашим решением.",
          "relationshipChanges": {"punk_vasya": -10}
        }
      ]
    }
  ]
}
```

---

## 8. Ключевые улучшения системы

### ✨ Что даст новая система:
1. **Динамические события** - события зависят от состава команды и отношений
2. **Реиграбельность** - разные комбинации NPC = разный геймплей
3. **Тактическая глубина** - баланс между способностями и негативами NPC
4. **Нарратив** - ветки диалогов и личные квесты NPC
5. **Progression** - система отношений и развитие персонажей

### 🔧 Технические преимущества:
1. Минимальные изменения существующего кода
2. Модульная архитектура (легко расширять)
3. Готовность к data-driven подходу (JSON события)
4. Синглтоны для глобального доступа
5. Чистое разделение ответственности

---

## 9. Следующие шаги

1. **Начать с Фазы 1** - расширить базовые структуры
2. **Создать тестовые данные** - 2-3 NPC и 5 событий для проверки
3. **Постепенная интеграция** - по одной фазе за раз
4. **Тестирование** - проверять каждую фазу перед переходом к следующей

---

**Готов начать реализацию! С чего начнём?**
