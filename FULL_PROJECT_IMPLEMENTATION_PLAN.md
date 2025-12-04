# FULL PROJECT IMPLEMENTATION PLAN
**KKURS - Complete Implementation Roadmap**

**Created:** 2025-11-28
**Current Status:** Phase 5 - Advanced Integration (75% complete)
**Target:** Full Release 1.0

---

## 📊 EXECUTIVE SUMMARY

### Project Status
- **Code Completion:** 75%
- **UI Completion:** 85%
- **Systems Completion:** 80%
- **Tests Passing:** 88/97 (91%)
- **Current Phase:** Phase 5 - Advanced Integration
- **Resolution:** 1300x800 (MacBook Air M1)
- **Framework:** SFML 3.x (migration in progress)

### Critical Issues
1. **🔴 19 TODO comments** violate Principle 1 (Immediate Functionality)
2. **🔴 8 unimplemented scenes** with existing headers
3. **🔴 SFML 3.x migration** incomplete (80% done)
4. **🟡 28 header files** without .cpp implementation
5. **🟡 Tab-based navigation** needs refinement

---

## 🎯 PHASE-BY-PHASE BREAKDOWN

---

## PHASE 5: ADVANCED INTEGRATION & CLEANUP (Current Phase)
**Timeline:** 2-3 weeks
**Priority:** 🔴 CRITICAL

### 5.1 SFML 3.x Migration Completion (3-5 days)
**Status:** 80% complete

#### Remaining Files to Migrate:
1. **DialogueScene.h/cpp**
   - Update event handling to `event.getIf<>()`
   - Update Text construction (font in constructor)
   - Test typewriter effect with new SFML 3.x timing

2. **MotelScene.h/cpp** (not yet implemented)
   - Implement with SFML 3.x from start
   - No migration needed

3. **GameOverScene.h/cpp** (not yet implemented)
   - Implement with SFML 3.x from start
   - No migration needed

4. **PauseMenuScene.h/cpp** (not yet implemented)
   - Implement with SFML 3.x from start
   - No migration needed

#### Testing:
- Compile all scenes individually
- Run full game loop through all scenes
- Verify event handling across all inputs
- Check text rendering on all screens
- Validate vector types throughout

**Deliverables:**
- [ ] All scenes compile with SFML 3.x
- [ ] All event handling uses new API
- [ ] All text uses font-in-constructor pattern
- [ ] All vector operations use sf::Vector2f wrappers
- [ ] Update SFML3_MIGRATION_STATUS.md to 100%

---

### 5.2 REMOVE ALL TODO COMMENTS (2-3 days)
**Priority:** 🔴 CRITICAL (Violates Principle 1)

#### 5.2.1 LocationScene.cpp (3 TODO)

**TODO Line 232: Add items to inventory on purchase**
```cpp
// Current: TODO: Actually add purchased items to inventory
// Implementation needed:
void LocationScene::handlePurchase(const std::string& itemId) {
    // 1. Get item from shop inventory
    // 2. Check player has enough money
    // 3. Deduct money from player
    // 4. Add item to PlayerState inventory via InventorySystem
    // 5. Update UI to reflect purchase
    // 6. Play purchase sound (if audio system ready)
}
```
**Micro-moments:**
- User selects item → highlight item
- User confirms purchase → check funds
- Insufficient funds → show error message (red text)
- Sufficient funds → deduct money (-$XXX animation)
- Add item to inventory → show item icon moving to inventory
- Update shop UI → item quantity decreases
- Show success message → "Purchased: [Item Name]"

**TODO Line 246: Implement car repair**
```cpp
// Current: TODO: Implement actual car repair mechanics
// Implementation needed:
void LocationScene::handleRepair() {
    // 1. Check car damage level from PlayerState
    // 2. Calculate repair cost (damage% * BASE_REPAIR_COST)
    // 3. Apply NPC mechanic discount if present
    // 4. Check player funds
    // 5. Deduct money and restore car health
    // 6. Update PlayerState car condition
}
```
**Micro-moments:**
- User selects "Repair Car"
- System checks car condition (0-100%)
- Display damage assessment: "Condition: 45% - Repair Cost: $250"
- User confirms repair
- Check funds → deduct money
- Animate repair process (2 second progress bar)
- Update car condition to 100%
- Show success: "Car repaired! Condition: 100%"

**TODO Line 286: Open dialogue system**
```cpp
// Current: TODO: Open dialogue system with NPC
// Implementation needed:
void LocationScene::handleDialogue(const std::string& npcId) {
    // 1. Load NPC dialogue tree from DialogueManager
    // 2. Create DialogueScene with NPC data
    // 3. Transition to DialogueScene
    // 4. Return to LocationScene when dialogue ends
    // 5. Apply dialogue consequences (relationship changes, quest updates)
}
```
**Micro-moments:**
- User clicks "Talk" on NPC
- Fade out LocationScene (0.3s)
- Load DialogueScene with NPC portrait
- Initialize dialogue tree from DialogueManager
- Display first dialogue line with typewriter effect
- User selects response → branch dialogue
- Dialogue ends → update quest states
- Fade back to LocationScene (0.3s)
- Show any dialogue rewards ("Gained: Quest X")

#### 5.2.2 RestScene.cpp (4 TODO)

**TODO Line 196: Add status/buff system**
```cpp
// Current: TODO: Add buff system
// Implementation needed:
class BuffSystem {
    struct Buff {
        std::string id;
        std::string name;
        float duration;        // seconds remaining
        float multiplier;      // effect multiplier
        std::string statType;  // "energy", "fuel_efficiency", etc.
    };

    std::vector<Buff> activeBuffs;

    void applyBuff(const Buff& buff);
    void updateBuffs(float deltaTime);
    void removeBuff(const std::string& buffId);
    float getStatMultiplier(const std::string& statType);
};
```
**Micro-moments:**
- User rests at location
- System applies "Well Rested" buff
- Buff icon appears in HUD (top-left corner)
- Buff shows duration timer (e.g., "Well Rested: 30:00")
- Buff effect applies: +20% energy regeneration
- Timer counts down each second
- At 0:00, buff expires → fade out icon
- Show notification: "Buff expired: Well Rested"

**TODO Line 221: Restore health**
```cpp
// Current: TODO: Restore health when resting
// Implementation needed:
void RestScene::restoreHealth(float amount) {
    // 1. Get current health from PlayerState
    // 2. Calculate new health (current + amount, capped at max)
    // 3. Update PlayerState health
    // 4. Animate health bar increase
    // 5. Show +health indicator
}
```
**Micro-moments:**
- User selects "Full Rest" ($40)
- Fade to black (0.5s) - "Resting..."
- Health bar animates from 45% → 100% (2s)
- Green "+55" floats up from health bar
- Energy bar animates from 30% → 100% (2s)
- Green "+70" floats up from energy bar
- Fade in (0.5s) - "Fully rested!"
- Auto-save triggers (save icon flash)

**TODO Line 224: Restore morale**
```cpp
// Current: TODO: Restore morale for companions
// Implementation needed:
void RestScene::restoreMorale() {
    // 1. Get all companions from PlayerState
    // 2. For each companion:
    //    - Increase morale by REST_MORALE_BONUS
    //    - Cap at max morale (100)
    //    - Show morale increase animation
    // 3. Update companion UI
}
```
**Micro-moments:**
- User rests with 2 companions
- System iterates through companions
- Companion 1: "Ivan" - Morale 60% → 85% (+25)
- Companion 2: "Maria" - Morale 45% → 70% (+25)
- Show stacked notifications:
  - "Ivan feels better! Morale: 85%"
  - "Maria feels better! Morale: 70%"
- Update companion portraits (happier expression if sprites exist)

**TODO Line 227: Buff system integration**
```cpp
// Current: TODO: Apply resting buffs
// Implementation needed:
void RestScene::applyRestBuffs(RestType type) {
    BuffSystem& buffs = BuffSystem::getInstance();

    switch(type) {
        case RestType::QUICK:  // $20, +50 energy
            buffs.applyBuff({"quick_rest", "Quick Rest", 900.0f, 1.1f, "energy_regen"});
            break;
        case RestType::FULL:   // $40, full restore + buff
            buffs.applyBuff({"well_rested", "Well Rested", 1800.0f, 1.2f, "energy_regen"});
            buffs.applyBuff({"refreshed", "Refreshed", 1200.0f, 1.15f, "fuel_efficiency"});
            break;
    }
}
```

#### 5.2.3 AbilityTreeSystem.cpp (4 TODO)

**TODO Line 175: Check items through InventorySystem**
```cpp
// Current: TODO: Implement item checking
// Implementation needed:
bool AbilityTreeSystem::hasRequiredItems(const std::string& abilityId) {
    const AbilityNode* ability = getAbilityById(abilityId);
    if (!ability) return false;

    const auto& req = ability->getRequirement();
    InventorySystem& inv = InventorySystem::getInstance();

    for (const auto& [itemId, quantity] : req.requiredItems) {
        if (inv.getItemCount(itemId) < quantity) {
            return false;
        }
    }
    return true;
}
```

