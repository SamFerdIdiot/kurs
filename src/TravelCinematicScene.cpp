#include "TravelCinematicScene.h"
#include "Car.h"
#include "AbilityTreeSystem.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include <iostream>
#include <cmath>

TravelCinematicScene::TravelCinematicScene(const RoadData& roadData, PlayerState* playerState)
    : m_roadData(roadData),
      m_playerState(playerState),
      m_progress(0.0f),
      m_travelSpeed(1.67f),  // [MVP] 100km / 60sec = 1.67 km/sec (1 minute travel time for testing)
      m_distanceTraveled(0.0f),
      m_isFinished(false),
      m_arrived(false),
      m_activeEvent(nullptr),
      m_timeSinceLastEventCheck(0.0f),
      m_eventCheckInterval(5.0f),  // Check for events every 5 seconds
      m_fontLoaded(false),
      m_nextScene(SceneType::LOCATION),  // [MVP] Go to city after travel
      m_layer1Offset(0.0f),  // Clouds offset
      m_layer2Offset(0.0f),  // Mountains offset
      m_layer3Offset(0.0f),  // Hills offset
      m_layer4Offset(0.0f),  // Trees offset
      m_layer5Offset(0.0f),  // Fence offset
      m_roadOffset(0.0f),    // Road offset
      m_carBobTime(0.0f),
      m_carBobAmount(0.0f),
      m_dialogueTimer(0.0f),
      m_timeSinceLastDialogue(0.0f),
      m_dialogueCheckInterval(15.0f),  // Check for dialogues every 15 seconds
      m_hudPanel(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT),  // Fixed dimensions
      m_warningFlashTimer(0.0f),
      m_warningFlashVisible(false),
      m_arrivalNotificationTimer(0.0f),
      m_showArrivalNotification(false) {

    // [MVP] Disabled - Event system initialization (uncomment to enable)
    /*
    // Initialize default events
    m_eventManager.initializeDefaultEvents();
    */

    // [MVP] Disabled - Dialogue system initialization (uncomment to enable)
    /*
    // Initialize travel dialogues
    initializeTravelDialogues();
    */

    setupUI();
}

