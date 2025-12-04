#include "TravelSelectionScene.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include "GameStateManager.h"
#include <iostream>

TravelSelectionScene::TravelSelectionScene(const std::string& currentCityId)
    : m_currentCityId(currentCityId),
      m_currentCityName("Unknown"),
      m_selectedIndex(0),
      m_finished(false),
      m_nextScene(SceneType::NODE),
      m_fontLoaded(false) {

    // Load font
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "TravelSelectionScene: Failed to load font" << std::endl;
    }

    // Setup background
    m_background.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    m_background.setFillColor(UI::Color::BACKGROUND_DARK);

    // Load available destinations
    loadDestinations();

    // Get current city name (hardcoded for now, database system not yet implemented)
    m_currentCityName = "Moscow";
}

void TravelSelectionScene::loadDestinations() {
    m_destinations.clear();

    // [MVP] Bi-directional travel: Moscow ↔ N.Novgorod ↔ Kazan ↔ Ekaterinburg ↔ Vladivostok
    auto& playerState = GameStateManager::getInstance().getPlayerState();
    int currentCityIndex = playerState.getCurrentCityIndex();

    std::cout << "[MVP] TravelSelectionScene: currentCityIndex = " << currentCityIndex << std::endl;

    // City names array
    std::string cityNames[5] = {"Moscow", "N.Novgorod", "Kazan", "Ekaterinburg", "Vladivostok"};
    std::string cityIds[5] = {"city_0", "city_1", "city_2", "city_3", "city_4"};

    // [MVP] One-way connections configuration (modify to add one-way roads)
    // By default, ALL connections are bidirectional
    // To make a connection one-way: set isBidirectional = false for specific city pairs
    // Example: if you want Moscow→N.Novgorod to be one-way only, uncomment:
    // bool cityConnections[5][5]; // [from][to] = isBidirectional
    // For now, all connections are bidirectional (default behavior)

    // Update current city name
    if (currentCityIndex >= 0 && currentCityIndex < 5) {
        m_currentCityName = cityNames[currentCityIndex];
    }

    // [MVP] Show previous city (backward travel) if not at the start
    // Check if backward connection exists and is bidirectional
    if (currentCityIndex > 0) {
        int prevCityIndex = currentCityIndex - 1;

        // [MVP] By default all connections are bidirectional
        // To make one-way: add check here (e.g., if (cityConnections[currentCityIndex][prevCityIndex]))
        bool canGoBack = true;  // Default: bidirectional

        if (canGoBack) {
            CityConnection prevCity;
            prevCity.cityId = cityIds[prevCityIndex];
            prevCity.cityName = "← " + cityNames[prevCityIndex];  // Arrow to indicate backward
            prevCity.cityNumber = std::to_string(prevCityIndex + 1);
            prevCity.distance = 100.0f;  // Same distance both ways
            prevCity.fuelCost = 0.0f;
            prevCity.isAccessible = true;
            prevCity.isBidirectional = true;  // Default: bidirectional
            m_destinations.push_back(prevCity);

            std::cout << "[MVP] Previous destination: " << cityNames[prevCityIndex] << " (city " << prevCityIndex << ")" << std::endl;
        }
    }

    // [MVP] Show next city (forward travel) if not at the end
    if (currentCityIndex < 4) {
        int nextCityIndex = currentCityIndex + 1;

        CityConnection nextCity;
        nextCity.cityId = cityIds[nextCityIndex];
        nextCity.cityName = cityNames[nextCityIndex] + " →";  // Arrow to indicate forward
        nextCity.cityNumber = std::to_string(nextCityIndex + 1);
        nextCity.distance = 100.0f;  // Simplified for MVP
        nextCity.fuelCost = 0.0f;    // No fuel cost in MVP
        nextCity.isAccessible = true;
        nextCity.isBidirectional = true;  // Default: bidirectional (can travel back)
        m_destinations.push_back(nextCity);

        std::cout << "[MVP] Next destination: " << nextCity.cityName << " (city " << nextCityIndex << ")" << std::endl;
    }

    // If no destinations, add a "no routes" message
    if (m_destinations.empty()) {
        std::cerr << "[MVP] No available destinations from city " << currentCityIndex << std::endl;
    }
}

void TravelSelectionScene::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::W:
            case sf::Keyboard::Key::Up:
                selectPrevious();
                break;

            case sf::Keyboard::Key::S:
            case sf::Keyboard::Key::Down:
                selectNext();
                break;

            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                confirmSelection();
                break;

            case sf::Keyboard::Key::Escape:
                m_finished = true;
                m_nextScene = SceneType::NODE;
                break;

            default:
                break;
        }
    }
}

void TravelSelectionScene::update(float deltaTime) {
    // Update logic here if needed
}

void TravelSelectionScene::render(sf::RenderWindow& window) {
    // Draw background
    window.draw(m_background);

    if (!m_fontLoaded) return;

    // Render components
    renderHeader(window);
    renderDestinations(window);
    renderFooter(window);
}

void TravelSelectionScene::renderHeader(sf::RenderWindow& window) {
    float yPos = 40.0f;

    // Title
    sf::Text titleText(m_font, "SELECT DESTINATION", UI::FONT_SIZE_HUGE);
    titleText.setFillColor(UI::Color::TEXT_PRIMARY);
    titleText.setPosition(sf::Vector2f(40.0f, yPos));
    window.draw(titleText);

    yPos += 70.0f;

    // Current location
    std::string currentLocText = "Current: " + m_currentCityName;
    sf::Text currentText(m_font, currentLocText, UI::FONT_SIZE_NORMAL);
    currentText.setFillColor(UI::Color::ACCENT_YELLOW);
    currentText.setPosition(sf::Vector2f(40.0f, yPos));
    window.draw(currentText);

    yPos += 50.0f;

    // Separator
    sf::RectangleShape separator;
    separator.setSize(sf::Vector2f(UI::SCREEN_WIDTH - 80.0f, 2.0f));
    separator.setPosition(sf::Vector2f(40.0f, yPos));
    separator.setFillColor(UI::Color::BORDER_COLOR);
    window.draw(separator);
}

