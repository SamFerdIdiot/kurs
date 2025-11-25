#include "LocationScene.h"
#include <iostream>

LocationScene::LocationScene(LocationType locationType, const std::string& locationName)
    : m_locationType(locationType),
      m_locationName(locationName),
      m_isFinished(false),
      m_nextScene(SceneType::NODE),
      m_showInteractionMenu(false),
      m_fontLoaded(false),
      m_hudPanel(800.0f, 600.0f) {  // Initialize HUD
    
    setupLocationElements();
    setupInteractionOptions();
}

void LocationScene::setupLocationElements() {
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("LocationScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Load font
    m_fontLoaded = m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    
    if (!m_fontLoaded) {
        std::cerr << "Warning: Could not load font in LocationScene" << std::endl;
    }
    
    // Setup background with responsive sizing
    m_background.setSize(sf::Vector2f(layout.getTargetWidth(), layout.getTargetHeight()));
    m_background.setPosition(0, 0);
    m_background.setFillColor(sf::Color(60, 60, 70));
    
    // Setup counter/service area with responsive scaling
    m_counterBox.setSize(SCALE_SIZE(layout, 400, 200));
    m_counterBox.setPosition(SCALE_POS(layout, 483, 200));
    m_counterBox.setFillColor(sf::Color(80, 80, 90));
    m_counterBox.setOutlineColor(sf::Color(120, 120, 130));
    m_counterBox.setOutlineThickness(3);
    
    // Setup player character with responsive scaling
    m_playerBox.setSize(SCALE_SIZE(layout, 60, 100));
    m_playerBox.setPosition(SCALE_POS(layout, 300, 500));
    m_playerBox.setFillColor(sf::Color(100, 150, 200));
    
    // Setup exit door with responsive scaling
    m_exitDoorBox.setSize(SCALE_SIZE(layout, 100, 150));
    m_exitDoorBox.setPosition(SCALE_POS(layout, 50, 500));
    m_exitDoorBox.setFillColor(sf::Color(139, 69, 19)); // Brown door
    m_exitDoorBox.setOutlineColor(sf::Color(169, 99, 49));
    m_exitDoorBox.setOutlineThickness(2);
    
    // Setup location name text with responsive scaling
    if (m_fontLoaded) {
        m_locationNameText.setFont(m_font);
    }
    m_locationNameText.setCharacterSize(SCALE_FONT(layout, 28));
    m_locationNameText.setFillColor(sf::Color::White);
    m_locationNameText.setPosition(SCALE_POS(layout, 20, 20));
    
    std::string displayName = m_locationName.empty() ? 
        getLocationDescription() : m_locationName;
    m_locationNameText.setString(displayName);
    
    // Setup instructions text with responsive scaling
    if (m_fontLoaded) {
        m_instructionsText.setFont(m_font);
    }
    m_instructionsText.setCharacterSize(SCALE_FONT(layout, 18));
    m_instructionsText.setFillColor(sf::Color(200, 200, 200));
    m_instructionsText.setPosition(SCALE_POS(layout, 20, 700));
    m_instructionsText.setString("Press E to interact | ESC to leave");
    
    // Setup status text with responsive scaling
    if (m_fontLoaded) {
        m_statusText.setFont(m_font);
    }
    m_statusText.setCharacterSize(SCALE_FONT(layout, 20));
    m_statusText.setFillColor(sf::Color::White);
    m_statusText.setPosition(SCALE_POS(layout, 900, 20));
    
    // Add location-specific objects based on type
    switch (m_locationType) {
        case LocationType::GAS_STATION: {
            // Add gas pump
            sf::RectangleShape pump;
            pump.setSize(sf::Vector2f(80, 150));
            pump.setPosition(1100, 450);
            pump.setFillColor(sf::Color(200, 50, 50));
            m_objects.push_back(pump);
            break;
        }
        case LocationType::STORE: {
            // Add shelves
            for (int i = 0; i < 3; ++i) {
                sf::RectangleShape shelf;
                shelf.setSize(sf::Vector2f(150, 200));
                shelf.setPosition(800 + i * 180, 250);
                shelf.setFillColor(sf::Color(100, 70, 50));
                m_objects.push_back(shelf);
            }
            break;
        }
        case LocationType::MOTEL: {
            // Add bed
            sf::RectangleShape bed;
            bed.setSize(sf::Vector2f(200, 120));
            bed.setPosition(900, 300);
            bed.setFillColor(sf::Color(150, 100, 100));
            m_objects.push_back(bed);
            break;
        }
        default:
            break;
    }
}

void LocationScene::setupInteractionOptions() {
    // This would setup the interaction menu
    // For now, we'll handle interactions directly
}

void LocationScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            handleLeave();
        } else if (event.key.code == sf::Keyboard::E) {
            // Interact with location based on type
            switch (m_locationType) {
                case LocationType::GAS_STATION:
                    handleRefuel();
                    break;
                case LocationType::STORE:
                    handleShop();
                    break;
                case LocationType::GARAGE:
                case LocationType::MECHANIC:
                    handleRepair();
                    break;
                case LocationType::DINER:
                    handleEat();
                    break;
                case LocationType::MOTEL:
                    handleRest();
                    break;
                default:
                    break;
            }
        }
    }
}