**TODO Line 211: Remove items on use**
```cpp
// Current: TODO: Remove consumed items
// Implementation needed:
void AbilityTreeSystem::useActiveAbility(const std::string& abilityId) {
    const AbilityNode* ability = getAbilityById(abilityId);
    if (!ability || ability->getType() != AbilityType::ACTIVE) return;

    const auto& effect = ability->getEffect();

    // Check charges
    if (getCurrentCharges(abilityId) <= 0) return;

    // Consume charge
    decrementCharge(abilityId);

    // Apply effect
    applyActiveEffect(effect);

    // Remove consumed items if any
    InventorySystem& inv = InventorySystem::getInstance();
    for (const auto& [itemId, quantity] : effect.consumedItems) {
        inv.removeItem(itemId, quantity);
    }
}
```

**TODO Line 267-272: Save/Load JSON**
```cpp
// Current: TODO: Save and load from JSON
// Implementation needed:
nlohmann::json AbilityTreeSystem::toJSON() const {
    nlohmann::json j;
    j["unlockedAbilities"] = m_unlockedAbilities;
    j["activeCharges"] = m_activeCharges;
    j["skillPoints"] = m_skillPoints;
    return j;
}

void AbilityTreeSystem::fromJSON(const nlohmann::json& j) {
    m_unlockedAbilities = j.value("unlockedAbilities", std::vector<std::string>{});
    m_activeCharges = j.value("activeCharges", std::unordered_map<std::string, int>{});
    m_skillPoints = j.value("skillPoints", 0);
}
```

#### 5.2.4 QuestManager.cpp (2 TODO)

**TODO Line 51: JSON quest parsing**
```cpp
// Current: TODO: Load quests from JSON files
// Implementation needed:
void QuestManager::loadQuestsFromJSON(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to load quests from: " << filepath << std::endl;
        return;
    }

    nlohmann::json questData;
    file >> questData;

    for (const auto& qData : questData["quests"]) {
        Quest quest;
        quest.id = qData["id"];
        quest.title = qData["title"];
        quest.description = qData["description"];
        quest.questType = stringToQuestType(qData["type"]);
        quest.isActive = false;
        quest.isCompleted = false;

        // Parse objectives
        for (const auto& objData : qData["objectives"]) {
            Quest::Objective obj;
            obj.description = objData["description"];
            obj.type = stringToObjectiveType(objData["type"]);
            obj.targetId = objData.value("targetId", "");
            obj.targetQuantity = objData.value("quantity", 1);
            obj.currentQuantity = 0;
            obj.isCompleted = false;
            quest.objectives.push_back(obj);
        }

        // Parse rewards
        quest.rewards.money = qData["rewards"].value("money", 0.0f);
        quest.rewards.experience = qData["rewards"].value("experience", 0);
        // ... parse items, reputation, etc.

        m_allQuests[quest.id] = quest;
    }
}
```

**TODO Line 274: Give rewards to player**
```cpp
// Current: TODO: Actually give rewards
// Implementation needed:
void QuestManager::giveRewards(const Quest& quest) {
    PlayerState& player = GameStateManager::getInstance().getPlayerState();
    ExperienceSystem& exp = ExperienceSystem::getInstance();
    InventorySystem& inv = InventorySystem::getInstance();

    // Give money
    if (quest.rewards.money > 0) {
        player.addMoney(quest.rewards.money);
        std::cout << "Received: $" << quest.rewards.money << std::endl;
    }

    // Give experience
    if (quest.rewards.experience > 0) {
        exp.addExperience(quest.rewards.experience);
        std::cout << "Received: " << quest.rewards.experience << " XP" << std::endl;
    }

    // Give items
    for (const auto& [itemId, quantity] : quest.rewards.items) {
        inv.addItem(itemId, quantity);
        std::cout << "Received: " << itemId << " x" << quantity << std::endl;
    }

    // Apply reputation changes
    for (const auto& [factionId, amount] : quest.rewards.reputation) {
        // TODO: Implement faction system
        std::cout << "Reputation with " << factionId << ": +" << amount << std::endl;
    }
}
```

#### 5.2.5 EventManager.cpp (2 TODO)

**TODO Line 871: Check inventory items**
```cpp
// Current: TODO: Check if player has required items
// Implementation needed:
bool EventManager::hasRequiredItems(const std::string& itemId, int quantity) {
    InventorySystem& inv = InventorySystem::getInstance();
    return inv.getItemCount(itemId) >= quantity;
}
```

**TODO Line 901: Change car state**
```cpp
// Current: TODO: Modify car condition
// Implementation needed:
void EventManager::applyCarDamage(float damageAmount) {
    PlayerState& player = GameStateManager::getInstance().getPlayerState();
    Car& car = player.getCar();

    float currentCondition = car.getCondition();
    float newCondition = std::max(0.0f, currentCondition - damageAmount);

    car.setCondition(newCondition);

    std::cout << "Car damaged! Condition: " << currentCondition
              << "% -> " << newCondition << "%" << std::endl;

    if (newCondition <= 0) {
        // Car broken down - trigger breakdown event
        triggerBreakdown();
    }
}
```

#### 5.2.6 Other Files (6 TODO)

**AbilityTreeScene.cpp: Unlock sounds**
```cpp
// Add to attemptUnlock() function
if (success) {
    std::cout << "Unlocked ability: " << m_selectedAbility->getName() << std::endl;
    // TODO: Play unlock sound
    // SoundManager::getInstance().play("ability_unlock");
}
```

**InventorySystem.cpp: Item effects**
```cpp
void InventorySystem::useItem(const std::string& itemId) {
    // Apply item effects based on type
    // TODO: Implement effect system
}
```

**QuestsScene.cpp: QuestManager integration**
```cpp
void QuestsScene::loadQuests() {
    // TODO: Load from QuestManager
    QuestManager& qm = QuestManager::getInstance();
    m_activeQuests = qm.getActiveQuests();
    m_completedQuests = qm.getCompletedQuests();
}
```

**Structure.cpp: Sprite rendering**
```cpp
void Structure::render(sf::RenderWindow& window) {
    // TODO: Replace rectangles with sprites
    // For now: draw colored rectangles
    window.draw(m_shape);
}
```

**Deliverables:**
- [ ] All 19 TODO comments resolved
- [ ] All features fully functional
- [ ] Code compiles without warnings
- [ ] All tests pass
- [ ] Documentation updated

---

### 5.3 IMPLEMENT MISSING SCENES (5-7 days)
**Priority:** 🔴 HIGH

#### 5.3.1 DialogueScene (2 days)
**Purpose:** Branching conversations with NPCs

**Implementation Details:**

**Files to create:**
- `src/DialogueScene.cpp` (header already exists)

**Core Features:**
```cpp
class DialogueScene : public Scene {
private:
    // Data
    DialogueTree m_currentDialogue;
    DialogueNode* m_currentNode;
    std::string m_npcId;
    std::string m_npcName;

    // Typewriter effect
    std::string m_displayedText;
    size_t m_currentChar;
    float m_typewriterTimer;
    bool m_typewriterComplete;

    // Choices
    std::vector<DialogueChoice> m_currentChoices;
    int m_selectedChoice;

    // UI elements
    sf::Font m_font;
    sf::RectangleShape m_dialogueBox;
    sf::RectangleShape m_portraitBox;
    sf::Text m_npcNameText;
    sf::Text m_dialogueText;
    std::vector<sf::Text> m_choiceTexts;

public:
    DialogueScene(const std::string& npcId, const std::string& dialogueTreeId);

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    void loadDialogueTree(const std::string& treeId);
    void displayNode(const DialogueNode& node);
    void selectChoice(int index);
    void updateTypewriter(float deltaTime);
    void applyChoiceConsequences(const DialogueChoice& choice);
};
```

**Micro-moments - Full dialogue flow:**

1. **Scene Entry** (0.5s)
   - Fade from black
   - Load NPC data from DialogueManager
   - Initialize dialogue tree
   - Position UI elements

2. **First Line Display** (2-4s per line)
   - Show NPC portrait (if sprite exists, else colored box)
   - Display NPC name in header
   - Start typewriter effect for first line
   - Character-by-character reveal (50ms per char)
   - Sound effect per character (if audio enabled)

3. **Typewriter Complete** (instant)
   - Show all remaining text instantly if user presses Space
   - Display ▼ indicator at bottom right
   - Enable choice selection

4. **Choice Display** (0.3s)
   - List all available choices (1-4 options)
   - Highlight first choice (yellow text)
   - Show choice icons if applicable:
     - 💰 (requires money)
     - 🔒 (locked - requirements not met)
     - ⭐ (special/important choice)

5. **Choice Navigation**
   - W/S or ↑↓ to navigate
   - Selected choice highlighted (yellow)
   - Locked choices grayed out (dark gray)
   - Hover shows requirements tooltip

6. **Choice Selection** (Enter key)
   - Flash selected choice (0.1s)
   - Check requirements
     - If failed: red flash, error sound, show why
     - If passed: proceed to consequence

