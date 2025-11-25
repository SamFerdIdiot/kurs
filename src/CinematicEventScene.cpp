#include "CinematicEventScene.h"
#include "PlayerState.h"
#include "AbilityTreeSystem.h"
#include <iostream>

// Constructor
CinematicEventScene::CinematicEventScene()
    : m_event(nullptr),
      m_playerState(nullptr),
      m_isFinished(false),
      m_nextScene(SceneType::ROAD),
      m_selectedChoice(-1),
      m_hoveredChoice(-1),
      m_outcomeShown(false),
      m_fontLoaded(false),
      m_hudPanel(800.0f, 600.0f) {  // Initialize HUD
    setupUI();
}

CinematicEventScene::CinematicEventScene(GameEvent* event, PlayerState* playerState)
    : m_event(event),
      m_playerState(playerState),
      m_isFinished(false),
      m_nextScene(SceneType::ROAD),
      m_selectedChoice(-1),
      m_hoveredChoice(-1),
      m_outcomeShown(false),
      m_fontLoaded(false),
      m_hudPanel(800.0f, 600.0f) {  // Initialize HUD
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
    setupUI();
}

// Setup UI elements
void CinematicEventScene::setupUI() {
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("CinematicEventScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Try to load font (use system font as fallback)
    // Note: In production, use proper asset loading
    m_fontLoaded = m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    
    if (!m_fontLoaded) {
        std::cerr << "Warning: Could not load font, using default" << std::endl;
    }
    
    // Setup overlay (darkened background) with responsive sizing
    m_overlay.setSize(sf::Vector2f(layout.getTargetWidth(), layout.getTargetHeight()));
    m_overlay.setFillColor(sf::Color(0, 0, 0, 180));
    
    // Setup dialog box with responsive scaling
    m_dialogBox.setSize(SCALE_SIZE(layout, 1000, 600));
    m_dialogBox.setPosition(SCALE_POS(layout, 183, 84));
    m_dialogBox.setFillColor(sf::Color(40, 40, 45));
    m_dialogBox.setOutlineColor(sf::Color(200, 200, 200));
    m_dialogBox.setOutlineThickness(3);
    
    // Setup title text
    if (m_fontLoaded) {
        m_titleText.setFont(m_font);
    }
    m_titleText.setCharacterSize(32);
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(233, 120);
    
    // Setup description text
    if (m_fontLoaded) {
        m_descriptionText.setFont(m_font);
    }
    m_descriptionText.setCharacterSize(20);
    m_descriptionText.setFillColor(sf::Color(220, 220, 220));
    m_descriptionText.setPosition(233, 200);
    
    // Setup outcome text
    if (m_fontLoaded) {
        m_outcomeText.setFont(m_font);
    }
    m_outcomeText.setCharacterSize(22);
    m_outcomeText.setFillColor(sf::Color(255, 255, 150));
    m_outcomeText.setPosition(233, 400);
    
    // Setup continue prompt
    if (m_fontLoaded) {
        m_continuePrompt.setFont(m_font);
    }
    m_continuePrompt.setCharacterSize(18);
    m_continuePrompt.setFillColor(sf::Color(150, 150, 150));
    m_continuePrompt.setString("Нажмите ENTER чтобы продолжить / Press ENTER to continue");
    m_continuePrompt.setPosition(400, 650);
    
    // Update texts if event is set
    if (m_event) {
        m_titleText.setString(m_event->title);
        
        // Wrap description text
        std::string wrappedDesc = wrapText(m_event->description, 900, m_font, 20);
        m_descriptionText.setString(wrappedDesc);
        
        // Setup choice boxes
        m_choiceBoxes.clear();
        m_choiceTexts.clear();
        
        float choiceY = 350;
        for (size_t i = 0; i < m_event->choices.size(); ++i) {
            // Choice box
            sf::RectangleShape choiceBox;
            choiceBox.setSize(sf::Vector2f(900, 50));
            choiceBox.setPosition(233, choiceY);
            choiceBox.setFillColor(sf::Color(60, 60, 70));
            choiceBox.setOutlineColor(sf::Color(100, 100, 110));
            choiceBox.setOutlineThickness(2);
            m_choiceBoxes.push_back(choiceBox);
            
            // Choice text
            sf::Text choiceText;
            if (m_fontLoaded) {
                choiceText.setFont(m_font);
            }
            choiceText.setCharacterSize(18);
            choiceText.setFillColor(sf::Color::White);
            choiceText.setString(m_event->choices[i].text);
            choiceText.setPosition(243, choiceY + 15);
            m_choiceTexts.push_back(choiceText);
            
            choiceY += 60;
        }
    }
}

// Handle input
void CinematicEventScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
        updateChoiceHover(mousePos);
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (m_outcomeShown) {
                // Continue after showing outcome
                m_isFinished = true;
            } else if (m_hoveredChoice >= 0 && m_event) {
                // Select choice
                m_selectedChoice = m_hoveredChoice;
                m_outcomeShown = true;
                
                // Apply choice outcome
                if (m_playerState) {
                    const EventChoice& choice = m_event->choices[m_selectedChoice];
                    m_playerState->addFuel(choice.fuelChange);
                    m_playerState->addEnergy(choice.energyChange);
                    m_playerState->addMoney(static_cast<float>(choice.moneyChange));
                    
                    // If this is a perk choice, trigger cooldown
                    if (choice.isPerkChoice && !choice.perkId.empty()) {
                        AbilityTreeSystem::getInstance().useActivePerk(choice.perkId);
                    }
                    
                    // Show outcome
                    m_outcomeText.setString(choice.outcomeText);
                }
            }
        }
    }
    
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter || 
            event.key.code == sf::Keyboard::Space) {
            if (m_outcomeShown) {
                m_isFinished = true;
            }
        }
        
        if (event.key.code == sf::Keyboard::Escape) {
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
        if (bounds.contains(static_cast<float>(mousePos.x), 
                          static_cast<float>(mousePos.y))) {
            m_hoveredChoice = static_cast<int>(i);
            break;
        }
    }
}

