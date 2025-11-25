# Prompt for Branch 2: Dynamic NPC Dialogue System

## Context
You are working on the 'barzikol/kkurs' repository, a C++ SFML-based road trip game project. This branch is for implementing a comprehensive dynamic dialogue system with NPC character personalities, random dialogue events, and a progression system.

## Current Project State (from Branch 1 completion)

### ✅ Completed in Branch 1:
1. **Quest System** - 10 quests with 6 objective types
2. **Event System** - 25 events (weather, mechanical, social, economic, NPC hitchhiking)
3. **NPC Recruitment** - 5 free recruitable NPCs (Михаил, Сара, Джек, Эмма, Алекс)
4. **Basic Dialogue System** - 3-node recruitment dialogues for each NPC
5. **Test Coverage** - 51/51 tests passing (100%)

### 📊 Current Metrics:
- **Code**: ~17,500 lines production code
- **Tests**: 51/51 passing (100% pass rate)
- **Build**: GCC 13.3.0, C++17, SFML 2.6.1
- **Performance**: 60 FPS, ~15MB memory
- **NPCs**: 5 recruitable with basic abilities

---

## Mission: Dynamic NPC Dialogue & Personality System

### 🎯 Core Requirements:

#### 1. Random Dialogue Trigger System
**Requirement:** Each NPC должен триггерить случайные диалоговые события

**Features:**
- ~20% probability for **casual dialogues** (no choices, passive observation)
- ~80% probability for **choice-based dialogues** (player makes decisions)
- Dialogues trigger randomly during travel/events
- Frequency: configurable per NPC (e.g., every 5-15 minutes of gameplay)
- **CRITICAL:** Dialogues never repeat - each dialogue can only trigger once per playthrough
- System tracks used dialogues per NPC to prevent repetition

**Technical:**
```cpp
struct RandomDialogueTrigger {
    std::string npcId;
    float triggerProbability;      // Probability to trigger dialogue
    float minTimeBetweenTriggers;  // Cooldown in seconds
    float maxTimeBetweenTriggers;
    DialogueType type;             // CASUAL or CHOICE_BASED
    
    // Track used dialogues to prevent repetition
    std::set<std::string> usedDialogueIds;
    
    bool isDialogueUsed(const std::string& dialogueId) const;
    void markDialogueAsUsed(const std::string& dialogueId);
    std::vector<std::string> getAvailableDialogues() const;
};
```

#### 2. Animated Text Display System
**Requirement:** Всплывающие диалоги с анимацией текста буква за буквой

**UI Features:**
- **Pop-up dialogue box** appears on screen
- **Letter-by-letter animation** (typewriter effect)
- **Configurable speed** (e.g., 50ms per character)
- **Auto-pause** after NPC phrase for reading time (2-5 seconds)
- **Hero response** appears after pause
- **Fade in/out animations** for dialogue boxes

**Technical:**
```cpp
class AnimatedDialogueUI {
public:
    void displayDialogue(const Dialogue& dialogue);
    void setAnimationSpeed(float charsPerSecond);
    void setReadingPause(float seconds);
    bool isAnimating() const;
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    
private:
    std::string currentText;
    size_t currentCharIndex;
    float animationTimer;
    float pauseTimer;
    DialogueState state; // ANIMATING_NPC, PAUSING, ANIMATING_HERO, COMPLETE
};
```

#### 3. Unique NPC Personalities
**Requirement:** Каждый персонаж должен иметь свой уникальный характер

**5 NPC Personalities (Inspired by books/films):**

##### Михаил (Механик) - "The Grizzled Veteran"
- **Inspiration:** Hemingway's Jake Barnes, Mad Max's mechanics
- **Traits:** Laconic, practical, sardonic humor, worldly-wise
- **Speech style:** Short sentences, technical jargon, dry wit
- **Dialogue examples:**
  - Casual: "Видишь ту дорогу? Там в 93-м застрял на три дня. / See that road? Got stuck there for three days in '93."
  - Choice: "Двигатель барахлит. Что делаем? / Engine's acting up. What do we do?"

##### Сара (Торговец) - "The Sharp Entrepreneur"
- **Inspiration:** Arya Stark's cunning, Wolf of Wall Street's ambition
- **Traits:** Quick-witted, opportunistic, charming, street-smart
- **Speech style:** Fast-paced, persuasive, uses market metaphors
- **Dialogue examples:**
  - Casual: "Знаешь, в следующем городе цены на топливо упадут на 20%. Доверься мне. / You know, fuel prices will drop 20% in the next town. Trust me."
  - Choice: "Видишь тот рынок? Я знаю, что там можно купить дёшево... / See that market? I know what we can buy cheap there..."