7. **Apply Consequences** (0.5s)
   - Update relationship with NPC (±10 points)
   - Add/remove items from inventory
   - Start/update quests
   - Change game flags
   - Show consequence notifications:
     - "Relationship with Ivan: +10"
     - "Quest updated: Find the Package"

8. **Next Node** (0.3s)
   - Clear current text
   - Load next dialogue node
   - If end node: transition out
   - Else: goto step 2

9. **Dialogue End** (0.5s)
   - Show "Dialogue ended" message
   - Fade to black (0.3s)
   - Return to previous scene
   - Update quest log if needed

**Edge Cases:**
- Skip dialogue: Hold Spacebar to speed through
- Dialogue timeout: None (player-paced)
- No choices: Auto-advance after 3 seconds
- NPC leaves: Special "interrupted" ending

**Technical Requirements:**
```cpp
// Typewriter speed: 50ms per character
constexpr float TYPEWRITER_CHAR_DELAY = 0.05f;

// Dialogue box dimensions (1300x800 screen)
constexpr float DIALOGUE_BOX_WIDTH = 1100.0f;
constexpr float DIALOGUE_BOX_HEIGHT = 250.0f;
constexpr float DIALOGUE_BOX_X = 100.0f;
constexpr float DIALOGUE_BOX_Y = 500.0f;

// Portrait dimensions
constexpr float PORTRAIT_SIZE = 150.0f;
constexpr float PORTRAIT_X = 120.0f;
constexpr float PORTRAIT_Y = 300.0f;

// Choice layout
constexpr float CHOICE_START_Y = 300.0f;
constexpr float CHOICE_SPACING = 40.0f;
```

**Integration Points:**
- DialogueManager: Load dialogue trees
- QuestManager: Update quest states
- PlayerState: Check/modify resources
- InventorySystem: Check/add/remove items
- NPC system: Update relationships

#### 5.3.2 MotelScene (1.5 days)
**Purpose:** Rest and save game

**Implementation Details:**

**Files to create:**
- `src/MotelScene.cpp` (header already exists)

**Core Features:**
```cpp
class MotelScene : public Scene {
private:
    enum class RestOption {
        QUICK_REST,    // $20, +50 energy
        FULL_REST,     // $40, +100 energy + auto-save
        LEAVE
    };

    RestOption m_selectedOption;
    bool m_isResting;
    float m_restingTimer;

    // UI
    sf::Font m_font;
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    std::vector<sf::Text> m_optionTexts;
    sf::RectangleShape m_bedSprite;  // Placeholder for bed sprite

public:
    MotelScene();

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    void selectOption(RestOption option);
    void performRest(RestOption option);
    void renderRestingAnimation(sf::RenderWindow& window);
};
```

**Micro-moments - Rest flow:**

1. **Scene Entry** (0.5s)
   - Fade in from travel/location
   - Show motel exterior (placeholder: brown rectangle)
   - Display motel name: "MOTEL - Rest Here"
   - Show current player stats:
     - Money: $1250
     - Energy: 45%
     - Time: 14:30

2. **Menu Display**
   - Show 3 options:
     ```
     > [1] Quick Rest - $20
         Restore 50% energy (30 minutes)

       [2] Full Rest - $40
         Restore 100% energy + Auto-save (8 hours)

       [3] Leave
     ```
   - Selected option highlighted (yellow)
   - Insufficient funds shown in red

3. **Option Selection** (W/S navigation, Enter to confirm)

4. **Quick Rest Flow** (3 seconds total)
   - Check funds ($20)
   - Deduct money: $1250 → $1230
   - Fade to black (0.5s)
   - Show "Resting..." text
   - Progress bar fills (2s)
   - Energy: 45% → 95% (animate bar)
   - Green "+50" indicator
   - Fade back in (0.5s)
   - Return to menu

5. **Full Rest Flow** (4 seconds total)
   - Check funds ($40)
   - Deduct money: $1250 → $1210
   - Fade to black (0.5s)
   - Show "Sleeping..." with moon icon
   - Progress bar fills (2.5s)
   - Energy: 45% → 100% (animate bar)
   - Health: 85% → 100% (if health system exists)
   - Morale: +15 for all companions
   - **Auto-save triggers**
     - Show save icon (💾)
     - "Game Saved" notification
   - Advance time: 14:30 → 22:30 (8 hours)
   - Apply "Well Rested" buff (30 minutes, +20% energy regen)
   - Fade back in (0.5s)
   - Return to menu or auto-exit to node

6. **Leave**
   - Fade out (0.3s)
   - Return to NodeScene

**Additional Services (Optional - Phase 6):**
- Shower: $10, +10 morale
- Meal: $15, +20 energy
- Laundry: $5, +5 morale
- Parking: Free, +10 car security

**Technical Requirements:**
```cpp
// Rest costs
constexpr float QUICK_REST_COST = 20.0f;
constexpr float FULL_REST_COST = 40.0f;

// Energy restoration
constexpr float QUICK_REST_ENERGY = 50.0f;
constexpr float FULL_REST_ENERGY = 100.0f;

// Time advancement
constexpr float QUICK_REST_HOURS = 0.5f;
constexpr float FULL_REST_HOURS = 8.0f;

// Animation timings
constexpr float REST_ANIMATION_DURATION = 2.0f;
constexpr float FADE_DURATION = 0.5f;
```

#### 5.3.3 GameOverScene (1 day)
**Purpose:** Display game over state and options

**Implementation Details:**

**Files to create:**
- `src/GameOverScene.cpp` (header already exists)

**Core Features:**
```cpp
class GameOverScene : public Scene {
private:
    enum class GameOverReason {
        NO_FUEL,           // Ran out of fuel
        NO_MONEY,          // Bankrupt
        CAR_BROKEN,        // Car destroyed
        QUEST_FAILED,      // Critical quest failed
        PLAYER_DEATH,      // Player died (if health system)
        GOAL_REACHED       // Successfully reached destination (WIN)
    };

    GameOverReason m_reason;
    std::string m_reasonText;

    // Stats to display
    struct GameStats {
        float distanceTraveled;
        int daysPlayed;
        int questsCompleted;
        int eventsEncountered;
        float moneyEarned;
    } m_stats;

    enum class MenuOption {
        RESTART,
        LOAD_SAVE,
        MAIN_MENU,
        EXIT
    };
    MenuOption m_selectedOption;

public:
    GameOverScene(GameOverReason reason);

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    void loadGameStats();
    std::string getReasonText(GameOverReason reason);
};
```

**Micro-moments - Game Over flow:**

1. **Trigger** (from any scene)
   - Player fuel reaches 0% → trigger NO_FUEL
   - Player money < $0 → trigger NO_MONEY
   - Car condition 0% → trigger CAR_BROKEN
   - Reach final destination → trigger GOAL_REACHED

2. **Transition** (1s)
   - Fade to black from current scene (0.5s)
   - Show reason-specific animation:
     - NO_FUEL: Gas gauge drops to empty
     - NO_MONEY: Wallet empties
     - CAR_BROKEN: Car smoking/broken
     - GOAL_REACHED: Success fanfare

3. **Display Game Over Screen**
   ```
   ═══════════════════════════════════════
          GAME OVER - Out of Fuel
   ═══════════════════════════════════════

   You ran out of fuel in the middle of nowhere.
   Your journey ends here.

   ───────────── STATISTICS ─────────────
   Distance Traveled:    1,247 km
   Days Played:          12
   Quests Completed:     5 / 8
   Events Encountered:   23
   Money Earned:         $3,450
   ───────────────────────────────────────

   > [1] Restart from Last Save
     [2] Load Different Save
     [3] Main Menu
     [4] Exit Game
   ```

4. **Menu Navigation**
   - W/S to navigate
   - Enter to select
   - Escape = Main Menu

5. **Option Actions**
   - **Restart:** Load last auto-save → return to game
   - **Load Save:** Open load menu → select save
   - **Main Menu:** Return to MainMenuScene
   - **Exit:** Close application

**Win Condition (GOAL_REACHED):**
```
═══════════════════════════════════════
     CONGRATULATIONS - JOURNEY COMPLETE
═══════════════════════════════════════

You have successfully reached Vladivostok!
Your adventure across Russia is complete.

───────────── FINAL STATISTICS ─────────────
Distance Traveled:    6,417 km
Days Played:          28
Quests Completed:     15 / 18
Events Encountered:   89
Money Remaining:      $2,340
Companions:           3

Final Score:          8,750 points
Rank:                 S (Master Traveler)
───────────────────────────────────────────

[Special ending based on choices]
[Credits roll]

> [1] Play Again
  [2] Main Menu
  [3] Exit
```

