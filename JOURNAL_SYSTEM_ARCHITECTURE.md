# АРХИТЕКТУРА СИСТЕМЫ ЗАПИСОК (JOURNAL SYSTEM)
**KKURS - Унифицированный текстовый интерфейс**

**Дата создания:** 2025-12-06
**Статус:** 🎯 ПРОЕКТИРОВАНИЕ
**Цель:** Объединить весь геймплей в формат текстовых записок в стиле "60 Seconds!"

---

## 📋 ОГЛАВЛЕНИЕ

1. [Обзор концепции](#1-обзор-концепции)
2. [Типы записок](#2-типы-записок)
3. [Архитектура JournalScene](#3-архитектура-journalscene)
4. [Система Note (базовая записка)](#4-система-note-базовая-записка)
5. [Интеграция NodeScene → CityNote](#5-интеграция-nodescene--citynote)
6. [Интеграция TravelCinematicScene → TravelNote](#6-интеграция-travelcinematicscene--travelnote)
7. [Визуальный дизайн](#7-визуальный-дизайн)
8. [План миграции](#8-план-миграции)
9. [Преимущества подхода](#9-преимущества-подхода)

---

## 1. ОБЗОР КОНЦЕПЦИИ

### Текущая проблема
Сейчас в игре множество отдельных сцен с разными стилями UI:
- **MainMenuScene** - графическое меню
- **NodeScene** - карта города с кнопками
- **TravelCinematicScene** - 7-слойный параллакс
- **LocationScene** - визуальная локация
- **CinematicEventScene** - текстовое событие

**Проблемы:**
- ❌ Разрозненный визуальный стиль
- ❌ Сложная поддержка и балансировка
- ❌ Много дублирующегося кода
- ❌ Тяжелая графика для простых действий

### Новая концепция: Единый журнал
Весь геймплей превращается в **текстовый журнал с записками**.

**Пример игровой сессии:**
```
[ЗАПИСКА #1] Прибытие в Москву
"Машина останавливается у ржавого столба с табличкой 'МОСКВА'.
Вы здесь. Что делать дальше?"

→ Найти заправку
→ Найти работу
→ Отдохнуть в мотеле
→ Отправиться дальше

[ЗАПИСКА #2] Заправка "Лукойл"
"Заправщик в грязной спецовке подходит к машине..."

[ЗАПИСКА #3] Путешествие: Москва → Санкт-Петербург
"10:15 - Выезжаем из Москвы. Дорога свободная..."

[ЗАПИСКА #4] Событие: Сломанная машина
"На обочине видим Волгу с открытым капотом..."
```

### Философия дизайна
- ✅ **Один визуальный стиль** для всей игры
- ✅ **Текст > Графика** (фокус на истории и выборах)
- ✅ **Простота и гибкость** (легко добавлять контент)
- ✅ **Атмосфера книжки-игры** (как 60 Seconds, Reigns, Sunless Sea)

---

## 2. ТИПЫ ЗАПИСОК

### 2.1 CityNote (Записка города)
**Замена:** NodeScene
**Когда:** Прибытие в город, выбор действий в городе

**Формат:**
```
┌────────────────────────────────────────┐
│ 📍 МОСКВА                              │
│ День 5, 14:30                          │
├────────────────────────────────────────┤
│ Вы въезжаете в столицу. Пробки,       │
│ многоэтажки, запах выхлопов.           │
│ Бензобак на исходе, живот урчит.       │
│                                        │
│ ЧТО ДЕЛАТЬ?                            │
│ ▸ [1] Заправка (нужно 500₽)          │
│   [2] Работа (заработать денег)       │
│   [3] Мотель (отдохнуть, -1000₽)     │
│   [4] Продолжить путь                 │
│                                        │
│ [⛽12L] [⚡45] [₽350]                  │
└────────────────────────────────────────┘
```

**Свойства CityNote:**
- `cityId` - ID города (moscow, spb, etc.)
- `availableServices` - Список доступных сервисов (заправка, работа, мотель, магазин)
- `cityDescription` - Описание города (меняется в зависимости от времени суток, погоды)
- `choices` - Динамические выборы (зависят от ресурсов)

---

### 2.2 TravelNote (Записка путешествия)
**Замена:** TravelCinematicScene
**Когда:** Движение между городами

**Формат:**
```
┌────────────────────────────────────────┐
│ 🚗 ПУТЕШЕСТВИЕ: Москва → СПб          │
│ Прогресс: [=====>    ] 45% (54/120км) │
├────────────────────────────────────────┤
│ 10:15 - Выехали из Москвы.            │
│        Дорога М10, трасса свободная.   │
│                                        │
│ 11:30 - Проезжаем Тверь. Панк Вася    │
│        достал гитару, напевает что-то. │
│                                        │
│ 12:00 - ВНИМАНИЕ! Впереди развилка.   │
│                                        │
│ ЧТО ДЕЛАТЬ?                            │
│ ▸ [1] Свернуть на объездную (быстрее) │
│   [2] По главной дороге (безопаснее)  │
│                                        │
│ [⛽45L] [⚡60] [₽350]                  │
└────────────────────────────────────────┘
```

**Свойства TravelNote:**
- `fromCity` / `toCity` - Маршрут
- `progress` - Прогресс 0.0-1.0
- `journalEntries` - Список записей по ходу пути
- `autoScrollMode` - Автоматическая прокрутка или по клику игрока

**Механика:**
- Каждые 10-20% прогресса - новая запись
- Расход топлива/энергии в реальном времени
- Случайные события вставляются в журнал

---

### 2.3 EventNote (Событийная записка)
**Замена:** CinematicEventScene
**Когда:** Случайное событие (встреча NPC, дорожная ситуация, конфликт)

**Формат:**
```
┌────────────────────────────────────────┐
│ ⚡ СОБЫТИЕ: Сломанная машина           │
├────────────────────────────────────────┤
│ На обочине стоит старая "Волга" с     │
│ открытым капотом. Пожилой мужчина в   │
│ замасленной спецовке машет рукой.     │
│                                        │
│ - Помочь? - кричит он.                │
│                                        │
│ ЧТО ДЕЛАТЬ?                            │
│ ▸ [1] Остановиться и помочь с ремонтом│
│       (-10 энергии, +20 отношений)    │
│   [2] Помочь, но не приглашать        │
│       (-5 энергии, +10 отношений)     │
│   [3] Проехать мимо                   │
│       (без изменений)                 │
│                                        │
│ [В команде: Бабушка Галина, Панк Вася]│
└────────────────────────────────────────┘
```

**Свойства EventNote:**
- `eventType` - ENCOUNTER, COMPANION, ROAD, RESOURCE
- `triggerCondition` - Условия появления
- `choices` - Варианты действий с последствиями
- `oneTimeOnly` - Флаг уникальности

---

### 2.4 LocationNote (Записка локации)
**Замена:** LocationScene (заправка, мотель, магазин, работа)
**Когда:** Вход в конкретное место в городе

**Формат:**
```
┌────────────────────────────────────────┐
│ 🏛️ ЗАПРАВКА "ЛУКОЙЛ"                  │
│ г. Москва, трасса М10                  │
├────────────────────────────────────────┤
│ Грязная площадка, две колонки.         │
│ Заправщик в спецовке подходит к окну.  │
│                                        │
│ - Сколько литров? - спрашивает он.    │
│                                        │
│ ЧТО ДЕЛАТЬ?                            │
│ ▸ [1] Полный бак (100L, -800₽)       │
│   [2] 50 литров (-400₽)              │
│   [3] 20 литров (-160₽)              │
│   [4] Передумал, уехать               │
│                                        │
│ [Текущий бак: 12/100L] [Деньги: 350₽] │
└────────────────────────────────────────┘
```

**Типы LocationNote:**
- **GasStationNote** - Заправка (покупка топлива)
- **MotelNote** - Мотель (отдых, восстановление энергии)
- **WorkNote** - Работа (заработок денег)
- **ShopNote** - Магазин (покупка предметов)
- **GarageNote** - Гараж (ремонт машины)

---

## 3. АРХИТЕКТУРА JOURNALSCENE

### 3.1 Класс JournalScene (главный контроллер)

```cpp
class JournalScene : public Scene {
public:
    JournalScene(PlayerState* playerState);

    // Показать записку определенного типа
    void showCityNote(const std::string& cityId);
    void showTravelNote(const RoadData& roadData);
    void showEventNote(GameEvent* event);
    void showLocationNote(LocationType type, const std::string& cityId);

    // Scene interface
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    SceneType getNextScene() const override;
    bool isFinished() const override;

private:
    // === СОСТОЯНИЕ ===
    PlayerState* m_playerState;
    bool m_isFinished;
    SceneType m_nextScene;

    // === СИСТЕМА ЗАПИСОК ===
    std::unique_ptr<Note> m_currentNote;  // Текущая отображаемая записка
    std::vector<std::unique_ptr<Note>> m_noteHistory;  // История записок

    // === ВИЗУАЛИЗАЦИЯ ===
    sf::Font m_font;
    NoteRenderer m_renderer;  // Отдельный класс для рендеринга записок

    // === СИСТЕМЫ ===
    EventManager m_eventManager;
    NPCManager& m_npcManager;

    // === АНИМАЦИЯ ===
    TextTypewriterEffect m_typewriter;  // Эффект печатающей машинки
    float m_fadeInAlpha;
};
```

---

### 3.2 Иерархия классов Note

```cpp
// Базовый класс записки
class Note {
public:
    virtual ~Note() = default;

    // Основная информация
    virtual std::string getTitle() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::vector<NoteChoice> getChoices() const = 0;

    // Обработка выбора
    virtual void executeChoice(int choiceIndex, PlayerState* playerState) = 0;

    // Рендеринг
    virtual void render(sf::RenderWindow& window, const sf::Font& font) = 0;

    // Обновление (для TravelNote с прогрессом)
    virtual void update(float deltaTime) {}

    // Тип записки
    enum class Type { CITY, TRAVEL, EVENT, LOCATION };
    virtual Type getType() const = 0;

protected:
    std::string m_id;
    std::string m_title;
    std::string m_description;
    std::vector<NoteChoice> m_choices;
};

// Выбор в записке
struct NoteChoice {
    std::string text;                          // Текст выбора
    std::string outcomeText;                   // Текст результата
    std::function<void(PlayerState*)> action;  // Действие при выборе

    // Условия отображения (опционально)
    std::function<bool(PlayerState*)> condition = nullptr;

    // Визуальные подсказки
    bool isDisabled = false;
    std::string disabledReason = "";  // "Недостаточно денег"
};
```

---

### 3.3 Конкретные типы записок

#### CityNote - Город
```cpp
class CityNote : public Note {
public:
    CityNote(const std::string& cityId, PlayerState* playerState);

    Type getType() const override { return Type::CITY; }

    std::string getTitle() const override {
        return "📍 " + getCityName(m_cityId);
    }

    std::string getDescription() const override {
        return generateCityDescription();
    }

    std::vector<NoteChoice> getChoices() const override {
        std::vector<NoteChoice> choices;

        // Заправка (если есть деньги)
        if (m_playerState->getMoney() >= 500) {
            choices.push_back({
                "Заправка (500₽)",
                "Заправили бак. Можно ехать дальше.",
                [](PlayerState* ps) {
                    ps->modifyMoney(-500);
                    ps->setFuel(100.0f);
                }
            });
        } else {
            choices.push_back({
                "Заправка (недостаточно денег)",
                "",
                nullptr,
                nullptr,
                true,
                "Нужно 500₽"
            });
        }

        // Работа
        choices.push_back({
            "Найти работу",
            "Вы устроились грузчиком...",
            [this](PlayerState* ps) {
                m_nextAction = NoteAction::SHOW_WORK;
            }
        });

        // Продолжить путь (если есть топливо)
        if (m_playerState->getFuel() > 20.0f) {
            choices.push_back({
                "Отправиться в путь",
                "",
                [this](PlayerState* ps) {
                    m_nextAction = NoteAction::SHOW_TRAVEL;
                }
            });
        }

        return choices;
    }

    void executeChoice(int choiceIndex, PlayerState* playerState) override {
        const auto& choice = m_choices[choiceIndex];
        if (choice.action) {
            choice.action(playerState);
        }
    }

private:
    std::string m_cityId;
    PlayerState* m_playerState;

    std::string generateCityDescription() const {
        // Генерировать описание на основе города, времени суток, погоды
        std::string desc = "Вы въезжаете в " + getCityName(m_cityId) + ". ";

        // Добавить детали на основе времени
        int hour = m_playerState->getGameTime().hour;
        if (hour < 6) {
            desc += "Город спит. Улицы пусты, только редкие фонари горят.";
        } else if (hour < 12) {
            desc += "Утро. Город просыпается. Пробки уже начинаются.";
        } else if (hour < 18) {
            desc += "День в разгаре. Шум, движение, толпы людей.";
        } else {
            desc += "Вечер. Зажигаются огни, город готовится ко сну.";
        }

        // Добавить состояние игрока
        if (m_playerState->getFuel() < 20.0f) {
            desc += "\n\nБензобак почти пуст.";
        }
        if (m_playerState->getEnergy() < 30.0f) {
            desc += " Вы устали.";
        }
        if (m_playerState->getMoney() < 200) {
            desc += " В кармане почти ничего нет.";
        }

        return desc;
    }
};
```

#### TravelNote - Путешествие
```cpp
class TravelNote : public Note {
public:
    TravelNote(const RoadData& roadData, PlayerState* playerState);

    Type getType() const override { return Type::TRAVEL; }

    void update(float deltaTime) override {
        // Обновить прогресс
        m_progress += (m_travelSpeed * deltaTime) / m_roadData.distance;

        // Расходовать ресурсы
        consumeResources(deltaTime);

        // Проверить случайные события
        m_timeSinceLastEventCheck += deltaTime;
        if (m_timeSinceLastEventCheck >= 10.0f) {
            tryTriggerEvent();
            m_timeSinceLastEventCheck = 0.0f;
        }

        // Добавить автоматические записи журнала
        checkForAutoEntries();
    }

    std::string getDescription() const override {
        std::stringstream ss;

        // Заголовок с прогрессом
        ss << "Путешествие: " << getCityName(m_roadData.fromNodeId)
           << " → " << getCityName(m_roadData.toNodeId) << "\n";
        ss << "Прогресс: " << (int)(m_progress * 100) << "% ";
        ss << "(" << (int)(m_progress * m_roadData.distance) << "/"
           << (int)m_roadData.distance << " км)\n\n";

        // Записи журнала (последние 5)
        int startIdx = std::max(0, (int)m_journalEntries.size() - 5);
        for (int i = startIdx; i < m_journalEntries.size(); i++) {
            ss << m_journalEntries[i].time << " - "
               << m_journalEntries[i].text << "\n\n";
        }

        return ss.str();
    }

    std::vector<NoteChoice> getChoices() const override {
        // Если есть активное событие, показать выборы события
        if (m_activeEvent) {
            return convertEventChoices(m_activeEvent);
        }

        // Иначе только "Продолжить"
        return {
            {
                "Продолжить путь",
                "",
                [](PlayerState* ps) { /* Продолжить автоскролл */ }
            }
        };
    }

private:
    RoadData m_roadData;
    PlayerState* m_playerState;
    float m_progress;  // 0.0 - 1.0
    float m_travelSpeed;  // км/сек

    struct JournalEntry {
        std::string time;   // "10:15"
        std::string text;   // "Выехали из Москвы..."
        float progress;     // На каком прогрессе это произошло
    };

    std::vector<JournalEntry> m_journalEntries;
    GameEvent* m_activeEvent;
    float m_timeSinceLastEventCheck;

    void checkForAutoEntries() {
        // Добавить автоматические записи каждые 25% прогресса
        if (m_progress > 0.25f && !hasEntryAtProgress(0.25f)) {
            addJournalEntry(generateRandomDescription());
        }
        if (m_progress > 0.50f && !hasEntryAtProgress(0.50f)) {
            addJournalEntry("Половина пути пройдена.");
        }
        if (m_progress > 0.75f && !hasEntryAtProgress(0.75f)) {
            addJournalEntry("Впереди уже виднеется пункт назначения.");
        }
    }

    void tryTriggerEvent() {
        GameEvent* event = m_eventManager.getRandomEvent(
            m_playerState->getFuel(),
            m_playerState->getEnergy(),
            static_cast<int>(m_playerState->getMoney()),
            m_roadData.toNodeId,
            m_roadData.roadType
        );

        if (event) {
            m_activeEvent = event;
            addJournalEntry("⚡ СОБЫТИЕ: " + event->title);
        }
    }
};
```

#### EventNote - Событие
```cpp
class EventNote : public Note {
public:
    EventNote(GameEvent* event, PlayerState* playerState)
        : m_event(event), m_playerState(playerState) {}

    Type getType() const override { return Type::EVENT; }

    std::string getTitle() const override {
        return "⚡ СОБЫТИЕ: " + m_event->title;
    }

    std::string getDescription() const override {
        return m_event->description;
    }

    std::vector<NoteChoice> getChoices() const override {
        std::vector<NoteChoice> choices;

        for (const auto& eventChoice : m_event->choices) {
            choices.push_back({
                eventChoice.text,
                eventChoice.outcomeText,
                [this, eventChoice](PlayerState* ps) {
                    applyEventChoice(eventChoice, ps);
                }
            });
        }

        return choices;
    }

private:
    GameEvent* m_event;
    PlayerState* m_playerState;

    void applyEventChoice(const EventChoice& choice, PlayerState* ps) {
        // Применить изменения ресурсов
        ps->modifyFuel(choice.fuelChange);
        ps->modifyEnergy(choice.energyChange);
        ps->modifyMoney(choice.moneyChange);

        // Применить изменения отношений, рекрутинг и т.д.
        EventManager::getInstance().applyChoice(choice,
            ps->getFuel(), ps->getEnergy(), ps->getMoney());
    }
};
```

---

## 4. СИСТЕМА NOTE (БАЗОВАЯ ЗАПИСКА)

### 4.1 Структура данных

```cpp
// Базовый класс Note - определен выше в разделе 3.2

// Вспомогательная структура для визуализации
struct NoteVisuals {
    sf::Color backgroundColor = sf::Color(245, 235, 220);  // Старая бумага
    sf::Color textColor = sf::Color(50, 40, 30);           // Темно-коричневый
    sf::Color accentColor = sf::Color(180, 50, 50);        // Красный для выборов

    float padding = 40.0f;
    float lineSpacing = 1.3f;
    unsigned int fontSize = 18;
    unsigned int titleFontSize = 24;
};
```

### 4.2 NoteRenderer - Рендеринг записок

```cpp
class NoteRenderer {
public:
    NoteRenderer(const sf::Font& font);

    void render(const Note& note, sf::RenderWindow& window,
                int selectedChoiceIndex = -1);

    void setTypewriterProgress(float progress) { m_typewriterProgress = progress; }
    void setFadeAlpha(uint8_t alpha) { m_fadeAlpha = alpha; }

private:
    const sf::Font& m_font;
    NoteVisuals m_visuals;
    float m_typewriterProgress;  // 0.0 - 1.0
    uint8_t m_fadeAlpha;         // 0 - 255

    void renderBackground(sf::RenderWindow& window);
    void renderTitle(const std::string& title, sf::RenderWindow& window);
    void renderDescription(const std::string& desc, sf::RenderWindow& window);
    void renderChoices(const std::vector<NoteChoice>& choices,
                      sf::RenderWindow& window, int selectedIndex);
    void renderResources(PlayerState* ps, sf::RenderWindow& window);

    // Эффект печатающей машинки
    std::string applyTypewriterEffect(const std::string& fullText) const {
        int visibleChars = (int)(fullText.length() * m_typewriterProgress);
        return fullText.substr(0, visibleChars);
    }
};
```

### 4.3 TextTypewriterEffect - Эффект печатающей машинки

```cpp
class TextTypewriterEffect {
public:
    TextTypewriterEffect(float charsPerSecond = 50.0f)
        : m_speed(charsPerSecond), m_progress(0.0f) {}

    void start(const std::string& fullText) {
        m_fullText = fullText;
        m_progress = 0.0f;
    }

    void update(float deltaTime) {
        if (m_progress < 1.0f) {
            m_progress += (m_speed * deltaTime) / m_fullText.length();
            m_progress = std::min(1.0f, m_progress);
        }
    }

    std::string getCurrentText() const {
        int visibleChars = (int)(m_fullText.length() * m_progress);
        return m_fullText.substr(0, visibleChars);
    }

    bool isComplete() const { return m_progress >= 1.0f; }

    void skipToEnd() { m_progress = 1.0f; }

private:
    std::string m_fullText;
    float m_progress;  // 0.0 - 1.0
    float m_speed;     // Символов в секунду
};
```

---

## 5. ИНТЕГРАЦИЯ NODESCENE → CITYNOTE

### 5.1 Что убираем из NodeScene

**Удаляется полностью:**
```cpp
// ❌ Визуальная карта города
std::vector<sf::CircleShape> m_locationCircles;
std::vector<sf::Text> m_locationLabels;

// ❌ Графические кнопки
std::vector<sf::RectangleShape> m_locationButtons;

// ❌ Сложная навигация мышью
void handleMouseHover(const sf::Vector2f& mousePos);
void handleMouseClick(const sf::Vector2f& mousePos);
```

### 5.2 Что сохраняем и переносим

**Сохраняется в CityNote:**
```cpp
// ✅ Логика сервисов
struct CityService {
    std::string id;           // "gas_station", "motel", "work"
    std::string name;         // "Заправка"
    bool isAvailable;         // Доступен ли сейчас
    int cost;                 // Стоимость (-1 если бесплатно)
};

// ✅ Состояние города
struct CityState {
    std::string id;
    std::vector<CityService> services;
    std::string description;  // Текстовое описание
};
```

### 5.3 Пример миграции: NodeScene::handleGasStation() → CityNote choice

**До (NodeScene.cpp):**
```cpp
void NodeScene::handleGasStation() {
    if (m_playerState->getMoney() >= 500) {
        m_playerState->modifyMoney(-500);
        m_playerState->setFuel(100.0f);
        showMessage("Заправили бак!");
    } else {
        showMessage("Недостаточно денег!");
    }
}
```

**После (CityNote.cpp):**
```cpp
NoteChoice gasStationChoice = {
    .text = "Заправка (500₽)",
    .outcomeText = "Заправили бак! Машина готова к дороге.",
    .action = [](PlayerState* ps) {
        ps->modifyMoney(-500);
        ps->setFuel(100.0f);
    },
    .condition = [](PlayerState* ps) {
        return ps->getMoney() >= 500;
    },
    .isDisabled = (m_playerState->getMoney() < 500),
    .disabledReason = "Недостаточно денег (нужно 500₽)"
};
```

---

## 6. ИНТЕГРАЦИЯ TRAVELCINEMATICSCENE → TRAVELNOTE

### 6.1 Что убираем

**Удаляется полностью:**
```cpp
// ❌ Вся 7-слойная параллакс система
sf::RectangleShape m_sky;
sf::RectangleShape m_layer1_clouds;
sf::RectangleShape m_layer2_mountains;
sf::RectangleShape m_layer3_hills;
sf::RectangleShape m_layer4_trees;
sf::RectangleShape m_layer5_fence;
sf::RectangleShape m_road;

// ❌ Анимация машины
sf::RectangleShape m_car;
float m_carBobTime;
float m_carBobAmount;

// ❌ Встречные машины
std::vector<sf::RectangleShape> m_oncomingCars;

// ❌ Дорожная разметка
std::vector<sf::RectangleShape> m_roadLines;

// ❌ HUD панель Keep Driving
HUDPanel m_hudPanel;
sf::RectangleShape m_hudBackground;
// ... и т.д.
```

### 6.2 Что сохраняем

**Сохраняется логика:**
```cpp
// ✅ Система прогресса
float m_progress;              // 0.0 - 1.0
float m_travelSpeed;           // км/сек
float m_distanceTraveled;      // км

// ✅ Расход ресурсов
void consumeResources(float deltaTime);
float calculateFuelConsumption() const;

// ✅ Система событий
EventManager m_eventManager;
void tryTriggerEvent();

// ✅ Система диалогов компаньонов
std::vector<TravelDialogue> m_availableDialogues;
void tryTriggerDialogue();
```

### 6.3 Преобразование в текстовый журнал

**До (TravelCinematicScene.cpp:575-630):**
```cpp
void TravelCinematicScene::update(float deltaTime) {
    // Обновить параллакс-слои
    m_layer1Offset += m_travelSpeed * 0.05f * deltaTime;
    m_layer2Offset += m_travelSpeed * 0.1f * deltaTime;
    // ... и т.д.

    // Анимация машины
    m_carBobTime += deltaTime * 5.0f;
    m_carBobAmount = std::sin(m_carBobTime) * 3.0f;
    m_car.setPosition(400, 450 + m_carBobAmount);

    // ... сложная графика
}
```

**После (TravelNote.cpp):**
```cpp
void TravelNote::update(float deltaTime) {
    // Обновить прогресс
    m_progress += (m_travelSpeed * deltaTime) / m_roadData.distance;

    // Расходовать ресурсы
    consumeResources(deltaTime);

    // Добавить автоматические записи журнала
    if (shouldAddEntry()) {
        addJournalEntry(generateTravelDescription());
    }

    // Проверить события
    if (shouldCheckEvent()) {
        tryTriggerEvent();
    }
}
```

---

## 7. ВИЗУАЛЬНЫЙ ДИЗАЙН

### 7.1 Единый визуальный стиль

**Цветовая палитра:**
```cpp
namespace JournalTheme {
    const sf::Color PAPER_BG(245, 235, 220);        // Старая бумага
    const sf::Color TEXT_DARK(50, 40, 30);          // Темно-коричневый текст
    const sf::Color TEXT_LIGHT(120, 110, 100);      // Светло-коричневый (детали)
    const sf::Color ACCENT_RED(180, 50, 50);        // Красный (выбор)
    const sf::Color ACCENT_BLUE(50, 100, 150);      // Синий (информация)
    const sf::Color HIGHLIGHT(255, 240, 200, 100);  // Подсветка выбора
}
```

**Шрифты:**
```cpp
// Основной шрифт: Liberation Mono (моноширинный, как печатная машинка)
// Заголовки: Liberation Sans Bold
// Размеры:
// - Заголовок: 28px
// - Основной текст: 18px
// - Детали/метаданные: 14px
```

### 7.2 Макет экрана

```
┌─────────────────────────────────────────────────────┐
│                  [ЭКРАН 1300x800]                   │
├─────────────────────────────────────────────────────┤
│                                                     │
│  ┌─────────────────────────────────────────────┐   │
│  │ ЗАГОЛОВОК ЗАПИСКИ (28px, bold)              │   │
│  │ Метаданные (14px, светлый)                  │   │
│  ├─────────────────────────────────────────────┤   │
│  │                                             │   │
│  │  Основной текст описания (18px)            │   │
│  │  Межстрочный интервал: 1.3                  │   │
│  │                                             │   │
│  │  Текст может быть многострочным и           │   │
│  │  занимать центральную часть экрана.         │   │
│  │                                             │   │
│  ├─────────────────────────────────────────────┤   │
│  │  ВАШ ВЫБОР:                                 │   │
│  │  ▸ [1] Первый вариант                      │   │ ← Подсветка
│  │    [2] Второй вариант                       │   │
│  │    [3] Третий вариант (недоступно)          │   │
│  │                                             │   │
│  ├─────────────────────────────────────────────┤   │
│  │  [⛽45L] [⚡60] [₽1200] [Команда: 2/3]      │   │
│  └─────────────────────────────────────────────┘   │
│                                                     │
└─────────────────────────────────────────────────────┘
```

### 7.3 Анимации и эффекты

**1. Эффект печатающей машинки:**
- Текст появляется посимвольно со скоростью 50 символов/сек
- Можно пропустить анимацию кликом
- Звук печатающей машинки (опционально)

**2. Fade-in/Fade-out:**
- При смене записки - плавное затухание/появление (0.3 сек)
- Alpha канал от 0 до 255

**3. Подсветка выбора:**
- Плавное перемещение подсветки при смене выбора
- Легкий glow-эффект

---

## 8. ПЛАН МИГРАЦИИ

### Этап 1: Создание базовой структуры (3 дня)

**Задачи:**
1. Создать `Note.h` и `Note.cpp` - базовый класс
2. Создать `NoteRenderer.h` и `NoteRenderer.cpp`
3. Создать `JournalScene.h` и `JournalScene.cpp` - главный контроллер
4. Создать `TextTypewriterEffect.h` и `TextTypewriterEffect.cpp`
5. Тесты: Создать простейшую записку и отобразить

**Файлы:**
```
include/Journal/Note.h
include/Journal/NoteRenderer.h
include/Journal/JournalScene.h
include/Journal/TextTypewriterEffect.h

src/Journal/Note.cpp
src/Journal/NoteRenderer.cpp
src/Journal/JournalScene.cpp
src/Journal/TextTypewriterEffect.cpp
```

---

### Этап 2: Реализация CityNote (2 дня)

**Задачи:**
1. Создать `CityNote.h` и `CityNote.cpp`
2. Портировать логику сервисов из NodeScene
3. Генерация динамических описаний города
4. Интеграция с PlayerState
5. Тесты: Прибытие в город, выбор сервисов

**Миграция данных:**
- Перенести структуры городов из `NodeScene`
- Перенести данные сервисов
- Адаптировать под текстовый формат

---

### Этап 3: Реализация TravelNote (4 дня)

**Задачи:**
1. Создать `TravelNote.h` и `TravelNote.cpp`
2. Портировать логику прогресса из TravelCinematicScene
3. Реализовать систему журнальных записей
4. Интеграция событий в журнал
5. Автоматические записи каждые 20-25% прогресса
6. Тесты: Полное путешествие Москва → СПб

**Что мигрировать:**
```cpp
// ИЗ TravelCinematicScene.cpp:
// ✅ Сохранить:
void updateTravelProgress(float deltaTime);
void consumeResources(float deltaTime);
float calculateFuelConsumption() const;
void tryTriggerEvent();
void checkArrival();

// ❌ Удалить:
void updateParallax(float deltaTime);
void updateCarAnimation(float deltaTime);
void renderLayers(sf::RenderWindow& window);
```

---

### Этап 4: Реализация EventNote (2 дня)

**Задачи:**
1. Создать `EventNote.h` и `EventNote.cpp`
2. Адаптировать существующие GameEvent в Note формат
3. Интеграция с EventManager
4. Тесты: Все 16 событий отображаются корректно

**Преобразование:**
- `GameEvent` → `EventNote`
- `EventChoice` → `NoteChoice`
- Применение последствий через NoteChoice::action

---

### Этап 5: Реализация LocationNote (3 дня)

**Задачи:**
1. Создать базовый `LocationNote` класс
2. Создать подклассы:
   - `GasStationNote` (заправка)
   - `MotelNote` (мотель)
   - `WorkNote` (работа)
   - `ShopNote` (магазин)
   - `GarageNote` (гараж)
3. Портировать логику из LocationScene
4. Тесты: Все типы локаций работают

---

### Этап 6: Интеграция и полировка (4 дня)

**Задачи:**
1. Интегрировать JournalScene в MainMenuScene
2. Заменить все старые сцены на JournalScene
3. Обновить SceneManager для работы с записками
4. Добавить анимации и звуки
5. Балансировка (частота событий, описания)
6. Полное тестирование игровой сессии

**Обновить CMakeLists.txt:**
```cmake
# Новые файлы журнальной системы
set(JOURNAL_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Journal/Note.cpp
    ${CMAKE_SOURCE_DIR}/src/Journal/NoteRenderer.cpp
    ${CMAKE_SOURCE_DIR}/src/Journal/JournalScene.cpp
    ${CMAKE_SOURCE_DIR}/src/Journal/CityNote.cpp
    ${CMAKE_SOURCE_DIR}/src/Journal/TravelNote.cpp
    ${CMAKE_SOURCE_DIR}/src/Journal/EventNote.cpp
    ${CMAKE_SOURCE_DIR}/src/Journal/LocationNote.cpp
    ${CMAKE_SOURCE_DIR}/src/Journal/TextTypewriterEffect.cpp
)

# Удалить старые сцены (опционально, можно оставить для совместимости)
# ${CMAKE_SOURCE_DIR}/src/NodeScene.cpp  # DEPRECATED
# ${CMAKE_SOURCE_DIR}/src/TravelCinematicScene.cpp  # DEPRECATED
```

---

## 9. ПРЕИМУЩЕСТВА ПОДХОДА

### 9.1 Технические преимущества

1. **Упрощение кодовой базы:**
   - Одна система вместо 5+ разных сцен
   - Единая логика рендеринга
   - Меньше дублирования кода

2. **Легкость добавления контента:**
   - Новое событие = новая EventNote (просто текст + выборы)
   - Новый город = новая CityNote (описание + сервисы)
   - Не нужно рисовать графику

3. **Производительность:**
   - Нет тяжелых параллакс-анимаций
   - Нет множества текстур
   - Только текст и простые фигуры

4. **Легкость тестирования:**
   - Можно тестировать события как текстовые данные
   - Не нужна графика для unit-тестов
   - Проще автоматизировать

### 9.2 Игровые преимущества

1. **Фокус на истории:**
   - Текст позволяет передавать детали и атмосферу
   - Больше пространства для описаний
   - Игрок использует воображение

2. **Гибкость выборов:**
   - Можно предлагать 5-7 вариантов вместо 2-3
   - Условные выборы (показываются только при условиях)
   - Подсказки о последствиях

3. **Соответствие жанру:**
   - Текстовые игры (60 Seconds, Reigns, Sunless Sea)
   - Атмосфера дневника путешествия
   - Ретро-эстетика

4. **Легкость локализации:**
   - Вся игра - это текст
   - Легко переводить на другие языки
   - Не нужно адаптировать UI

### 9.3 Экономические преимущества

1. **Не нужен художник:**
   - Только текст и простые формы
   - Можно использовать бесплатные шрифты
   - Минимальные графические требования

2. **Быстрая разработка:**
   - Добавить новое событие = 10-15 минут
   - Изменить баланс = редактирование текста
   - Не нужно перерисовывать графику

3. **Малый размер игры:**
   - Нет тяжелых текстур
   - Нет больших спрайтов
   - Игра может быть <50MB

---

## 10. ПРИМЕРЫ ИГРОВЫХ СЕССИЙ

### Пример 1: Прибытие в город

```
┌────────────────────────────────────────┐
│ 📍 МОСКВА                              │
│ День 3, 15:45                          │
│ Погода: Облачно, моросит дождь         │
├────────────────────────────────────────┤
│ Вы въезжаете в столицу. Серое небо,   │
│ мокрый асфальт. Дворники скрипят по    │
│ стеклу. Пробки начинаются уже на       │
│ въезде — сплошной поток машин.         │
│                                        │
│ В салоне духота. Механик Михалыч       │
│ дремлет на заднем сиденье. Бензобак    │
│ почти пуст — еще километров на 50.     │
│ В кармане - всего 350 рублей.          │
│                                        │
│ ЧТО ДЕЛАТЬ?                            │
│ ▸ [1] Заправка (500₽) ⚠️ НЕДОСТУПНО  │
│   [2] Найти работу                     │
│   [3] Мотель - отдохнуть (1000₽) ⚠️   │
│   [4] Магазин                          │
│   [5] Продолжить путь ⚠️ МАЛО ТОПЛИВА │
│                                        │
│ [⛽15/100L] [⚡45/100] [₽350]          │
│ [Команда: Механик Михалыч (1/3)]      │
└────────────────────────────────────────┘
```

### Пример 2: Путешествие с событием

```
┌────────────────────────────────────────┐
│ 🚗 ПУТЕШЕСТВИЕ: Москва → СПб          │
│ Прогресс: [=====>    ] 45% (54/120км) │
│ Трасса М10, асфальт, день              │
├────────────────────────────────────────┤
│ 10:15 - Выехали из Москвы. Трасса М10,│
│        дорога свободная. Михалыч       │
│        рассказывает байки про работу.  │
│                                        │
│ 11:30 - Проезжаем Тверь. Остановились │
│        заправиться. Цены выше, чем в   │
│        Москве, но выбора нет.          │
│                                        │
│ 12:00 - Впереди развилка. Навигатор   │
│        молчит (нет сигнала).           │
│                                        │
│ ⚡ СОБЫТИЕ: РАЗВИЛКА                   │
│                                        │
│ Два указателя:                         │
│ ← "СПб через Новгород (130 км)"       │
│ → "СПб объездная (95 км)"             │
│                                        │
│ Михалыч говорит: "Объездная быстрее,  │
│ но там дорога убитая. Через Новгород  │
│ дольше, зато асфальт хороший."         │
│                                        │
│ ЧТО ДЕЛАТЬ?                            │
│ ▸ [1] Объездная (быстрее, +расход)   │
│   [2] Через Новгород (дольше, надежно)│
│   [3] Спросить совета у Михалыча      │
│                                        │
│ [⛽65/100L] [⚡60/100] [₽350]          │
└────────────────────────────────────────┘
```

### Пример 3: Работа в городе

```
┌────────────────────────────────────────┐
│ 🏛️ РАБОТА: Разгрузка фур              │
│ г. Москва, промзона                    │
├────────────────────────────────────────┤
│ Подъезжаете к складу. Табличка:       │
│ "Требуются грузчики. 500₽/смена"      │
│                                        │
│ Вас встречает прораб:                  │
│ - Работа простая - ящики с фуры на    │
│   склад таскать. 4 часа работы.        │
│                                        │
│ Михалыч предлагает:                    │
│ - Я помогу. Вдвоем быстрее управимся.  │
│                                        │
│ ЧТО ДЕЛАТЬ?                            │
│ ▸ [1] Работать самому                 │
│       (+500₽, -20 энергии, 4 часа)    │
│   [2] Работать вдвоем с Михалычем     │
│       (+700₽, -15 энергии, 2 часа)    │
│       +10 отношений с Михалычем        │
│   [3] Передумать, уехать              │
│       (без изменений)                  │
│                                        │
│ [⛽15/100L] [⚡45/100] [₽350]          │
└────────────────────────────────────────┘
```

---

## 11. ТЕХНИЧЕСКИЕ ДЕТАЛИ РЕАЛИЗАЦИИ

### 11.1 Управление потоком записок

```cpp
class JournalScene {
private:
    std::stack<std::unique_ptr<Note>> m_noteStack;

public:
    void pushNote(std::unique_ptr<Note> note) {
        m_noteStack.push(std::move(note));
        m_currentNote = m_noteStack.top().get();
    }

    void popNote() {
        if (!m_noteStack.empty()) {
            m_noteStack.pop();
            m_currentNote = m_noteStack.empty() ? nullptr : m_noteStack.top().get();
        }
    }

    // Пример: переход от города к путешествию
    void transitionFromCityToTravel(const RoadData& roadData) {
        auto travelNote = std::make_unique<TravelNote>(roadData, m_playerState);
        pushNote(std::move(travelNote));
    }

    // Пример: событие прерывает путешествие
    void showEvent(GameEvent* event) {
        auto eventNote = std::make_unique<EventNote>(event, m_playerState);
        pushNote(std::move(eventNote));  // Поверх TravelNote
    }

    // После завершения события - вернуться к путешествию
    void finishEvent() {
        popNote();  // Убрать EventNote, вернуться к TravelNote
    }
};
```

### 11.2 Сохранение истории записок

```cpp
class JournalHistory {
public:
    void addEntry(const std::string& title, const std::string& description,
                  const std::string& choice, const std::string& outcome) {
        HistoryEntry entry;
        entry.timestamp = getCurrentGameTime();
        entry.title = title;
        entry.description = description;
        entry.choiceMade = choice;
        entry.outcome = outcome;

        m_history.push_back(entry);
    }

    // Показать историю путешествия
    std::vector<HistoryEntry> getHistory() const { return m_history; }

    // Сохранить в файл
    void saveToFile(const std::string& filename) const;

private:
    struct HistoryEntry {
        GameTime timestamp;
        std::string title;
        std::string description;
        std::string choiceMade;
        std::string outcome;
    };

    std::vector<HistoryEntry> m_history;
};
```

---

## 12. ЗАКЛЮЧЕНИЕ

### Итоговая структура проекта

```
include/
  Journal/
    Note.h                      ← Базовый класс
    NoteRenderer.h              ← Рендеринг
    JournalScene.h              ← Главный контроллер
    CityNote.h                  ← Город
    TravelNote.h                ← Путешествие
    EventNote.h                 ← События
    LocationNote.h              ← Локации (заправка, мотель и т.д.)
    TextTypewriterEffect.h      ← Эффект печатания
    JournalHistory.h            ← История путешествия

src/
  Journal/
    Note.cpp
    NoteRenderer.cpp
    JournalScene.cpp
    CityNote.cpp
    TravelNote.cpp
    EventNote.cpp
    LocationNote.cpp
    TextTypewriterEffect.cpp
    JournalHistory.cpp
```

### Оценка объема работ

| Этап | Задача | Дни | Строк кода |
|------|--------|-----|-----------|
| 1 | Базовая структура | 3 | ~800 |
| 2 | CityNote | 2 | ~400 |
| 3 | TravelNote | 4 | ~600 |
| 4 | EventNote | 2 | ~300 |
| 5 | LocationNote | 3 | ~500 |
| 6 | Интеграция | 4 | ~400 |
| **ИТОГО** | | **18 дней** | **~3000 строк** |

### Что получаем

✅ **Единый визуальный стиль** для всей игры
✅ **Простота разработки** (не нужна сложная графика)
✅ **Фокус на истории** (детальные описания, выборы)
✅ **Легкость балансировки** (все в текстовом формате)
✅ **Малый размер игры** (нет тяжелых текстур)
✅ **Быстрое прототипирование** (новое событие = 10 минут)
✅ **Соответствие жанру** (текстовая survival-игра)

---

**Статус:** 🎯 ГОТОВ К РЕАЛИЗАЦИИ
**Приоритет:** 🔴 ВЫСОКИЙ
**Следующий шаг:** Создание базовых классов Note + JournalScene

**Авторы:** Development Team
**Дата последнего обновления:** 2025-12-06
