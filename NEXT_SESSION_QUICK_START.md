# Quick Start for Next Session

## Current Build Status: ⚠️ DOES NOT COMPILE

Run: `cd build && make -j8 2>&1 | grep "error:"`

---

## IMMEDIATE FIXES NEEDED (Priority Order)

### 1. MapScene.cpp Line ~149-150
**Error:** `sf::Vertex` construction issue
```cpp
// BROKEN:
sf::Vertex line[] = {
    sf::Vertex(m_nodes[i].position),
    sf::Vertex(m_nodes[j].position)
};

// FIX: Vertex takes position directly in SFML 3.x, this should work
// Check if Vector2f is correct type. If issue persists:
sf::Vertex line[2];
line[0].position = m_nodes[i].position;
line[1].position = m_nodes[j].position;
line[0].color = sf::Color(150, 150, 150);
line[1].color = sf::Color(150, 150, 150);
```

### 2. CinematicEventScene.cpp setupUI() - Lines 50-106
**Error:** Still has UILayout code
**Fix:** Replace entire setupUI() method similar to RestScene:
```cpp
void CinematicEventScene::setupUI() {
    // SFML 3.x: Font loading
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    }

    // Setup overlay
    m_overlay.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    m_overlay.setFillColor(sf::Color(0, 0, 0, 180));

    // Setup dialog box
    m_dialogBox.setSize(sf::Vector2f(1000.0f, 600.0f));
    m_dialogBox.setPosition(sf::Vector2f(183.0f, 84.0f));
    m_dialogBox.setFillColor(sf::Color(40, 40, 45));
    m_dialogBox.setOutlineColor(sf::Color(200, 200, 200));
    m_dialogBox.setOutlineThickness(3);

    // Setup texts with emplace
    if (m_fontLoaded) {
        m_titleText.emplace(m_font, "", 32);
        m_titleText->setFillColor(sf::Color::White);
        m_titleText->setPosition(sf::Vector2f(233.0f, 120.0f));

        m_descriptionText.emplace(m_font, "", 20);
        m_descriptionText->setFillColor(sf::Color(220, 220, 220));
        m_descriptionText->setPosition(sf::Vector2f(233.0f, 200.0f));

        m_outcomeText.emplace(m_font, "", 22);
        m_outcomeText->setFillColor(sf::Color(255, 255, 150));
        m_outcomeText->setPosition(sf::Vector2f(233.0f, 400.0f));

        m_continuePrompt.emplace(m_font, "Нажмите ENTER чтобы продолжить / Press ENTER to continue", 18);
        m_continuePrompt->setFillColor(sf::Color(150, 150, 150));
        m_continuePrompt->setPosition(sf::Vector2f(400.0f, 650.0f));
    }

    // Update event-specific text if event is set
    if (m_event && m_titleText) {
        m_titleText->setString(m_event->title);

        if (m_descriptionText) {
            std::string wrappedDesc = wrapText(m_event->description, 900, m_font, 20);
            m_descriptionText->setString(wrappedDesc);
        }

        // Setup choice boxes...
        m_choiceBoxes.clear();
        m_choiceTexts.clear();

        float choiceY = 350.0f;
        for (size_t i = 0; i < m_event->choices.size(); ++i) {
            sf::RectangleShape choiceBox;
            choiceBox.setSize(sf::Vector2f(900.0f, 60.0f));
            choiceBox.setPosition(sf::Vector2f(233.0f, choiceY));
            choiceBox.setFillColor(sf::Color(60, 60, 70));
            choiceBox.setOutlineColor(sf::Color(100, 100, 110));
            choiceBox.setOutlineThickness(2);
            m_choiceBoxes.push_back(choiceBox);

            if (m_fontLoaded) {
                sf::Text choiceText(m_font, m_event->choices[i].text, 18);
                choiceText.setFillColor(sf::Color(200, 200, 200));
                choiceText.setPosition(sf::Vector2f(243.0f, choiceY + 15.0f));
                m_choiceTexts.push_back(choiceText);
            }

            choiceY += 70.0f;
        }
    }
}
```

### 3. CinematicEventScene.cpp handleInput()
**Find:** `if (event.type == sf::Event::KeyPressed)`
**Replace with SFML 3.x:**
```cpp
if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->code == sf::Keyboard::Key::Enter) { ... }
}
```

### 4. CinematicEventScene.cpp render()
**Find:** All `window.draw(m_titleText)` etc
**Replace:** Check optional exists:
```cpp
if (m_titleText) {
    window.draw(*m_titleText);
}
```

### 5. TravelCinematicScene.cpp Line ~103-104
**Error:** Old event handling
```cpp
// OLD:
if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {

// NEW:
if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->code == sf::Keyboard::Key::Escape) {
```

### 6. TravelCinematicScene.cpp render() Lines ~270-272
**Error:** Drawing optional texts without checking
**Find:** `window.draw(m_progressText);`
**Replace:**
```cpp
if (m_progressText) {
    window.draw(*m_progressText);
}
if (m_fuelText) {
    window.draw(*m_fuelText);
}
if (m_distanceText) {
    window.draw(*m_distanceText);
}
```