**Technical Requirements:**
```cpp
// Statistics tracking (add to PlayerState)
struct GameStatistics {
    float totalDistance = 0.0f;
    int daysPlayed = 0;
    int questsCompleted = 0;
    int questsTotal = 0;
    int eventsEncountered = 0;
    float totalMoneyEarned = 0.0f;
};

// Score calculation
int calculateFinalScore() {
    int score = 0;
    score += (int)(stats.totalDistance / 10);  // 1 point per 10km
    score += stats.questsCompleted * 100;      // 100 per quest
    score += stats.eventsEncountered * 10;     // 10 per event
    score += (int)(stats.totalMoneyEarned / 10); // 1 per $10

    // Time bonus (faster = better)
    if (stats.daysPlayed < 20) score += 1000;
    else if (stats.daysPlayed < 30) score += 500;

    return score;
}

// Rank determination
std::string getRank(int score) {
    if (score >= 10000) return "S - Master Traveler";
    if (score >= 7500) return "A - Expert Driver";
    if (score >= 5000) return "B - Skilled Adventurer";
    if (score >= 2500) return "C - Novice Explorer";
    return "D - Lucky Survivor";
}
```

**Deliverables:**
- [ ] DialogueScene fully functional
- [ ] MotelScene with rest options
- [ ] GameOverScene with all endings
- [ ] All scenes integrated into SceneManager
- [ ] Tests for scene transitions

---

### 5.4 DELETE/MERGE UNUSED SCENES (1 day)
**Priority:** 🟡 MEDIUM

#### Scenes to DELETE:
1. **TownScene** - Merged into NodeScene
   - Delete: `include/TownScene.h`
   - Remove from SceneType enum
   - Remove from SceneManager factory

2. **RouteSelectionScene** - Replaced by TravelSelectionScene
   - Delete: `include/RouteSelectionScene.h`
   - Remove from SceneType enum
   - Update any references to TravelSelectionScene

3. **WorldMapScene** - Replaced by MapScene
   - Delete: `include/WorldMapScene.h`
   - Remove from SceneType enum

4. **ExampleResponsiveScene** - Example code, not for production
   - Delete: `include/ExampleResponsiveScene.h`
   - Delete: `src/ExampleResponsiveScene.cpp`
   - Remove from SceneType enum

5. **PauseMenuScene** - Optional, low priority
   - Decision: Keep header, implement in Phase 6
   - Or delete if not needed (use Escape = auto-pause)

#### Headers to DELETE:
1. **Services** (3 files)
   - `include/GarageService.h` - not used
   - `include/StoreService.h` - not used
   - `include/WorkAgencyService.h` - not used

2. **UI** (2 files)
   - `include/TownMenu.h` - replaced by NodeScene
   - `include/InteractionMenu.h` - not used

3. **Systems** (2 files)
   - `include/TutorialSystem.h` - no tutorial planned
   - `include/POIManager.h` - not used

4. **World** (3 files)
   - `include/LocationGenerator.h` - not used (manual placement)
   - `include/StructurePlacement.h` - not used
   - `include/StartZone.h` - not used

**Total to delete:** ~13-15 files

**Deliverables:**
- [ ] Delete all unused scene headers
- [ ] Delete all unused system headers
- [ ] Update SceneType enum
- [ ] Update SceneManager.cpp
- [ ] Update CMakeLists.txt
- [ ] Verify compilation
- [ ] Update documentation

---

### 5.5 TAB-BASED NAVIGATION REFINEMENT (2 days)
**Priority:** 🟡 MEDIUM

**Current State:**
- Tab system implemented in NodeScene
- 12 tabs maximum
- Empty tabs shown as `[ ]`
- Fixed tabs: CHR, INV, COM, QST, ABL, TRV, NOT, MAP
- Dynamic structure tabs: GAS, STR, MOT, etc.

**Refinement Needed:**

#### 5.5.1 Dynamic Structure Loading
```cpp
void NodeScene::loadStructures() {
    m_tabs.clear();

    // Add 8 fixed system tabs first
    addSystemTabs();

    // Load structures from current location
    LocationData& location = GameStateManager::getInstance().getCurrentLocation();
    const std::vector<Structure>& structures = location.getStructures();

    // Add structure tabs
    for (const auto& structure : structures) {
        if (m_tabs.size() >= MAX_TABS) break;

        Tab structureTab;
        structureTab.name = getStructureAbbreviation(structure.type);
        structureTab.fullName = structure.name;
        structureTab.targetScene = getStructureScene(structure.type);
        structureTab.isStructure = true;
        m_tabs.push_back(structureTab);
    }

    // Fill remaining with empty tabs
    while (m_tabs.size() < MAX_TABS) {
        Tab emptyTab;
        emptyTab.name = "";  // Renders as [ ]
        m_tabs.push_back(emptyTab);
    }
}

std::string NodeScene::getStructureAbbreviation(LocationType type) {
    switch(type) {
        case LocationType::GAS_STATION: return "GAS";
        case LocationType::STORE: return "STR";
        case LocationType::MOTEL: return "MOT";
        case LocationType::GARAGE: return "GAR";
        case LocationType::DINER: return "DIN";
        default: return "???";
    }
}
```

#### 5.5.2 Tab Rendering Improvements
```cpp
void NodeScene::renderTopBar(sf::RenderWindow& window) {
    const float TAB_WIDTH = 100.0f;
    const float TAB_HEIGHT = 50.0f;
    const float SPACING = 5.0f;
    const float START_X = 10.0f;
    const float START_Y = 10.0f;

    for (size_t i = 0; i < m_tabs.size(); ++i) {
        const Tab& tab = m_tabs[i];

        // Tab background
        sf::RectangleShape tabBg;
        tabBg.setSize(sf::Vector2f(TAB_WIDTH, TAB_HEIGHT));
        tabBg.setPosition(sf::Vector2f(
            START_X + i * (TAB_WIDTH + SPACING),
            START_Y
        ));

        // Color based on state
        if (i == m_currentTabIndex) {
            tabBg.setFillColor(sf::Color(0, 255, 100));      // Active: bright green
            tabBg.setOutlineColor(sf::Color(0, 255, 100));
        } else if (tab.name.empty()) {
            tabBg.setFillColor(sf::Color(40, 40, 50));       // Empty: dark gray
            tabBg.setOutlineColor(sf::Color(80, 80, 90));
        } else {
            tabBg.setFillColor(sf::Color(20, 20, 30));       // Inactive: darker
            tabBg.setOutlineColor(sf::Color(0, 200, 0));
        }
        tabBg.setOutlineThickness(2.0f);
        window.draw(tabBg);

        // Tab text
        std::string displayText = tab.name.empty() ? "[ ]" : tab.name;
        sf::Text tabText(m_font, displayText, 16);
        tabText.setFillColor(i == m_currentTabIndex ?
                            sf::Color::Black :
                            sf::Color(0, 255, 100));

        // Center text in tab
        sf::FloatRect textBounds = tabText.getLocalBounds();
        tabText.setPosition(sf::Vector2f(
            START_X + i * (TAB_WIDTH + SPACING) + (TAB_WIDTH - textBounds.size.x) / 2,
            START_Y + (TAB_HEIGHT - textBounds.size.y) / 2
        ));
        window.draw(tabText);
    }
}
```

#### 5.5.3 Tab Description Panel
```cpp
void NodeScene::renderDescriptionPanel(sf::RenderWindow& window) {
    if (m_currentTabIndex >= m_tabs.size()) return;

    const Tab& currentTab = m_tabs[m_currentTabIndex];
    if (currentTab.name.empty()) return;  // Skip empty tabs

    // Panel background
    sf::RectangleShape panel;
    panel.setSize(sf::Vector2f(1200.0f, 150.0f));
    panel.setPosition(sf::Vector2f(50.0f, 100.0f));
    panel.setFillColor(sf::Color(20, 20, 30));
    panel.setOutlineColor(sf::Color(0, 200, 0));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);

    // Title
    sf::Text title(m_font, currentTab.fullName, 28);
    title.setFillColor(sf::Color(0, 255, 100));
    title.setPosition(sf::Vector2f(70.0f, 120.0f));
    window.draw(title);

    // Description
    sf::Text desc(m_font, currentTab.description, 16);
    desc.setFillColor(sf::Color(200, 200, 200));
    desc.setPosition(sf::Vector2f(70.0f, 160.0f));
    window.draw(desc);

    // Action hint
    std::string actionText = currentTab.name.empty() ?
        "" : "[Enter] Open  [A/D] Navigate Tabs  [Esc] Exit";
    sf::Text action(m_font, actionText, 14);
    action.setFillColor(sf::Color(150, 150, 150));
    action.setPosition(sf::Vector2f(70.0f, 210.0f));
    window.draw(action);
}
```

**Deliverables:**
- [ ] Dynamic structure tab loading
- [ ] Improved tab rendering (colors, borders)
- [ ] Description panel for selected tab
- [ ] Keyboard shortcuts documentation
- [ ] Empty tab handling

---

### 5.6 TRAVEL SYSTEM REFINEMENT (3 days)
**Priority:** 🔴 HIGH

**Current Implementation:**
- TravelSelectionScene: Menu-based route selection
- NotesScene: Text notepad for player notes
- MapScene: Keep Driving style visual reference

**Refinement Needed:**

