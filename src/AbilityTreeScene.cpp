#include "AbilityTreeScene.h"
#include <iostream>
#include <sstream>
#include <iomanip>

AbilityTreeScene::AbilityTreeScene()
    : m_isFinished(false),
      m_nextScene(SceneType::NODE),
      m_currentCategory(AbilityCategory::DRIVING),
      m_selectedIndex(0),
      m_selectedAbility(nullptr),
      m_fontLoaded(false),
      m_blinkTimer(0.0f),
      m_blinkState(true),
      m_playerLevel(0),
      m_availableSkillPoints(0) {
    
    setupTerminalStyle();
    setupUI();
    loadAbilityData();
}

void AbilityTreeScene::setupTerminalStyle() {
    // Pip-Boy monochrome green color scheme
    m_colorBackground = sf::Color(0, 0, 0);           // Black
    m_colorText = sf::Color(0, 255, 0);               // Bright green
    m_colorTextDim = sf::Color(0, 119, 0);            // Dim green
    m_colorHighlight = sf::Color(0, 255, 0);          // Bright green
    m_colorLocked = sf::Color(0, 68, 0);              // Very dim green
    m_colorBorder = sf::Color(0, 170, 0);             // Medium green
}

void AbilityTreeScene::setupUI() {
    // Fixed layout for 1440x900 (MacBook Air M1)
    // Try to load monospace font (SFML 3.x: openFromFile returns bool)
    // Prefer terminal-style fonts like Courier New or Fixedsys
    m_fontLoaded = m_terminalFont.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");

    if (!m_fontLoaded) {
        // Try alternative fonts
        m_fontLoaded = m_terminalFont.openFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");
    }

    if (!m_fontLoaded) {
        std::cerr << "Warning: Could not load monospace font for AbilityTreeScene" << std::endl;
    }
}

void AbilityTreeScene::loadAbilityData() {
    // Get player data
    m_playerLevel = ExperienceSystem::getInstance().getLevel();
    m_availableSkillPoints = AbilityTreeSystem::getInstance().getAvailableSkillPoints();
    
    // Load abilities for current category
    switchCategory(m_currentCategory);
}

void AbilityTreeScene::switchCategory(AbilityCategory category) {
    m_currentCategory = category;
    m_selectedIndex = 0;
    m_selectedAbility = nullptr;
    
    // Get all abilities in this category
    m_displayedAbilities.clear();
    auto abilities = AbilityTreeSystem::getInstance().getAbilitiesByCategory(category);
    
    // Convert to pointers for display
    for (const auto& ability : abilities) {
        m_displayedAbilities.push_back(&ability);
    }
    
    // Select first ability if available
    if (!m_displayedAbilities.empty()) {
        m_selectedAbility = m_displayedAbilities[0];
    }
}

void AbilityTreeScene::navigateUp() {
    if (m_selectedIndex > 0) {
        m_selectedIndex--;
        if (m_selectedIndex < static_cast<int>(m_displayedAbilities.size())) {
            m_selectedAbility = m_displayedAbilities[m_selectedIndex];
        }
    }
}

void AbilityTreeScene::navigateDown() {
    if (m_selectedIndex < static_cast<int>(m_displayedAbilities.size()) - 1) {
        m_selectedIndex++;
        m_selectedAbility = m_displayedAbilities[m_selectedIndex];
    }
}

void AbilityTreeScene::selectAbility() {
    // Selection handled by navigation, detail panel shows selected ability
}

void AbilityTreeScene::attemptUnlock() {
    if (!m_selectedAbility) return;
    
    // Try to unlock the selected ability
    bool success = AbilityTreeSystem::getInstance().unlockAbility(m_selectedAbility->getId());
    
    if (success) {
        std::cout << "Unlocked ability: " << m_selectedAbility->getName() << std::endl;
        // Refresh data
        loadAbilityData();
        // Note: Sound effect would play here if sound system is implemented
    } else {
        std::cout << "Cannot unlock ability: " << m_selectedAbility->getName() << std::endl;
        // Note: Error sound would play here if sound system is implemented
    }
}

void AbilityTreeScene::goBack() {
    m_isFinished = true;
}

