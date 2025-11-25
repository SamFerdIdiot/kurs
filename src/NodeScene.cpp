#include "NodeScene.h"
#include "GameStateManager.h"
#include <iostream>

NodeScene::NodeScene(const std::string& nodeId, const std::string& nodeName)
    : m_nodeId(nodeId),
      m_nodeName(nodeName),
      m_currentPageIndex(0),
      m_selectedOptionIndex(0),
      m_finished(false),
      m_nextScene(SceneType::NODE),
      m_fontLoaded(false) {
    
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("NodeScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Try to load font with fallback options
    // Priority: 1. assets/fonts, 2. images fonts, 3. system fonts
    if (m_font.loadFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
        std::cout << "NodeScene: Loaded font from assets/fonts/font.ttf" << std::endl;
    } else if (m_font.loadFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
        std::cout << "NodeScene: Loaded font from images/Press_Start_2P" << std::endl;
    } else if (m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
        std::cout << "NodeScene: Loaded system font DejaVuSans" << std::endl;
    } else {
        m_fontLoaded = false;
        std::cerr << "NodeScene: WARNING - Could not load any font! Text may not display." << std::endl;
    }
    
    // Initialize structures first - this populates m_structures
    initializeStructures();
    
    // Initialize menu pages based on available structures
    initializePages();
    
    // Setup visual elements - use responsive scaling
    m_background.setSize(sf::Vector2f(layout.getTargetWidth(), layout.getTargetHeight()));
    m_background.setFillColor(sf::Color(40, 40, 50));
    m_background.setPosition(0.0f, 0.0f);
    
    m_selectionHighlight.setSize(SCALE_SIZE(layout, 1000.0f, 50.0f));
    m_selectionHighlight.setFillColor(sf::Color(70, 130, 180, 100));
    
    // Setup title text with responsive scaling
    m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(SCALE_FONT(layout, 32));
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(SCALE_POS(layout, 100.0f, 50.0f));
    
    // Setup page indicator with responsive scaling
    m_pageIndicatorText.setFont(m_font);
    m_pageIndicatorText.setCharacterSize(SCALE_FONT(layout, 20));
    m_pageIndicatorText.setFillColor(sf::Color(200, 200, 200));
    m_pageIndicatorText.setPosition(SCALE_POS(layout, 1100.0f, 60.0f));
}

void NodeScene::initializeStructures() {
    // Initialize structures that are present in this node
    // This would normally come from node data, but for now we hardcode some
    
    // Add a gas station
    m_structures.push_back(std::make_unique<Structure>(
        LocationType::GAS_STATION,
        "Pete's Gas Station",
        sf::Vector2f(150.0f, 500.0f)
    ));
    
    // Add a store
    m_structures.push_back(std::make_unique<Structure>(
        LocationType::STORE,
        "Roadside Store",
        sf::Vector2f(400.0f, 500.0f)
    ));
    
    // Add a motel
    m_structures.push_back(std::make_unique<Structure>(
        LocationType::MOTEL,
        "Highway Motel",
        sf::Vector2f(650.0f, 500.0f)
    ));
    
    // Note: Different nodes could have different structures
    // For example, some nodes might have a DINER instead of MOTEL
    // or a GARAGE instead of STORE
}

void NodeScene::initializePages() {
    // Создание единого меню навигации с переходами в разные сцены
    // Новая структура меню согласно Phase 3 спецификации
    m_pages.clear();
    
    // Единственная страница меню с основными опциями
    MenuPage mainMenu;
    mainMenu.title = "NODE MENU";
    mainMenu.structureType = "main_menu";
    mainMenu.locationType = LocationType::UNKNOWN;
    mainMenu.options = {
        "1. Map",
        "2. Character",
        "3. Inventory",
        "4. Companions",
        "5. Quests",
        "6. Ability Tree"
    };
    mainMenu.descriptions = {
        "View world map and travel to next destination",
        "Check character stats, level, and progression",
        "Manage inventory and equipment",
        "View party members and companions",
        "Check active and completed quests",
        "Unlock and upgrade abilities"
    };
    m_pages.push_back(mainMenu);
}

void NodeScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::A:
            case sf::Keyboard::Left:
                previousPage();
                break;
            case sf::Keyboard::D:
            case sf::Keyboard::Right:
                nextPage();
                break;
            case sf::Keyboard::Up:
                selectPrevious();
                break;
            case sf::Keyboard::Down:
                selectNext();
                break;
            case sf::Keyboard::Enter:
                confirmSelection();
                break;
            case sf::Keyboard::Escape:
                m_finished = true;
                m_nextScene = SceneType::MAIN_MENU;
                break;
            // Keyboard shortcuts for quick access
            case sf::Keyboard::M:
                m_finished = true;
                m_nextScene = SceneType::WORLD_MAP;
                break;
            case sf::Keyboard::C:
                m_finished = true;
                m_nextScene = SceneType::CHARACTER;
                break;
            case sf::Keyboard::I:
                m_finished = true;
                m_nextScene = SceneType::INVENTORY;
                break;
            case sf::Keyboard::P:
                m_finished = true;
                m_nextScene = SceneType::COMPANIONS;
                break;
            case sf::Keyboard::Q:
                m_finished = true;
                m_nextScene = SceneType::QUESTS;
                break;
            case sf::Keyboard::T:
                m_finished = true;
                m_nextScene = SceneType::ABILITY_TREE;
                break;
            default:
                break;
        }
    }
}