#### 5.6.1 City Connection System
```cpp
// New file: include/CityGraph.h
class CityGraph {
public:
    struct City {
        std::string id;
        std::string name;
        std::string number;     // e.g., "01", "02"
        sf::Vector2f mapPosition;
        std::vector<std::string> connectedCities;
    };

    struct Route {
        std::string fromCityId;
        std::string toCityId;
        float distance;         // km
        float fuelCost;         // liters
        int estimatedTime;      // minutes
        RoadType roadType;      // HIGHWAY, ROAD, PATH
    };

    static CityGraph& getInstance();

    void loadFromJSON(const std::string& filepath);
    const City* getCity(const std::string& cityId) const;
    std::vector<Route> getAvailableRoutes(const std::string& fromCityId) const;
    bool areConnected(const std::string& city1, const std::string& city2) const;

private:
    std::unordered_map<std::string, City> m_cities;
    std::vector<Route> m_routes;
};
```

**City Data (JSON format):**
```json
{
  "cities": [
    {
      "id": "moscow",
      "name": "Moscow",
      "number": "01",
      "mapPosition": [200, 450],
      "connections": ["kazan", "nizhny", "tver"]
    },
    {
      "id": "kazan",
      "name": "Kazan",
      "number": "02",
      "mapPosition": [500, 400],
      "connections": ["moscow", "yekaterinburg", "ufa"]
    }
  ],
  "routes": [
    {
      "from": "moscow",
      "to": "kazan",
      "distance": 797.0,
      "fuelCost": 23.9,
      "estimatedTime": 720,
      "roadType": "HIGHWAY"
    }
  ]
}
```

#### 5.6.2 Enhanced TravelSelectionScene
```cpp
void TravelSelectionScene::loadDestinations() {
    m_destinations.clear();

    CityGraph& graph = CityGraph::getInstance();
    std::vector<CityGraph::Route> routes = graph.getAvailableRoutes(m_currentCityId);

    for (const auto& route : routes) {
        CityConnection conn;
        conn.cityId = route.toCityId;

        const CityGraph::City* city = graph.getCity(route.toCityId);
        if (city) {
            conn.cityName = city->name;
            conn.cityNumber = city->number;
        }

        conn.distance = route.distance;
        conn.fuelCost = route.fuelCost;
        conn.estimatedTime = route.estimatedTime;
        conn.roadType = route.roadType;

        // Check if accessible (enough fuel)
        PlayerState& player = GameStateManager::getInstance().getPlayerState();
        conn.isAccessible = (player.getFuel() >= route.fuelCost);

        m_destinations.push_back(conn);
    }

    // Sort by distance (closest first)
    std::sort(m_destinations.begin(), m_destinations.end(),
        [](const CityConnection& a, const CityConnection& b) {
            return a.distance < b.distance;
        });
}
```

**Enhanced Rendering:**
```cpp
void TravelSelectionScene::renderDestinations(sf::RenderWindow& window) {
    float yPos = 200.0f;

    for (size_t i = 0; i < m_destinations.size(); ++i) {
        const CityConnection& dest = m_destinations[i];

        // Selection highlight
        if (i == m_selectedIndex) {
            sf::RectangleShape highlight;
            highlight.setSize(sf::Vector2f(1220.0f, 80.0f));
            highlight.setPosition(sf::Vector2f(40.0f, yPos - 5.0f));
            highlight.setFillColor(sf::Color(0, 100, 0, 100));
            window.draw(highlight);
        }

        // City number and name
        std::string cityText = (i == m_selectedIndex ? "> " : "  ");
        cityText += "[" + dest.cityNumber + "] " + dest.cityName;

        sf::Text nameText(m_font, cityText, 24);
        nameText.setFillColor(dest.isAccessible ?
            (i == m_selectedIndex ? sf::Color(255, 200, 0) : sf::Color::White) :
            sf::Color(100, 100, 100));
        nameText.setPosition(sf::Vector2f(60.0f, yPos));
        window.draw(nameText);

        // Distance and fuel
        std::string infoText =
            "Distance: " + std::to_string(static_cast<int>(dest.distance)) + " km  |  " +
            "Fuel: " + std::to_string(static_cast<int>(dest.fuelCost)) + " L  |  " +
            "Time: " + formatTime(dest.estimatedTime) + "  |  " +
            "Road: " + roadTypeToString(dest.roadType);

        sf::Text infoTextObj(m_font, infoText, 14);
        infoTextObj.setFillColor(sf::Color(180, 180, 180));
        infoTextObj.setPosition(sf::Vector2f(80.0f, yPos + 30.0f));
        window.draw(infoTextObj);

        // Accessibility warning
        if (!dest.isAccessible) {
            sf::Text warning(m_font, "⚠ Not enough fuel!", 14);
            warning.setFillColor(sf::Color(255, 0, 0));
            warning.setPosition(sf::Vector2f(80.0f, yPos + 55.0f));
            window.draw(warning);
        }

        yPos += 90.0f;
    }
}

std::string TravelSelectionScene::formatTime(int minutes) {
    int hours = minutes / 60;
    int mins = minutes % 60;
    return std::to_string(hours) + "h " + std::to_string(mins) + "m";
}

std::string TravelSelectionScene::roadTypeToString(RoadType type) {
    switch(type) {
        case RoadType::HIGHWAY: return "Highway";
        case RoadType::ROAD: return "Road";
        case RoadType::PATH: return "Path";
        default: return "Unknown";
    }
}
```

#### 5.6.3 Static Map Enhancement
```cpp
// MapScene: Full Russia map with all cities

void MapScene::initializeMap() {
    m_nodes.clear();

    // Load all cities from CityGraph
    CityGraph& graph = CityGraph::getInstance();

    // Full city list (30+ cities)
    // Positioned to represent actual geography
    addCity("moscow", "Moscow", "01", 300, 450, true);
    addCity("kazan", "Kazan", "02", 550, 400, false);
    addCity("yekaterinburg", "Yekaterinburg", "03", 750, 380, false);
    addCity("omsk", "Omsk", "04", 900, 350, false);
    addCity("novosibirsk", "Novosibirsk", "05", 1000, 330, false);
    // ... 25 more cities
    addCity("vladivostok", "Vladivostok", "30", 1200, 420, false);
}

void MapScene::addCity(const std::string& id, const std::string& name,
                       const std::string& number, float x, float y,
                       bool isUnlocked) {
    MapNode node;
    node.id = id;
    node.name = name + " [" + number + "]";
    node.position = sf::Vector2f(x, y);
    node.isUnlocked = isUnlocked;
    node.isCurrentNode = (id == m_currentNodeId);
    m_nodes.push_back(node);
}

void MapScene::renderConnections(sf::RenderWindow& window) {
    CityGraph& graph = CityGraph::getInstance();

    for (const auto& node : m_nodes) {
        if (!node.isUnlocked) continue;

        const CityGraph::City* city = graph.getCity(node.id);
        if (!city) continue;

        // Draw connections to all connected cities
        for (const std::string& connectedId : city->connectedCities) {
            const MapNode* connectedNode = findNode(connectedId);
            if (!connectedNode) continue;

            // Line from this node to connected node
            sf::Vertex line[2];
            line[0].position = node.position;
            line[1].position = connectedNode->position;

            // Color based on accessibility
            sf::Color lineColor = connectedNode->isUnlocked ?
                sf::Color(0, 200, 0, 150) :   // Green if unlocked
                sf::Color(100, 100, 100, 100); // Gray if locked

            line[0].color = lineColor;
            line[1].color = lineColor;

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }
}
```

#### 5.6.4 NotesScene Template Enhancement
```cpp
void NotesScene::loadNotes() {
    std::ifstream file("saves/player_notes.txt");
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        m_notesContent = buffer.str();
    } else {
        // Enhanced template with more structure
        m_notesContent =
            "═══════════════════════════════════════\n"
            "       TRAVEL NOTES & PLANNING\n"
            "═══════════════════════════════════════\n"
            "\n"
            "CITY CONNECTIONS:\n"
            "──────────────────────────────────────\n"
            "[01] Moscow → [02] Kazan (797km)\n"
            "[01] Moscow → [03] Nizhny (422km)\n"
            "\n"
            "\n"
            "ROUTE PLAN:\n"
            "──────────────────────────────────────\n"
            "1. Moscow [01] START\n"
            "2. \n"
            "3. \n"
            "\n"
            "\n"
            "QUESTS & OBJECTIVES:\n"
            "──────────────────────────────────────\n"
            "[ ] \n"
            "[ ] \n"
            "\n"
            "\n"
            "IMPORTANT NOTES:\n"
            "──────────────────────────────────────\n"
            "\n"
            "\n"
            "═══════════════════════════════════════\n";
    }

    m_cursorPosition = m_notesContent.length();
}
```

**Deliverables:**
- [ ] CityGraph system with JSON loading
- [ ] Enhanced TravelSelectionScene with road types
- [ ] Full Russia map with 30+ cities
- [ ] Connection visualization
- [ ] Enhanced notes template
- [ ] Testing with multiple routes

---

### 5.7 RESPONSIVE LAYOUT FINALIZATION (2 days)
**Priority:** 🟡 MEDIUM

**Goal:** Ensure all UI elements work perfectly at 1300x800

