#include "CinematicEventScene.h"
#include "PlayerState.h"
#include "AbilityTreeSystem.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include <iostream>

// Constructor
CinematicEventScene::CinematicEventScene()
    : m_event(nullptr),
      m_playerState(nullptr),
      m_isFinished(false),
      m_nextScene(SceneType::TRAVEL_CINEMATIC),  // Changed from ROAD
      m_selectedChoice(-1),
      m_hoveredChoice(-1),
      m_outcomeShown(false),
      m_fontLoaded(false),
      m_hudPanel(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT),  // Fixed dimensions
      m_fadeAlpha(0.0f),
      m_animationTime(0.0f) {
    setupUI();
}

CinematicEventScene::CinematicEventScene(GameEvent* event, PlayerState* playerState)
    : m_event(event),
      m_playerState(playerState),
      m_isFinished(false),
      m_nextScene(SceneType::TRAVEL_CINEMATIC),  // Changed from ROAD
      m_selectedChoice(-1),
      m_hoveredChoice(-1),
      m_outcomeShown(false),
      m_fontLoaded(false),
      m_hudPanel(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT),  // Fixed dimensions
      m_fadeAlpha(0.0f),
      m_animationTime(0.0f) {
    setupUI();
}

// Set the event to display
void CinematicEventScene::setEvent(GameEvent* event, PlayerState* playerState) {
    m_event = event;
    m_playerState = playerState;
    m_selectedChoice = -1;
    m_hoveredChoice = -1;
    m_outcomeShown = false;
    m_isFinished = false;
    m_fadeAlpha = 0.0f;
    m_animationTime = 0.0f;

    // [SOUND PLACEHOLDER] Play event appear sound
    std::cout << "[SOUND] Event appeared: " << (event ? event->id : "none") << std::endl;

    setupUI();
}

// Setup UI elements
void CinematicEventScene::setupUI() {
    // Fixed layout for 1440x900
    const float SCREEN_WIDTH = 1440.0f;
    const float SCREEN_HEIGHT = 900.0f;

    // Try to load font (use system font as fallback) - SFML 3.x uses openFromFile
    if (m_font.openFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "CinematicEventScene: WARNING - Could not load any font!" << std::endl;
    }

    // Setup overlay (darkened background) with fixed sizing
    m_overlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_overlay.setFillColor(sf::Color(0, 0, 0, 180));
    m_overlay.setPosition(sf::Vector2f(0.0f, 0.0f));

    // Setup dialog box with fixed sizing
    m_dialogBox.setSize(sf::Vector2f(1000.0f, 600.0f));
    m_dialogBox.setPosition(sf::Vector2f(220.0f, 150.0f));
    m_dialogBox.setFillColor(sf::Color(40, 40, 45));

    // Dynamic outline color based on event type
    sf::Color outlineColor = getEventTypeColor();
    m_dialogBox.setOutlineColor(outlineColor);
    m_dialogBox.setOutlineThickness(4.0f);

    // Setup event icon (colored rectangle indicator)
    m_eventIcon.setSize(sf::Vector2f(10.0f, 560.0f));
    m_eventIcon.setPosition(sf::Vector2f(222.0f, 165.0f));
    m_eventIcon.setFillColor(outlineColor);

    // Setup title text - SFML 3.x requires font in constructor
    if (m_fontLoaded) {
        m_titleText.emplace(m_font, "", 32);
        m_titleText->setFillColor(sf::Color::White);
        m_titleText->setPosition(sf::Vector2f(270.0f, 180.0f));

        // Setup description text
        m_descriptionText.emplace(m_font, "", 20);
        m_descriptionText->setFillColor(sf::Color(220, 220, 220));
        m_descriptionText->setPosition(sf::Vector2f(270.0f, 240.0f));

        // Setup outcome text
        m_outcomeText.emplace(m_font, "", 22);
        m_outcomeText->setFillColor(sf::Color(255, 255, 150));
        m_outcomeText->setPosition(sf::Vector2f(270.0f, 450.0f));

        // Setup continue prompt
        m_continuePrompt.emplace(m_font, "Нажмите ENTER чтобы продолжить / Press ENTER to continue", 18);
        m_continuePrompt->setFillColor(sf::Color(150, 150, 150));
        m_continuePrompt->setPosition(sf::Vector2f(400.0f, 680.0f));
    }

    // Update texts if event is set
    if (m_event) {
        if (m_titleText) {
            m_titleText->setString(m_event->title);
        }

        // Wrap description text
        if (m_descriptionText) {
            std::string wrappedDesc = wrapText(m_event->description, 900, m_font, 20);
            m_descriptionText->setString(wrappedDesc);
        }

        // Setup choice boxes
        m_choiceBoxes.clear();
        m_choiceTexts.clear();

        float choiceY = 400.0f;
        for (size_t i = 0; i < m_event->choices.size(); ++i) {
            // Choice box
            sf::RectangleShape choiceBox;
            choiceBox.setSize(sf::Vector2f(900.0f, 50.0f));
            choiceBox.setPosition(sf::Vector2f(270.0f, choiceY));
            choiceBox.setFillColor(sf::Color(60, 60, 70));
            choiceBox.setOutlineColor(sf::Color(100, 100, 110));
            choiceBox.setOutlineThickness(2.0f);
            m_choiceBoxes.push_back(choiceBox);

            // Choice text - SFML 3.x: Text requires font in constructor
            if (m_fontLoaded) {
                sf::Text choiceText(m_font, m_event->choices[i].text, 18);
                choiceText.setFillColor(sf::Color::White);
                choiceText.setPosition(sf::Vector2f(280.0f, choiceY + 15.0f));
                m_choiceTexts.push_back(choiceText);
            }

            choiceY += 60.0f;
        }
    }
}

