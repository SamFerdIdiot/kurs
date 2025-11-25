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
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("AbilityTreeScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Try to load monospace font
    // Prefer terminal-style fonts like Courier New or Fixedsys
    m_fontLoaded = m_terminalFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
    
    if (!m_fontLoaded) {
        // Try alternative fonts
        m_fontLoaded = m_terminalFont.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");
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
        // TODO: Play unlock sound
    } else {
        std::cout << "Cannot unlock ability: " << m_selectedAbility->getName() << std::endl;
        // TODO: Play error sound
    }
}

void AbilityTreeScene::goBack() {
    m_isFinished = true;
}

void AbilityTreeScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            // Navigation
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                navigateUp();
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                navigateDown();
                break;
            
            // Category selection (1-5 keys)
            case sf::Keyboard::Num1:
                switchCategory(AbilityCategory::DRIVING);
                break;
            case sf::Keyboard::Num2:
                switchCategory(AbilityCategory::TRADING);
                break;
            case sf::Keyboard::Num3:
                switchCategory(AbilityCategory::SURVIVAL);
                break;
            case sf::Keyboard::Num4:
                switchCategory(AbilityCategory::MECHANICS);
                break;
            case sf::Keyboard::Num5:
                switchCategory(AbilityCategory::SOCIAL);
                break;
            
            // Actions
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                attemptUnlock();
                break;
            case sf::Keyboard::Escape:
            case sf::Keyboard::Tab:
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
    border.setPosition(PADDING, PADDING);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(m_colorBorder);
    border.setOutlineThickness(BORDER_THICKNESS);
    window.draw(border);
}

void AbilityTreeScene::renderHeader(sf::RenderWindow& window) {
    // Title
    sf::Text title;
    title.setFont(m_terminalFont);
    title.setCharacterSize(28);
    title.setFillColor(m_colorText);
    title.setString("ДЕРЕВО СПОСОБНОСТЕЙ / ABILITY TREE");
    title.setPosition(WINDOW_WIDTH / 2 - 300, PADDING + 15);
    window.draw(title);
    
    // Player info
    std::ostringstream ss;
    ss << "УРОВЕНЬ / LEVEL: " << m_playerLevel 
       << "   ОЧКИ / POINTS: " << m_availableSkillPoints;
    
    sf::Text info;
    info.setFont(m_terminalFont);
    info.setCharacterSize(CHAR_SIZE);
    info.setFillColor(m_colorTextDim);
    info.setString(ss.str());
    info.setPosition(WINDOW_WIDTH / 2 - 200, PADDING + 55);
    window.draw(info);
    
    // Separator line
    sf::RectangleShape separator;
    separator.setSize(sf::Vector2f(WINDOW_WIDTH - 4 * PADDING, 2));
    separator.setPosition(2 * PADDING, PADDING + 90);
    separator.setFillColor(m_colorBorder);
    window.draw(separator);
}

void AbilityTreeScene::renderCategoryList(sf::RenderWindow& window) {
    float yPos = PADDING + 110;
    
    // Category tabs
    std::vector<std::pair<AbilityCategory, std::string>> categories = {
        {AbilityCategory::DRIVING, "[1] ВОЖДЕНИЕ / DRIVING"},
        {AbilityCategory::TRADING, "[2] ТОРГОВЛЯ / TRADING"},
        {AbilityCategory::SURVIVAL, "[3] ВЫЖИВАНИЕ / SURVIVAL"},
        {AbilityCategory::MECHANICS, "[4] МЕХАНИКА / MECHANICS"},
        {AbilityCategory::SOCIAL, "[5] СОЦИАЛЬНЫЕ / SOCIAL"}
    };
    
    for (const auto& [cat, name] : categories) {
        sf::Text catText;
        catText.setFont(m_terminalFont);
        catText.setCharacterSize(CHAR_SIZE);
        
        if (cat == m_currentCategory) {
            catText.setFillColor(m_colorHighlight);
            catText.setString("> " + name);
        } else {
            catText.setFillColor(m_colorTextDim);
            catText.setString("  " + name);
        }
        
        catText.setPosition(2 * PADDING, yPos);
        window.draw(catText);
        yPos += LINE_HEIGHT;
    }
}

