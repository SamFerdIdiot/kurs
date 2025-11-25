#include "TravelCinematicScene.h"
#include "Car.h"
#include "AbilityTreeSystem.h"
#include <iostream>
#include <cmath>

TravelCinematicScene::TravelCinematicScene(const RoadData& roadData, PlayerState* playerState)
    : m_roadData(roadData),
      m_playerState(playerState),
      m_progress(0.0f),
      m_travelSpeed(50.0f),  // 50 km per in-game second (accelerated for gameplay)
      m_distanceTraveled(0.0f),
      m_isFinished(false),
      m_arrived(false),
      m_activeEvent(nullptr),
      m_timeSinceLastEventCheck(0.0f),
      m_eventCheckInterval(5.0f),  // Check for events every 5 seconds
      m_fontLoaded(false),
      m_nextScene(SceneType::NODE),
      m_farOffset(0.0f),
      m_midOffset(0.0f),
      m_roadOffset(0.0f),
      m_hudPanel(800.0f, 600.0f) {  // Initialize HUD
    
    // Initialize default events
    m_eventManager.initializeDefaultEvents();
    
    setupUI();
}

void TravelCinematicScene::setupUI() {
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("TravelCinematicScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Try to load font
    m_fontLoaded = m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    
    if (!m_fontLoaded) {
        std::cerr << "Warning: Could not load font in TravelCinematicScene" << std::endl;
    }
    
    // Setup parallax layers with responsive sizing
    // Sky
    m_sky.setSize(sf::Vector2f(layout.getTargetWidth(), 300));
    m_sky.setPosition(0, 0);
    m_sky.setFillColor(sf::Color(135, 206, 235)); // Sky blue
    
    // Far background (mountains/horizon)
    m_farBackground.setSize(sf::Vector2f(layout.getTargetWidth(), 200));
    m_farBackground.setPosition(0, 100);
    m_farBackground.setFillColor(sf::Color(100, 120, 140));
    
    // Mid background (trees/buildings)
    m_midBackground.setSize(sf::Vector2f(layout.getTargetWidth(), 250));
    m_midBackground.setPosition(0, 250);
    m_midBackground.setFillColor(sf::Color(80, 100, 80));
    
    // Road
    m_road.setSize(sf::Vector2f(layout.getTargetWidth(), 268));
    m_road.setPosition(0, 500);
    m_road.setFillColor(sf::Color(60, 60, 70));
    
    // Car (side view, positioned on the road)
    m_car.setSize(sf::Vector2f(150, 80));
    m_car.setPosition(300, 450);
    m_car.setFillColor(sf::Color(200, 50, 50)); // Red car
    
    // Progress bar
    m_progressBarBg.setSize(sf::Vector2f(400, 30));
    m_progressBarBg.setPosition(483, 20);
    m_progressBarBg.setFillColor(sf::Color(50, 50, 50));
    m_progressBarBg.setOutlineColor(sf::Color(150, 150, 150));
    m_progressBarBg.setOutlineThickness(2);
    
    m_progressBar.setSize(sf::Vector2f(0, 26));
    m_progressBar.setPosition(485, 22);
    m_progressBar.setFillColor(sf::Color(100, 200, 100));
    
    // Progress text
    if (m_fontLoaded) {
        m_progressText.setFont(m_font);
    }
    m_progressText.setCharacterSize(18);
    m_progressText.setFillColor(sf::Color::White);
    m_progressText.setPosition(620, 60);
    
    // Resource display
    if (m_fontLoaded) {
        m_fuelText.setFont(m_font);
        m_distanceText.setFont(m_font);
    }
    m_fuelText.setCharacterSize(20);
    m_fuelText.setFillColor(sf::Color::White);
    m_fuelText.setPosition(20, 20);
    
    m_distanceText.setCharacterSize(20);
    m_distanceText.setFillColor(sf::Color::White);
    m_distanceText.setPosition(20, 50);
}

void TravelCinematicScene::handleInput(const sf::Event& event) {
    // If there's an active event, delegate input to it
    if (m_activeEvent) {
        m_activeEvent->handleInput(event);
        return;
    }
    
    // Allow skipping with ESC (only if no active event)
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            // Fast travel to destination
            m_progress = 1.0f;
            m_arrived = true;
            m_isFinished = true;
        }
    }
}