void TravelSelectionScene::renderDestinations(sf::RenderWindow& window) {
    float yPos = 200.0f;

    if (m_destinations.empty()) {
        sf::Text noRoutesText(m_font, "No routes available from this location", UI::FONT_SIZE_NORMAL);
        noRoutesText.setFillColor(UI::Color::TEXT_SECONDARY);
        noRoutesText.setPosition(sf::Vector2f(40.0f, yPos));
        window.draw(noRoutesText);
        return;
    }

    for (size_t i = 0; i < m_destinations.size(); ++i) {
        const CityConnection& dest = m_destinations[i];

        // Selection highlight
        if (i == m_selectedIndex) {
            sf::RectangleShape highlight;
            highlight.setSize(sf::Vector2f(UI::SCREEN_WIDTH - 80.0f, 60.0f));
            highlight.setPosition(sf::Vector2f(40.0f, yPos - 5.0f));
            highlight.setFillColor(UI::Color::SELECTION_HIGHLIGHT);
            window.draw(highlight);
        }

        // City number and name
        std::string cityText = (i == m_selectedIndex ? "> " : "  ");
        cityText += "[" + dest.cityNumber + "] " + dest.cityName;
        sf::Text nameText(m_font, cityText, UI::FONT_SIZE_SUBTITLE);
        nameText.setFillColor(i == m_selectedIndex ? UI::Color::ACCENT_YELLOW : UI::Color::TEXT_PRIMARY);
        nameText.setPosition(sf::Vector2f(60.0f, yPos));
        window.draw(nameText);

        // Distance and fuel info
        std::string infoText = "Distance: " + std::to_string(static_cast<int>(dest.distance)) + " km  |  " +
                              "Fuel: " + std::to_string(static_cast<int>(dest.fuelCost)) + " L";
        sf::Text infoTextObj(m_font, infoText, UI::FONT_SIZE_SMALL);
        infoTextObj.setFillColor(UI::Color::TEXT_SECONDARY);
        infoTextObj.setPosition(sf::Vector2f(80.0f, yPos + 30.0f));
        window.draw(infoTextObj);

        yPos += 80.0f;
    }
}

void TravelSelectionScene::renderFooter(sf::RenderWindow& window) {
    float yPos = UI::SCREEN_HEIGHT - 100.0f;

    // Player resources
    // [MVP] Disabled - Resource display (uncomment to enable)
    /*
    PlayerState& playerState = GameStateManager::getInstance().getPlayerState();
    std::string resourcesText =
        "Your Fuel: " + std::to_string(static_cast<int>(playerState.getFuel())) + " L  |  " +
        "Money: $" + std::to_string(static_cast<int>(playerState.getMoney()));
    sf::Text resourcesTextObj(m_font, resourcesText, UI::FONT_SIZE_NORMAL);
    resourcesTextObj.setFillColor(UI::Color::ACCENT_BLUE);
    resourcesTextObj.setPosition(sf::Vector2f(UI::SCREEN_WIDTH / 2.0f - 150.0f, 800.0f));
    window.draw(resourcesTextObj);
    */

    // [MVP] Simple placeholder
    std::string resourcesText = "Ready to travel";
    sf::Text resourcesTextObj(m_font, resourcesText, UI::FONT_SIZE_NORMAL);
    resourcesTextObj.setFillColor(UI::Color::ACCENT_BLUE);
    resourcesTextObj.setPosition(sf::Vector2f(40.0f, yPos));
    window.draw(resourcesTextObj);

    yPos += 40.0f;

    // Controls
    sf::Text controlsText(m_font,
        "[W/S] Select  [Enter] Travel  [Esc] Back",
        UI::FONT_SIZE_SMALL);
    controlsText.setFillColor(UI::Color::TEXT_SECONDARY);
    controlsText.setPosition(sf::Vector2f(40.0f, yPos));
    window.draw(controlsText);
}

void TravelSelectionScene::selectPrevious() {
    if (!m_destinations.empty()) {
        m_selectedIndex = (m_selectedIndex - 1 + m_destinations.size()) % m_destinations.size();
    }
}

void TravelSelectionScene::selectNext() {
    if (!m_destinations.empty()) {
        m_selectedIndex = (m_selectedIndex + 1) % m_destinations.size();
    }
}

void TravelSelectionScene::confirmSelection() {
    if (m_destinations.empty() || m_selectedIndex < 0 || m_selectedIndex >= m_destinations.size()) {
        return;
    }

    const CityConnection& selected = m_destinations[m_selectedIndex];

    // [MVP] Disabled - Fuel check (uncomment to enable)
    /*
    // Check if player has enough fuel
    PlayerState& playerState = GameStateManager::getInstance().getPlayerState();
    if (playerState.getFuel() < selected.fuelCost) {
        std::cout << "Not enough fuel! Need " << selected.fuelCost << "L, have " << playerState.getFuel() << "L" << std::endl;
        // Error message shown in console (UI popup not yet implemented)
        return;
    }
    */

    std::cout << "Traveling to: " << selected.cityName << std::endl;

    // Store selected destination
    m_selectedDestinationId = selected.cityId;

    // Transition to travel cinematic
    m_finished = true;
    m_nextScene = SceneType::TRAVEL_CINEMATIC;
}

SceneType TravelSelectionScene::getNextScene() const {
    return m_nextScene;
}

bool TravelSelectionScene::isFinished() const {
    return m_finished;
}