// Handle input
void CinematicEventScene::handleInput(const sf::Event& event) {
    // SFML 3.x event handling
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos(mouseMoved->position.x, mouseMoved->position.y);
        updateChoiceHover(mousePos);
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            if (m_outcomeShown) {
                // Continue after showing outcome
                m_isFinished = true;

                // [SOUND PLACEHOLDER] Play confirm/continue sound
                std::cout << "[SOUND] Event closed" << std::endl;
            } else if (m_hoveredChoice >= 0 && m_event) {
                // Select choice
                m_selectedChoice = m_hoveredChoice;
                m_outcomeShown = true;

                // [SOUND PLACEHOLDER] Play choice selection sound
                std::cout << "[SOUND] Choice selected: " << m_selectedChoice << std::endl;

                // Apply choice outcome
                if (m_playerState && m_outcomeText) {
                    const EventChoice& choice = m_event->choices[m_selectedChoice];
                    // [MVP] Disabled - Resource changes (uncomment to enable)
                    /*
                    m_playerState->addFuel(choice.fuelChange);
                    m_playerState->addEnergy(choice.energyChange);
                    m_playerState->addMoney(static_cast<float>(choice.moneyChange));
                    */

                    // If this is a perk choice, trigger cooldown
                    if (choice.isPerkChoice && !choice.perkId.empty()) {
                        AbilityTreeSystem::getInstance().useActivePerk(choice.perkId);
                    }

                    // Show outcome
                    m_outcomeText->setString(choice.outcomeText);

                    // [SOUND PLACEHOLDER] Play outcome sound based on result
                    if (choice.energyChange > 0 || choice.fuelChange > 0 || choice.moneyChange > 0) {
                        std::cout << "[SOUND] Positive outcome" << std::endl;
                    } else if (choice.energyChange < -10 || choice.fuelChange < -10 || choice.moneyChange < -200) {
                        std::cout << "[SOUND] Negative outcome" << std::endl;
                    }
                }
            }
        }
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Enter ||
            keyPressed->code == sf::Keyboard::Key::Space) {
            if (m_outcomeShown) {
                m_isFinished = true;
            }
        }

        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            // Skip event
            m_isFinished = true;
        }
    }
}

// Update choice hover
void CinematicEventScene::updateChoiceHover(const sf::Vector2i& mousePos) {
    m_hoveredChoice = -1;

    if (m_outcomeShown) {
        return; // No hover after choice is made
    }

    for (size_t i = 0; i < m_choiceBoxes.size(); ++i) {
        sf::FloatRect bounds = m_choiceBoxes[i].getGlobalBounds();
        // SFML 3.x: contains() requires Vector2f wrapper
        if (bounds.contains(sf::Vector2f(static_cast<float>(mousePos.x),
                                         static_cast<float>(mousePos.y)))) {
            m_hoveredChoice = static_cast<int>(i);
            break;
        }
    }
}