void AbilityTreeScene::handleInput(const sf::Event& event) {
    // SFML 3.x event handling
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            // Navigation
            case sf::Keyboard::Key::Up:
            case sf::Keyboard::Key::W:
                navigateUp();
                break;
            case sf::Keyboard::Key::Down:
            case sf::Keyboard::Key::S:
                navigateDown();
                break;

            // Category selection (1-5 keys)
            case sf::Keyboard::Key::Num1:
                switchCategory(AbilityCategory::DRIVING);
                break;
            case sf::Keyboard::Key::Num2:
                switchCategory(AbilityCategory::TRADING);
                break;
            case sf::Keyboard::Key::Num3:
                switchCategory(AbilityCategory::SURVIVAL);
                break;
            case sf::Keyboard::Key::Num4:
                switchCategory(AbilityCategory::MECHANICS);
                break;
            case sf::Keyboard::Key::Num5:
                switchCategory(AbilityCategory::SOCIAL);
                break;

            // Actions
            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                attemptUnlock();
                break;
            case sf::Keyboard::Key::Escape:
            case sf::Keyboard::Key::Tab:
                goBack();
                break;

            default:
                break;
        }
    }
}

void AbilityTreeScene::update(float deltaTime) {
    // Update blink timer for cursor effect
    m_blinkTimer += deltaTime;
    if (m_blinkTimer >= 0.5f) {
        m_blinkState = !m_blinkState;
        m_blinkTimer = 0.0f;
    }
    
    // Refresh player data
    m_playerLevel = ExperienceSystem::getInstance().getLevel();
    m_availableSkillPoints = AbilityTreeSystem::getInstance().getAvailableSkillPoints();
}

void AbilityTreeScene::render(sf::RenderWindow& window) {
    if (!m_fontLoaded) return;
    
    // Clear to black background
    window.clear(m_colorBackground);
    
    // Render terminal border
    renderTerminalBorder(window);
    
    // Render UI sections
    renderHeader(window);
    renderCategoryList(window);
    renderAbilityTree(window);
    renderDetailPanel(window);
    renderFooter(window);
}

void AbilityTreeScene::renderTerminalBorder(sf::RenderWindow& window) {
    // Outer border - ASCII box drawing characters
    sf::RectangleShape border;
    border.setSize(sf::Vector2f(WINDOW_WIDTH - 2 * PADDING, WINDOW_HEIGHT - 2 * PADDING));
    border.setPosition(sf::Vector2f(PADDING, PADDING));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(m_colorBorder);
    border.setOutlineThickness(BORDER_THICKNESS);
    window.draw(border);
}

void AbilityTreeScene::renderHeader(sf::RenderWindow& window) {
    // Title (SFML 3.x: Text requires font in constructor)
    sf::Text title(m_terminalFont, "ABILITY TREE", 28);
    title.setFillColor(m_colorText);
    title.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 300, PADDING + 15));
    window.draw(title);

    // Player info
    std::ostringstream ss;
    ss << "LEVEL: " << m_playerLevel
       << "   POINTS: " << m_availableSkillPoints;

    sf::Text info(m_terminalFont, ss.str(), CHAR_SIZE);
    info.setFillColor(m_colorTextDim);
    info.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 200, PADDING + 55));
    window.draw(info);

    // Separator line
    sf::RectangleShape separator;
    separator.setSize(sf::Vector2f(WINDOW_WIDTH - 4 * PADDING, 2));
    separator.setPosition(sf::Vector2f(2 * PADDING, PADDING + 90));
    separator.setFillColor(m_colorBorder);
    window.draw(separator);
}