// Update
void CinematicEventScene::update(float deltaTime) {
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
    
    // Update HUD
    if (m_playerState) {
        m_hudPanel.setMoney(m_playerState->getMoney());
        m_hudPanel.setFuel(m_playerState->getFuel(), 100.0f);
        m_hudPanel.setEnergy(m_playerState->getEnergy(), 100.0f);
        float cargoWeight = m_playerState->getInventory().getTotalWeight();
        m_hudPanel.setCargo(cargoWeight, 100.0f);
    }
    m_hudPanel.updateTopRight(deltaTime);
}

// Render
void CinematicEventScene::render(sf::RenderWindow& window) {
    // Draw overlay
    window.draw(m_overlay);
    
    // Draw dialog box
    window.draw(m_dialogBox);
    
    // Draw title
    if (m_fontLoaded) {
        window.draw(m_titleText);
    }
    
    // Draw description
    if (m_fontLoaded) {
        window.draw(m_descriptionText);
    }
    
    // Draw choices or outcome
    if (m_outcomeShown) {
        // Show outcome
        if (m_fontLoaded) {
            window.draw(m_outcomeText);
            window.draw(m_continuePrompt);
        }
    } else {
        // Show choices
        for (size_t i = 0; i < m_choiceBoxes.size(); ++i) {
            window.draw(m_choiceBoxes[i]);
            if (m_fontLoaded) {
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
            sf::Text testText(testLine, font, charSize);
            
            if (testText.getGlobalBounds().width > maxWidth && !currentLine.empty()) {
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
        sf::Text testText(testLine, font, charSize);
        
        if (testText.getGlobalBounds().width > maxWidth && !currentLine.empty()) {
            result += currentLine + "\n" + word;
        } else {
            result += testLine;
        }
    } else if (!currentLine.empty()) {
        result += currentLine;
    }
    
    return result;
}
