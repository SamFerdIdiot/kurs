# SFML 3.x Migration Status

## Date: 2025-11-27
## Target Resolution: 1440x900 (MacBook Air M1 - Fixed, no scaling)

---

## ✅ COMPLETED FILES

### Core System Files
1. **main.cpp** - ✅ DONE
   - VideoMode updated to `sf::VideoMode({width, height})`
   - Uses UI::SCREEN_WIDTH and UI::SCREEN_HEIGHT

2. **SceneManager.cpp** - ✅ DONE
   - Event loop updated to `std::optional<sf::Event>`
   - Event handling uses `EventHelper::isClosed()`

3. **UIConstants.h** - ✅ CREATED
   - All fixed dimensions for 1440x900
   - Color constants
   - Font paths with fallback chain

4. **FontLoader.h** - ✅ CREATED
   - Helper for SFML 3.x font loading with fallbacks
   - Uses `openFromFile()` returning bool

5. **EventHelper.h** - ✅ CREATED
   - Simplified SFML 3.x event checking
   - `isKeyPressed()`, `isClosed()` helpers

6. **SFML3_MIGRATION_PLAN.md** - ✅ CREATED
   - Comprehensive migration guide

### Scene Files (FULLY MIGRATED)
1. **MainMenuScene.cpp / .h** - ✅ DONE
   - Text members converted to `std::optional<sf::Text>`
   - Font loading uses FontLoader
   - Event handling uses SFML 3.x `getIf<>`
   - All setPosition use Vector2f

2. **NodeScene.cpp / .h** - ✅ DONE
   - Simplified menu navigation
   - All Text uses SFML 3.x constructors
   - Event handling updated
   - Fixed dimensions, NO UILayout

3. **MapScene.cpp / .h** - ✅ DONE
   - sf::Vertex updated (color set separately)
   - PrimitiveType::Lines instead of sf::Lines
   - setOrigin uses Vector2f
   - Event handling updated

4. **CharacterCreationMenu.cpp / .h** - ✅ DONE
   - openFromFile instead of loadFromFile
   - Event handling updated
   - All Text constructors fixed
   - setLineSpacing() removed (not in SFML 3.x)

5. **AbilityTreeScene.cpp** - ✅ DONE
   - ALL setPosition calls fixed with Vector2f
   - ALL sf::Text objects use font-in-constructor
   - Event handling fully updated for SFML 3.x
   - No UILayout, uses fixed window dimensions

6. **RestScene.cpp / .h** - ✅ DONE
   - FontLoader integration
   - Text members to std::optional
   - Event handling updated (Key enums, getIf<>)
   - All setPosition with Vector2f
   - render() checks optional texts

7. **LocationScene.cpp / .h** - ✅ DONE
   - UILayout removed, uses UI::SCREEN_WIDTH/HEIGHT
   - FontLoader integration
   - Text members to std::optional
   - Event handling updated
   - All setPosition with Vector2f

---

## ⚠️ PARTIALLY MIGRATED FILES

### CinematicEventScene.cpp / .h - 🔄 IN PROGRESS
**Status:** Header updated, cpp needs completion

**What's Done:**
- Header: UILayout include removed
- Header: Text members converted to std::optional
- Source: UIConstants and FontLoader includes added
- Source: HudPanel constructor uses UI::SCREEN_WIDTH/HEIGHT

**What's Left:**
1. Replace `setupUI()` method completely:
   - Remove all UILayout code
   - Remove `m_font.loadFromFile()` → use `FontLoader::load()`
   - Update all text initialization:
     ```cpp
     // OLD:
     m_titleText.setFont(m_font);
     m_titleText.setCharacterSize(32);
     m_titleText.setString("Title");
     m_titleText.setPosition(233, 120);

     // NEW:
     if (m_fontLoaded) {
         m_titleText.emplace(m_font, "Title", 32);
         m_titleText->setFillColor(sf::Color::White);
         m_titleText->setPosition(sf::Vector2f(233.0f, 120.0f));
     }
     ```

2. Fix all `setPosition()` calls:
   - Find: `setPosition(x, y)`
   - Replace: `setPosition(sf::Vector2f(x, y))`
   - Locations: lines 70, 81, 89, 97, 106, 125, 139

3. Update `handleInput()` method:
   ```cpp
   // OLD:
   if (event.type == sf::Event::KeyPressed) {
       if (event.key.code == sf::Keyboard::Return) { ... }
   }

   // NEW:
   if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
       if (keyPressed->code == sf::Keyboard::Key::Enter) { ... }
   }
   ```

4. Update `render()` method:
   - Check if optional texts exist before drawing:
     ```cpp
     if (m_titleText) {
         window.draw(*m_titleText);
     }
     ```

5. Fix `setupUI()` text updates when `m_event` is set:
   ```cpp
   // OLD:
   m_titleText.setString(m_event->title);

   // NEW:
   if (m_titleText) {
       m_titleText->setString(m_event->title);
   }
   ```