void AbilityTreeScene::renderAbilityTree(sf::RenderWindow& window) {
    float yPos = PADDING + 110;
    float xPos = WINDOW_WIDTH / 2 - 200;
    
    // Tree title
    sf::Text treeTitle;
    treeTitle.setFont(m_terminalFont);
    treeTitle.setCharacterSize(CHAR_SIZE + 2);
    treeTitle.setFillColor(m_colorText);
    treeTitle.setString("СПОСОБНОСТИ / ABILITIES");
    treeTitle.setPosition(xPos, yPos);
    window.draw(treeTitle);
    yPos += LINE_HEIGHT + 10;
    
    // Render abilities in tree structure
    for (size_t i = 0; i < m_displayedAbilities.size(); ++i) {
        const AbilityNode* ability = m_displayedAbilities[i];
        if (!ability) continue;
        
        sf::Text abilityText;
        abilityText.setFont(m_terminalFont);
        abilityText.setCharacterSize(CHAR_SIZE);
        abilityText.setFillColor(getAbilityColor(ability));
        
        // Add cursor for selected
        std::string line = (i == static_cast<size_t>(m_selectedIndex) && m_blinkState) ? "> " : "  ";
        line += getAbilityLine(ability, 0);
        
        abilityText.setString(line);
        abilityText.setPosition(xPos, yPos);
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
    panel.setPosition(xPos, yPos);
    panel.setFillColor(sf::Color::Transparent);
    panel.setOutlineColor(m_colorBorder);
    panel.setOutlineThickness(2);
    window.draw(panel);
    
    yPos += 15;
    xPos += 15;
    
    // Ability name
    sf::Text name;
    name.setFont(m_terminalFont);
    name.setCharacterSize(20);
    name.setFillColor(m_colorText);
    name.setString(m_selectedAbility->getName());
    name.setPosition(xPos, yPos);
    window.draw(name);
    yPos += LINE_HEIGHT + 10;
    
    // State symbol
    std::string stateStr = getAbilityStateSymbol(m_selectedAbility);
    sf::Text state;
    state.setFont(m_terminalFont);
    state.setCharacterSize(CHAR_SIZE);
    state.setFillColor(getAbilityColor(m_selectedAbility));
    state.setString(stateStr);
    state.setPosition(xPos, yPos);
    window.draw(state);
    yPos += LINE_HEIGHT + 10;
    
    // Description (wrapped)
    sf::Text desc;
    desc.setFont(m_terminalFont);
    desc.setCharacterSize(16);
    desc.setFillColor(m_colorTextDim);
    desc.setString(m_selectedAbility->getDescription());
    desc.setPosition(xPos, yPos);
    window.draw(desc);
    yPos += LINE_HEIGHT * 3;
    
    // Requirements
    const auto& req = m_selectedAbility->getRequirement();
    std::ostringstream reqSS;
    reqSS << "Требования / Requirements:\n"
          << "Уровень / Level: " << req.requiredLevel << "\n"
          << "Очки / Cost: " << req.skillPointCost;
    
    sf::Text reqText;
    reqText.setFont(m_terminalFont);
    reqText.setCharacterSize(16);
    reqText.setFillColor(m_colorTextDim);
    reqText.setString(reqSS.str());
    reqText.setPosition(xPos, yPos);
    window.draw(reqText);
    yPos += LINE_HEIGHT * 4;
    
    // Effect
    const auto& effect = m_selectedAbility->getEffect();
    std::ostringstream effectSS;
    effectSS << "Эффект / Effect:\n";
    
    if (m_selectedAbility->getType() == AbilityType::PASSIVE) {
        if (effect.bonusMultiplier != 0.0f) {
            effectSS << "Множитель / Multiplier: +" 
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
    
    sf::Text effectText;
    effectText.setFont(m_terminalFont);
    effectText.setCharacterSize(16);
    effectText.setFillColor(m_colorText);
    effectText.setString(effectSS.str());
    effectText.setPosition(xPos, yPos);
    window.draw(effectText);
}

void AbilityTreeScene::renderFooter(sf::RenderWindow& window) {
    float yPos = WINDOW_HEIGHT - PADDING - 40;
    
    sf::Text controls;
    controls.setFont(m_terminalFont);
    controls.setCharacterSize(16);
    controls.setFillColor(m_colorTextDim);
    controls.setString("[1-5] Категория / Category  [W/S] Навигация / Navigate  "
                      "[Enter] Открыть / Unlock  [Esc] Выход / Exit");
    controls.setPosition(WINDOW_WIDTH / 2 - 500, yPos);
    window.draw(controls);
}

std::string AbilityTreeScene::getAbilityStateSymbol(const AbilityNode* ability) const {
    if (!ability) return "[?]";
    
    if (ability->isUnlocked()) {
        return "[✓] ОТКРЫТО / UNLOCKED";
    }
    
    // Check if can be unlocked
    std::vector<std::string> unlockedIds;
    for (const auto& node : AbilityTreeSystem::getInstance().getAllAbilities()) {
        if (node.isUnlocked()) {
            unlockedIds.push_back(node.getId());
        }
    }
    
    if (ability->canUnlock(m_playerLevel, m_availableSkillPoints, unlockedIds)) {
        return "[ ] ДОСТУПНО / AVAILABLE";
    }
    
    return "[✗] ЗАБЛОКИРОВАНО / LOCKED";
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