void TravelCinematicScene::update(float deltaTime) {
    // If there's an active event, update it instead of travel
    if (m_activeEvent) {
        m_activeEvent->update(deltaTime);
        
        // Check if event is finished
        if (m_activeEvent->isFinished()) {
            m_activeEvent.reset();
        }
        return;
    }
    
    // Update ability cooldowns (convert seconds to minutes)
    AbilityTreeSystem::getInstance().updateCooldowns(deltaTime / 60.0f);
    
    // Update travel progress
    updateTravel(deltaTime);
    
    // Consume resources
    consumeResources(deltaTime);
    
    // Check for random events
    m_timeSinceLastEventCheck += deltaTime;
    if (m_timeSinceLastEventCheck >= m_eventCheckInterval) {
        tryTriggerEvent();
        m_timeSinceLastEventCheck = 0.0f;
    }
    
    // Update parallax offsets
    float parallaxSpeed = 100.0f * deltaTime;
    m_farOffset -= parallaxSpeed * 0.2f;
    m_midOffset -= parallaxSpeed * 0.5f;
    m_roadOffset -= parallaxSpeed * 1.0f;
    
    // Wrap offsets
    if (m_farOffset < -1366) m_farOffset += 1366;
    if (m_midOffset < -1366) m_midOffset += 1366;
    if (m_roadOffset < -1366) m_roadOffset += 1366;
    
    // Update HUD with real-time fuel consumption
    if (m_playerState) {
        m_hudPanel.setMoney(m_playerState->getMoney());
        m_hudPanel.setFuel(m_playerState->getFuel(), 100.0f);
        m_hudPanel.setEnergy(m_playerState->getEnergy(), 100.0f);
        // Get cargo from inventory if available
        float cargoWeight = m_playerState->getInventory().getTotalWeight();
        float cargoCapacity = 100.0f; // Default, should come from Car
        m_hudPanel.setCargo(cargoWeight, cargoCapacity);
    }
    m_hudPanel.updateTopRight(deltaTime);
}

void TravelCinematicScene::updateTravel(float deltaTime) {
    if (m_arrived) return;
    
    // Update distance traveled
    float distanceThisFrame = m_travelSpeed * deltaTime;
    m_distanceTraveled += distanceThisFrame;
    
    // Update progress (0.0 to 1.0)
    m_progress = m_distanceTraveled / m_roadData.distance;
    
    if (m_progress >= 1.0f) {
        m_progress = 1.0f;
    }
}

void TravelCinematicScene::consumeResources(float deltaTime) {
    if (!m_playerState || m_arrived) return;
    
    // Calculate fuel consumption rate based on car and road type
    float fuelPerKm = calculateFuelConsumption();
    float distanceThisFrame = m_travelSpeed * deltaTime;
    float fuelConsumed = distanceThisFrame * fuelPerKm;
    
    m_playerState->addFuel(-fuelConsumed);
    
    // Check if out of fuel
    if (m_playerState->getFuel() <= 0.0f) {
        m_playerState->setFuel(0.0f);
        // TODO: Trigger "out of fuel" event
        std::cerr << "Out of fuel!" << std::endl;
    }
}

float TravelCinematicScene::calculateFuelConsumption() const {
    if (!m_playerState) return 2.0f;
    
    // Get base fuel consumption based on car type and road type
    float baseFuel = 2.0f; // Default for "road" type
    
    // Adjust based on road type
    if (m_roadData.roadType == "highway") {
        baseFuel = 1.5f;
    } else if (m_roadData.roadType == "road") {
        baseFuel = 2.0f;
    } else if (m_roadData.roadType == "path") {
        baseFuel = 2.8f;
    }
    
    // TODO: Add car-specific fuel consumption based on m_playerState->getCarType()
    // For now, use the base values
    
    return baseFuel;
}

void TravelCinematicScene::tryTriggerEvent() {
    if (!m_playerState) return;
    
    // Don't trigger events if almost arrived
    if (m_progress > 0.9f) return;
    
    // Get a random event from the event manager
    GameEvent* event = m_eventManager.getRandomEvent(
        m_playerState->getFuel(),
        m_playerState->getEnergy(),
        static_cast<int>(m_playerState->getMoney()),
        m_roadData.toNodeId
    );
    
    if (event) {
        // Add ability perk choices to the event
        m_eventManager.addPerkChoicesToEvent(
            event,
            m_playerState->getFuel(),
            m_playerState->getEnergy(),
            static_cast<int>(m_playerState->getMoney())
        );
        
        std::cout << "Event triggered: " << event->title << std::endl;
        m_activeEvent = std::make_unique<CinematicEventScene>(event, m_playerState);
    }
}

void TravelCinematicScene::checkArrival() {
    if (m_progress >= 1.0f && !m_arrived) {
        m_arrived = true;
        m_isFinished = true;
        std::cout << "Arrived at destination: " << m_roadData.toNodeId << std::endl;
    }
}

void TravelCinematicScene::render(sf::RenderWindow& window) {
    // Render parallax layers
    window.draw(m_sky);
    window.draw(m_farBackground);
    window.draw(m_midBackground);
    window.draw(m_road);
    
    // Render car
    window.draw(m_car);
    
    // Render UI
    window.draw(m_progressBarBg);
    window.draw(m_progressBar);
    
    if (m_fontLoaded) {
        window.draw(m_progressText);
        window.draw(m_fuelText);
        window.draw(m_distanceText);
    }
    
    // Render HUD
    m_hudPanel.render(window);
    
    // Render active event on top if present
    if (m_activeEvent) {
        m_activeEvent->render(window);
    }
}

SceneType TravelCinematicScene::getNextScene() const {
    return m_nextScene;
}

bool TravelCinematicScene::isFinished() const {
    return m_isFinished;
}
