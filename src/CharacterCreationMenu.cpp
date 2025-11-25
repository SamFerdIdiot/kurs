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
    if (!m_font.loadFromFile(fontPath)) {
        std::cerr << "Failed to load font: " << fontPath << std::endl;
        return false;
    }
    
    // Setup initial menu items (backgrounds) with Russian descriptions
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
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                if (m_currentStep == 0) {
                    // Background selection (0, 1, 2)
                    m_selectedIndex = (m_selectedIndex - 1 + 3) % 3;
                } else {
                    // Car selection (0, 1, 2)
                    m_selectedIndex = (m_selectedIndex - 1 + 3) % 3;
                }
                break;
                
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                if (m_currentStep == 0) {
                    m_selectedIndex = (m_selectedIndex + 1) % 3;
                } else {
                    m_selectedIndex = (m_selectedIndex + 1) % 3;
                }
                break;
                
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                if (m_currentStep == 0) {
                    // Confirm background selection
                    m_selectedBackground = m_selectedIndex;
                    m_currentStep = 1;
                    m_selectedIndex = 0;
                    
                    // Update menu items for car selection (from CAR_SPECIFICATIONS.md)
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
                
            case sf::Keyboard::Escape:
                if (m_currentStep == 1) {
                    // Go back to background selection
                    m_currentStep = 0;
                    m_selectedIndex = m_selectedBackground;
                    m_selectedBackground = -1;
                    
                    // Restore background menu
                    m_menuItems = {
                        "1. Бывший гонщик (Ex-Racer)",
                        "   Бонус: 16% шанс пропустить событие",
                        "   ",
                        "2. Студент (Student)",
                        "   Бонус: Попутчики получают эффект радости",
                        "   ",
                        "3. Безработный (Unemployed)",
                        "   Особенность: Хард режим, 2 уникальные концовки"
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
    // Получение адаптивной разметки для MacBook Air M1
    // Get responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("CharacterCreationMenu");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    sf::RectangleShape frame;
    frame.setSize(SCALE_SIZE(layout, FRAME_WIDTH, FRAME_HEIGHT));
    frame.setPosition(SCALE_POS(layout, FRAME_X, FRAME_Y));
    frame.setFillColor(sf::Color(20, 20, 30, 220));
    frame.setOutlineColor(sf::Color(0, 255, 0, 180)); // Green terminal border
    frame.setOutlineThickness(3);
    
    window.draw(frame);
}

// Draw header
void CharacterCreationMenu::drawHeader(sf::RenderWindow& window) {
    // Получение адаптивной разметки
    // Get responsive layout
    UILayout& layout = GET_LAYOUT("CharacterCreationMenu");
    
    sf::Text header;
    header.setFont(m_font);
    
    if (m_currentStep == 0) {
        // Split into two lines to prevent overflow
        header.setString("ВЫБОР ПРЕДЫСТОРИИ\nSELECT BACKGROUND");
    } else {
        header.setString("ВЫБОР АВТОМОБИЛЯ\nSELECT VEHICLE");
    }
    
    header.setCharacterSize(SCALE_FONT(layout, 16));  // Slightly smaller font size
    header.setFillColor(sf::Color(255, 200, 0)); // Amber/Orange
    header.setPosition(SCALE_POS(layout, FRAME_X + 280, HEADER_Y));  // More centered
    header.setLineSpacing(1.2f);
    
    window.draw(header);
}

// Draw menu items with typewriter effect
void CharacterCreationMenu::drawMenuItems(sf::RenderWindow& window) {
    // Получение адаптивной разметки
    // Get responsive layout
    UILayout& layout = GET_LAYOUT("CharacterCreationMenu");
    
    float yPos = MENU_START_Y;
    
    for (size_t i = 0; i < m_menuItems.size(); ++i) {
        sf::Text text;
        text.setFont(m_font);
        
        // Apply typewriter effect
        std::string displayText = m_menuItems[i].substr(0, m_displayedChars[i]);
        text.setString(displayText);
        
        text.setCharacterSize(SCALE_FONT(layout, 14));
        text.setFillColor(sf::Color(0, 255, 0)); // Green terminal text
        text.setPosition(SCALE_POS(layout, FRAME_X + 100, yPos));
        
        window.draw(text);
        yPos += 30; // Line spacing (можно тоже масштабировать, но для простоты оставим фиксированным)
    }
}

// Draw blinking arrow selector
void CharacterCreationMenu::drawArrow(sf::RenderWindow& window) {
    if (!m_arrowVisible) {
        return;
    }
    
    // Получение адаптивной разметки
    // Get responsive layout
    UILayout& layout = GET_LAYOUT("CharacterCreationMenu");
    
    // Calculate arrow position based on selected index
    // Items 0, 3, 6 are the main options (others are descriptions)
    int visualIndex = m_selectedIndex * 3;
    float yPos = MENU_START_Y + (visualIndex * 30);
    
    sf::Text arrow;
    arrow.setFont(m_font);
    arrow.setString(">");
    arrow.setCharacterSize(SCALE_FONT(layout, 18));
    arrow.setFillColor(sf::Color(255, 150, 0)); // Orange arrow
    arrow.setPosition(SCALE_POS(layout, FRAME_X + 60, yPos));
    
    window.draw(arrow);
}
