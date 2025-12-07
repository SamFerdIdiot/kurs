#include "MainMenuScene.h"
#include "GameStateManager.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include <iostream>

// Constructor
MainMenuScene::MainMenuScene()
    : m_selectedIndex(0),
      m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU),
      m_fontLoaded(false) {

    // Try to load font using FontLoader helper
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "Failed to load any font" << std::endl;
        m_fontLoaded = false;
        return;
    }

    // Setup title text with fixed dimensions (using emplace for SFML 3.x)
    m_titleText.emplace(m_font, "69 Crossroads", UI::FONT_SIZE_HUGE);
    m_titleText->setFillColor(UI::Color::ACCENT_YELLOW);
    m_titleText->setPosition(sf::Vector2f(480.0f, 150.0f));

    // Setup menu items with fixed positions
    m_newGameText.emplace(m_font, "NEW GAME", UI::FONT_SIZE_SUBTITLE);
    m_newGameText->setFillColor(UI::Color::ACCENT_GREEN);
    m_newGameText->setPosition(sf::Vector2f(550.0f, 350.0f));

    // Check if save exists to enable/disable Continue
    bool hasSave = GameStateManager::getInstance().hasSaveGame();
    m_continueText.emplace(m_font, "CONTINUE", UI::FONT_SIZE_SUBTITLE);
    m_continueText->setFillColor(hasSave ? UI::Color::ACCENT_GREEN : UI::Color::TEXT_SECONDARY);
    m_continueText->setPosition(sf::Vector2f(550.0f, 420.0f));

    m_journalText.emplace(m_font, "JOURNAL (TEST)", UI::FONT_SIZE_SUBTITLE);
    m_journalText->setFillColor(UI::Color::ACCENT_GREEN);
    m_journalText->setPosition(sf::Vector2f(550.0f, 490.0f));

    m_exitText.emplace(m_font, "EXIT", UI::FONT_SIZE_SUBTITLE);
    m_exitText->setFillColor(UI::Color::ACCENT_GREEN);
    m_exitText->setPosition(sf::Vector2f(550.0f, 560.0f));

    // Setup background boxes with fixed dimensions
    m_titleBox.setSize(sf::Vector2f(600.0f, 80.0f));
    m_titleBox.setPosition(sf::Vector2f(420.0f, 140.0f));
    m_titleBox.setFillColor(UI::Color::BACKGROUND_DARK);
    m_titleBox.setOutlineColor(UI::Color::ACCENT_GREEN);
    m_titleBox.setOutlineThickness(2.0f);

    // Setup selector (orange blinking arrow)
    m_selectorBox.setSize(sf::Vector2f(30.0f, 30.0f));
    m_selectorBox.setFillColor(UI::Color::ACCENT_YELLOW);
    m_selectorBox.setPosition(sf::Vector2f(500.0f, 350.0f));
}

// Handle input
void MainMenuScene::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Up:
            case sf::Keyboard::Key::W:
                m_selectedIndex = (m_selectedIndex - 1 + 4) % 4;
                break;

            case sf::Keyboard::Key::Down:
            case sf::Keyboard::Key::S:
                m_selectedIndex = (m_selectedIndex + 1) % 4;
                break;

            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                // Handle selection
                switch (m_selectedIndex) {
                    case 0: // New Game - skip character creation, go straight to travel
                        // [MVP] Character creation removed - start directly at Moscow
                        GameStateManager::getInstance().getPlayerState().setCurrentCityIndex(0);
                        m_nextScene = SceneType::TRAVEL_SELECTION;
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
                    case 2: // Journal (Test)
                        m_nextScene = SceneType::JOURNAL;
                        m_isFinished = true;
                        break;
                    case 3: // Exit
                        m_nextScene = SceneType::EXIT;
                        m_isFinished = true;
                        break;
                }
                break;

            case sf::Keyboard::Key::Escape:
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
    // Update selector position based on selected index with fixed positions
    float yPositions[] = {350.0f, 420.0f, 490.0f, 560.0f};
    m_selectorBox.setPosition(sf::Vector2f(500.0f, yPositions[m_selectedIndex]));

    // Simple blinking effect for selector
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

    // Highlight selected item (using optional -> operator)
    if (m_newGameText) {
        m_newGameText->setFillColor((m_selectedIndex == 0) ? UI::Color::ACCENT_YELLOW : UI::Color::ACCENT_GREEN);
    }
    // Continue is yellow if selected AND save exists, gray if no save, green otherwise
    if (m_continueText) {
        if (m_selectedIndex == 1) {
            m_continueText->setFillColor(hasSave ? UI::Color::ACCENT_YELLOW : UI::Color::TEXT_SECONDARY);
        } else {
            m_continueText->setFillColor(hasSave ? UI::Color::ACCENT_GREEN : UI::Color::TEXT_SECONDARY);
        }
    }
    if (m_journalText) {
        m_journalText->setFillColor((m_selectedIndex == 2) ? UI::Color::ACCENT_YELLOW : UI::Color::ACCENT_GREEN);
    }
    if (m_exitText) {
        m_exitText->setFillColor((m_selectedIndex == 3) ? UI::Color::ACCENT_YELLOW : UI::Color::ACCENT_GREEN);
    }
}

// Render
void MainMenuScene::render(sf::RenderWindow& window) {
    // Clear with dark background
    window.clear(UI::Color::BACKGROUND_DARK);

    if (!m_fontLoaded) {
        return;
    }

    // Draw title box and text
    window.draw(m_titleBox);
    if (m_titleText) {
        window.draw(*m_titleText);
    }

    // Draw menu items
    if (m_newGameText) {
        window.draw(*m_newGameText);
    }
    if (m_continueText) {
        window.draw(*m_continueText);
    }
    if (m_journalText) {
        window.draw(*m_journalText);
    }
    if (m_exitText) {
        window.draw(*m_exitText);
    }

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
