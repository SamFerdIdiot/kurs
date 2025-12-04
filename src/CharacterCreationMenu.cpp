#include "CharacterCreationMenu.h"
#include <iostream>

// Constructor
CharacterCreationMenu::CharacterCreationMenu()
    : m_currentStep(0),
      m_selectedIndex(0),
      m_selectedBackground(-1),
      m_selectedCar(-1),
      m_isComplete(false),
      m_goBack(false),
      m_typewriterTimer(0.0f),
      m_typewriterComplete(false),
      m_arrowTimer(0.0f),
      m_arrowVisible(true) {
}

// Initialize with font
bool CharacterCreationMenu::initialize(const std::string& fontPath) {
    // SFML 3.x: openFromFile returns bool
    if (!m_font.openFromFile(fontPath)) {
        std::cerr << "Failed to load font: " << fontPath << std::endl;
        return false;
    }
    
    // Setup initial menu items (backgrounds)
    m_menuItems = {
        "1. Ex-Racer",
        "   Bonus: 16% chance to skip events",
        "   ",
        "2. Student",
        "   Bonus: Companions get joy effect",
        "   ",
        "3. Unemployed",
        "   Feature: Hard mode, 2 unique endings"
    };
    
    // Initialize displayed characters to 0
    m_displayedChars.resize(m_menuItems.size(), 0);
    
    return true;
}

// Handle input
void CharacterCreationMenu::handleInput(const sf::Event& event) {
    // SFML 3.x event handling
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            // Page navigation with A/D keys (previous/next)
            case sf::Keyboard::Key::A:
            case sf::Keyboard::Key::Left:
                if (m_currentStep == 0) {
                    // Previous background page
                    m_selectedIndex = (m_selectedIndex - 1 + 3) % 3;
                } else {
                    // Previous car page
                    m_selectedIndex = (m_selectedIndex - 1 + 3) % 3;
                }
                break;

            case sf::Keyboard::Key::D:
            case sf::Keyboard::Key::Right:
                if (m_currentStep == 0) {
                    // Next background page
                    m_selectedIndex = (m_selectedIndex + 1) % 3;
                } else {
                    // Next car page
                    m_selectedIndex = (m_selectedIndex + 1) % 3;
                }
                break;

            // W/S still work as alternative navigation
            case sf::Keyboard::Key::Up:
            case sf::Keyboard::Key::W:
                if (m_currentStep == 0) {
                    m_selectedIndex = (m_selectedIndex - 1 + 3) % 3;
                } else {
                    m_selectedIndex = (m_selectedIndex - 1 + 3) % 3;
                }
                break;

            case sf::Keyboard::Key::Down:
            case sf::Keyboard::Key::S:
                if (m_currentStep == 0) {
                    m_selectedIndex = (m_selectedIndex + 1) % 3;
                } else {
                    m_selectedIndex = (m_selectedIndex + 1) % 3;
                }
                break;

            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                if (m_currentStep == 0) {
                    // Confirm background selection
                    m_selectedBackground = m_selectedIndex;
                    m_currentStep = 1;
                    m_selectedIndex = 0;

                    // Update menu items for car selection
                    m_menuItems = {
                        "1. BMW E30 Coupe",
                        "   Sports coupe, fast, 3 seats, 32 slots",
                        "   ",
                        "2. Volvo 940 Sedan",
                        "   Reliable sedan, balanced, 4 seats, 64 slots",
                        "   ",
                        "3. Mercedes-Benz Sprinter",
                        "   Cargo van, huge trunk, 2 seats, 124 slots"
                    };
                    
                    // Reset typewriter for new step
                    m_displayedChars.clear();
                    m_displayedChars.resize(m_menuItems.size(), 0);
                    m_typewriterTimer = 0.0f;
                    m_typewriterComplete = false;
                } else {
                    // Confirm car selection
                    m_selectedCar = m_selectedIndex;
                    m_isComplete = true;
                }
                break;

            case sf::Keyboard::Key::Escape:
                if (m_currentStep == 1) {
                    // Go back to background selection
                    m_currentStep = 0;
                    m_selectedIndex = m_selectedBackground;
                    m_selectedBackground = -1;

                    // Restore background menu
                    m_menuItems = {
                        "1. Ex-Racer",
                        "   Bonus: 16% chance to skip events",
                        "   ",
                        "2. Student",
                        "   Bonus: Companions get joy effect",
                        "   ",
                        "3. Unemployed",
                        "   Feature: Hard mode, 2 unique endings"
                    };
                    
                    m_displayedChars.clear();
                    m_displayedChars.resize(m_menuItems.size(), 0);
                    m_typewriterTimer = 0.0f;
                    m_typewriterComplete = false;
                } else {
                    // Go back to main menu
                    m_goBack = true;
                }
                break;
                
            default:
                break;
        }
    }
}

// Update
void CharacterCreationMenu::update(float deltaTime) {
    updateTypewriter(deltaTime);
    updateArrow(deltaTime);
}