##### Джек (Навигатор) - "The Old Road Dog"
- **Inspiration:** Gandalf's wisdom, Kerouac's wanderers
- **Traits:** Philosophical, nostalgic, patient, loves storytelling
- **Speech style:** Long-winded, metaphorical, historical anecdotes
- **Dialogue examples:**
  - Casual: "В эти края я впервые приехал в 87-м. Тогда ещё Союз был... / I first came to these parts in '87. The Union still existed then..."
  - Choice: "Два пути: быстрый через горы или безопасный через степь. Что выберешь? / Two paths: fast through mountains or safe through steppe. What do you choose?"

##### Эмма (Врач) - "The Compassionate Professional"
- **Inspiration:** Florence Nightingale's dedication, M*A*S*H's Hawkeye
- **Traits:** Caring, analytical, calm under pressure, slightly cynical
- **Speech style:** Medical terminology, empathetic, educational
- **Dialogue examples:**
  - Casual: "Вижу, ты не высыпаешься. Это опасно за рулём. / I see you're not getting enough sleep. That's dangerous while driving."
  - Choice: "У тебя повышенное давление. Нужен отдых или можешь продолжать? / Your blood pressure is high. Need rest or can you continue?"

##### Алекс (Компаньон) - "The Eternal Optimist"
- **Inspiration:** Samwise Gamgee's loyalty, Forrest Gump's innocence
- **Traits:** Cheerful, naive, loyal, sees beauty everywhere
- **Speech style:** Enthusiastic, simple vocabulary, uses exclamations
- **Dialogue examples:**
  - Casual: "Смотри! Закат! Как красиво! Остановимся? / Look! Sunset! So beautiful! Should we stop?"
  - Choice: "Хочешь спеть песню? У меня отличное настроение! / Want to sing a song? I'm in a great mood!"

#### 4. Choice-Based Dialogues with Consequences
**Requirement:** Диалоги с выбором дают бонусы/дебаффы за правильные/неправильные ответы

**System Design:**
```cpp
struct DialogueChoice {
    std::string text;
    std::string nextNodeId;
    
    // Correctness and consequences
    bool isCorrect;                  // Is this the "right" answer?
    std::vector<StatusEffect> buffs; // Buffs for correct choice
    std::vector<StatusEffect> debuffs; // Debuffs for wrong choice
    int experienceReward;            // XP for correct choice
    
    // Relationship impact
    int relationshipChange;          // +/- NPC relationship
};

struct StatusEffect {
    std::string id;
    std::string name;
    EffectType type;                 // BUFF or DEBUFF
    float magnitude;                 // e.g., +10% fuel efficiency
    float duration;                  // seconds or -1 for permanent
    std::string affectedStat;        // "fuel_consumption", "morale", etc.
};
```

**Example Dialogue with Choices:**
```
NPC (Михаил): "Двигатель перегревается. Что делать?"
Hero choices:
1. "Остановимся и дадим остыть" [CORRECT] → +50 XP, +10 relationship, buff: "Cool Head" (+5% car health for 1 hour)
2. "Продолжим, потерпит" [WRONG] → -25 XP, -5 relationship, debuff: "Overheated Engine" (-15% performance for 30 min)
3. "Ты механик, решай сам" [NEUTRAL] → +10 XP, +2 relationship, no effects
```

#### 5. Experience & Progression System
**Requirement:** Каждый правильный выбор даёт опыт герою и NPC - система прокачки

**XP System:**
```cpp
class DialogueProgressionSystem {
public:
    // Award XP for dialogue choices
    void awardExperience(const std::string& characterId, int xp);
    void awardExperience(const std::string& npcId, int xp);
    
    // Check level ups
    bool checkLevelUp(const std::string& entityId);
    void levelUp(const std::string& entityId);
    
    // Relationship tracking
    void updateRelationship(const std::string& npcId, int change);
    int getRelationshipLevel(const std::string& npcId) const;
    
private:
    std::map<std::string, int> experiencePoints;
    std::map<std::string, int> relationshipPoints;
};
```

**XP Rewards:**
- Correct choice in easy dialogue: +25 XP (hero), +15 XP (NPC)
- Correct choice in medium dialogue: +50 XP (hero), +30 XP (NPC)
- Correct choice in hard dialogue: +100 XP (hero), +60 XP (NPC)
- Casual dialogue completion: +5 XP (both)

**Level Benefits:**
- Hero levels up: +skill points, unlock new dialogue options
- NPC levels up: abilities improve (e.g., mechanic -10% → -15% fuel consumption)

