#include "MainMenuScene.h"
#include "GameStateManager.h"
#include <iostream>

// Constructor
MainMenuScene::MainMenuScene() 
    : m_selectedIndex(0),
      m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU),
      m_fontLoaded(false) {
    
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("MainMenuScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Try to load font
    if (!m_font.loadFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        m_fontLoaded = false;
    } else {
        m_fontLoaded = true;
    }
    
    // Setup title text with responsive scaling
    m_titleText.setFont(m_font);
    m_titleText.setString("69 CROSSROADS");
    m_titleText.setCharacterSize(SCALE_FONT(layout, 32));
    m_titleText.setFillColor(sf::Color(255, 200, 0)); // Orange/Amber
    m_titleText.setPosition(SCALE_POS(layout, 400.0f, 150.0f));
    
    // Setup menu items with responsive scaling
    m_newGameText.setFont(m_font);
    m_newGameText.setString("NEW GAME");
    m_newGameText.setCharacterSize(SCALE_FONT(layout, 20));
    m_newGameText.setFillColor(sf::Color(0, 255, 0)); // Green terminal style
    m_newGameText.setPosition(SCALE_POS(layout, 500.0f, 300.0f));
    
    m_continueText.setFont(m_font);
    m_continueText.setString("CONTINUE");
    m_continueText.setCharacterSize(SCALE_FONT(layout, 20));
    // Check if save exists to enable/disable Continue
    bool hasSave = GameStateManager::getInstance().hasSaveGame();
    m_continueText.setFillColor(hasSave ? sf::Color(0, 255, 0) : sf::Color(100, 100, 100)); // Gray if disabled
    m_continueText.setPosition(SCALE_POS(layout, 500.0f, 360.0f));
    
    m_exitText.setFont(m_font);
    m_exitText.setString("EXIT");
    m_exitText.setCharacterSize(SCALE_FONT(layout, 20));
    m_exitText.setFillColor(sf::Color(0, 255, 0));
    m_exitText.setPosition(SCALE_POS(layout, 500.0f, 420.0f));
    
    // Setup background boxes with responsive scaling
    m_titleBox.setSize(SCALE_SIZE(layout, 400, 60));
    m_titleBox.setPosition(SCALE_POS(layout, 380, 140));
    m_titleBox.setFillColor(sf::Color(20, 20, 30, 200));
    m_titleBox.setOutlineColor(sf::Color(0, 255, 0));
    m_titleBox.setOutlineThickness(2);
    
    // Setup selector (orange blinking arrow) with responsive scaling
    m_selectorBox.setSize(SCALE_SIZE(layout, 30, 30));
    m_selectorBox.setFillColor(sf::Color(255, 150, 0)); // Orange arrow
    m_selectorBox.setPosition(SCALE_POS(layout, 450.0f, 300.0f));
}

// Handle input
void MainMenuScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                m_selectedIndex = (m_selectedIndex - 1 + 3) % 3;
                break;
                
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                m_selectedIndex = (m_selectedIndex + 1) % 3;
                break;
                
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                // Handle selection
                switch (m_selectedIndex) {
                    case 0: // New Game
                        m_nextScene = SceneType::CHARACTER_CREATION;
                        m_isFinished = true;
                        break;
                    case 1: // Continue
                        // Only allow continue if save exists
                        if (GameStateManager::getInstance().hasSaveGame()) {
                            // Load the game
                            if (GameStateManager::getInstance().loadGame()) {
                                // After loading, go to the node scene (or appropriate scene)
                                m_nextScene = SceneType::NODE;
                                m_isFinished = true;
                            } else {
                                std::cerr << "Failed to load game, staying on main menu" << std::endl;
                            }
                        } else {
                            std::cout << "No save file found, cannot continue" << std::endl;
                        }
                        break;
                    case 2: // Exit
                        m_nextScene = SceneType::EXIT;
                        m_isFinished = true;
                        break;
                }
                break;
                
            case sf::Keyboard::Escape:
                m_nextScene = SceneType::EXIT;
                m_isFinished = true;
                break;
                
            default:
                break;
        }
    }
}

// Update
void MainMenuScene::update(float deltaTime) {
    // Получение адаптивной разметки для обновления позиций
    // Get responsive layout for updating positions
    UILayout& layout = GET_LAYOUT("MainMenuScene");
    
    // Update selector position based on selected index with responsive scaling
    float yPositions[] = {300.0f, 360.0f, 420.0f};
    sf::Vector2f selectorPos = SCALE_POS(layout, 450.0f, yPositions[m_selectedIndex]);
    m_selectorBox.setPosition(selectorPos);
    
    // Simple blinking effect for selector (can be enhanced)
    static float blinkTimer = 0.0f;
    blinkTimer += deltaTime;
    if (blinkTimer > 0.5f) {
        blinkTimer = 0.0f;
        // Toggle visibility by changing alpha
        sf::Color color = m_selectorBox.getFillColor();
        color.a = (color.a == 255) ? 100 : 255;
        m_selectorBox.setFillColor(color);
    }
    
    // Check if save exists and update Continue text color
    bool hasSave = GameStateManager::getInstance().hasSaveGame();
    
    // Highlight selected item
    m_newGameText.setFillColor((m_selectedIndex == 0) ? sf::Color(255, 255, 0) : sf::Color(0, 255, 0));
    // Continue is yellow if selected AND save exists, gray if no save, green otherwise
    if (m_selectedIndex == 1) {
        m_continueText.setFillColor(hasSave ? sf::Color(255, 255, 0) : sf::Color(100, 100, 100));
    } else {
        m_continueText.setFillColor(hasSave ? sf::Color(0, 255, 0) : sf::Color(100, 100, 100));
    }
    m_exitText.setFillColor((m_selectedIndex == 2) ? sf::Color(255, 255, 0) : sf::Color(0, 255, 0));
}

// Render
void MainMenuScene::render(sf::RenderWindow& window) {
    // Clear with dark background
    window.clear(sf::Color(10, 10, 15));
    
    if (!m_fontLoaded) {
        return;
    }
    
    // Draw title box and text
    window.draw(m_titleBox);
    window.draw(m_titleText);
    
    // Draw menu items
    window.draw(m_newGameText);
    window.draw(m_continueText);
    window.draw(m_exitText);
    
    // Draw selector (blinking arrow)
    window.draw(m_selectorBox);
}

// Get next scene
SceneType MainMenuScene::getNextScene() const {
    return m_nextScene;
}

// Check if finished
bool MainMenuScene::isFinished() const {
    return m_isFinished;
}