---

## ❌ NOT YET MIGRATED FILES

### Scene Files (Need Full Migration)

#### TravelCinematicScene.cpp / .h
**Estimated Changes:**
- Same pattern as CinematicEventScene
- Remove UILayout
- FontLoader integration
- Text → std::optional<sf::Text>
- Event handling update
- All setPosition with Vector2f

**Search patterns to fix:**
```bash
# Find all SFML 2.x patterns:
grep -n "loadFromFile\|setFont\|setCharacterSize\|event.type\|event.key.code" TravelCinematicScene.cpp
```

#### CharacterScene.cpp / .h
**Estimated Changes:**
- Remove UILayout → use UIConstants
- FontLoader integration
- Text → std::optional<sf::Text>
- Event handling: `getIf<sf::Event::KeyPressed>()`
- Keyboard enums: `sf::Keyboard::Key::Up`
- All setPosition(x, y) → setPosition(Vector2f(x, y))

#### CompanionsScene.cpp / .h
**Similar to CharacterScene**

#### QuestsScene.cpp / .h
**Similar to CharacterScene**

#### InventoryScene.cpp / .h
**Similar to CharacterScene**

#### CharacterCreationScene.cpp / .h
**May already be partially done - needs verification**

### UI Component Files

#### UI/UIComponent.cpp / .hpp
**Check if exists, if so:**
- Remove UILayout dependencies
- Update Text handling if any

#### UI/HUDPanel.cpp / .hpp
**Needs:**
- Verify no UILayout usage
- Check Text member variables
- Update event handling if any

#### UI/ResourceBar.cpp / .hpp
**Needs:**
- Same as HUDPanel

---

## 🔧 SFML 3.x API CHANGES REFERENCE

### Font Loading
```cpp
// OLD:
m_font.loadFromFile("path/to/font.ttf");

// NEW:
if (auto fontOpt = FontLoader::load()) {
    m_font = std::move(*fontOpt);
    m_fontLoaded = true;
}
```

### Text Creation
```cpp
// OLD:
sf::Text text;
text.setFont(m_font);
text.setString("Hello");
text.setCharacterSize(24);

// NEW (for member variables):
std::optional<sf::Text> m_text;  // in header

m_text.emplace(m_font, "Hello", 24);  // in constructor

// NEW (for local variables):
sf::Text text(m_font, "Hello", 24);
```

### Event Handling
```cpp
// OLD:
if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Up) { ... }
}

// NEW:
if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->code == sf::Keyboard::Key::Up) { ... }
}

// OLD:
event.type == sf::Event::MouseMoved
event.mouseMove.x, event.mouseMove.y

// NEW:
const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()
mouseMoved->position.x, mouseMoved->position.y

// OLD:
event.mouseButton.button == sf::Mouse::Left

// NEW:
mousePressed->button == sf::Mouse::Button::Left
```

### Position and Size
```cpp
// OLD:
shape.setPosition(100, 200);

// NEW:
shape.setPosition(sf::Vector2f(100.0f, 200.0f));

// OLD:
shape.setOrigin(50, 75);

// NEW:
shape.setOrigin(sf::Vector2f(50.0f, 75.0f));
```

### Vertex and Primitives
```cpp
// OLD:
sf::Vertex line[] = {
    sf::Vertex(pos1, color1),
    sf::Vertex(pos2, color2)
};
window.draw(line, 2, sf::Lines);

// NEW:
sf::Vertex line[] = {
    sf::Vertex(pos1),
    sf::Vertex(pos2)
};
line[0].color = color1;
line[1].color = color2;
window.draw(line, 2, sf::PrimitiveType::Lines);
```

### VideoMode
```cpp
// OLD:
sf::RenderWindow window(sf::VideoMode(1440, 900), "Title");

// NEW:
sf::RenderWindow window(sf::VideoMode({1440, 900}), "Title");
```

### Event Loop
```cpp
// OLD:
sf::Event event;
while (window.pollEvent(event)) { ... }

// NEW:
while (const std::optional<sf::Event> event = window.pollEvent()) {
    // use *event
}
```

---

## 📝 SYSTEMATIC MIGRATION STEPS FOR REMAINING FILES

### For Each Scene File:

1. **Update includes:**
   ```cpp
   #include "UIConstants.h"  // instead of "UILayout.h"
   #include "FontLoader.h"
   ```

2. **Update header (.h file):**
   - Add `#include <optional>`
   - Change `sf::Text m_text;` to `std::optional<sf::Text> m_text;`
   - Remove UILayout include

3. **Update constructor:**
   - Change HUD panel initialization to use `UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT`

4. **Update setupUI() or equivalent:**
   - Remove all UILayout code
   - Use FontLoader for font loading
   - Use emplace() for optional text members:
     ```cpp
     if (m_fontLoaded) {
         m_text.emplace(m_font, "string", size);
         m_text->setFillColor(color);
         m_text->setPosition(sf::Vector2f(x, y));
     }
     ```