// Update
void CinematicEventScene::update(float deltaTime) {
    // Update fade-in animation
    m_animationTime += deltaTime;
    if (m_fadeAlpha < 255.0f) {
        m_fadeAlpha = std::min(255.0f, m_fadeAlpha + deltaTime * 400.0f);  // Fade in over ~0.6 seconds

        // Apply fade to dialog box
        sf::Color boxColor = m_dialogBox.getFillColor();
        boxColor.a = static_cast<std::uint8_t>(m_fadeAlpha * (180.0f / 255.0f));
        m_dialogBox.setFillColor(boxColor);

        // Apply fade to outline
        sf::Color outlineColor = m_dialogBox.getOutlineColor();
        outlineColor.a = static_cast<std::uint8_t>(m_fadeAlpha);
        m_dialogBox.setOutlineColor(outlineColor);
        m_eventIcon.setFillColor(outlineColor);
    }

    // Update choice box colors based on hover
    for (size_t i = 0; i < m_choiceBoxes.size(); ++i) {
        if (m_outcomeShown) {
            if (static_cast<int>(i) == m_selectedChoice) {
                m_choiceBoxes[i].setFillColor(sf::Color(100, 150, 100));
            } else {
                m_choiceBoxes[i].setFillColor(sf::Color(40, 40, 50));
            }
        } else if (static_cast<int>(i) == m_hoveredChoice) {
            m_choiceBoxes[i].setFillColor(sf::Color(80, 80, 100));
        } else {
            m_choiceBoxes[i].setFillColor(sf::Color(60, 60, 70));
        }
    }

    // [MVP] Disabled - HUD update (uncomment to enable)
    /*
    // Update HUD
    if (m_playerState) {
        m_hudPanel.setMoney(m_playerState->getMoney());
        m_hudPanel.setFuel(m_playerState->getFuel(), 100.0f);
        m_hudPanel.setEnergy(m_playerState->getEnergy(), 100.0f);
        float cargoWeight = m_playerState->getInventory().getTotalWeight();
        m_hudPanel.setCargo(cargoWeight, 100.0f);
    }
    m_hudPanel.updateTopRight(deltaTime);
    */
}

// Render
void CinematicEventScene::render(sf::RenderWindow& window) {
    // Draw overlay
    window.draw(m_overlay);

    // Draw dialog box
    window.draw(m_dialogBox);

    // Draw event type indicator
    window.draw(m_eventIcon);

    // Draw title
    if (m_titleText) {
        window.draw(*m_titleText);
    }

    // Draw description
    if (m_descriptionText) {
        window.draw(*m_descriptionText);
    }

    // Draw choices or outcome
    if (m_outcomeShown) {
        // Show outcome
        if (m_outcomeText) {
            window.draw(*m_outcomeText);
        }
        if (m_continuePrompt) {
            window.draw(*m_continuePrompt);
        }
    } else {
        // Show choices
        for (size_t i = 0; i < m_choiceBoxes.size(); ++i) {
            window.draw(m_choiceBoxes[i]);
            if (m_fontLoaded && i < m_choiceTexts.size()) {
                window.draw(m_choiceTexts[i]);
            }
        }
    }

    // Draw HUD
    m_hudPanel.render(window);
}

// Get next scene
SceneType CinematicEventScene::getNextScene() const {
    return m_nextScene;
}

// Check if finished
bool CinematicEventScene::isFinished() const {
    return m_isFinished;
}

// Check if an outcome was selected
bool CinematicEventScene::hasOutcome() const {
    return m_selectedChoice >= 0;
}

// Get the selected choice
const EventChoice* CinematicEventScene::getSelectedChoice() const {
    if (m_event && m_selectedChoice >= 0 && 
        m_selectedChoice < static_cast<int>(m_event->choices.size())) {
        return &m_event->choices[m_selectedChoice];
    }
    return nullptr;
}