---

## Implementation Plan

### Phase 1: Data Structures & Core Systems (Week 1)
**Goal:** Implement foundational classes and data structures

**Tasks:**
1. **Create RandomDialogueTrigger system**
   - `RandomDialogueTrigger` struct
   - `DialogueTriggerManager` class
   - Probability and cooldown logic
   - Integration with game loop

2. **Extend Dialogue structures**
   - Add `DialogueType` enum (CASUAL, CHOICE_BASED)
   - Add `StatusEffect` struct
   - Extend `DialogueChoice` with correctness/rewards
   - Create `CharacterPersonality` struct

3. **Implement DialogueProgressionSystem**
   - XP tracking for hero and NPCs
   - Relationship system
   - Level-up detection and rewards

**Deliverables:**
- `include/RandomDialogueSystem.h`
- `src/RandomDialogueSystem.cpp`
- `include/DialogueProgressionSystem.h`
- `src/DialogueProgressionSystem.cpp`
- Unit tests for core logic

---

### Phase 2: NPC Personalities & Content (Week 2)
**Goal:** Create unique personality profiles and dialogue content

**Tasks:**
1. **Define personality profiles**
   - Character traits and speech patterns
   - Dialogue templates for each NPC
   - Personality-based response generation

2. **Write dialogue content**
   - **1-7 casual dialogues per NPC** (recommended: 5 per NPC, 25 total)
   - **10-20 choice-based dialogues per NPC** (recommended: 15 per NPC, 75 total)
   - Total: ~100 unique dialogues across all NPCs
   - Categorize choice dialogues by difficulty (easy, medium, hard)
   - **Each dialogue is unique and never repeats in gameplay**

3. **Implement DialogueContentManager**
   - Load dialogues from data files
   - Filter by NPC, type, difficulty
   - Random selection logic (excluding used dialogues)
   - Track dialogue usage per save file
   - Persist used dialogues to prevent repetition across sessions

**Deliverables:**
- `data/dialogues/mikhail_casual.json` (x5 NPCs)
- `data/dialogues/mikhail_choices.json` (x5 NPCs)
- `include/DialogueContentManager.h`
- `src/DialogueContentManager.cpp`
- Content validation tests

**Example Content Structure:**
```json
{
  "npc_id": "mikhail",
  "personality": "grizzled_veteran",
  "casual_dialogues": [
    {
      "id": "mikhail_casual_01",
      "trigger_conditions": {
        "location_types": ["highway", "road"],
        "time_of_day": ["any"]
      },
      "npc_text": "Видишь ту дорогу? Там в 93-м застрял на три дня.",
      "hero_response": "Что случилось?",
      "xp_reward": 5,
      "one_time_only": true
    }
  ],
  "choice_dialogues": [
    {
      "id": "mikhail_choice_01",
      "difficulty": "medium",
      "npc_text": "Двигатель перегревается. Что делать?",
      "one_time_only": true,
      "choices": [
        {
          "text": "Остановимся и дадим остыть",
          "is_correct": true,
          "xp_reward": 50,
          "relationship_change": 10,
          "buffs": [{"type": "car_health", "magnitude": 5, "duration": 3600}]
        }
      ]
    }
  ]
}
```

**CRITICAL: Dialogue Repetition Prevention**

Each dialogue has `"one_time_only": true` flag. The system must:
1. Track which dialogues have been shown per NPC
2. Persist this data in save files
3. Filter out used dialogues when selecting random dialogue
4. Provide debug info if all dialogues are exhausted

**Implementation:**
```cpp
class DialogueHistoryManager {
public:
    // Mark dialogue as used
    void markDialogueUsed(const std::string& npcId, const std::string& dialogueId);
    
    // Check if dialogue has been used
    bool isDialogueUsed(const std::string& npcId, const std::string& dialogueId) const;
    
    // Get list of available (unused) dialogues for NPC
    std::vector<std::string> getAvailableDialogues(const std::string& npcId, 
                                                    DialogueType type) const;
    
    // Save/Load from file
    void saveToFile(const std::string& savePath) const;
    void loadFromFile(const std::string& savePath);
    
    // Statistics
    int getTotalDialoguesUsed(const std::string& npcId) const;
    int getRemainingDialogues(const std::string& npcId, DialogueType type) const;
    
private:
    // npcId -> set of used dialogue IDs
    std::map<std::string, std::set<std::string>> usedDialogues;
};
```

---

### Phase 3: Animated UI System (Week 3)
**Goal:** Implement typewriter text animation and dialogue UI

