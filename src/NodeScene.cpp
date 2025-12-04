#include "NodeScene.h"
#include "GameStateManager.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include <iostream>

NodeScene::NodeScene(const std::string& nodeId, const std::string& nodeName)
    : m_nodeId(nodeId),
      m_nodeName(nodeName),
      m_currentTabIndex(0),
      m_finished(false),
      m_nextScene(SceneType::NODE),
      m_fontLoaded(false) {

    // Load font
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "NodeScene: Failed to load font" << std::endl;
    }

    // Setup background
    m_background.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    m_background.setFillColor(UI::Color::BACKGROUND_DARK);

    // Initialize tabs
    initializeTabs();
}

void NodeScene::initializeTabs() {
    m_tabs.clear();

    // Fixed tabs (system menus)
    // [MVP] CHR tab removed - not implementing character stats in MVP

    Tab invTab;
    invTab.name = "INV";
    invTab.fullName = "Inventory";
    invTab.description = "Manage your inventory and equipment";
    invTab.targetScene = SceneType::INVENTORY;
    invTab.isStructure = false;
    m_tabs.push_back(invTab);

    Tab comTab;
    comTab.name = "COM";
    comTab.fullName = "Companions";
    comTab.description = "View and manage your party members";
    comTab.targetScene = SceneType::COMPANIONS;
    comTab.isStructure = false;
    m_tabs.push_back(comTab);

    // [MVP] QST tab removed - not implementing quests in MVP
    // [MVP] ABL tab removed - not implementing abilities in MVP

    Tab trvTab;
    trvTab.name = "TRV";
    trvTab.fullName = "Travel";
    trvTab.description = "Select your next destination and plan your route";
    trvTab.targetScene = SceneType::TRAVEL_SELECTION;
    trvTab.isStructure = false;
    m_tabs.push_back(trvTab);

    Tab notTab;
    notTab.name = "NOT";
    notTab.fullName = "Notes";
    notTab.description = "Write and manage your personal travel notes";
    notTab.targetScene = SceneType::NOTES;
    notTab.isStructure = false;
    m_tabs.push_back(notTab);

    Tab mapTab;
    mapTab.name = "MAP";
    mapTab.fullName = "World Map";
    mapTab.description = "View static world map (reference only)";
    mapTab.targetScene = SceneType::MAP;  // Opens static map viewer
    mapTab.isStructure = false;
    m_tabs.push_back(mapTab);

    // Load structure tabs dynamically
    loadStructures();

    // Fill remaining slots with empty tabs if needed
    while (m_tabs.size() < MAX_TABS) {
        Tab emptyTab;
        emptyTab.name = "";
        emptyTab.fullName = "";
        emptyTab.description = "";
        emptyTab.targetScene = SceneType::NODE;
        emptyTab.isStructure = false;
        m_tabs.push_back(emptyTab);
    }
}

void NodeScene::loadStructures() {
    // Get available structures from GameStateManager
    // For now, add default structures (will be dynamic later)

    Tab gasTab;
    gasTab.name = "GAS";
    gasTab.fullName = "Gas Station";
    gasTab.description = "Refuel your vehicle and buy supplies";
    gasTab.targetScene = SceneType::LOCATION;
    gasTab.locationType = LocationType::GAS_STATION;
    gasTab.isStructure = true;
    m_tabs.push_back(gasTab);

    Tab strTab;
    strTab.name = "STR";
    strTab.fullName = "Store";
    strTab.description = "Buy items and equipment";
    strTab.targetScene = SceneType::LOCATION;
    strTab.locationType = LocationType::STORE;
    strTab.isStructure = true;
    m_tabs.push_back(strTab);

    Tab motTab;
    motTab.name = "MOT";
    motTab.fullName = "Motel";
    motTab.description = "Rest and restore your energy";
    motTab.targetScene = SceneType::LOCATION;
    motTab.locationType = LocationType::MOTEL;
    motTab.isStructure = true;
    m_tabs.push_back(motTab);
}