// Update typewriter effect
void CharacterCreationMenu::updateTypewriter(float deltaTime) {
    if (m_typewriterComplete) {
        return;
    }
    
    m_typewriterTimer += deltaTime;
    
    if (m_typewriterTimer >= TYPEWRITER_SPEED) {
        m_typewriterTimer = 0.0f;
        
        // Add one character to display
        bool allComplete = true;
        for (size_t i = 0; i < m_menuItems.size(); ++i) {
            if (m_displayedChars[i] < static_cast<int>(m_menuItems[i].length())) {
                m_displayedChars[i]++;
                allComplete = false;
                break; // One character at a time
            }
        }
        
        m_typewriterComplete = allComplete;
    }
}

// Update arrow blinking
void CharacterCreationMenu::updateArrow(float deltaTime) {
    m_arrowTimer += deltaTime;
    
    if (m_arrowTimer >= ARROW_BLINK_SPEED) {
        m_arrowTimer = 0.0f;
        m_arrowVisible = !m_arrowVisible;
    }
}

// Render
void CharacterCreationMenu::render(sf::RenderWindow& window) {
    // Clear with dark terminal background
    window.clear(sf::Color(10, 10, 15));
    
    drawFrame(window);
    drawHeader(window);
    drawMenuItems(window);
    drawArrow(window);
}

// Draw frame
void CharacterCreationMenu::drawFrame(sf::RenderWindow& window) {
    // Fixed sizing for 1440x900
    sf::RectangleShape frame;
    frame.setSize(sf::Vector2f(FRAME_WIDTH, FRAME_HEIGHT));
    frame.setPosition(sf::Vector2f(FRAME_X, FRAME_Y));
    frame.setFillColor(sf::Color(20, 20, 30, 220));
    frame.setOutlineColor(sf::Color(0, 255, 0, 180)); // Green terminal border
    frame.setOutlineThickness(3.0f);

    window.draw(frame);
}

// Draw header
void CharacterCreationMenu::drawHeader(sf::RenderWindow& window) {
    std::string headerText;
    if (m_currentStep == 0) {
        headerText = "SELECT BACKGROUND";
    } else {
        headerText = "SELECT VEHICLE";
    }

    // SFML 3.x: Text requires font in constructor
    sf::Text header(m_font, headerText, 16);
    header.setFillColor(sf::Color(255, 200, 0)); // Amber/Orange
    header.setPosition(sf::Vector2f(FRAME_X + 280.0f, HEADER_Y));  // Centered
    // Note: setLineSpacing() removed in SFML 3.x

    window.draw(header);

    // Draw page indicator (e.g., "1/3", "2/3", "3/3")
    std::string pageIndicator = std::to_string(m_selectedIndex + 1) + "/3";
    sf::Text pageText(m_font, pageIndicator, 14);
    pageText.setFillColor(sf::Color(100, 255, 100)); // Light green
    pageText.setPosition(sf::Vector2f(FRAME_X + FRAME_WIDTH - 100.0f, HEADER_Y));
    window.draw(pageText);

    // Draw navigation hint
    sf::Text navHint(m_font, "[A/D] Page  [Enter] Select  [Esc] Back", 12);
    navHint.setFillColor(sf::Color(150, 150, 150)); // Gray
    navHint.setPosition(sf::Vector2f(FRAME_X + 120.0f, FRAME_Y + FRAME_HEIGHT - 40.0f));
    window.draw(navHint);
}

// Draw menu items with typewriter effect - shows only current page
void CharacterCreationMenu::drawMenuItems(sf::RenderWindow& window) {
    float yPos = MENU_START_Y;

    // Calculate which lines to show (current page only)
    // Each option occupies 3 lines: title, description, blank
    int startLine = m_selectedIndex * 3;
    int endLine = startLine + 3;

    for (int i = startLine; i < endLine && i < static_cast<int>(m_menuItems.size()); ++i) {
        // Apply typewriter effect
        std::string displayText = m_menuItems[i].substr(0, m_displayedChars[i]);

        // SFML 3.x: Text requires font in constructor
        unsigned int fontSize = (i == startLine) ? 18 : 14; // Larger for title
        sf::Text text(m_font, displayText, fontSize);

        // Title line gets brighter color
        if (i == startLine) {
            text.setFillColor(sf::Color(0, 255, 100)); // Bright green for title
        } else {
            text.setFillColor(sf::Color(0, 200, 0)); // Dimmer green for description
        }

        text.setPosition(sf::Vector2f(FRAME_X + 150.0f, yPos));

        window.draw(text);
        yPos += 40.0f; // Line spacing
    }
}

// Draw blinking arrow selector (now shows on current page)
void CharacterCreationMenu::drawArrow(sf::RenderWindow& window) {
    if (!m_arrowVisible) {
        return;
    }

    // Arrow position for current page display
    float yPos = MENU_START_Y;

    // SFML 3.x: Text requires font in constructor
    sf::Text arrow(m_font, ">", 20);
    arrow.setFillColor(sf::Color(255, 150, 0)); // Orange arrow
    arrow.setPosition(sf::Vector2f(FRAME_X + 100.0f, yPos));

    window.draw(arrow);
}