**Tasks:**
1. **Create AnimatedDialogueUI class**
   - SFML-based rendering
   - Letter-by-letter animation
   - Configurable timing
   - Fade effects

2. **Design dialogue box UI**
   - NPC portrait/icon
   - Text area with typewriter effect
   - Choice buttons (for choice-based dialogues)
   - Progress indicator

3. **Implement DialogueUIController**
   - State machine (IDLE, ANIMATING, PAUSING, WAITING_CHOICE)
   - Input handling for choices
   - Integration with RandomDialogueSystem

**Deliverables:**
- `include/UI/AnimatedDialogueUI.h`
- `src/UI/AnimatedDialogueUI.cpp`
- `include/UI/DialogueUIController.h`
- `src/UI/DialogueUIController.cpp`
- UI mockups and tests

**UI Specifications:**
- Font: Monospace for typewriter effect
- Animation speed: 50ms per character (configurable)
- Reading pause: 3 seconds default
- Dialogue box: 80% screen width, bottom 30% of screen
- Colors: Match game's 90s terminal aesthetic

---

### Phase 4: Status Effects & Buff/Debuff System (Week 4)
**Goal:** Implement consequences for dialogue choices

**Tasks:**
1. **Create StatusEffectManager**
   - Active effects tracking
   - Duration management
   - Stat modification system

2. **Implement effect types**
   - Fuel efficiency buffs/debuffs
   - Morale/energy modifiers
   - Car health effects
   - Money income modifiers

3. **Visual indicators**
   - Status effect icons
   - Buff/debuff notifications
   - Timer displays

**Deliverables:**
- `include/StatusEffectManager.h`
- `src/StatusEffectManager.cpp`
- `include/UI/StatusEffectUI.h`
- `src/UI/StatusEffectUI.cpp`
- Effect balance testing

---

### Phase 5: Integration & Testing (Week 5)
**Goal:** Connect all systems and ensure stability

**Tasks:**
1. **Integrate with game loop**
   - Hook dialogue triggers into travel scenes
   - Connect progression system to ExperienceSystem
   - Link status effects to game mechanics

2. **Comprehensive testing**
   - Unit tests for all new systems
   - Integration tests for dialogue flow
   - Playtesting for balance
   - Performance profiling

3. **Polish and optimization**
   - Fix bugs
   - Optimize UI rendering
   - Balance XP rewards and effects
   - Add sound effects (optional)

**Deliverables:**
- Full test suite (target: 70+ tests)
- Performance report
- Balance spreadsheet
- Bug fixes and polish

---

## Technical Specifications

### File Structure
```
include/
  - RandomDialogueSystem.h
  - DialogueProgressionSystem.h
  - DialogueContentManager.h
  - StatusEffectManager.h
  - UI/
    - AnimatedDialogueUI.h
    - DialogueUIController.h
    - StatusEffectUI.h

src/
  - RandomDialogueSystem.cpp
  - DialogueProgressionSystem.cpp
  - DialogueContentManager.cpp
  - StatusEffectManager.cpp
  - UI/
    - AnimatedDialogueUI.cpp
    - DialogueUIController.cpp
    - StatusEffectUI.cpp

data/
  - dialogues/
    - mikhail_casual.json
    - mikhail_choices.json
    - sara_casual.json
    - sara_choices.json
    - jack_casual.json
    - jack_choices.json
    - emma_casual.json
    - emma_choices.json
    - alex_casual.json
    - alex_choices.json

tests/
  - unit/
    - test_random_dialogue_system.cpp
    - test_dialogue_progression.cpp
    - test_status_effects.cpp
    - test_dialogue_ui.cpp
```

### Dependencies
- **Existing:** SFML 2.6.1, C++17, GoogleTest
- **New:** JSON parser (nlohmann/json or similar)
- **Optional:** SFML Audio for sound effects

### Performance Targets
- Dialogue trigger check: <1ms per frame
- UI animation: 60 FPS maintained
- Memory: +5MB max for dialogue system
- Load time: <100ms for all dialogue content

---

## Content Guidelines

### Dialogue Writing Rules
1. **Bilingual:** All dialogues in Russian + English
2. **Length:** Casual dialogues: 1-3 sentences. Choice dialogues: 2-5 sentences
3. **Tone:** Match 90s Russian road trip atmosphere
4. **Personality consistency:** Each NPC stays in character
5. **Cultural authenticity:** Use period-appropriate references

### Balance Guidelines
1. **XP Rewards:**
   - Easy dialogue: 25-50 XP
   - Medium dialogue: 50-100 XP
   - Hard dialogue: 100-200 XP