void TravelCinematicScene::setupUI() {
    // SFML 3.x: Font loading with FontLoader helper
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "Warning: Could not load font in TravelCinematicScene" << std::endl;
    }

    // [MVP] 7-layer parallax system (Keep Driving style) - WIDE layers for seamless scrolling
    // Use 5x screen width to avoid visible seams
    float layerWidth = UI::SCREEN_WIDTH * 5.0f;

    // Layer 0: Sky (static)
    m_sky.setSize(sf::Vector2f(UI::SCREEN_WIDTH, 250.0f));
    m_sky.setPosition(sf::Vector2f(0.0f, 0.0f));
    m_sky.setFillColor(sf::Color(197, 231, 219)); // Mint/pale green sky (Keep Driving style)

    // Layer 1: Clouds (very slow - speed x0.05)
    m_layer1_clouds.setSize(sf::Vector2f(layerWidth, 80.0f));
    m_layer1_clouds.setPosition(sf::Vector2f(0.0f, 50.0f));
    m_layer1_clouds.setFillColor(sf::Color(245, 245, 250, 120)); // Very light clouds

    // Layer 2: Mountains (slow - speed x0.1)
    m_layer2_mountains.setSize(sf::Vector2f(layerWidth, 120.0f));
    m_layer2_mountains.setPosition(sf::Vector2f(0.0f, 150.0f));
    m_layer2_mountains.setFillColor(sf::Color(150, 170, 180)); // Distant blue-gray mountains

    // Layer 3: Sunflower field (medium - speed x0.3) - KEY ELEMENT from reference
    m_layer3_hills.setSize(sf::Vector2f(layerWidth, 100.0f));
    m_layer3_hills.setPosition(sf::Vector2f(0.0f, 250.0f));
    m_layer3_hills.setFillColor(sf::Color(255, 215, 0)); // BRIGHT YELLOW sunflowers!

    // Layer 4: Trees (medium-fast - speed x0.6) - Dark green trees
    m_layer4_trees.setSize(sf::Vector2f(layerWidth, 150.0f));
    m_layer4_trees.setPosition(sf::Vector2f(0.0f, 330.0f));
    m_layer4_trees.setFillColor(sf::Color(80, 120, 70)); // Dark green trees

    // Layer 5: Grass/foreground (fast - speed x0.9)
    m_layer5_fence.setSize(sf::Vector2f(layerWidth, 100.0f));
    m_layer5_fence.setPosition(sf::Vector2f(0.0f, 450.0f));
    m_layer5_fence.setFillColor(sf::Color(120, 150, 80)); // Green grass

    // Layer 6: Road (fastest - speed x1.0) - VERY NARROW like Keep Driving (~11% of screen)
    m_road.setSize(sf::Vector2f(layerWidth, 100.0f));  // Very narrow: 100px (~11% of 900px)
    m_road.setPosition(sf::Vector2f(0.0f, 650.0f));    // Moved down to 650 (more space for background)
    m_road.setFillColor(sf::Color(60, 60, 70)); // Dark gray road

    // Road lane markings (dashed white lines - vertical rectangles)
    for (int i = 0; i < 15; ++i) {
        sf::RectangleShape line;
        line.setSize(sf::Vector2f(12.0f, 40.0f));  // Vertical dash (wider and taller)
        line.setFillColor(sf::Color(255, 255, 255));  // Pure white
        m_roadLines.push_back(line);
    }

    // White posts along road edges (Keep Driving style)
    for (int i = 0; i < 20; ++i) {
        sf::RectangleShape post;
        post.setSize(sf::Vector2f(8.0f, 40.0f));  // Thin vertical posts
        post.setFillColor(sf::Color(240, 240, 240));  // White
        m_roadPosts.push_back(post);
    }

    // [MVP] Speed visualization squares (moving fast to show speed)
    for (int i = 0; i < 15; ++i) {
        sf::RectangleShape square;
        square.setSize(sf::Vector2f(30.0f, 30.0f));
        square.setFillColor(sf::Color(255, 200, 0, 200)); // Yellow-orange
        square.setOutlineColor(sf::Color(255, 255, 255));
        square.setOutlineThickness(2.0f);
        m_speedSquares.push_back(square);
    }

    // [MVP] Layer objects for parallax depth - squares on each layer
    // Layer 1 - Clouds objects (sparse, large, very slow)
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape obj;
        obj.setSize(sf::Vector2f(80.0f, 50.0f));
        obj.setFillColor(sf::Color(240, 240, 250, 120));
        obj.setOutlineColor(sf::Color(255, 255, 255, 80));
        obj.setOutlineThickness(1.0f);
        m_layer1Objects.push_back(obj);
    }

    // Layer 2 - Mountains objects (medium size, slow)
    for (int i = 0; i < 12; ++i) {
        sf::RectangleShape obj;
        obj.setSize(sf::Vector2f(60.0f, 90.0f));
        obj.setFillColor(sf::Color(80, 100, 120, 180));
        obj.setOutlineColor(sf::Color(60, 80, 100));
        obj.setOutlineThickness(2.0f);
        m_layer2Objects.push_back(obj);
    }

    // Layer 3 - Hills objects (medium)
    for (int i = 0; i < 15; ++i) {
        sf::RectangleShape obj;
        obj.setSize(sf::Vector2f(50.0f, 70.0f));
        obj.setFillColor(sf::Color(100, 140, 100, 200));
        obj.setOutlineColor(sf::Color(80, 120, 80));
        obj.setOutlineThickness(2.0f);
        m_layer3Objects.push_back(obj);
    }

    // Layer 4 - Trees objects (smaller, medium-fast)
    for (int i = 0; i < 20; ++i) {
        sf::RectangleShape obj;
        obj.setSize(sf::Vector2f(40.0f, 60.0f));
        obj.setFillColor(sf::Color(60, 120, 60, 220));
        obj.setOutlineColor(sf::Color(40, 80, 40));
        obj.setOutlineThickness(2.0f);
        m_layer4Objects.push_back(obj);
    }

    // Layer 5 - Fence objects (small, fast)
    for (int i = 0; i < 25; ++i) {
        sf::RectangleShape obj;
        obj.setSize(sf::Vector2f(20.0f, 50.0f));
        obj.setFillColor(sf::Color(160, 120, 80, 240));
        obj.setOutlineColor(sf::Color(120, 80, 40));
        obj.setOutlineThickness(2.0f);
        m_layer5Objects.push_back(obj);
    }

    // Car (side view, positioned on the road - BMW E30 touring style)
    m_car.setSize(sf::Vector2f(150.0f, 60.0f));       // Smaller height for very narrow road
    m_car.setPosition(sf::Vector2f(300.0f, 670.0f));  // On narrow road (650 + 20)
    m_car.setFillColor(sf::Color(200, 50, 50));       // Red BMW E30
    m_car.setOutlineColor(sf::Color(255, 255, 255));  // White outline for visibility
    m_car.setOutlineThickness(2.0f);

    // [MVP] Oncoming traffic - cars moving in opposite direction (right to left)
    for (int i = 0; i < 5; ++i) {
        sf::RectangleShape oncomingCar;
        oncomingCar.setSize(sf::Vector2f(120.0f, 60.0f));  // Slightly smaller than player car
        oncomingCar.setFillColor(sf::Color(50, 100, 150, 200)); // Blue-ish color
        oncomingCar.setOutlineColor(sf::Color(255, 255, 255, 100));
        oncomingCar.setOutlineThickness(1.0f);
        m_oncomingCars.push_back(oncomingCar);

        // Initial offsets spread across screen
        m_oncomingCarOffsets.push_back(UI::SCREEN_WIDTH + (i * 400.0f));
    }

    // Progress bar
    m_progressBarBg.setSize(sf::Vector2f(400.0f, 30.0f));
    m_progressBarBg.setPosition(sf::Vector2f(483.0f, 20.0f));  // Fixed: Vector2f wrapper
    m_progressBarBg.setFillColor(sf::Color(50, 50, 50));
    m_progressBarBg.setOutlineColor(sf::Color(150, 150, 150));
    m_progressBarBg.setOutlineThickness(2);

    m_progressBar.setSize(sf::Vector2f(0.0f, 26.0f));
    m_progressBar.setPosition(sf::Vector2f(485.0f, 22.0f));  // Fixed: Vector2f wrapper
    m_progressBar.setFillColor(sf::Color(100, 200, 100));

    // SFML 3.x: Text requires font in constructor
    if (m_fontLoaded) {
        m_progressText.emplace(m_font, "", 18);
        m_progressText->setFillColor(sf::Color::White);
        m_progressText->setPosition(sf::Vector2f(620.0f, 60.0f));  // Fixed: Vector2f wrapper

        m_fuelText.emplace(m_font, "", 20);
        m_fuelText->setFillColor(sf::Color::White);
        m_fuelText->setPosition(sf::Vector2f(20.0f, 20.0f));  // Fixed: Vector2f wrapper

        m_distanceText.emplace(m_font, "", 20);
        m_distanceText->setFillColor(sf::Color::White);
        m_distanceText->setPosition(sf::Vector2f(20.0f, 50.0f));  // Fixed: Vector2f wrapper

        // Dialogue UI (compact box at bottom)
        m_dialogueNameText.emplace(m_font, "", 18);
        m_dialogueNameText->setFillColor(sf::Color::Yellow);
        m_dialogueNameText->setPosition(sf::Vector2f(130.0f, 695.0f));

        m_dialogueText.emplace(m_font, "", 16);
        m_dialogueText->setFillColor(sf::Color::White);
        m_dialogueText->setPosition(sf::Vector2f(130.0f, 720.0f));
    }

    // Dialogue box (compact, bottom of screen)
    m_dialogueBox.setSize(sf::Vector2f(1150.0f, 80.0f));
    m_dialogueBox.setPosition(sf::Vector2f(83.0f, 685.0f));
    m_dialogueBox.setFillColor(sf::Color(30, 30, 40, 220));  // Semi-transparent
    m_dialogueBox.setOutlineColor(sf::Color(100, 100, 120));
    m_dialogueBox.setOutlineThickness(2);

    // NPC portrait placeholder
    m_npcPortrait.setSize(sf::Vector2f(60.0f, 60.0f));
    m_npcPortrait.setPosition(sf::Vector2f(95.0f, 695.0f));
    m_npcPortrait.setFillColor(sf::Color(80, 120, 160));  // Blue placeholder
    m_npcPortrait.setOutlineColor(sf::Color::White);
    m_npcPortrait.setOutlineThickness(2);

    // UI Improvements: Warning indicators (top-right corner)
    m_fuelWarningIcon.setRadius(15.0f);
    m_fuelWarningIcon.setPosition(sf::Vector2f(1350.0f, 100.0f));
    m_fuelWarningIcon.setFillColor(sf::Color(200, 0, 0, 180));  // Red, semi-transparent
    m_fuelWarningIcon.setOutlineColor(sf::Color::White);
    m_fuelWarningIcon.setOutlineThickness(2);

    m_energyWarningIcon.setRadius(15.0f);
    m_energyWarningIcon.setPosition(sf::Vector2f(1350.0f, 140.0f));
    m_energyWarningIcon.setFillColor(sf::Color(200, 150, 0, 180));  // Orange, semi-transparent
    m_energyWarningIcon.setOutlineColor(sf::Color::White);
    m_energyWarningIcon.setOutlineThickness(2);

    if (m_fontLoaded) {
        m_fuelWarningText.emplace(m_font, "LOW FUEL!", 16);
        m_fuelWarningText->setFillColor(sf::Color::Red);
        m_fuelWarningText->setPosition(sf::Vector2f(1240.0f, 105.0f));

        m_energyWarningText.emplace(m_font, "TIRED!", 16);
        m_energyWarningText->setFillColor(sf::Color(255, 200, 0));  // Orange
        m_energyWarningText->setPosition(sf::Vector2f(1260.0f, 145.0f));
    }

    // UI Improvements: Speedometer (bottom-right)
    m_speedometerBg.setSize(sf::Vector2f(120.0f, 80.0f));
    m_speedometerBg.setPosition(sf::Vector2f(1330.0f, 650.0f));
    m_speedometerBg.setFillColor(sf::Color(30, 30, 40, 200));  // Dark, semi-transparent
    m_speedometerBg.setOutlineColor(sf::Color(100, 100, 120));
    m_speedometerBg.setOutlineThickness(2);

    if (m_fontLoaded) {
        m_speedText.emplace(m_font, "0 km/h", 20);
        m_speedText->setFillColor(sf::Color::White);
        m_speedText->setPosition(sf::Vector2f(1345.0f, 680.0f));
    }

    // UI Improvements: Arrival notification (center screen)
    m_arrivalNotificationBg.setSize(sf::Vector2f(600.0f, 120.0f));
    m_arrivalNotificationBg.setPosition(sf::Vector2f(413.0f, 324.0f));  // Center screen
    m_arrivalNotificationBg.setFillColor(sf::Color(20, 20, 30, 230));
    m_arrivalNotificationBg.setOutlineColor(sf::Color(100, 200, 100));
    m_arrivalNotificationBg.setOutlineThickness(3);

    if (m_fontLoaded) {
        m_arrivalNotificationText.emplace(m_font, "Arriving at destination", 24);
        m_arrivalNotificationText->setFillColor(sf::Color(100, 255, 100));
        m_arrivalNotificationText->setPosition(sf::Vector2f(480.0f, 370.0f));
    }

    // [Decorative] Initialize Keep Driving style HUD panel
    const float HUD_HEIGHT = 150.0f;
    const float HUD_Y = UI::SCREEN_HEIGHT - HUD_HEIGHT;  // Bottom of screen

    // Main HUD background
    m_hudBackground.setSize(sf::Vector2f(UI::SCREEN_WIDTH, HUD_HEIGHT));
    m_hudBackground.setPosition(sf::Vector2f(0.0f, HUD_Y));
    m_hudBackground.setFillColor(sf::Color(30, 35, 30));  // Dark green-gray (Keep Driving style)

    // Left panel (ENERGY/STATUS)
    m_hudLeftPanel.setSize(sf::Vector2f(350.0f, HUD_HEIGHT));
    m_hudLeftPanel.setPosition(sf::Vector2f(0.0f, HUD_Y));
    m_hudLeftPanel.setFillColor(sf::Color(35, 40, 35, 200));
    m_hudLeftPanel.setOutlineColor(sf::Color(70, 80, 60));
    m_hudLeftPanel.setOutlineThickness(2.0f);

    // Center panel (date/time/weather)
    m_hudCenterPanel.setSize(sf::Vector2f(740.0f, HUD_HEIGHT));
    m_hudCenterPanel.setPosition(sf::Vector2f(350.0f, HUD_Y));
    m_hudCenterPanel.setFillColor(sf::Color(40, 45, 40, 200));
    m_hudCenterPanel.setOutlineColor(sf::Color(80, 90, 70));
    m_hudCenterPanel.setOutlineThickness(2.0f);

    // Right panel (CAR/GAS)
    m_hudRightPanel.setSize(sf::Vector2f(350.0f, HUD_HEIGHT));
    m_hudRightPanel.setPosition(sf::Vector2f(1090.0f, HUD_Y));
    m_hudRightPanel.setFillColor(sf::Color(35, 40, 35, 200));
    m_hudRightPanel.setOutlineColor(sf::Color(70, 80, 60));
    m_hudRightPanel.setOutlineThickness(2.0f);

    if (m_fontLoaded) {
        // Labels
        m_hudEnergyLabel.emplace(m_font, "ENERGY", 18);
        m_hudEnergyLabel->setFillColor(sf::Color(180, 220, 180));
        m_hudEnergyLabel->setPosition(sf::Vector2f(20.0f, HUD_Y + 10.0f));

        m_hudStatusLabel.emplace(m_font, "STATUS", 18);
        m_hudStatusLabel->setFillColor(sf::Color(180, 220, 180));
        m_hudStatusLabel->setPosition(sf::Vector2f(20.0f, HUD_Y + 80.0f));

        m_hudCarLabel.emplace(m_font, "CAR", 18);
        m_hudCarLabel->setFillColor(sf::Color(180, 220, 180));
        m_hudCarLabel->setPosition(sf::Vector2f(1260.0f, HUD_Y + 10.0f));

        m_hudGasLabel.emplace(m_font, "GAS", 18);
        m_hudGasLabel->setFillColor(sf::Color(180, 220, 180));
        m_hudGasLabel->setPosition(sf::Vector2f(1260.0f, HUD_Y + 80.0f));

        // Center date/time display (decorative)
        m_hudDateTimeText.emplace(m_font, "SAT, JUN 2ND, 10:45 AM", 16);
        m_hudDateTimeText->setFillColor(sf::Color(160, 200, 160));
        m_hudDateTimeText->setPosition(sf::Vector2f(565.0f, HUD_Y + 15.0f));
    }

    // Energy bars (left panel)
    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(50.0f, 12.0f));
        bar.setPosition(sf::Vector2f(20.0f + (i * 60.0f), HUD_Y + 35.0f));
        bar.setFillColor(sf::Color(100, 200, 100));  // Green energy
        bar.setOutlineColor(sf::Color(150, 255, 150));
        bar.setOutlineThickness(1.0f);
        m_hudEnergyBars.push_back(bar);
    }

    // Car fuel bars (right panel)
    for (int i = 0; i < 5; ++i) {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(40.0f, 10.0f));
        bar.setPosition(sf::Vector2f(1260.0f + (i * 15.0f), HUD_Y + 35.0f - (i * 3.0f)));  // Stagger
        bar.setFillColor(sf::Color(100, 180, 255));  // Blue fuel
        bar.setOutlineColor(sf::Color(150, 220, 255));
        bar.setOutlineThickness(1.0f);
        m_hudCarBars.push_back(bar);
    }

    // Character sprite placeholder (left panel)
    m_hudCharacterSprite.setSize(sf::Vector2f(50.0f, 60.0f));
    m_hudCharacterSprite.setPosition(sf::Vector2f(150.0f, HUD_Y + 80.0f));
    m_hudCharacterSprite.setFillColor(sf::Color(180, 140, 100));  // Brown character
    m_hudCharacterSprite.setOutlineColor(sf::Color(200, 160, 120));
    m_hudCharacterSprite.setOutlineThickness(2.0f);

    // Car top-view placeholder (center panel)
    m_hudCarTopView.setSize(sf::Vector2f(120.0f, 60.0f));
    m_hudCarTopView.setPosition(sf::Vector2f(660.0f, HUD_Y + 60.0f));
    m_hudCarTopView.setFillColor(sf::Color(200, 50, 50));  // Red BMW
    m_hudCarTopView.setOutlineColor(sf::Color(255, 100, 100));
    m_hudCarTopView.setOutlineThickness(2.0f);
}