void AbilityTreeScene::renderCategoryList(sf::RenderWindow& window) {
    float yPos = PADDING + 110;
    
    // Category tabs
    std::vector<std::pair<AbilityCategory, std::string>> categories = {
        {AbilityCategory::DRIVING, "[1] DRIVING"},
        {AbilityCategory::TRADING, "[2] TRADING"},
        {AbilityCategory::SURVIVAL, "[3] SURVIVAL"},
        {AbilityCategory::MECHANICS, "[4] MECHANICS"},
        {AbilityCategory::SOCIAL, "[5] SOCIAL"}
    };
    
    for (const auto& [cat, name] : categories) {
        std::string displayText = (cat == m_currentCategory) ? ("> " + name) : ("  " + name);
        sf::Color textColor = (cat == m_currentCategory) ? m_colorHighlight : m_colorTextDim;

        // SFML 3.x: Text requires font in constructor
        sf::Text catText(m_terminalFont, displayText, CHAR_SIZE);
        catText.setFillColor(textColor);
        catText.setPosition(sf::Vector2f(2 * PADDING, yPos));
        window.draw(catText);
        yPos += LINE_HEIGHT;
    }
}

void AbilityTreeScene::renderAbilityTree(sf::RenderWindow& window) {
    float yPos = PADDING + 110;
    float xPos = WINDOW_WIDTH / 2 - 200;

    // Tree title (SFML 3.x: Text requires font in constructor)
    sf::Text treeTitle(m_terminalFont, "ABILITIES", CHAR_SIZE + 2);
    treeTitle.setFillColor(m_colorText);
    treeTitle.setPosition(sf::Vector2f(xPos, yPos));
    window.draw(treeTitle);
    yPos += LINE_HEIGHT + 10;

    // Render abilities in tree structure
    for (size_t i = 0; i < m_displayedAbilities.size(); ++i) {
        const AbilityNode* ability = m_displayedAbilities[i];
        if (!ability) continue;

        // Add cursor for selected
        std::string line = (i == static_cast<size_t>(m_selectedIndex) && m_blinkState) ? "> " : "  ";
        line += getAbilityLine(ability, 0);

        sf::Text abilityText(m_terminalFont, line, CHAR_SIZE);
        abilityText.setFillColor(getAbilityColor(ability));
        abilityText.setPosition(sf::Vector2f(xPos, yPos));
        window.draw(abilityText);
        yPos += LINE_HEIGHT;
    }
}

void AbilityTreeScene::renderDetailPanel(sf::RenderWindow& window) {
    if (!m_selectedAbility) return;
    
    float xPos = WINDOW_WIDTH - 450;
    float yPos = PADDING + 110;
    
    // Panel border
    sf::RectangleShape panel;
    panel.setSize(sf::Vector2f(400, 500));
    panel.setPosition(sf::Vector2f(xPos, yPos));  // Fixed: Vector2f wrapper
    panel.setFillColor(sf::Color::Transparent);
    panel.setOutlineColor(m_colorBorder);
    panel.setOutlineThickness(2);
    window.draw(panel);

    yPos += 15;
    xPos += 15;

    // Ability name (SFML 3.x: Text requires font in constructor)
    sf::Text name(m_terminalFont, m_selectedAbility->getName(), 20);
    name.setFillColor(m_colorText);
    name.setPosition(sf::Vector2f(xPos, yPos));  // Fixed: Vector2f wrapper
    window.draw(name);
    yPos += LINE_HEIGHT + 10;

    // State symbol (SFML 3.x: Text requires font in constructor)
    std::string stateStr = getAbilityStateSymbol(m_selectedAbility);
    sf::Text state(m_terminalFont, stateStr, CHAR_SIZE);
    state.setFillColor(getAbilityColor(m_selectedAbility));
    state.setPosition(sf::Vector2f(xPos, yPos));  // Fixed: Vector2f wrapper
    window.draw(state);
    yPos += LINE_HEIGHT + 10;

    // Description (wrapped) (SFML 3.x: Text requires font in constructor)
    sf::Text desc(m_terminalFont, m_selectedAbility->getDescription(), 16);
    desc.setFillColor(m_colorTextDim);
    desc.setPosition(sf::Vector2f(xPos, yPos));  // Fixed: Vector2f wrapper
    window.draw(desc);
    yPos += LINE_HEIGHT * 3;

    // Requirements
    const auto& req = m_selectedAbility->getRequirement();
    std::ostringstream reqSS;
    reqSS << "Requirements:\n"
          << "Level: " << req.requiredLevel << "\n"
          << "Cost: " << req.skillPointCost;

    // SFML 3.x: Text requires font in constructor
    sf::Text reqText(m_terminalFont, reqSS.str(), 16);
    reqText.setFillColor(m_colorTextDim);
    reqText.setPosition(sf::Vector2f(xPos, yPos));  // Fixed: Vector2f wrapper
    window.draw(reqText);
    yPos += LINE_HEIGHT * 4;

    // Effect
    const auto& effect = m_selectedAbility->getEffect();
    std::ostringstream effectSS;
    effectSS << "Effect:\n";

    if (m_selectedAbility->getType() == AbilityType::PASSIVE) {
        if (effect.bonusMultiplier != 0.0f) {
            effectSS << "Multiplier: +"
                    << std::fixed << std::setprecision(3)
                    << effect.bonusMultiplier << "\n";
        }
        if (effect.bonusFlat != 0) {
            effectSS << "Flat Bonus: +" << effect.bonusFlat << "\n";
        }
    } else {
        if (effect.preserveAmount > 0) {
            effectSS << "Preserve: "
                    << effect.preserveAmount << "\n";
        }
        if (effect.maxCharges > 0) {
            effectSS << "Max Charges: "
                    << effect.maxCharges << "\n";
        }
    }

    // SFML 3.x: Text requires font in constructor
    sf::Text effectText(m_terminalFont, effectSS.str(), 16);
    effectText.setFillColor(m_colorText);
    effectText.setPosition(sf::Vector2f(xPos, yPos));  // Fixed: Vector2f wrapper
    window.draw(effectText);
}