void LocationScene::update(float deltaTime) {
    // Update status display
    std::string status = "Money: $" + std::to_string(static_cast<int>(m_playerState.getMoney())) +
                        " | Fuel: " + std::to_string(static_cast<int>(m_playerState.getFuel())) + "L" +
                        " | Energy: " + std::to_string(static_cast<int>(m_playerState.getEnergy())) + "%";
    m_statusText.setString(status);
    
    // Update HUD
    m_hudPanel.setMoney(m_playerState.getMoney());
    m_hudPanel.setFuel(m_playerState.getFuel(), 100.0f);
    m_hudPanel.setEnergy(m_playerState.getEnergy(), 100.0f);
    float cargoWeight = m_playerState.getInventory().getTotalWeight();
    m_hudPanel.setCargo(cargoWeight, 100.0f);
    m_hudPanel.updateTopRight(deltaTime);
}

void LocationScene::render(sf::RenderWindow& window) {
    // Draw background
    window.draw(m_background);
    
    // Draw counter
    window.draw(m_counterBox);
    
    // Draw objects
    for (const auto& obj : m_objects) {
        window.draw(obj);
    }
    
    // Draw exit door
    window.draw(m_exitDoorBox);
    
    // Draw player
    window.draw(m_playerBox);
    
    // Draw UI
    if (m_fontLoaded) {
        window.draw(m_locationNameText);
        window.draw(m_instructionsText);
        window.draw(m_statusText);
    }
    
    // Draw HUD
    m_hudPanel.render(window);
}

SceneType LocationScene::getNextScene() const {
    return m_nextScene;
}

bool LocationScene::isFinished() const {
    return m_isFinished;
}

// Interaction handlers
void LocationScene::handleRefuel() {
    float fuelCost = 50.0f; // Cost per liter
    float fuelAmount = 50.0f; // Amount to refuel
    float totalCost = fuelAmount * fuelCost;
    
    if (m_playerState.canAfford(totalCost)) {
        m_playerState.addMoney(-totalCost);
        m_playerState.addFuel(fuelAmount);
        std::cout << "Refueled " << fuelAmount << "L for $" << totalCost << std::endl;
    } else {
        std::cout << "Not enough money to refuel!" << std::endl;
    }
}

void LocationScene::handleShop() {
    // Simple shop interaction
    std::cout << "Shop interaction - buying supplies" << std::endl;
    float itemCost = 100.0f;
    
    if (m_playerState.canAfford(itemCost)) {
        m_playerState.addMoney(-itemCost);
        // TODO: Add items to inventory
        std::cout << "Bought supplies for $" << itemCost << std::endl;
    } else {
        std::cout << "Not enough money!" << std::endl;
    }
}

void LocationScene::handleRepair() {
    // Simple repair interaction
    std::cout << "Repair interaction" << std::endl;
    float repairCost = 200.0f;
    
    if (m_playerState.canAfford(repairCost)) {
        m_playerState.addMoney(-repairCost);
        // TODO: Repair car
        std::cout << "Repaired car for $" << repairCost << std::endl;
    } else {
        std::cout << "Not enough money for repairs!" << std::endl;
    }
}

void LocationScene::handleEat() {
    // Simple eat interaction
    std::cout << "Eating at diner" << std::endl;
    float mealCost = 50.0f;
    float energyGain = 20.0f;
    
    if (m_playerState.canAfford(mealCost)) {
        m_playerState.addMoney(-mealCost);
        m_playerState.addEnergy(energyGain);
        std::cout << "Ate meal for $" << mealCost << ", gained " << energyGain << " energy" << std::endl;
    } else {
        std::cout << "Not enough money!" << std::endl;
    }
}

void LocationScene::handleRest() {
    // Simple rest interaction
    std::cout << "Resting at motel" << std::endl;
    float restCost = 150.0f;
    float energyGain = 50.0f;
    
    if (m_playerState.canAfford(restCost)) {
        m_playerState.addMoney(-restCost);
        m_playerState.addEnergy(energyGain);
        std::cout << "Rested for $" << restCost << ", gained " << energyGain << " energy" << std::endl;
    } else {
        std::cout << "Not enough money!" << std::endl;
    }
}

void LocationScene::handleTalk() {
    // Simple talk interaction
    std::cout << "Talking to NPC" << std::endl;
    // TODO: Open dialogue system
}

void LocationScene::handleLeave() {
    std::cout << "Leaving location" << std::endl;
    m_isFinished = true;
}

std::string LocationScene::getLocationDescription() const {
    switch (m_locationType) {
        case LocationType::HOME:
            return "Home";
        case LocationType::GAS_STATION:
            return "Gas Station";
        case LocationType::STORE:
            return "Store";
        case LocationType::GARAGE:
            return "Garage";
        case LocationType::MOTEL:
            return "Motel";
        case LocationType::DINER:
            return "Diner";
        case LocationType::MECHANIC:
            return "Mechanic Shop";
        default:
            return "Unknown Location";
    }
}