void NodeScene::update(float deltaTime) {
    // Update page indicator text
    if (!m_pages.empty()) {
        m_pageIndicatorText.setString(
            "Page " + std::to_string(m_currentPageIndex + 1) + 
            "/" + std::to_string(m_pages.size())
        );
    }
}

void NodeScene::render(sf::RenderWindow& window) {
    // Draw background
    window.draw(m_background);
    
    if (m_pages.empty() || m_currentPageIndex >= m_pages.size()) {
        return;
    }
    
    const MenuPage& currentPage = m_pages[m_currentPageIndex];
    
    // Draw title
    m_titleText.setString(currentPage.title);
    window.draw(m_titleText);
    
    // Draw page indicator
    window.draw(m_pageIndicatorText);
    
    // Draw options - adjusted for full window size
    float startY = 150.0f;
    float spacing = 80.0f;
    
    for (size_t i = 0; i < currentPage.options.size(); ++i) {
        float yPos = startY + i * spacing;
        
        // Draw selection highlight
        if (i == m_selectedOptionIndex) {
            m_selectionHighlight.setPosition(150.0f, yPos - 10.0f);
            window.draw(m_selectionHighlight);
        }
        
        // Draw option text
        sf::Text optionText;
        optionText.setFont(m_font);  // Always set font, even if it failed to load (SFML requires a font)
        optionText.setString((i == m_selectedOptionIndex ? "> " : "  ") + currentPage.options[i]);
        optionText.setCharacterSize(24);
        optionText.setFillColor(i == m_selectedOptionIndex ? sf::Color::Yellow : sf::Color::White);
        optionText.setPosition(170.0f, yPos);
        window.draw(optionText);
        
        // Draw description
        sf::Text descText;
        descText.setFont(m_font);  // Always set font
        descText.setString(currentPage.descriptions[i]);
        descText.setCharacterSize(16);
        descText.setFillColor(sf::Color(180, 180, 180));
        descText.setPosition(190.0f, yPos + 30.0f);
        window.draw(descText);
    }
    
    // Draw player status - positioned for full window with better spacing
    PlayerState& playerState = GameStateManager::getInstance().getPlayerState();
    sf::Text statusText;
    statusText.setFont(m_font);  // Always set font
    statusText.setString("Money: $" + std::to_string(static_cast<int>(playerState.getMoney())) + 
                        "  |  Fuel: " + std::to_string(static_cast<int>(playerState.getFuel())) + "L" +
                        "  |  Energy: " + std::to_string(static_cast<int>(playerState.getEnergy())));
    statusText.setCharacterSize(16);
    statusText.setFillColor(sf::Color::Cyan);
    statusText.setPosition(100.0f, 660.0f);
    window.draw(statusText);
    
    // Draw controls hint - positioned for full window with more spacing
    sf::Text controlsText;
    controlsText.setFont(m_font);  // Always set font
    controlsText.setString("[Up/Down: Select] [Enter: Confirm] [ESC: Exit] [M/C/I/P/Q/T: Quick Access]");
    controlsText.setCharacterSize(14);
    controlsText.setFillColor(sf::Color(150, 150, 150));
    controlsText.setPosition(100.0f, 710.0f);
    window.draw(controlsText);
}

