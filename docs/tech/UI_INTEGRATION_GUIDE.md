# UI Component Architecture Integration Guide
# Руководство по интеграции архитектуры UI-компонентов

**Version:** 1.0  
**Date:** 2025-11-12  
**Status:** Implementation Complete

---

## Table of Contents / Содержание

1. [Overview / Обзор](#overview)
2. [Architecture Principles / Архитектурные принципы](#architecture-principles)
3. [Component Hierarchy / Иерархия компонентов](#component-hierarchy)
4. [Integration with EventBus / Интеграция с EventBus](#integration-with-eventbus)
5. [Usage Examples / Примеры использования](#usage-examples)
6. [Migration Guide / Руководство по миграции](#migration-guide)
7. [Testing Strategy / Стратегия тестирования](#testing-strategy)

---

## Overview / Обзор

### English

The UI Component Architecture provides a unified, extensible framework for building consistent user interfaces across all game scenes. The architecture follows SOLID principles and implements the Composite pattern for component hierarchies.

**Key Features:**
- **Unified Interface**: All UI elements inherit from `UIComponent` base class
- **Event-Driven**: Integrates with EventBus for inter-component communication
- **Reusable Components**: Panel, Label, Button, ProgressBar, Image
- **Hierarchical**: Support for nested components (parent-child relationships)
- **Customizable**: Easy to extend with new component types
- **Thread-Safe**: EventBus provides thread-safe event handling

### Русский

Архитектура UI-компонентов предоставляет единый, расширяемый фреймворк для создания согласованных пользовательских интерфейсов во всех игровых сценах. Архитектура следует принципам SOLID и реализует паттерн Composite для иерархий компонентов.

**Ключевые особенности:**
- **Единый интерфейс**: Все UI-элементы наследуются от базового класса `UIComponent`
- **Событийная модель**: Интеграция с EventBus для межкомпонентного взаимодействия
- **Переиспользуемые компоненты**: Panel, Label, Button, ProgressBar, Image
- **Иерархическая структура**: Поддержка вложенных компонентов (отношения родитель-потомок)
- **Настраиваемость**: Легко расширяется новыми типами компонентов
- **Потокобезопасность**: EventBus обеспечивает потокобезопасную обработку событий

---

## Architecture Principles / Архитектурные принципы

### SOLID Principles Application / Применение принципов SOLID

**1. Single Responsibility Principle (SRP)**
- Each component has one clear purpose
- `UIButton` handles button logic and rendering
- `UIPanel` handles container logic
- Separation of concerns maintained

**2. Open/Closed Principle (OCP)**
- Components are open for extension (inheritance)
- Closed for modification (base class stable)
- Easy to add new component types without changing existing code

**3. Liskov Substitution Principle (LSP)**
- All components can be used wherever `UIComponent*` is expected
- Derived classes honor the base class contract
- Polymorphic behavior works correctly

**4. Interface Segregation Principle (ISP)**
- Minimal required interface in base class
- Components only implement methods they need
- No forced dependencies on unused functionality

**5. Dependency Inversion Principle (DIP)**
- High-level modules depend on abstractions (UIComponent)
- Low-level modules (concrete components) also depend on abstractions
- Easy to swap implementations

---

## Component Hierarchy / Иерархия компонентов

### Class Diagram / Диаграмма классов

```
                    ┌───────────────┐
                    │  UIComponent  │  (Abstract Base)
                    └───────┬───────┘
                            │
         ┌──────────────────┼──────────────────┐
         │                  │                  │
    ┌────▼────┐      ┌─────▼──────┐    ┌─────▼──────┐
    │ UIPanel │      │  UIButton  │    │  UILabel   │
    └─────────┘      └────────────┘    └────────────┘
         │                                     
    ┌────▼───────────┐              ┌─────────────────┐
    │ UIProgressBar  │              │    UIImage      │
    └────────────────┘              └─────────────────┘
```

### Component Relationships / Связи компонентов

**Parent-Child Relationships:**
```
UIPanel (root)
  ├─ UILabel (title)
  ├─ UIButton (action1)
  ├─ UIButton (action2)
  └─ UIProgressBar (status)
```

**Event Flow:**
```
User Input → Root Component → Children (top to bottom)
         ↓
    Event Handled? → Yes: Stop propagation
         ↓
        No: Continue to next component
```

---

## Integration with EventBus / Интеграция с EventBus

### Event-Driven UI Updates / Событийное обновление UI

**Example: Resource Change Updates UI**

```cpp
// In ResourceManager
void ResourceManager::modifyFuel(float delta) {
    float oldValue = m_fuel;
    m_fuel = std::max(0.f, std::min(m_fuel + delta, m_maxFuel));
    
    // Publish event
    ResourceChangeEvent event{
        ResourceChangeEvent::ResourceType::FUEL,
        oldValue,
        m_fuel,
        delta
    };
    EventBus::getInstance().publish(EVENT_RESOURCE_CHANGED, event);
}

// In HUD Panel or UI Component
void HUDPanel::setupEventListeners() {
    m_fuelListenerId = EventBus::getInstance().subscribe(EVENT_RESOURCE_CHANGED,
        [this](const std::any& data) {
            auto event = std::any_cast<ResourceChangeEvent>(data);
            if (event.type == ResourceChangeEvent::ResourceType::FUEL) {
                // Update fuel progress bar
                m_fuelBar->setValue(event.newValue);
            }
        });
}

// Cleanup in destructor
HUDPanel::~HUDPanel() {
    EventBus::getInstance().unsubscribe(EVENT_RESOURCE_CHANGED, m_fuelListenerId);
}
```

### Inter-Scene Communication / Межсценовое взаимодействие

**Example: Scene Transition with Data Transfer**

```cpp
// From WorldMapScene when starting journey
void WorldMapScene::startJourney() {
    SceneTransitionEvent event;
    event.fromScene = "WorldMap";
    event.toScene = "CinematicTravel";
    event.transferData["fromNode"] = m_currentNode;
    event.transferData["toNode"] = m_selectedNode;
    event.transferData["distance"] = calculateDistance();
    
    EventBus::getInstance().publish(EVENT_SCENE_TRANSITION, event);
    m_finished = true;
    m_nextScene = SceneType::CINEMATIC_EVENT;
}

// In CinematicEventScene constructor
CinematicEventScene::CinematicEventScene() {
    // Subscribe to scene transition events
    EventBus::getInstance().subscribe(EVENT_SCENE_TRANSITION,
        [this](const std::any& data) {
            auto event = std::any_cast<SceneTransitionEvent>(data);
            if (event.toScene == "CinematicTravel") {
                // Extract transfer data
                m_fromNode = std::any_cast<int>(event.transferData["fromNode"]);
                m_toNode = std::any_cast<int>(event.transferData["toNode"]);
                m_distance = std::any_cast<float>(event.transferData["distance"]);
            }
        });
}
```

### UI to System Communication / Связь UI с системами

**Example: Button Click Triggers Inventory Action**

```cpp
// In InventoryScene
void InventoryScene::setupUI() {
    auto sortButton = std::make_shared<UIButton>("Sort", sf::Vector2f(10, 10), 
                                                  sf::Vector2f(80, 30));
    sortButton->setCallback([this]() {
        // Sort inventory and publish event
        m_inventorySystem->sortItems();
        
        InventoryChangeEvent event{
            InventoryChangeEvent::ChangeType::ITEM_MOVED,
            "",  // No specific item
            0,
            -1, -1
        };
        EventBus::getInstance().publish(EVENT_INVENTORY_CHANGED, event);
    });
    
    m_rootPanel->addChild(sortButton);
}
```

---

## Usage Examples / Примеры использования

### Example 1: Creating a Simple Dialog / Создание простого диалога

```cpp
// Create dialog panel
auto dialogPanel = std::make_shared<UIPanel>(
    sf::Vector2f(300, 200),  // Position (centered)
    sf::Vector2f(600, 400),  // Size
    sf::Color(40, 40, 50)    // Background color
);

// Add title label
auto titleLabel = std::make_shared<UILabel>(
    "Confirm Action",
    sf::Vector2f(320, 220),
    24  // Font size
);
titleLabel->setTextColor(sf::Color::Yellow);
titleLabel->setStyle(sf::Text::Bold);
dialogPanel->addChild(titleLabel);

// Add message label
auto messageLabel = std::make_shared<UILabel>(
    "Are you sure you want to proceed?",
    sf::Vector2f(320, 280),
    16
);
dialogPanel->addChild(messageLabel);

// Add confirm button
auto confirmButton = std::make_shared<UIButton>(
    "Confirm",
    sf::Vector2f(350, 450),
    sf::Vector2f(120, 40)
);
confirmButton->setCallback([this]() {
    onConfirm();
});
dialogPanel->addChild(confirmButton);

// Add cancel button
auto cancelButton = std::make_shared<UIButton>(
    "Cancel",
    sf::Vector2f(490, 450),
    sf::Vector2f(120, 40)
);
cancelButton->setCallback([this]() {
    onCancel();
});
dialogPanel->addChild(cancelButton);
```

### Example 2: Creating HUD Panel / Создание панели HUD

```cpp
class GameHUD {
public:
    void setupHUD() {
        // Create root panel
        m_hudPanel = std::make_shared<UIPanel>(
            sf::Vector2f(0, 0),
            sf::Vector2f(1366, 80),
            sf::Color(20, 20, 30, 200)  // Semi-transparent
        );
        
        // Energy bar
        m_energyBar = std::make_shared<UIProgressBar>(
            sf::Vector2f(20, 20),
            sf::Vector2f(200, 20),
            0.f, 100.f
        );
        m_energyBar->setBarColor(sf::Color(50, 200, 50));
        m_hudPanel->addChild(m_energyBar);
        
        // Money label
        m_moneyLabel = std::make_shared<UILabel>(
            "$200",
            sf::Vector2f(250, 20),
            18
        );
        m_moneyLabel->setTextColor(sf::Color::Yellow);
        m_hudPanel->addChild(m_moneyLabel);
        
        // Fuel bar
        m_fuelBar = std::make_shared<UIProgressBar>(
            sf::Vector2f(1100, 20),
            sf::Vector2f(200, 20),
            0.f, 100.f
        );
        m_fuelBar->setBarColor(sf::Color(200, 150, 50));
        m_hudPanel->addChild(m_fuelBar);
        
        // Subscribe to events
        setupEventListeners();
    }
    
    void setupEventListeners() {
        EventBus::getInstance().subscribe(EVENT_RESOURCE_CHANGED,
            [this](const std::any& data) {
                auto event = std::any_cast<ResourceChangeEvent>(data);
                updateResource(event);
            });
    }
    
    void updateResource(const ResourceChangeEvent& event) {
        switch (event.type) {
            case ResourceChangeEvent::ResourceType::ENERGY:
                m_energyBar->setValue(event.newValue);
                break;
            case ResourceChangeEvent::ResourceType::MONEY:
                m_moneyLabel->setText("$" + std::to_string(static_cast<int>(event.newValue)));
                break;
            case ResourceChangeEvent::ResourceType::FUEL:
                m_fuelBar->setValue(event.newValue);
                break;
        }
    }
    
    void render(sf::RenderWindow& window) {
        m_hudPanel->render(window);
    }
    
private:
    std::shared_ptr<UIPanel> m_hudPanel;
    std::shared_ptr<UIProgressBar> m_energyBar;
    std::shared_ptr<UIProgressBar> m_fuelBar;
    std::shared_ptr<UILabel> m_moneyLabel;
};
```

### Example 3: Event-Driven Menu / Событийное меню

```cpp
class PauseMenu {
public:
    void setupMenu() {
        m_menuPanel = std::make_shared<UIPanel>(
            sf::Vector2f(400, 200),
            sf::Vector2f(566, 400),
            sf::Color(30, 30, 40)
        );
        
        // Resume button
        auto resumeButton = std::make_shared<UIButton>(
            "Resume Game",
            sf::Vector2f(450, 300),
            sf::Vector2f(200, 40)
        );
        resumeButton->setCallback([this]() {
            // Publish resume event
            GameEvent event;
            event.eventId = "game_resumed";
            event.eventType = "system";
            EventBus::getInstance().publish(EVENT_GAME_EVENT, event);
        });
        m_menuPanel->addChild(resumeButton);
        
        // Save button
        auto saveButton = std::make_shared<UIButton>(
            "Save Game",
            sf::Vector2f(450, 360),
            sf::Vector2f(200, 40)
        );
        saveButton->setCallback([this]() {
            EventBus::getInstance().publish(EVENT_SAVE_GAME, std::string("quicksave"));
        });
        m_menuPanel->addChild(saveButton);
        
        // Quit button
        auto quitButton = std::make_shared<UIButton>(
            "Quit to Menu",
            sf::Vector2f(450, 420),
            sf::Vector2f(200, 40)
        );
        quitButton->setCallback([this]() {
            SceneTransitionEvent event;
            event.fromScene = "Pause";
            event.toScene = "MainMenu";
            EventBus::getInstance().publish(EVENT_SCENE_TRANSITION, event);
        });
        m_menuPanel->addChild(quitButton);
    }
    
private:
    std::shared_ptr<UIPanel> m_menuPanel;
};
```

---

## Migration Guide / Руководство по миграции

### Migrating Existing Scenes / Миграция существующих сцен

**Step 1: Replace Raw SFML Objects with UI Components**

Before:
```cpp
class OldScene : public Scene {
private:
    sf::RectangleShape m_panel;
    sf::Text m_title;
    // Manual button handling
    bool m_buttonHovered;
};
```

After:
```cpp
class NewScene : public Scene {
private:
    std::shared_ptr<UIPanel> m_panel;
    std::shared_ptr<UILabel> m_title;
    std::shared_ptr<UIButton> m_actionButton;
};
```

**Step 2: Move Rendering Logic to Components**

Before:
```cpp
void OldScene::render(sf::RenderWindow& window) {
    window.draw(m_panel);
    window.draw(m_title);
    // Manual button rendering with state management
}
```

After:
```cpp
void NewScene::render(sf::RenderWindow& window) {
    m_panel->render(window);  // Renders all children automatically
}
```

**Step 3: Replace Manual Event Handling with EventBus**

Before:
```cpp
void OldScene::handleInput(const sf::Event& event) {
    // Manual mouse tracking
    if (event.type == sf::Event::MouseMoved) {
        // Check button hover...
    }
    // Manual resource updates
    if (buttonClicked) {
        PlayerState::getInstance().modifyFuel(-10);
        updateFuelDisplay();
    }
}
```

After:
```cpp
void NewScene::setupUI() {
    m_button->setCallback([this]() {
        // Button handles click automatically
        PlayerState::getInstance().modifyFuel(-10);
        // EventBus notifies all listeners automatically
    });
    
    // Subscribe to events
    EventBus::getInstance().subscribe(EVENT_RESOURCE_CHANGED,
        [this](const std::any& data) {
            // UI updates automatically
        });
}
```

---

## Testing Strategy / Стратегия тестирования

### Unit Tests for UI Components

```cpp
TEST(UIComponent, VisibilityControl) {
    UIPanel panel({0, 0}, {100, 100});
    ASSERT_TRUE(panel.isVisible());
    
    panel.setVisible(false);
    ASSERT_FALSE(panel.isVisible());
}

TEST(UIComponent, HierarchyManagement) {
    auto parent = std::make_shared<UIPanel>({0, 0}, {200, 200});
    auto child = std::make_shared<UILabel>("Test", {10, 10});
    
    parent->addChild(child);
    ASSERT_EQ(parent->getChildren().size(), 1);
    
    parent->removeChild(child);
    ASSERT_EQ(parent->getChildren().size(), 0);
}

TEST(UIButton, CallbackExecution) {
    bool clicked = false;
    UIButton button("Click Me", {0, 0}, {100, 40});
    button.setCallback([&clicked]() { clicked = true; });
    
    // Simulate click
    sf::Event event;
    event.type = sf::Event::MouseButtonPressed;
    event.mouseButton.button = sf::Mouse::Left;
    event.mouseButton.x = 50;
    event.mouseButton.y = 20;
    
    button.handleInput(event);
    
    event.type = sf::Event::MouseButtonReleased;
    button.handleInput(event);
    
    ASSERT_TRUE(clicked);
}
```

### Integration Tests for EventBus

```cpp
TEST(EventBus, PublishSubscribe) {
    bool eventReceived = false;
    int receivedValue = 0;
    
    int listenerId = EventBus::getInstance().subscribe("TestEvent",
        [&](const std::any& data) {
            eventReceived = true;
            receivedValue = std::any_cast<int>(data);
        });
    
    EventBus::getInstance().publish("TestEvent", 42);
    
    ASSERT_TRUE(eventReceived);
    ASSERT_EQ(receivedValue, 42);
    
    EventBus::getInstance().unsubscribe("TestEvent", listenerId);
}

TEST(EventBus, EventQueue) {
    int eventCount = 0;
    
    EventBus::getInstance().subscribe("QueueTest",
        [&](const std::any&) { eventCount++; });
    
    EventBus::getInstance().queueEvent("QueueTest", 1);
    EventBus::getInstance().queueEvent("QueueTest", 2);
    EventBus::getInstance().queueEvent("QueueTest", 3);
    
    ASSERT_EQ(eventCount, 0);  // Not processed yet
    
    EventBus::getInstance().processQueue();
    
    ASSERT_EQ(eventCount, 3);  // All processed
}
```

---

## Best Practices / Лучшие практики

### English

1. **Always unsubscribe from events in destructors**
   - Prevents memory leaks
   - Avoids dangling callbacks

2. **Use EventBus for cross-component communication**
   - Decouples components
   - Makes testing easier

3. **Keep UI logic in UI components**
   - Don't mix game logic with UI
   - Use callbacks and events for communication

4. **Reuse standard components**
   - Don't create custom components unless necessary
   - Extend existing components when possible

5. **Process EventBus queue once per frame**
   - Call `processQueue()` in main loop
   - Prevents event processing delays

### Русский

1. **Всегда отписывайтесь от событий в деструкторах**
   - Предотвращает утечки памяти
   - Избегает висячих обратных вызовов

2. **Используйте EventBus для межкомпонентной связи**
   - Развязывает компоненты
   - Упрощает тестирование

3. **Держите UI-логику в UI-компонентах**
   - Не смешивайте игровую логику с UI
   - Используйте callbacks и события для связи

4. **Переиспользуйте стандартные компоненты**
   - Не создавайте кастомные компоненты без необходимости
   - Расширяйте существующие компоненты когда возможно

5. **Обрабатывайте очередь EventBus один раз за кадр**
   - Вызывайте `processQueue()` в главном цикле
   - Предотвращает задержки обработки событий

---

## Conclusion / Заключение

### English

The UI Component Architecture and EventBus system provide a solid foundation for building maintainable, scalable, and testable user interfaces in the Road Trip Adventure RPG. By following the principles and patterns outlined in this guide, developers can create consistent, reusable UI elements that integrate seamlessly with the game's event system.

### Русский

Архитектура UI-компонентов и система EventBus предоставляют надежную основу для создания поддерживаемых, масштабируемых и тестируемых пользовательских интерфейсов в Road Trip Adventure RPG. Следуя принципам и паттернам, описанным в этом руководстве, разработчики могут создавать согласованные, переиспользуемые UI-элементы, которые легко интегрируются с игровой событийной системой.

---

**Document Version:** 1.0  
**Last Updated:** 2025-11-12  
**Status:** Complete