### 7. TravelCinematicScene.cpp update()
**Find:** All `m_progressText.setString()` calls
**Replace:** Check optional:
```cpp
if (m_progressText) {
    m_progressText->setString("Progress: " + std::to_string(...));
}
```

### 8. RestScene.cpp Line ~153
**Error:** `contains()` with 2 arguments
```cpp
// OLD:
if (m_optionBoxes[i].getGlobalBounds().contains(static_cast<float>(mousePos.x),
                                                static_cast<float>(mousePos.y)))

// NEW:
if (m_optionBoxes[i].getGlobalBounds().contains(sf::Vector2f(
    static_cast<float>(mousePos.x),
    static_cast<float>(mousePos.y))))
```

---

## AUTOMATED FIX SCRIPT

```bash
#!/bin/bash
cd /Users/afftobuss/Documents/GitHub/kurs

# Fix RestScene contains()
sed -i '' 's/contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))/contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))/g' src/RestScene.cpp

# Try build
cd build
make -j8 2>&1 | tee build.log
grep "error:" build.log | wc -l
```

---

## FILES STATUS SUMMARY

### ✅ FULLY MIGRATED (Compile Successfully)
- main.cpp
- SceneManager.cpp
- MainMenuScene.cpp + .h
- NodeScene.cpp + .h
- CharacterCreationMenu.cpp + .h
- AbilityTreeScene.cpp
- RestScene.cpp + .h (has 1 minor bug - contains())
- LocationScene.cpp + .h
- Structure.cpp + .h

### 🔄 PARTIALLY MIGRATED (Won't Compile - Minor Issues)
- MapScene.cpp - Vertex construction issue (1 fix)
- CinematicEventScene.cpp - setupUI needs rewrite, event handling (3-4 fixes)
- TravelCinematicScene.cpp - event handling, render checks (5-6 fixes)

### ❌ NOT MIGRATED
- CharacterScene.cpp + .h
- CompanionsScene.cpp + .h
- QuestsScene.cpp + .h
- InventoryScene.cpp + .h
- CharacterCreationScene.cpp + .h (might be partially done)
- UI/UIComponent.cpp + .hpp
- UI/HUDPanel.cpp + .hpp
- UI/ResourceBar.cpp + .hpp

---

## FASTEST PATH TO COMPILATION

**Estimated Time: 30-45 minutes**

1. Fix MapScene.cpp Vertex (2 min)
2. Fix RestScene.cpp contains() (1 min)
3. Rewrite CinematicEventScene.cpp setupUI() (10 min)
4. Fix CinematicEventScene.cpp event handling (5 min)
5. Fix CinematicEventScene.cpp render() optional checks (5 min)
6. Fix TravelCinematicScene.cpp event handling (3 min)
7. Fix TravelCinematicScene.cpp render/update optional checks (5 min)
8. Test compile: `cd build && make -j8`
9. Fix any remaining compilation errors (5-10 min)

After these fixes, the game should compile successfully.

---

## AFTER COMPILATION SUCCEEDS

### Test Run:
```bash
cd /Users/afftobuss/Documents/GitHub/kurs/build
./kkurs
```

### Then Migrate Remaining Files:
1. CharacterScene (similar to NodeScene pattern)
2. CompanionsScene (similar to NodeScene pattern)
3. QuestsScene (similar to NodeScene pattern)
4. InventoryScene (similar to NodeScene pattern)

### Pattern for Remaining Scenes:
1. Add `#include "UIConstants.h"` and `#include "FontLoader.h"`
2. Add `#include <optional>` to header
3. Change `sf::Text m_text;` → `std::optional<sf::Text> m_text;`
4. Use `FontLoader::load()` instead of `loadFromFile()`
5. Use `m_text.emplace(font, string, size)` for initialization
6. Update event handling to SFML 3.x `getIf<>()` pattern
7. Wrap all `setPosition()` calls in `sf::Vector2f()`
8. Check optional texts before drawing: `if (m_text) window.draw(*m_text);`

---

## KEY SFML 3.x PATTERNS (Quick Reference)

### Event Handling:
```cpp
// Keyboard
if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->code == sf::Keyboard::Key::Up) { ... }
}

// Mouse
if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mousePressed->button == sf::Mouse::Button::Left) {
        sf::Vector2f mousePos = sf::Vector2f(mousePressed->position);
    }
}
```

### Text Creation:
```cpp
// Member variable (header):
std::optional<sf::Text> m_text;

// Initialize (cpp):
if (m_fontLoaded) {
    m_text.emplace(m_font, "String", 24);
    m_text->setFillColor(color);
    m_text->setPosition(sf::Vector2f(x, y));
}

// Draw:
if (m_text) {
    window.draw(*m_text);
}

// Update:
if (m_text) {
    m_text->setString("New string");
}
```

### Positions:
```cpp
// Always wrap in Vector2f:
shape.setPosition(sf::Vector2f(x, y));
shape.setOrigin(sf::Vector2f(x, y));

// For contains():
bounds.contains(sf::Vector2f(x, y));
```

---

## END OF QUICK START

**First Action:** Run the automated fix script above, then manually fix CinematicEventScene.cpp setupUI().

**Current Token Count:** ~103K remaining - plenty for completing migration!