void TravelCinematicScene::handleInput(const sf::Event& event) {
    // [MVP] Disabled - Event delegation (uncomment to enable)
    /*
    // If there's an active event, delegate input to it
    if (m_activeEvent) {
        m_activeEvent->handleInput(event);
        return;
    }
    */

    // [MVP] ACTIVE - Allow skipping with ESC
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            // Fast travel to destination
            m_progress = 1.0f;
            m_arrived = true;
            m_isFinished = true;
        }
    }
}

void TravelCinematicScene::update(float deltaTime) {
    // [MVP] Disabled - Event handling (uncomment to enable)
    /*
    // If there's an active event, update it instead of travel
    if (m_activeEvent) {
        m_activeEvent->update(deltaTime);

        // Check if event is finished
        if (m_activeEvent->isFinished()) {
            m_activeEvent.reset();
        }
        return;
    }
    */

    // [MVP] Disabled - Ability cooldowns (uncomment to enable)
    /*
    // Update ability cooldowns (convert seconds to minutes)
    AbilityTreeSystem::getInstance().updateCooldowns(deltaTime / 60.0f);
    */

    // Update travel progress
    updateTravel(deltaTime);

    // [MVP] Disabled - Resource consumption (uncomment to enable)
    /*
    // Consume resources
    consumeResources(deltaTime);
    */

    // [MVP] Disabled - Random events (uncomment to enable)
    /*
    // Check for random events
    m_timeSinceLastEventCheck += deltaTime;
    if (m_timeSinceLastEventCheck >= m_eventCheckInterval) {
        tryTriggerEvent();
        m_timeSinceLastEventCheck = 0.0f;
    }
    */

    // [MVP] Disabled - Travel dialogues (uncomment to enable)
    /*
    // Check for travel dialogues (if no active event)
    if (!m_activeEvent) {
        m_timeSinceLastDialogue += deltaTime;
        if (m_timeSinceLastDialogue >= m_dialogueCheckInterval) {
            tryTriggerDialogue();
            m_timeSinceLastDialogue = 0.0f;
        }

        // Update dialogue timer
        if (m_currentDialogue) {
            m_dialogueTimer += deltaTime;
            if (m_dialogueTimer >= m_currentDialogue->displayDuration) {
                m_currentDialogue.reset();  // Hide dialogue
                m_dialogueTimer = 0.0f;
            }
        }
    }
    */
    
    // [MVP] ACTIVE - 7-layer parallax animation (faster movement)
    // Base speed increased for more dynamic feel (simulating ~90 km/h visual speed)
    float baseSpeed = 400.0f * deltaTime;  // Increased from 150 to 400 for faster visuals

    // Update each layer with its parallax multiplier
    m_layer1Offset -= baseSpeed * 0.05f;   // Clouds - very slow
    m_layer2Offset -= baseSpeed * 0.1f;    // Mountains - slow
    m_layer3Offset -= baseSpeed * 0.3f;    // Hills - medium
    m_layer4Offset -= baseSpeed * 0.6f;    // Trees - medium-fast
    m_layer5Offset -= baseSpeed * 0.9f;    // Fence - fast
    m_roadOffset -= baseSpeed * 1.0f;      // Road - fastest

    // Wrap offsets for infinite scrolling
    float wrapWidth = UI::SCREEN_WIDTH;
    if (m_layer1Offset < -wrapWidth) m_layer1Offset += wrapWidth;
    if (m_layer2Offset < -wrapWidth) m_layer2Offset += wrapWidth;
    if (m_layer3Offset < -wrapWidth) m_layer3Offset += wrapWidth;
    if (m_layer4Offset < -wrapWidth) m_layer4Offset += wrapWidth;
    if (m_layer5Offset < -wrapWidth) m_layer5Offset += wrapWidth;
    if (m_roadOffset < -wrapWidth) m_roadOffset += wrapWidth;

    // Apply offsets to layer positions
    m_layer1_clouds.setPosition(sf::Vector2f(m_layer1Offset, 50.0f));
    m_layer2_mountains.setPosition(sf::Vector2f(m_layer2Offset, 150.0f));
    m_layer3_hills.setPosition(sf::Vector2f(m_layer3Offset, 250.0f));
    m_layer4_trees.setPosition(sf::Vector2f(m_layer4Offset, 330.0f));
    m_layer5_fence.setPosition(sf::Vector2f(m_layer5Offset, 450.0f));
    m_road.setPosition(sf::Vector2f(m_roadOffset, 650.0f));  // Very narrow road at bottom

    // [MVP] ACTIVE - Car animation (BMW E30 on very narrow road)
    // Animate car (gentle bobbing to simulate movement)
    m_carBobTime += deltaTime * 3.0f;  // Bob frequency
    m_carBobAmount = std::sin(m_carBobTime) * 2.0f;  // +/- 2 pixels (less bob for narrow road)
    m_car.setPosition(sf::Vector2f(300.0f, 670.0f + m_carBobAmount));  // On narrow road (visible above HUD)

    // Update road lane markings positions (scrolling effect)
    float lineSpacing = 150.0f;  // Closer spacing for more frequent dashes
    for (size_t i = 0; i < m_roadLines.size(); ++i) {
        float lineX = m_roadOffset + (i * lineSpacing);
        // Wrap around
        while (lineX < -100.0f) lineX += lineSpacing * m_roadLines.size();
        // Position in center of road: road is 650-750, center is 680
        m_roadLines[i].setPosition(sf::Vector2f(lineX, 680.0f));  // Center of 100px road
    }

    // Update white posts along road edges (Keep Driving style)
    float postSpacing = 120.0f;  // Frequent posts
    for (size_t i = 0; i < m_roadPosts.size(); ++i) {
        float postX = m_roadOffset * 0.95f + (i * postSpacing);  // Slightly slower for depth
        // Wrap around
        while (postX < -100.0f) postX += postSpacing * m_roadPosts.size();

        // Alternate between left and right edges of narrow road
        float postY;
        if (i % 2 == 0) {
            postY = 655.0f;  // Top edge of road (just above)
        } else {
            postY = 715.0f;  // Bottom edge of road
        }
        m_roadPosts[i].setPosition(sf::Vector2f(postX, postY));
    }

    // [MVP] ACTIVE - Speed visualization squares (fast movement)
    float squareSpacing = 120.0f;
    float squareSpeed = baseSpeed * 2.5f;  // Move even faster than road
    for (size_t i = 0; i < m_speedSquares.size(); ++i) {
        float squareX = m_roadOffset * 2.5f + (i * squareSpacing);  // Extra fast
        // Wrap around
        while (squareX < -100.0f) squareX += squareSpacing * m_speedSquares.size();
        // Distribute vertically along fence and road layers
        float squareY = 480.0f + (i % 3) * 50.0f;  // Stagger vertically
        m_speedSquares[i].setPosition(sf::Vector2f(squareX, squareY));
    }

    // [MVP] ACTIVE - Oncoming traffic movement (right to left, opposite direction)
    float oncomingSpeed = baseSpeed * 1.8f;  // Move faster to create relative motion effect
    for (size_t i = 0; i < m_oncomingCarOffsets.size(); ++i) {
        // Move right-to-left (subtract offset)
        m_oncomingCarOffsets[i] -= oncomingSpeed * deltaTime;

        // Wrap around when car goes off-screen left
        if (m_oncomingCarOffsets[i] < -200.0f) {
            // Reset to far right with some spacing
            m_oncomingCarOffsets[i] = UI::SCREEN_WIDTH + (std::rand() % 400);
        }

        // Update oncoming car position on very narrow road
        m_oncomingCars[i].setPosition(sf::Vector2f(m_oncomingCarOffsets[i], 675.0f));  // On narrow road
    }

    // [MVP] ACTIVE - Layer objects positioning (parallax depth)
    float layerWidth = UI::SCREEN_WIDTH * 5.0f;

    // Layer 1 - Clouds objects (very slow, sparse distribution)
    float layer1Spacing = layerWidth / m_layer1Objects.size();
    for (size_t i = 0; i < m_layer1Objects.size(); ++i) {
        float objX = m_layer1Offset + (i * layer1Spacing);
        float objY = 60.0f + (i % 2) * 25.0f;  // Slight vertical variation
        m_layer1Objects[i].setPosition(sf::Vector2f(objX, objY));
    }

    // Layer 2 - Mountains objects (slow, medium distribution)
    float layer2Spacing = layerWidth / m_layer2Objects.size();
    for (size_t i = 0; i < m_layer2Objects.size(); ++i) {
        float objX = m_layer2Offset + (i * layer2Spacing);
        float objY = 160.0f + (i % 3) * 30.0f;  // Vertical variation
        m_layer2Objects[i].setPosition(sf::Vector2f(objX, objY));
    }

    // Layer 3 - Hills objects (medium speed)
    float layer3Spacing = layerWidth / m_layer3Objects.size();
    for (size_t i = 0; i < m_layer3Objects.size(); ++i) {
        float objX = m_layer3Offset + (i * layer3Spacing);
        float objY = 260.0f + (i % 3) * 25.0f;
        m_layer3Objects[i].setPosition(sf::Vector2f(objX, objY));
    }

    // Layer 4 - Trees objects (medium-fast)
    float layer4Spacing = layerWidth / m_layer4Objects.size();
    for (size_t i = 0; i < m_layer4Objects.size(); ++i) {
        float objX = m_layer4Offset + (i * layer4Spacing);
        float objY = 340.0f + (i % 4) * 20.0f;
        m_layer4Objects[i].setPosition(sf::Vector2f(objX, objY));
    }

    // Layer 5 - Fence objects (fast, dense distribution)
    float layer5Spacing = layerWidth / m_layer5Objects.size();
    for (size_t i = 0; i < m_layer5Objects.size(); ++i) {
        float objX = m_layer5Offset + (i * layer5Spacing);
        float objY = 460.0f + (i % 2) * 15.0f;
        m_layer5Objects[i].setPosition(sf::Vector2f(objX, objY));
    }

    // [MVP] Disabled - HUD Panel (uncomment to enable)
    /*
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
    */

    // [MVP] Disabled - Warning indicators (uncomment to enable)
    /*
    // UI Improvements: Update warning flash animation (flash every 0.5 seconds)
    m_warningFlashTimer += deltaTime;
    if (m_warningFlashTimer >= 0.5f) {
        m_warningFlashVisible = !m_warningFlashVisible;
        m_warningFlashTimer = 0.0f;
    }
    */

    // [MVP] ACTIVE - Speedometer (showing ~90 km/h visual speed)
    // Calculate display speed (baseSpeed 400 → 90 km/h, time still 1 min)
    float displaySpeed = 90.0f;  // Fixed visual speed for consistent feel
    if (m_fontLoaded && m_speedText) {
        m_speedText->setString(std::to_string(static_cast<int>(displaySpeed)) + " km/h");
    }

    // [MVP] Disabled - Progress text with km (uncomment to enable)
    /*
    if (m_fontLoaded && m_progressText) {
        std::string progressStr = std::to_string(static_cast<int>(m_distanceTraveled)) + " / " +
                                  std::to_string(static_cast<int>(m_roadData.distance)) + " km";
        m_progressText->setString(progressStr);
    }
    */

    // [MVP] ACTIVE - Progress bar update (no text, just visual bar)
    m_progressBar.setSize(sf::Vector2f(396.0f * m_progress, 26.0f));

    // [MVP] ACTIVE - Auto-completion check
    checkArrival();

    // [MVP] Disabled - Arrival notification (uncomment to enable)
    /*
    // UI Improvements: Trigger arrival notification when close to destination
    if (m_progress >= 0.95f && !m_showArrivalNotification && !m_arrived) {
        m_showArrivalNotification = true;
        m_arrivalNotificationTimer = 0.0f;
    }

    // Update arrival notification timer (show for 3 seconds)
    if (m_showArrivalNotification) {
        m_arrivalNotificationTimer += deltaTime;
        if (m_arrivalNotificationTimer >= 3.0f) {
            m_showArrivalNotification = false;
        }
    }
    */
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

// [MVP] Disabled - Resource consumption (uncomment to enable)
void TravelCinematicScene::consumeResources(float deltaTime) {
    /*
    if (!m_playerState || m_arrived) return;

    // Calculate fuel consumption rate based on car and road type
    float fuelPerKm = calculateFuelConsumption();
    float distanceThisFrame = m_travelSpeed * deltaTime;
    float fuelConsumed = distanceThisFrame * fuelPerKm;

    m_playerState->addFuel(-fuelConsumed);

    // Check if out of fuel
    if (m_playerState->getFuel() <= 0.0f) {
        m_playerState->setFuel(0.0f);
        // Out of fuel - special event system not yet implemented
        std::cerr << "Out of fuel!" << std::endl;
    }
    */
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

    // Note: Car-specific fuel modifiers not yet implemented
    // Currently using base values for all car types

    return baseFuel;
}

// [MVP] Disabled - Random events (uncomment to enable)
void TravelCinematicScene::tryTriggerEvent() {
    /*
    if (!m_playerState) return;

    // Don't trigger events if almost arrived
    if (m_progress > 0.9f) return;

    // Get a random event from the event manager (pass road type for context)
    GameEvent* event = m_eventManager.getRandomEvent(
        m_playerState->getFuel(),
        m_playerState->getEnergy(),
        static_cast<int>(m_playerState->getMoney()),
        m_roadData.toNodeId,
        m_roadData.roadType  // Pass road type for road-specific events
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
    */
}

void TravelCinematicScene::checkArrival() {
    if (m_progress >= 1.0f && !m_arrived) {
        m_arrived = true;
        m_isFinished = true;
        std::cout << "Arrived at destination: " << m_roadData.toNodeId << std::endl;

        // [MVP] Update city index based on travel direction (forward or backward)
        if (m_playerState) {
            int currentCityIndex = m_playerState->getCurrentCityIndex();

            // Parse destination city index from toNodeId (e.g., "city_1" → 1)
            int destinationIndex = currentCityIndex;  // Default: stay in same city
            if (m_roadData.toNodeId.find("city_") == 0) {
                try {
                    destinationIndex = std::stoi(m_roadData.toNodeId.substr(5));  // Extract number after "city_"
                } catch (...) {
                    std::cerr << "[MVP] Failed to parse destination city index from: " << m_roadData.toNodeId << std::endl;
                }
            }

            // Determine direction and update city index
            if (destinationIndex > currentCityIndex) {
                // Moving forward
                m_playerState->advanceToNextCity();
                std::cout << "[MVP] Forward travel: " << currentCityIndex << " -> " << m_playerState->getCurrentCityIndex() << std::endl;
            } else if (destinationIndex < currentCityIndex) {
                // Moving backward
                m_playerState->setCurrentCityIndex(destinationIndex);
                std::cout << "[MVP] Backward travel: " << currentCityIndex << " -> " << destinationIndex << std::endl;
            } else {
                std::cout << "[MVP] Warning: Traveling to same city index " << currentCityIndex << std::endl;
            }
        }
    }
}

void TravelCinematicScene::render(sf::RenderWindow& window) {
    // [MVP] ACTIVE - 7-layer parallax system (Keep Driving style)

    // Layer 0: Sky (static, no movement)
    window.draw(m_sky);

    // Layer 1: Clouds (very slow)
    window.draw(m_layer1_clouds);
    sf::RectangleShape clouds_dup = m_layer1_clouds;
    clouds_dup.setPosition(sf::Vector2f(m_layer1Offset + UI::SCREEN_WIDTH, 50.0f));
    window.draw(clouds_dup);

    // Layer 1 objects (clouds)
    for (const auto& obj : m_layer1Objects) {
        window.draw(obj);
    }

    // Layer 2: Mountains (slow)
    window.draw(m_layer2_mountains);
    sf::RectangleShape mountains_dup = m_layer2_mountains;
    mountains_dup.setPosition(sf::Vector2f(m_layer2Offset + UI::SCREEN_WIDTH, 150.0f));
    window.draw(mountains_dup);

    // Layer 2 objects (mountains)
    for (const auto& obj : m_layer2Objects) {
        window.draw(obj);
    }

    // Layer 3: Hills (medium)
    window.draw(m_layer3_hills);
    sf::RectangleShape hills_dup = m_layer3_hills;
    hills_dup.setPosition(sf::Vector2f(m_layer3Offset + UI::SCREEN_WIDTH, 250.0f));
    window.draw(hills_dup);

    // Layer 3 objects (hills)
    for (const auto& obj : m_layer3Objects) {
        window.draw(obj);
    }

    // Layer 4: Trees (medium-fast)
    window.draw(m_layer4_trees);
    sf::RectangleShape trees_dup = m_layer4_trees;
    trees_dup.setPosition(sf::Vector2f(m_layer4Offset + UI::SCREEN_WIDTH, 330.0f));
    window.draw(trees_dup);

    // Layer 4 objects (trees)
    for (const auto& obj : m_layer4Objects) {
        window.draw(obj);
    }

    // Layer 5: Fence (fast)
    window.draw(m_layer5_fence);
    sf::RectangleShape fence_dup = m_layer5_fence;
    fence_dup.setPosition(sf::Vector2f(m_layer5Offset + UI::SCREEN_WIDTH, 450.0f));
    window.draw(fence_dup);

    // Layer 5 objects (fence)
    for (const auto& obj : m_layer5Objects) {
        window.draw(obj);
    }

    // Layer 6: Road (fastest)
    window.draw(m_road);
    sf::RectangleShape road_dup = m_road;
    road_dup.setPosition(sf::Vector2f(m_roadOffset + UI::SCREEN_WIDTH, 500.0f));
    window.draw(road_dup);

    // Road lane markings (fast scrolling for speed effect)
    for (const auto& line : m_roadLines) {
        window.draw(line);
    }

    // White posts along road edges (Keep Driving style)
    for (const auto& post : m_roadPosts) {
        window.draw(post);
    }

    // [MVP] ACTIVE - Speed visualization squares (super fast movement)
    for (const auto& square : m_speedSquares) {
        window.draw(square);
    }

    // [MVP] ACTIVE - Oncoming traffic rendering (draw before player car)
    for (const auto& oncomingCar : m_oncomingCars) {
        window.draw(oncomingCar);
    }

    // [MVP] ACTIVE - Car rendering
    window.draw(m_car);

    // [MVP] ACTIVE - Progress bar UI (visual only, no text)
    window.draw(m_progressBarBg);
    window.draw(m_progressBar);

    // [MVP] Disabled - Progress text with km (uncomment to enable)
    /*
    if (m_fontLoaded) {
        if (m_progressText) {
            window.draw(*m_progressText);
        }
    }
    */

    // [MVP] Disabled - Resource text displays (uncomment to enable)
    /*
    if (m_fontLoaded) {
        if (m_fuelText) {
            window.draw(*m_fuelText);
        }
        if (m_distanceText) {
            window.draw(*m_distanceText);
        }
    }
    */

    // [MVP] Disabled - HUD Panel (uncomment to enable)
    /*
    m_hudPanel.render(window);
    */

    // [MVP] ACTIVE - Speedometer (shows ~90 km/h)
    window.draw(m_speedometerBg);
    if (m_fontLoaded && m_speedText) {
        window.draw(*m_speedText);
    }

    // [MVP] Disabled - Warning indicators (uncomment to enable)
    /*
    if (m_playerState && m_warningFlashVisible) {
        // Low fuel warning (< 20%)
        if (m_playerState->getFuel() < 20.0f) {
            window.draw(m_fuelWarningIcon);
            if (m_fontLoaded && m_fuelWarningText) {
                window.draw(*m_fuelWarningText);
            }
        }

        // Low energy warning (< 20%)
        if (m_playerState->getEnergy() < 20.0f) {
            window.draw(m_energyWarningIcon);
            if (m_fontLoaded && m_energyWarningText) {
                window.draw(*m_energyWarningText);
            }
        }
    }
    */

    // [MVP] Disabled - Travel dialogues (uncomment to enable)
    /*
    if (m_currentDialogue.has_value() && !m_activeEvent) {
        window.draw(m_dialogueBox);
        window.draw(m_npcPortrait);
        if (m_dialogueNameText) window.draw(*m_dialogueNameText);
        if (m_dialogueText) window.draw(*m_dialogueText);
    }
    */

    // [MVP] Disabled - Arrival notification (uncomment to enable)
    /*
    if (m_showArrivalNotification) {
        // Fade in/out effect based on timer
        float alpha = 255.0f;
        if (m_arrivalNotificationTimer < 0.5f) {
            // Fade in
            alpha = (m_arrivalNotificationTimer / 0.5f) * 255.0f;
        } else if (m_arrivalNotificationTimer > 2.5f) {
            // Fade out
            alpha = ((3.0f - m_arrivalNotificationTimer) / 0.5f) * 255.0f;
        }

        sf::Color bgColor = m_arrivalNotificationBg.getFillColor();
        bgColor.a = static_cast<uint8_t>(alpha * 0.9f);  // 90% of alpha for background
        m_arrivalNotificationBg.setFillColor(bgColor);

        window.draw(m_arrivalNotificationBg);

        if (m_fontLoaded && m_arrivalNotificationText) {
            sf::Color textColor = m_arrivalNotificationText->getFillColor();
            textColor.a = static_cast<uint8_t>(alpha);
            m_arrivalNotificationText->setFillColor(textColor);
            window.draw(*m_arrivalNotificationText);
        }
    }
    */

    // [MVP] Disabled - Active event rendering (uncomment to enable)
    /*
    if (m_activeEvent) {
        m_activeEvent->render(window);
    }
    */

    // [Decorative] Keep Driving style HUD panel (drawn last, on top of everything)
    window.draw(m_hudBackground);
    window.draw(m_hudLeftPanel);
    window.draw(m_hudCenterPanel);
    window.draw(m_hudRightPanel);

    // Draw energy bars
    for (const auto& bar : m_hudEnergyBars) {
        window.draw(bar);
    }

    // Draw car fuel bars
    for (const auto& bar : m_hudCarBars) {
        window.draw(bar);
    }

    // Draw character and car sprites
    window.draw(m_hudCharacterSprite);
    window.draw(m_hudCarTopView);

    // Draw text labels
    if (m_fontLoaded) {
        if (m_hudEnergyLabel) window.draw(*m_hudEnergyLabel);
        if (m_hudStatusLabel) window.draw(*m_hudStatusLabel);
        if (m_hudCarLabel) window.draw(*m_hudCarLabel);
        if (m_hudGasLabel) window.draw(*m_hudGasLabel);
        if (m_hudDateTimeText) window.draw(*m_hudDateTimeText);
    }
}

SceneType TravelCinematicScene::getNextScene() const {
    return m_nextScene;
}

bool TravelCinematicScene::isFinished() const {
    return m_isFinished;
}

void TravelCinematicScene::initializeTravelDialogues() {
    // Simple travel dialogues (companions chatting during road trip)
    m_availableDialogues.push_back(TravelDialogue(
        "Ivan",
        "Эх, дорога... Километры летят как птицы. / Ah, the road... kilometers fly like birds.",
        8.0f
    ));

    m_availableDialogues.push_back(TravelDialogue(
        "Katya",
        "Помнишь то место с лучшим кофе? Надо заехать. / Remember that place with the best coffee?",
        7.0f
    ));

    m_availableDialogues.push_back(TravelDialogue(
        "Dmitry",
        "Машина шумит... Проверим на следующей станции. / Car's making noise... Check at next station.",
        6.0f
    ));

    m_availableDialogues.push_back(TravelDialogue(
        "Anna",
        "Закат красивый сегодня. Остановимся? / Beautiful sunset. Should we stop?",
        7.0f
    ));

    m_availableDialogues.push_back(TravelDialogue(
        "Sergey",
        "Сколько еще до города? Устал смотреть в окно. / How much longer? Tired of looking out.",
        7.0f
    ));

    m_availableDialogues.push_back(TravelDialogue(
        "You",
        "Эта дорога никогда не кончается... / This road never ends...",
        6.0f
    ));

    m_availableDialogues.push_back(TravelDialogue(
        "Radio",
        "📻 Новости: Топливо подорожало на 5%. / News: Fuel prices up 5%.",
        8.0f
    ));

    m_availableDialogues.push_back(TravelDialogue(
        "Olga",
        "У меня есть перекус, если кто голодный. / I have snacks if anyone's hungry.",
        7.0f
    ));
}

// [MVP] Disabled - Travel dialogues (uncomment to enable)
void TravelCinematicScene::tryTriggerDialogue() {
    /*
    // Don't trigger if too close to arrival or if already showing dialogue
    if (m_progress > 0.85f || m_currentDialogue.has_value()) return;

    // Don't trigger if no companions (check PlayerState companions list)
    // For now, 50% chance to trigger a random dialogue
    if (rand() % 100 < 50 && !m_availableDialogues.empty()) {
        size_t index = rand() % m_availableDialogues.size();
        m_currentDialogue = m_availableDialogues[index];
        m_dialogueTimer = 0.0f;

        // Update UI text
        if (m_dialogueNameText && m_dialogueText) {
            m_dialogueNameText->setString(m_currentDialogue->npcName + ":");
            m_dialogueText->setString(m_currentDialogue->text);
        }

        std::cout << "[Dialogue] " << m_currentDialogue->npcName << ": "
                  << m_currentDialogue->text << std::endl;
    }
    */
}