void AbilityTreeScene::renderFooter(sf::RenderWindow& window) {
    float yPos = WINDOW_HEIGHT - PADDING - 40;

    // SFML 3.x: Text requires font in constructor
    sf::Text controls(m_terminalFont,
                     "[1-5] Category  [W/S] Navigate  "
                     "[Enter] Unlock  [Esc] Exit",
                     16);
    controls.setFillColor(m_colorTextDim);
    controls.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 500, yPos));  // Fixed: Vector2f wrapper
    window.draw(controls);
}

std::string AbilityTreeScene::getAbilityStateSymbol(const AbilityNode* ability) const {
    if (!ability) return "[?]";
    
    if (ability->isUnlocked()) {
        return "[✓] UNLOCKED";
    }

    // Check if can be unlocked
    std::vector<std::string> unlockedIds;
    for (const auto& node : AbilityTreeSystem::getInstance().getAllAbilities()) {
        if (node.isUnlocked()) {
            unlockedIds.push_back(node.getId());
        }
    }

    if (ability->canUnlock(m_playerLevel, m_availableSkillPoints, unlockedIds)) {
        return "[ ] AVAILABLE";
    }

    return "[✗] LOCKED";
}

std::string AbilityTreeScene::getAbilityLine(const AbilityNode* ability, int indentLevel) const {
    if (!ability) return "";
    
    std::ostringstream ss;
    
    // Indent
    for (int i = 0; i < indentLevel; ++i) {
        ss << "  ";
    }
    
    // State symbol
    ss << getAbilityStateSymbol(ability) << " ";
    
    // Name
    ss << ability->getName();
    
    // Type indicator for active perks
    if (ability->getType() == AbilityType::ACTIVE) {
        ss << " 💡";
    }
    
    return ss.str();
}

sf::Color AbilityTreeScene::getAbilityColor(const AbilityNode* ability) const {
    if (!ability) return m_colorTextDim;
    
    if (ability->isUnlocked()) {
        return m_colorText;  // Bright green
    }
    
    // Check if available
    std::vector<std::string> unlockedIds;
    for (const auto& node : AbilityTreeSystem::getInstance().getAllAbilities()) {
        if (node.isUnlocked()) {
            unlockedIds.push_back(node.getId());
        }
    }
    
    if (ability->canUnlock(m_playerLevel, m_availableSkillPoints, unlockedIds)) {
        return m_colorHighlight;  // Bright green (available to unlock)
    }
    
    return m_colorLocked;  // Very dim green (locked)
}

SceneType AbilityTreeScene::getNextScene() const {
    return m_nextScene;
}

bool AbilityTreeScene::isFinished() const {
    return m_isFinished;
}
