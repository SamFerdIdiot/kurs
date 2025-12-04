#ifndef TRAVEL_SELECTION_SCENE_H
#define TRAVEL_SELECTION_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>

// City/Node connection structure
struct CityConnection {
    std::string cityId;           // Unique city ID
    std::string cityName;         // Display name (e.g., "Moscow")
    std::string cityNumber;       // Number for reference (e.g., "01")
    float distance;               // Distance in km
    float fuelCost;               // Estimated fuel consumption
    bool isAccessible;            // Can travel to this city from current location
    bool isBidirectional;         // Can travel back (true by default)

    CityConnection()
        : distance(0.0f), fuelCost(0.0f), isAccessible(false), isBidirectional(true) {}
};

// TravelSelectionScene - Menu for selecting travel destination
// Features:
// - List of connected cities from current location
// - Shows distance and fuel cost
// - W/S navigation, Enter to confirm
// - Only shows accessible cities based on connections
//
// This replaces the visual map interaction - player must use
// the static world map (MAP tab) and notes (NOT tab) to navigate
class TravelSelectionScene : public Scene {
public:
    TravelSelectionScene(const std::string& currentCityId);
    ~TravelSelectionScene() override = default;

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    SceneType getNextScene() const override;
    bool isFinished() const override;

private:
    // Load available destinations from current city
    void loadDestinations();

    // Navigation
    void selectPrevious();
    void selectNext();
    void confirmSelection();

    // Rendering
    void renderHeader(sf::RenderWindow& window);
    void renderDestinations(sf::RenderWindow& window);
    void renderFooter(sf::RenderWindow& window);

    // Current location
    std::string m_currentCityId;
    std::string m_currentCityName;

    // Available destinations
    std::vector<CityConnection> m_destinations;
    int m_selectedIndex;

    // Font
    sf::Font m_font;
    bool m_fontLoaded;

    // Scene state
    bool m_finished;
    SceneType m_nextScene;
    std::string m_selectedDestinationId;

    // Visual elements
    sf::RectangleShape m_background;
};

#endif // TRAVEL_SELECTION_SCENE_H