void NodeScene::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::A:
            case sf::Keyboard::Key::Left:
                selectPreviousTab();
                break;

            case sf::Keyboard::Key::D:
            case sf::Keyboard::Key::Right:
                selectNextTab();
                break;

            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                confirmSelection();
                break;

            case sf::Keyboard::Key::Escape:
                m_finished = true;
                m_nextScene = SceneType::MAIN_MENU;
                break;

            default:
                break;
        }
    }
}

void NodeScene::update(float deltaTime) {
    // Update logic here if needed
}

void NodeScene::render(sf::RenderWindow& window) {
    // Draw background
    window.draw(m_background);

    if (!m_fontLoaded) return;

    // Render all components
    renderTopBar(window);
    renderPageContent(window);
    renderStatusBar(window);
    renderControls(window);
}

void NodeScene::renderTopBar(sf::RenderWindow& window) {
    // Top bar background
    sf::RectangleShape topBar;
    topBar.setSize(sf::Vector2f(UI::SCREEN_WIDTH, TOP_BAR_HEIGHT));
    topBar.setPosition(sf::Vector2f(0.0f, 0.0f));
    topBar.setFillColor(UI::Color::BACKGROUND_LIGHT);
    window.draw(topBar);

    // Draw tabs
    float xPos = TAB_SPACING;
    for (size_t i = 0; i < m_tabs.size(); ++i) {
        const Tab& tab = m_tabs[i];

        // Tab box
        sf::RectangleShape tabBox;
        tabBox.setSize(sf::Vector2f(TAB_WIDTH, TOP_BAR_HEIGHT - 20.0f));
        tabBox.setPosition(sf::Vector2f(xPos, 10.0f));

        // Highlight current tab
        if (i == m_currentTabIndex) {
            tabBox.setFillColor(UI::Color::ACCENT_BLUE);
            tabBox.setOutlineColor(UI::Color::ACCENT_YELLOW);
            tabBox.setOutlineThickness(2.0f);
        } else {
            tabBox.setFillColor(UI::Color::BUTTON_NORMAL);
            tabBox.setOutlineColor(UI::Color::BORDER_COLOR);
            tabBox.setOutlineThickness(1.0f);
        }

        window.draw(tabBox);

        // Tab text
        if (!tab.name.empty()) {
            sf::Text tabText(m_font, tab.name, 14);
            tabText.setFillColor(UI::Color::TEXT_PRIMARY);

            // Center text in tab
            sf::FloatRect textBounds = tabText.getLocalBounds();
            tabText.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
            tabText.setPosition(sf::Vector2f(xPos + TAB_WIDTH / 2.0f, TOP_BAR_HEIGHT / 2.0f));

            window.draw(tabText);
        } else {
            // Empty tab - draw brackets []
            sf::Text emptyText(m_font, "[ ]", 14);
            emptyText.setFillColor(UI::Color::TEXT_SECONDARY);

            sf::FloatRect textBounds = emptyText.getLocalBounds();
            emptyText.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
            emptyText.setPosition(sf::Vector2f(xPos + TAB_WIDTH / 2.0f, TOP_BAR_HEIGHT / 2.0f));

            window.draw(emptyText);
        }

        xPos += TAB_WIDTH + TAB_SPACING;
    }
}

void NodeScene::renderPageContent(sf::RenderWindow& window) {
    if (m_currentTabIndex < 0 || m_currentTabIndex >= m_tabs.size()) return;

    const Tab& currentTab = m_tabs[m_currentTabIndex];
    if (currentTab.name.empty()) return;

    float contentY = TOP_BAR_HEIGHT + CONTENT_PADDING;

    // Draw full name
    sf::Text titleText(m_font, currentTab.fullName, UI::FONT_SIZE_HUGE);
    titleText.setFillColor(UI::Color::TEXT_PRIMARY);
    titleText.setPosition(sf::Vector2f(CONTENT_PADDING, contentY));
    window.draw(titleText);

    contentY += 80.0f;

    // Draw description
    sf::Text descText(m_font, currentTab.description, UI::FONT_SIZE_NORMAL);
    descText.setFillColor(UI::Color::TEXT_SECONDARY);
    descText.setPosition(sf::Vector2f(CONTENT_PADDING, contentY));
    window.draw(descText);

    contentY += 60.0f;

    // Draw action prompt
    sf::Text actionText(m_font, "Press [ENTER] to open", UI::FONT_SIZE_SUBTITLE);
    actionText.setFillColor(UI::Color::ACCENT_YELLOW);
    actionText.setPosition(sf::Vector2f(CONTENT_PADDING, contentY));
    window.draw(actionText);
}