#### Update UIConstants.h
```cpp
namespace UI {
    // Screen dimensions
    constexpr float SCREEN_WIDTH = 1300.0f;
    constexpr float SCREEN_HEIGHT = 800.0f;

    // Safe area (accounting for borders)
    constexpr float SAFE_MARGIN = 40.0f;
    constexpr float SAFE_WIDTH = SCREEN_WIDTH - (2 * SAFE_MARGIN);
    constexpr float SAFE_HEIGHT = SCREEN_HEIGHT - (2 * SAFE_MARGIN);

    // Font sizes (optimized for 1300x800)
    constexpr unsigned int FONT_SIZE_HUGE = 36;
    constexpr unsigned int FONT_SIZE_LARGE = 28;
    constexpr unsigned int FONT_SIZE_SUBTITLE = 24;
    constexpr unsigned int FONT_SIZE_NORMAL = 20;
    constexpr unsigned int FONT_SIZE_SMALL = 16;
    constexpr unsigned int FONT_SIZE_TINY = 14;

    // Layout helpers
    inline float centerX(float elementWidth) {
        return (SCREEN_WIDTH - elementWidth) / 2.0f;
    }

    inline float centerY(float elementHeight) {
        return (SCREEN_HEIGHT - elementHeight) / 2.0f;
    }

    inline sf::Vector2f center(float width, float height) {
        return sf::Vector2f(centerX(width), centerY(height));
    }
}
```

#### Update all scenes to use UIConstants
- MainMenuScene
- CharacterCreationMenu
- NodeScene
- TravelSelectionScene
- NotesScene
- All other scenes

**Deliverables:**
- [ ] UIConstants.h finalized
- [ ] All scenes use UIConstants
- [ ] No hardcoded dimensions
- [ ] Test on 1300x800 screen
- [ ] Document layout system

---

## PHASE 6: CONTENT EXPANSION
**Timeline:** 3-4 weeks
**Priority:** 🟡 MEDIUM

### 6.1 EVENT EXPANSION (1 week)

**Goal:** Expand from 5 events to 20+ unique events

**Current Events:**
1. Gas Station (fuel < 30%)
2. Hitchhiker (+200₽, -10 energy)
3. Police Checkpoint (-300₽ or worse)
4. Breakdown (energy < 40%)
5. Rest Stop (+20 energy, -150₽)

**New Events to Add (15 more):**

1. **Roadside Mechanic**
   - Condition: Car condition < 60%
   - Choices:
     - Pay for repair ($150)
     - Attempt self-repair (skill check)
     - Ignore and continue
   - Consequences:
     - Paid: +40% car condition
     - Self: 50% chance +30%, 50% chance -10%
     - Ignore: Risk breakdown

2. **Suspicious Cargo Offer**
   - Condition: Random, cargo space > 50%
   - Choices:
     - Accept ($500, illegal cargo, risk)
     - Decline (safe)
   - Consequences:
     - Accept: +$500, 30% chance police find it
     - Found: -$1000 fine, -reputation
     - Not found: +$500 profit

3. **Accident on Road**
   - Condition: Random
   - Choices:
     - Stop and help (-30 min, +relationship)
     - Call emergency and continue (-10 min)
     - Drive past (no penalty)
   - Consequences:
     - Help: May meet recruitable NPC
     - Call: +karma
     - Pass: -karma

4. **Food Vendor**
   - Condition: Random
   - Choices:
     - Buy meal ($25, +15 energy)
     - Buy snack ($10, +5 energy)
     - Pass
   - Consequences: Energy restoration

5. **Weather: Heavy Rain**
   - Condition: Random, 20% chance
   - Effect: -20% fuel efficiency for this trip
   - Choices:
     - Wait it out (-30 min, no penalty)
     - Continue (fuel penalty)
   - No money cost

6. **Abandoned Car**
   - Condition: Random
   - Choices:
     - Search for parts (find random item)
     - Search for fuel (10-30L)
     - Ignore
   - Consequences:
     - Parts: Random car part item
     - Fuel: +fuel
     - Risk: 10% chance encounter bandits

7. **Toll Road**
   - Condition: Highway route
   - Choices:
     - Pay toll ($50, faster route, -30min)
     - Take detour (free, +1 hour)
   - Consequences: Time vs money trade-off

8. **Motel Robbery**
   - Condition: Resting at motel, 5% chance
   - Effect: Lose 10-30% of money
   - Choices:
     - Fight back (skill check, risky)
     - Let them take it (safe, lose money)
   - Consequences:
     - Fight success: Keep money, minor injury
     - Fight fail: Lose money + injury
     - Let them: Only lose money

9. **Wildlife Crossing**
   - Condition: Road/Path routes, 15% chance
   - Choices:
     - Brake suddenly (-5% car condition)
     - Swerve (skill check)
     - Hit animal (-20% car condition, meat item)
   - Consequences: Car damage vs food

10. **Friendly Local**
    - Condition: Random at cities
    - Choices:
      - Accept local tips (learn shortcuts)
      - Decline politely
    - Consequences:
      - Accept: Unlock secret route
      - Bonus: 20% chance recruit as companion

11. **Car Thieves**
    - Condition: Leaving car unattended
    - Effect: 10% chance car theft attempt
    - Choices:
      - Fight (risky, keep car)
      - Call police (safe, lose time)
      - Car security system (if installed)
    - Consequences:
      - Success: Keep car
      - Fail: Lose car → GAME OVER

12. **Rare Item Merchant**
    - Condition: Random, 5% chance
    - Choices:
      - Buy rare item ($500)
      - Haggle (skill check, $350)
      - Decline
    - Consequences: Access to unique items

13. **Companion Personal Quest**
    - Condition: Have companion, random
    - Effect: Companion asks for personal favor
    - Choices:
      - Help (+relationship, side quest)
      - Decline (-relationship)
    - Consequences: Companion loyalty

14. **Shortcut Discovery**
    - Condition: Exploration skill check
    - Effect: Find unmarked shortcut
    - Consequences:
      - Save 20% distance
      - Unlock for future use
      - But: rough road, more fuel

15. **Rest Area Encounter**
    - Condition: Random at rest stops
    - Choices:
      - Trade items with traveler
      - Share stories (+morale)
      - Rest alone
    - Consequences:
      - Trade: Barter system
      - Stories: +morale for party

**Event Implementation Template:**
```cpp
// data/events/event_roadside_mechanic.json
{
  "id": "roadside_mechanic",
  "title": "Roadside Mechanic",
  "description": "You notice a small garage by the roadside. A mechanic waves you over, offering to check your car.",
  "condition": {
    "carCondition": {"max": 60.0},
    "probability": 0.3
  },
  "choices": [
    {
      "id": "pay_repair",
      "text": "Pay for professional repair ($150)",
      "requirements": {
        "money": 150.0
      },
      "consequences": {
        "money": -150.0,
        "carCondition": "+40%"
      }
    },
    {
      "id": "self_repair",
      "text": "Attempt to repair it yourself (Mechanics skill check)",
      "requirements": {
        "skill": "mechanics",
        "skillLevel": 2
      },
      "consequences": {
        "success": {
          "carCondition": "+30%",
          "message": "You successfully repaired the car!"
        },
        "failure": {
          "carCondition": "-10%",
          "message": "You made it worse..."
        }
      }
    },
    {
      "id": "ignore",
      "text": "Decline and continue driving",
      "consequences": {
        "message": "You drive on, hoping the car holds up."
      }
    }
  ]
}
```

**Deliverables:**
- [ ] 15 new events designed
- [ ] JSON files for each event
- [ ] EventManager.cpp updated to load from JSON
- [ ] Event probability balancing
- [ ] Testing all event triggers
- [ ] Documentation

---

### 6.2 QUEST EXPANSION (1 week)

**Goal:** Add 15+ diverse quests

**Current Quests:**
- Basic placeholder quests (2-3)

**New Quest Types:**

**Main Quests (5):**
1. **Cross-Country Delivery**
   - Objective: Deliver package from Moscow to Vladivostok
   - Reward: $2000, 500 XP
   - Time limit: 30 days
   - Special: Unlocks special ending

2. **Find the Lost Convoy**
   - Objective: Locate missing supply convoy
   - Multi-stage: Talk to NPCs → Follow clues → Find location
   - Reward: $1500, 400 XP, rare items

3. **Prove Your Driving Skills**
   - Objective: Complete challenging route under time limit
   - Requirement: Pass through 5 checkpoints
   - Reward: $1000, 300 XP, "Master Driver" title

4. **Rescue Operation**
   - Objective: Rescue stranded travelers
   - Requirement: Must have vehicle space
   - Reward: $800, 250 XP, +3 companions

5. **Investigate Rumors**
   - Objective: Investigate strange occurrences
   - Multi-stage mystery quest
   - Reward: $1200, 350 XP, unique item

**Side Quests (10):**

1. **Fuel Crisis**
   - Objective: Deliver fuel to remote gas station
   - Reward: $300, free fuel forever at that station

2. **Collector's Items**
   - Objective: Find 5 rare items across Russia
   - Reward: $500, rare collectible

3. **Mechanic's Apprentice**
   - Objective: Help mechanic with 3 repairs
   - Reward: $400, +2 Mechanics skill