// Wrap text helper
std::string CinematicEventScene::wrapText(const std::string& text, int maxWidth,
                                          const sf::Font& font, unsigned int charSize) {
    if (!m_fontLoaded) {
        return text;
    }

    std::string result;
    std::string currentLine;
    std::string word;

    for (char c : text) {
        if (c == ' ' || c == '\n') {
            // Check if adding word exceeds max width
            std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
            sf::Text testText(font, testLine, charSize);  // SFML 3.x: font first

            // SFML 3.x: Use .size.x instead of .width
            if (testText.getGlobalBounds().size.x > maxWidth && !currentLine.empty()) {
                result += currentLine + "\n";
                currentLine = word;
            } else {
                currentLine = testLine;
            }

            word.clear();

            if (c == '\n') {
                result += currentLine + "\n";
                currentLine.clear();
            }
        } else {
            word += c;
        }
    }

    // Add remaining word and line
    if (!word.empty()) {
        std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
        sf::Text testText(font, testLine, charSize);  // SFML 3.x: font first

        // SFML 3.x: Use .size.x instead of .width
        if (testText.getGlobalBounds().size.x > maxWidth && !currentLine.empty()) {
            result += currentLine + "\n" + word;
        } else {
            result += testLine;
        }
    } else if (!currentLine.empty()) {
        result += currentLine;
    }

    return result;
}

// Get color for event type
sf::Color CinematicEventScene::getEventTypeColor() const {
    if (!m_event) {
        return sf::Color(200, 200, 200);  // Default gray
    }

    // Determine event type based on ID prefix or conditions
    const std::string& id = m_event->id;

    // Road type specific colors
    if (id.find("highway") != std::string::npos) {
        return sf::Color(100, 150, 255);  // Blue for highway
    }
    if (id.find("road") != std::string::npos) {
        return sf::Color(150, 200, 100);  // Green for road
    }
    if (id.find("path") != std::string::npos) {
        return sf::Color(180, 120, 80);  // Brown for path
    }

    // Companion events
    if (id.find("companion") != std::string::npos) {
        return sf::Color(255, 200, 100);  // Orange for companions
    }

    // Hitchhiker/recruit events
    if (id.find("hitchhiker") != std::string::npos) {
        return sf::Color(255, 150, 200);  // Pink for recruitable NPCs
    }

    // Danger/mechanical events
    if (id.find("breakdown") != std::string::npos ||
        id.find("tire") != std::string::npos ||
        id.find("overheat") != std::string::npos ||
        id.find("pothole") != std::string::npos) {
        return sf::Color(255, 100, 100);  // Red for danger
    }

    // Economic/money events
    if (id.find("toll") != std::string::npos ||
        id.find("vendor") != std::string::npos ||
        id.find("trader") != std::string::npos ||
        id.find("smuggler") != std::string::npos) {
        return sf::Color(255, 215, 0);  // Gold for economic
    }

    // Social/positive events
    if (id.find("friend") != std::string::npos ||
        id.find("help") != std::string::npos ||
        id.find("tourist") != std::string::npos) {
        return sf::Color(150, 255, 150);  // Light green for social
    }

    // Weather events
    if (id.find("rain") != std::string::npos ||
        id.find("fog") != std::string::npos ||
        id.find("snow") != std::string::npos) {
        return sf::Color(180, 180, 220);  // Light blue for weather
    }

    return sf::Color(200, 200, 200);  // Default gray
}

// Get event type label
std::string CinematicEventScene::getEventTypeLabel() const {
    if (!m_event) {
        return "Event";
    }

    const std::string& id = m_event->id;

    // Determine label based on event ID
    if (id.find("highway") != std::string::npos) return "Highway";
    if (id.find("road") != std::string::npos) return "Road";
    if (id.find("path") != std::string::npos) return "Path";
    if (id.find("companion") != std::string::npos) return "Companion";
    if (id.find("hitchhiker") != std::string::npos) return "Recruit";
    if (id.find("breakdown") != std::string::npos ||
        id.find("tire") != std::string::npos ||
        id.find("mechanical") != std::string::npos) return "Mechanical";
    if (id.find("toll") != std::string::npos ||
        id.find("vendor") != std::string::npos ||
        id.find("trader") != std::string::npos) return "Economic";
    if (id.find("rain") != std::string::npos ||
        id.find("fog") != std::string::npos ||
        id.find("snow") != std::string::npos) return "Weather";
    if (id.find("police") != std::string::npos) return "Police";

    return "Event";
}