void NodeScene::renderStatusBar(sf::RenderWindow& window) {
    float statusY = UI::SCREEN_HEIGHT - STATUS_BAR_HEIGHT;

    // Status bar background
    sf::RectangleShape statusBar;
    statusBar.setSize(sf::Vector2f(UI::SCREEN_WIDTH, STATUS_BAR_HEIGHT));
    statusBar.setPosition(sf::Vector2f(0.0f, statusY));
    statusBar.setFillColor(UI::Color::BACKGROUND_LIGHT);
    window.draw(statusBar);

    // [MVP] Disabled - Resource status display (uncomment to enable)
    /*
    // Get player state
    PlayerState& playerState = GameStateManager::getInstance().getPlayerState();

    // Draw status text
    std::string statusString =
        "Money: $" + std::to_string(static_cast<int>(playerState.getMoney())) +
        "  |  Fuel: " + std::to_string(static_cast<int>(playerState.getFuel())) + "L" +
        "  |  Energy: " + std::to_string(static_cast<int>(playerState.getEnergy()));

    sf::Text statusText(m_font, statusString, UI::FONT_SIZE_NORMAL);
    statusText.setFillColor(UI::Color::ACCENT_BLUE);
    statusText.setPosition(sf::Vector2f(CONTENT_PADDING, statusY + 15.0f));
    window.draw(statusText);
    */
}

void NodeScene::renderControls(sf::RenderWindow& window) {
    float controlsY = UI::SCREEN_HEIGHT - STATUS_BAR_HEIGHT + 45.0f;

    sf::Text controlsText(m_font,
        "[A/D] Page  [Enter] Confirm  [Esc] Exit",
        UI::FONT_SIZE_SMALL);
    controlsText.setFillColor(UI::Color::TEXT_SECONDARY);
    controlsText.setPosition(sf::Vector2f(CONTENT_PADDING, controlsY));
    window.draw(controlsText);
}

void NodeScene::selectPreviousTab() {
    // Move to previous non-empty tab
    int originalIndex = m_currentTabIndex;

    do {
        m_currentTabIndex = (m_currentTabIndex - 1 + m_tabs.size()) % m_tabs.size();

        // Stop if we found a non-empty tab or cycled back
        if (!m_tabs[m_currentTabIndex].name.empty() || m_currentTabIndex == originalIndex) {
            break;
        }
    } while (true);
}

void NodeScene::selectNextTab() {
    // Move to next non-empty tab
    int originalIndex = m_currentTabIndex;

    do {
        m_currentTabIndex = (m_currentTabIndex + 1) % m_tabs.size();

        // Stop if we found a non-empty tab or cycled back
        if (!m_tabs[m_currentTabIndex].name.empty() || m_currentTabIndex == originalIndex) {
            break;
        }
    } while (true);
}

void NodeScene::confirmSelection() {
    if (m_currentTabIndex < 0 || m_currentTabIndex >= m_tabs.size()) return;

    const Tab& currentTab = m_tabs[m_currentTabIndex];
    if (currentTab.name.empty()) return;

    // Set location type if it's a structure
    if (currentTab.isStructure) {
        GameStateManager::getInstance().setCurrentLocationType(currentTab.locationType);
        std::cout << "Opening structure: " << currentTab.fullName << std::endl;
    } else {
        std::cout << "Opening scene: " << currentTab.fullName << std::endl;
    }

    // Transition to target scene
    m_finished = true;
    m_nextScene = currentTab.targetScene;
}

SceneType NodeScene::getNextScene() const {
    return m_nextScene;
}

bool NodeScene::isFinished() const {
    return m_finished;
}