4. **The Hitchhiker's Tales**
   - Objective: Give rides to 10 different hitchhikers
   - Reward: $600, +Social skill

5. **Road Photography**
   - Objective: Visit 15 scenic locations
   - Reward: $350, achievement

6. **Black Market Connection**
   - Objective: Find black market contact in each region
   - Reward: Access to black market shops

7. **Companion Loyalty**
   - Objective: Reach max relationship with any companion
   - Reward: Unique companion ability

8. **Speed Demon**
   - Objective: Complete route in record time
   - Reward: $700, "Speed Demon" title

9. **Generous Traveler**
   - Objective: Help 20 people on the road
   - Reward: $500, +Karma

10. **Master Trader**
    - Objective: Earn $5000 through trading
    - Reward: $1000 bonus, "Master Trader" title

**Quest Implementation:**
```json
{
  "quests": [
    {
      "id": "cross_country_delivery",
      "title": "Cross-Country Delivery",
      "description": "A mysterious client needs a package delivered to Vladivostok. No questions asked.",
      "questType": "MAIN",
      "objectives": [
        {
          "description": "Pick up package in Moscow",
          "type": "TRAVEL_TO",
          "targetId": "moscow"
        },
        {
          "description": "Deliver package to Vladivostok",
          "type": "TRAVEL_TO",
          "targetId": "vladivostok"
        }
      ],
      "requirements": {
        "minimumLevel": 1,
        "cargoSpace": 10
      },
      "timeLimit": 2592000,
      "rewards": {
        "money": 2000.0,
        "experience": 500,
        "items": [
          {"id": "rare_item_1", "quantity": 1}
        ],
        "unlocks": "special_ending"
      },
      "failureConditions": {
        "timeExpired": "Package delivery failed - time expired",
        "cargoLost": "Package delivery failed - cargo lost"
      }
    }
  ]
}
```

**Deliverables:**
- [ ] 15 quests designed and documented
- [ ] JSON files for all quests
- [ ] QuestManager JSON loading complete
- [ ] Multi-objective quest support
- [ ] Time-limited quest system
- [ ] Quest tracking in QuestsScene
- [ ] Quest reward system functional
- [ ] Testing all quest flows

---

### 6.3 NPC EXPANSION (1 week)

**Goal:** Add 15+ unique recruitable NPCs

**NPC Categories:**

**Mechanics (3 NPCs):**
1. **Ivan - The Veteran Mechanic**
   - Location: Kazan garage
   - Ability: -25% repair costs
   - Recruitment: Complete "Mechanic's Apprentice" quest
   - Backstory: Ex-Soviet military mechanic

2. **Olga - The Innovator**
   - Location: Yekaterinburg
   - Ability: Improve fuel efficiency +15%
   - Recruitment: Pay $500 or high Mechanics skill
   - Backstory: Engineer from closed factory

3. **Boris - The Welder**
   - Location: Omsk
   - Ability: Field repairs possible (anywhere)
   - Recruitment: Help in "Broken Down" event
   - Backstory: Traveling craftsman

**Traders (3 NPCs):**
1. **Dmitri - The Merchant**
   - Location: Moscow
   - Ability: -20% shop prices
   - Recruitment: $300 or complete trade quest
   - Backstory: Black market connections

2. **Elena - The Negotiator**
   - Location: Novosibirsk
   - Ability: +30% sell prices
   - Recruitment: Win dialogue skill check
   - Backstory: Former diplomat

3. **Alexei - The Smuggler**
   - Location: Secret location
   - Ability: Access to illegal goods
   - Recruitment: Find through rumors
   - Backstory: Underground network

**Drivers (3 NPCs):**
1. **Katya - The Racer**
   - Location: Moscow underground race
   - Ability: +20% driving speed
   - Recruitment: Win racing challenge
   - Backstory: Street racing champion

2. **Mikhail - The Trucker**
   - Location: Highway rest stop
   - Ability: +25% cargo capacity
   - Recruitment: Help with delivery
   - Backstory: Long-haul veteran

3. **Anna - The Navigator**
   - Location: St. Petersburg
   - Ability: Find shortcuts (discover hidden routes)
   - Recruitment: Complete navigation quest
   - Backstory: Geographer and explorer

**Survivalists (3 NPCs):**
1. **Viktor - The Hunter**
   - Location: Forest region
   - Ability: Find food/supplies in wilderness
   - Recruitment: Encounter in "Wildlife" event
   - Backstory: Forest ranger

2. **Natasha - The Medic**
   - Location: Hospital/clinic
   - Ability: Restore health +50% faster
   - Recruitment: $400 or help in emergency
   - Backstory: Field medic

3. **Sergei - The Survivalist**
   - Location: Remote area
   - Ability: +20% energy efficiency
   - Recruitment: Complete survival challenge
   - Backstory: Ex-special forces

**Social (3 NPCs):**
1. **Yuri - The Storyteller**
   - Location: Tavern
   - Ability: +morale for all companions
   - Recruitment: Buy him drinks ($50)
   - Backstory: Traveling bard

2. **Irina - The Psychologist**
   - Location: City clinic
   - Ability: Prevent morale loss
   - Recruitment: Complete therapy session
   - Backstory: Former university professor

3. **Pavel - The Diplomat**
   - Location: Government building
   - Ability: Better dialogue outcomes
   - Recruitment: High Social skill
   - Backstory: Retired bureaucrat

**NPC Data Structure:**
```json
{
  "npcs": [
    {
      "id": "ivan_mechanic",
      "name": "Ivan",
      "title": "The Veteran Mechanic",
      "category": "MECHANICS",
      "level": 5,
      "ability": {
        "id": "reduce_repair_cost",
        "name": "Expert Repairs",
        "description": "Reduces all repair costs by 25%",
        "type": "PASSIVE",
        "effect": {
          "repairCostMultiplier": 0.75
        }
      },
      "recruitment": {
        "location": "kazan_garage",
        "requirements": {
          "quest": "mechanics_apprentice",
          "or": {
            "money": 600,
            "skill": {"mechanics": 3}
          }
        }
      },
      "personality": {
        "likes": ["technical_talk", "vodka", "hard_work"],
        "dislikes": ["laziness", "cheap_parts"],
        "moraleModifiers": {
          "goodRepair": +5,
          "failedRepair": -10
        }
      },
      "backstory": "Ivan served as a mechanic in the Soviet Army. After the collapse, he opened a small garage in Kazan. He's seen every type of vehicle breakdown imaginable.",
      "dialogue": {
        "greeting": "Need something fixed? I've worked on worse than this rust bucket.",
        "recruited": "Alright, I'll come along. But I expect proper tools!",
        "happy": "This car runs like a dream now!",
        "unhappy": "If you don't take care of this vehicle, I'm leaving."
      }
    }
  ]
}
```

**NPC System Features:**
- Relationship system (0-100)
- Morale system (affects performance)
- Unique dialogues
- Personal quests
- Level progression
- Can leave if unhappy

**Deliverables:**
- [ ] 15 NPCs designed
- [ ] JSON data for all NPCs
- [ ] NPC recruitment system
- [ ] Relationship/morale system
- [ ] Companion management in CompanionsScene
- [ ] NPC abilities integration
- [ ] Testing recruitment flows

---

### 6.4 CITY/LOCATION EXPANSION (1 week)

**Goal:** Create 30+ cities across Russia map

**Regions:**

**Western Russia (5 cities):**
1. Moscow [01] - Capital, major hub
2. St. Petersburg [02] - Northern port city
3. Nizhny Novgorod [03] - Industrial center
4. Kazan [04] - Cultural center
5. Tver [05] - Historical town

**Volga Region (5 cities):**
6. Samara [06] - River port
7. Ufa [07] - Mountain gateway
8. Perm [08] - Industrial hub
9. Saratov [09] - Agricultural center
10. Volgograd [10] - Hero city

**Ural Region (5 cities):**
11. Yekaterinburg [11] - Ural capital
12. Chelyabinsk [12] - Industrial giant
13. Tyumen [13] - Oil region
14. Kurgan [14] - Small town
15. Orenburg [15] - Southern gateway

**Siberia West (5 cities):**
16. Omsk [16] - Siberian gateway
17. Novosibirsk [17] - Siberian capital
18. Tomsk [18] - University town
19. Barnaul [19] - Mountain region
20. Kemerovo [20] - Coal mining

**Siberia East (5 cities):**
21. Krasnoyarsk [21] - River city
22. Irkutsk [22] - Lake Baikal region
23. Ulan-Ude [23] - Buddhist center
24. Chita [24] - Trans-Siberian stop
25. Bratsk [25] - Dam city

**Far East (5 cities):**
26. Khabarovsk [26] - Far East capital
27. Vladivostok [27] - Pacific port (FINAL DESTINATION)
28. Blagoveshchensk [28] - Chinese border
29. Yakutsk [29] - Coldest city
30. Magadan [30] - Remote port