5. **Fix all setPosition() calls:**
   ```bash
   # Find all calls:
   grep -n "setPosition" filename.cpp

   # Ensure they all use Vector2f wrapper
   ```

6. **Update handleInput():**
   - Replace all `event.type ==` with `event.getIf<>`
   - Update keyboard enums to `sf::Keyboard::Key::`
   - Update mouse button enums to `sf::Mouse::Button::`

7. **Update render():**
   - Check if optional texts exist before drawing:
     ```cpp
     if (m_text) {
         window.draw(*m_text);
     }
     ```

8. **Update any text.setString() calls:**
   ```cpp
   // Ensure you check if optional exists:
   if (m_text) {
       m_text->setString("new string");
   }
   ```

---

## ⚡ QUICK GREP COMMANDS TO FIND ISSUES

```bash
# Find old font loading:
grep -rn "loadFromFile" src/

# Find old event handling:
grep -rn "event.type ==" src/
grep -rn "event.key.code" src/

# Find setFont/setCharacterSize/setString patterns (OLD API):
grep -rn "setFont\|setCharacterSize" src/ | grep -v "SFML 3.x"

# Find setPosition with two parameters:
grep -rn "setPosition([^V]" src/

# Find UILayout usage:
grep -rn "UILayout\|SCALE_SIZE\|SCALE_POS\|GET_LAYOUT" src/

# Find old keyboard enums:
grep -rn "sf::Keyboard::[A-Z]" src/ | grep -v "sf::Keyboard::Key::"
```

---

## 🎯 PRIORITY ORDER FOR NEXT SESSION

1. **CinematicEventScene.cpp** - Finish setupUI() and handleInput()
2. **TravelCinematicScene.cpp** - Full migration
3. **CharacterScene.cpp** - Full migration
4. **CompanionsScene.cpp** - Full migration
5. **QuestsScene.cpp** - Full migration
6. **InventoryScene.cpp** - Full migration
7. **CharacterCreationScene.cpp** - Verify/complete
8. **UI Components** - HUDPanel, ResourceBar, UIComponent
9. **Final Build Test** - Compile entire project
10. **Test Run** - Verify game starts and scenes work

---

## 🚀 COMPILATION STATUS

### Last Build Attempt:
```bash
cd /Users/afftobuss/Documents/GitHub/kurs/build
cmake .. -DBUILD_TESTS=OFF
make -j8
```

### Known Working Files:
- main.cpp
- SceneManager.cpp
- MainMenuScene.cpp
- NodeScene.cpp
- MapScene.cpp
- CharacterCreationMenu.cpp
- AbilityTreeScene.cpp
- RestScene.cpp
- LocationScene.cpp

### Files Expected to Fail Compilation:
- CinematicEventScene.cpp (partial migration)
- TravelCinematicScene.cpp (not migrated)
- CharacterScene.cpp (not migrated)
- CompanionsScene.cpp (not migrated)
- QuestsScene.cpp (not migrated)
- InventoryScene.cpp (not migrated)

---

## 📊 MIGRATION PROGRESS

**Total Scene Files:** ~13
**Fully Migrated:** 8 (61%)
**Partially Migrated:** 1 (CinematicEventScene)
**Not Migrated:** 4-5

**Estimated Time Remaining:** 2-3 hours of focused work

---

## 💡 TIPS FOR NEXT SESSION

1. **Start with CinematicEventScene** - it's partially done, finish it first
2. **Use batch editing** - many files have identical patterns
3. **Test compilation frequently** - `make -j8` after each file
4. **Keep CMakeLists.txt** - already configured for SFML 3.x
5. **Don't modify tests** - they're disabled with `-DBUILD_TESTS=OFF`
6. **UILayout system** - completely removed, don't add it back
7. **Fixed dimensions only** - 1440x900, no scaling

---

## 📞 DEBUGGING TIPS

If compilation fails:
```bash
# Clear build and rebuild:
rm -rf build && mkdir build && cd build
cmake .. -DBUILD_TESTS=OFF
make -j8 2>&1 | tee build.log

# Check for specific errors:
grep "error:" build.log

# Common errors and fixes:
# - "no matching function for call to 'sf::Text::Text()'"
#   → Text member needs to be std::optional<sf::Text>
#
# - "cannot convert 'int' to 'sf::Vector2f'"
#   → setPosition needs Vector2f wrapper
#
# - "'type' is not a member of 'sf::Event'"
#   → Update to event.getIf<>() pattern
#
# - "'Up' is not a member of 'sf::Keyboard'"
#   → Update to sf::Keyboard::Key::Up
```

---

## END OF STATUS DOCUMENT

**Last Updated:** 2025-11-27
**Updated By:** Claude Code Assistant
**Next Steps:** Complete remaining scene files following patterns above