2. **Status Effects:**
   - Buffs: +5% to +20% improvement
   - Debuffs: -10% to -30% penalty
   - Duration: 30 min to 2 hours

3. **Relationship:**
   - Correct choice: +5 to +15 points
   - Wrong choice: -5 to -10 points
   - Level thresholds: 100, 250, 500, 1000 points

---

## Testing Strategy

### Unit Tests
- RandomDialogueTrigger probability and cooldown logic
- DialogueProgressionSystem XP calculations
- StatusEffectManager effect application and expiration
- DialogueContentManager content loading and filtering

### Integration Tests
- Full dialogue flow from trigger to completion
- XP award and level-up sequence
- Status effect application and game stat modification
- UI state transitions

### Playtesting Checklist
- [ ] All 5 NPCs have unique personalities
- [ ] Dialogues trigger at appropriate frequency
- [ ] Text animation is readable and pleasant
- [ ] Choices feel meaningful
- [ ] XP progression feels rewarding
- [ ] Buffs/debuffs are noticeable but balanced
- [ ] No crashes or performance issues
- [ ] Bilingual content is accurate

---

## Success Criteria

### Functional Requirements
- ✅ 25-35 casual dialogues (1-7 per NPC) implemented
- ✅ 50-100 choice-based dialogues (10-20 per NPC) implemented
- ✅ **Dialogue repetition prevention system working correctly**
- ✅ All 5 NPCs have distinct personalities
- ✅ Random dialogue triggers work correctly
- ✅ Animated text UI renders smoothly
- ✅ Status effects apply and expire correctly
- ✅ XP awards and level-ups function
- ✅ Relationship system tracks progress
- ✅ Used dialogues persist across game sessions

### Quality Requirements
- ✅ 70+ unit tests passing
- ✅ 60 FPS maintained during dialogues
- ✅ No memory leaks
- ✅ All content is bilingual
- ✅ Code follows project style
- ✅ Documentation is complete

### User Experience
- ✅ Dialogues feel natural and engaging
- ✅ Personalities are distinct and memorable
- ✅ Choices have clear consequences
- ✅ Progression feels rewarding
- ✅ UI is polished and responsive

---

## Risks & Mitigation

### Risk 1: Content Creation Bottleneck
- **Risk:** Writing 100 quality dialogues takes too long
- **Mitigation:** Start with 5 dialogues per NPC (25 total), expand later

### Risk 2: UI Performance
- **Risk:** Letter-by-letter animation causes FPS drops
- **Mitigation:** Use efficient text rendering, profile early

### Risk 3: Balance Issues
- **Risk:** XP/buffs too powerful or too weak
- **Mitigation:** Iterative playtesting, configurable values

### Risk 4: Scope Creep
- **Risk:** Feature expands beyond 5 weeks
- **Mitigation:** Strict MVP definition, defer non-essential features

---

## Future Enhancements (Post-Branch 2)

- Voice acting for NPCs (Russian/English)
- Dynamic dialogue generation using templates
- More complex relationship states (friendship, rivalry)
- NPC-to-NPC dialogues
- Dialogue history and memory system
- Achievement system for dialogue choices
- Multiple endings based on relationship levels

---

## References & Inspiration

### Games with Great Dialogue Systems
- **Disco Elysium:** Deep character personalities, meaningful choices
- **The Walking Dead (Telltale):** Timed choices, relationship tracking
- **Fallout: New Vegas:** Skill-based dialogue options, faction reputation
- **Mass Effect:** Character relationships, loyalty missions

### Books/Films for Character Inspiration
- **Михаил:** "The Old Man and the Sea" (Hemingway), "Mad Max"
- **Сара:** "American Psycho" (Ellis), "The Wolf of Wall Street"
- **Джек:** "On the Road" (Kerouac), "Lord of the Rings"
- **Эмма:** "One Flew Over the Cuckoo's Nest", "M*A*S*H"
- **Алекс:** "Forrest Gump", "The Lord of the Rings" (Sam)

---

**Document Version:** 1.1  
**Created:** 2025-11-17  
**Updated:** 2025-11-17 (Content requirements updated)  
**Branch Target:** Branch 2 (Dynamic Dialogue System)  
**Estimated Duration:** 5 weeks  
**Estimated LOC:** 2,000-3,000 new lines + 75-125 dialogue files  
**Content Volume:** 75-125 total dialogues (25-35 casual + 50-100 choice-based)  
**Key Feature:** No dialogue repetition - each dialogue triggers only once per playthrough