**City Data Structure:**
```json
{
  "cities": [
    {
      "id": "moscow",
      "name": "Moscow",
      "number": "01",
      "region": "Western Russia",
      "population": "12M",
      "description": "The capital of Russia and the starting point of your journey.",
      "mapPosition": [200, 450],
      "connections": [
        "st_petersburg",
        "nizhny_novgorod",
        "kazan",
        "tver"
      ],
      "structures": [
        {"type": "GAS_STATION", "name": "Lukoil Station"},
        {"type": "STORE", "name": "General Store"},
        {"type": "MOTEL", "name": "Moscow Inn"},
        {"type": "GARAGE", "name": "Auto Repair Shop"},
        {"type": "DINER", "name": "Cafe Moscow"}
      ],
      "specialFeatures": [
        "character_creation_start",
        "black_market_access",
        "race_track"
      ],
      "npcs": [
        "dmitri_merchant",
        "katya_racer"
      ],
      "quests": [
        "cross_country_delivery_start"
      ],
      "lore": "Moscow, the heart of Russia. Once the center of a vast empire, now a city struggling through turbulent times. Your journey begins here."
    }
  ]
}
```

**Deliverables:**
- [ ] 30 cities defined
- [ ] JSON data for all cities
- [ ] Map positions calculated
- [ ] City connections defined (routes)
- [ ] Unique features per city
- [ ] City descriptions and lore
- [ ] Integration with MapScene
- [ ] Integration with TravelSelectionScene

---

## PHASE 7: UI & GRAPHICS
**Timeline:** 2-3 weeks
**Priority:** 🟡 LOW (after Phase 6)

### 7.1 SPRITE CREATION (2 weeks)

**Assets Needed:**

**Cars (3 models × 3 views = 9 sprites):**
- BMW E30 Coupe:
  - Top-down view (for NodeScene)
  - Side view (for TravelCinematicScene)
  - Portrait (for menus)
- Volvo 940 Sedan: (same 3 views)
- Mercedes Sprinter: (same 3 views)

**Structures (6 types):**
- Gas Station: Pumps + building
- Store: Shop front
- Motel: Building with sign
- Garage: Workshop
- Diner: Restaurant
- Mechanic: Auto shop

**UI Icons (20+):**
- Fuel can
- Money (rubles)
- Energy (battery)
- Health (heart)
- Morale (smile)
- Quest marker
- Locked icon
- Unlocked icon
- Save icon
- Settings icon

**Backgrounds:**
- Parallax layers for travel:
  - Sky (static or moving clouds)
  - Mountains (far layer)
  - Trees (mid layer)
  - Road (near layer)

**NPC Portraits (15 portraits):**
- One portrait per recruitable NPC
- 256x256 size
- Pixel art style

**Deliverables:**
- [ ] All car sprites
- [ ] All structure sprites
- [ ] All UI icons
- [ ] Parallax backgrounds
- [ ] NPC portraits
- [ ] Integration into scenes
- [ ] Asset loading system

---

### 7.2 HUD IMPROVEMENTS (3 days)

**Graphical HUD Panel:**
- Replace text-based indicators with graphics
- Animated bars for fuel/energy/health
- Icon-based resource display
- Mini-map in corner (optional)

**Deliverables:**
- [ ] Graphical HUD design
- [ ] HUD implementation
- [ ] Animations
- [ ] Integration

---

## PHASE 8: AUDIO & POLISH
**Timeline:** 1-2 weeks
**Priority:** 🟡 LOW

### 8.1 AUDIO SYSTEM (1 week)

**Music Needed:**
- Main menu theme
- Travel/driving music (multiple tracks)
- City ambient
- Event music (tense situations)
- Victory theme

**Sound Effects:**
- UI sounds (click, hover, error)
- Car sounds (engine, braking)
- Event sounds (police siren, rain, etc.)
- Dialogue blip sound

**Implementation:**
```cpp
class SoundManager {
public:
    static SoundManager& getInstance();

    void playMusic(const std::string& musicId, bool loop = true);
    void stopMusic();
    void playSound(const std::string& soundId);

    void setMusicVolume(float volume);  // 0-100
    void setSoundVolume(float volume);  // 0-100

private:
    sf::Music m_currentMusic;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::vector<sf::Sound> m_activeSounds;
};
```

**Deliverables:**
- [ ] Audio system implementation
- [ ] Music tracks (5-10)
- [ ] Sound effects (20+)
- [ ] Volume settings
- [ ] Integration

---

### 8.2 SETTINGS SYSTEM (2 days)

**Settings to implement:**
- Music volume
- Sound volume
- Resolution (if supporting multiple)
- Language (RU/EN)
- Controls rebinding

**Deliverables:**
- [ ] Settings scene
- [ ] Settings save/load
- [ ] Settings application
- [ ] UI for settings

---

### 8.3 GAME BALANCING (3 days)

**Balance passes:**
- Fuel consumption rates
- Money rewards/costs
- Quest difficulty
- Event probabilities
- NPC abilities
- Experience curve

**Deliverables:**
- [ ] Balancing spreadsheet
- [ ] Playtest sessions
- [ ] Adjustments
- [ ] Final balance

---

## PHASE 9: TESTING & RELEASE
**Timeline:** 2-3 weeks
**Priority:** 🟡 FINAL

### 9.1 COMPREHENSIVE TESTING (1-2 weeks)

**Test Categories:**

**Full Playthrough Testing:**
- Start → Finish complete run
- Test all endings
- Test game over scenarios

**Feature Testing:**
- Every scene functional
- All quests completable
- All events trigger correctly
- All NPCs recruitable
- Save/load works perfectly

**Edge Case Testing:**
- Zero fuel
- Zero money
- Full inventory
- Max level
- All companions recruited

**Performance Testing:**
- No memory leaks
- Stable framerate
- Fast scene transitions

**Deliverables:**
- [ ] Test plan document
- [ ] Bug tracking
- [ ] All critical bugs fixed
- [ ] Performance optimized

---

### 9.2 DOCUMENTATION FINALIZATION (3 days)

**Documents to finalize:**
- README.md (updated)
- DOCUMENTATION.md (player guide)
- TECHNICAL.md (developer guide)
- CHANGELOG.md (version history)
- LICENSE

**Deliverables:**
- [ ] All documentation complete
- [ ] User manual
- [ ] Developer documentation
- [ ] License added

---

### 9.3 RELEASE (2 days)

**Release Checklist:**
- [ ] All features complete
- [ ] All tests passing
- [ ] Documentation complete
- [ ] Build for multiple platforms
- [ ] Create GitHub release
- [ ] Write release notes
- [ ] Create trailer (optional)
- [ ] Publish

---

## 📅 TIMELINE SUMMARY

| Phase | Duration | Priority | Status |
|-------|----------|----------|--------|
| Phase 5: Integration & Cleanup | 2-3 weeks | 🔴 CRITICAL | 🔄 In Progress |
| Phase 6: Content Expansion | 3-4 weeks | 🟡 HIGH | ⏳ Pending |
| Phase 7: UI & Graphics | 2-3 weeks | 🟡 MEDIUM | ⏳ Pending |
| Phase 8: Audio & Polish | 1-2 weeks | 🟡 LOW | ⏳ Pending |
| Phase 9: Testing & Release | 2-3 weeks | 🟡 FINAL | ⏳ Pending |
| **TOTAL** | **10-15 weeks** | - | **~3-4 months** |

---

## 🎯 IMMEDIATE NEXT STEPS (Week 1)

### Day 1-2: TODO Removal
- [ ] Implement all 19 TODO items
- [ ] Test each implementation
- [ ] Remove TODO comments

### Day 3: Scene Cleanup
- [ ] Delete unused scene headers
- [ ] Update SceneType enum
- [ ] Update SceneManager
- [ ] Update CMakeLists.txt

### Day 4-5: DialogueScene Implementation
- [ ] Create DialogueScene.cpp
- [ ] Implement typewriter effect
- [ ] Test dialogue trees

### Day 6-7: MotelScene & GameOverScene
- [ ] Implement MotelScene
- [ ] Implement GameOverScene
- [ ] Integration testing

---

## 📝 NOTES

### Following the 8 Core Principles:

1. ✅ **Immediate Functionality** - No TODO after Phase 5.2
2. ✅ **Independence** - All dependencies documented
3. ✅ **Strict Adherence** - Following this plan exactly
4. ✅ **Unified Plan** - This is THE plan
5. ✅ **Clean Structure** - All docs in root, organized
6. ✅ **UI + Assets** - UI implemented immediately, assets in Phase 7
7. ✅ **Modular Structure** - Clear separation maintained
8. ✅ **Documentation** - This plan + regular updates

### Risk Management:

**High Risk:**
- SFML 3.x compatibility issues → Mitigation: Test early and often
- Scope creep → Mitigation: Stick to plan, no additions

**Medium Risk:**
- Time overruns → Mitigation: Prioritize ruthlessly
- Balance issues → Mitigation: Playtesting in Phase 8

**Low Risk:**
- Asset creation delays → Mitigation: Placeholder graphics OK for alpha

---

**END OF IMPLEMENTATION PLAN**

**Last Updated:** 2025-11-28
**Next Review:** After Phase 5 completion
**Maintained By:** Development Team
**Status:** ACTIVE PLAN