SceneType NodeScene::getNextScene() const {
    return m_nextScene;
}

bool NodeScene::isFinished() const {
    return m_finished;
}

void NodeScene::nextPage() {
    if (!m_pages.empty()) {
        m_currentPageIndex = (m_currentPageIndex + 1) % m_pages.size();
        m_selectedOptionIndex = 0;  // Reset selection when switching pages
    }
}

void NodeScene::previousPage() {
    if (!m_pages.empty()) {
        m_currentPageIndex = (m_currentPageIndex - 1 + m_pages.size()) % m_pages.size();
        m_selectedOptionIndex = 0;  // Reset selection when switching pages
    }
}

void NodeScene::selectPrevious() {
    if (!m_pages.empty() && m_currentPageIndex < m_pages.size()) {
        const MenuPage& page = m_pages[m_currentPageIndex];
        if (!page.options.empty()) {
            m_selectedOptionIndex = (m_selectedOptionIndex - 1 + page.options.size()) % page.options.size();
        }
    }
}

void NodeScene::selectNext() {
    if (!m_pages.empty() && m_currentPageIndex < m_pages.size()) {
        const MenuPage& page = m_pages[m_currentPageIndex];
        if (!page.options.empty()) {
            m_selectedOptionIndex = (m_selectedOptionIndex + 1) % page.options.size();
        }
    }
}

void NodeScene::confirmSelection() {
    if (!m_pages.empty() && m_currentPageIndex < m_pages.size()) {
        const MenuPage& page = m_pages[m_currentPageIndex];
        if (m_selectedOptionIndex < page.options.size()) {
            executeAction(m_currentPageIndex, m_selectedOptionIndex);
        }
    }
}

void NodeScene::executeAction(int pageIndex, int optionIndex) {
    if (pageIndex < 0 || pageIndex >= m_pages.size()) return;
    
    const MenuPage& page = m_pages[pageIndex];
    std::cout << "Executing action: " << page.title << ", Option " << optionIndex << std::endl;
    
    PlayerState& playerState = GameStateManager::getInstance().getPlayerState();
    
    // Обработка главного меню
    if (page.structureType == "main_menu") {
        switch (optionIndex) {
            case 0: // Map
                m_finished = true;
                m_nextScene = SceneType::WORLD_MAP;
                std::cout << "Opening world map..." << std::endl;
                break;
            case 1: // Character
                m_finished = true;
                m_nextScene = SceneType::CHARACTER;
                std::cout << "Opening character screen..." << std::endl;
                break;
            case 2: // Inventory
                m_finished = true;
                m_nextScene = SceneType::INVENTORY;
                std::cout << "Opening inventory..." << std::endl;
                break;
            case 3: // Companions
                m_finished = true;
                m_nextScene = SceneType::COMPANIONS;
                std::cout << "Opening companions screen..." << std::endl;
                break;
            case 4: // Quests
                m_finished = true;
                m_nextScene = SceneType::QUESTS;
                std::cout << "Opening quest journal..." << std::endl;
                break;
            case 5: // Ability Tree
                m_finished = true;
                m_nextScene = SceneType::ABILITY_TREE;
                std::cout << "Opening ability tree..." << std::endl;
                break;
        }
    }
}

