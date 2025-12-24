#include "MainMenuScene.h"
#include "GameStateManager.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include <iostream>


MainMenuScene::MainMenuScene()
    : m_selectedIndex(0),
      m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU),
      m_fontLoaded(false) {


    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "Failed to load any font" << std::endl;
        m_fontLoaded = false;
        return;
    }


    m_titleText.emplace(m_font, "69 Crossroads", UI::FONT_SIZE_HUGE);
    m_titleText->setFillColor(UI::Color::ACCENT_YELLOW);
    m_titleText->setPosition(sf::Vector2f(480.0f, 150.0f));


    m_newGameText.emplace(m_font, "NEW GAME", UI::FONT_SIZE_SUBTITLE);
    m_newGameText->setFillColor(UI::Color::ACCENT_GREEN);
    m_newGameText->setPosition(sf::Vector2f(550.0f, 350.0f));


    bool hasSave = GameStateManager::getInstance().hasSaveGame();
    m_continueText.emplace(m_font, "CONTINUE", UI::FONT_SIZE_SUBTITLE);
    m_continueText->setFillColor(hasSave ? UI::Color::ACCENT_GREEN : UI::Color::TEXT_SECONDARY);
    m_continueText->setPosition(sf::Vector2f(550.0f, 420.0f));

    m_journalText.emplace(m_font, "TESTS", UI::FONT_SIZE_SUBTITLE);
    m_journalText->setFillColor(UI::Color::ACCENT_GREEN);
    m_journalText->setPosition(sf::Vector2f(550.0f, 490.0f));

    m_exitText.emplace(m_font, "EXIT", UI::FONT_SIZE_SUBTITLE);
    m_exitText->setFillColor(UI::Color::ACCENT_GREEN);
    m_exitText->setPosition(sf::Vector2f(550.0f, 560.0f));


    m_titleBox.setSize(sf::Vector2f(600.0f, 80.0f));
    m_titleBox.setPosition(sf::Vector2f(420.0f, 140.0f));
    m_titleBox.setFillColor(UI::Color::BACKGROUND_DARK);
    m_titleBox.setOutlineColor(UI::Color::ACCENT_GREEN);
    m_titleBox.setOutlineThickness(2.0f);


    m_selectorBox.setSize(sf::Vector2f(30.0f, 30.0f));
    m_selectorBox.setFillColor(UI::Color::ACCENT_YELLOW);
    m_selectorBox.setPosition(sf::Vector2f(500.0f, 350.0f));
}


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

                switch (m_selectedIndex) {
                    case 0:


                        GameStateManager::getInstance().getPlayerState().setCurrentCityIndex(0);
                        GameStateManager::getInstance().getPlayerState().setCurrentNotebookEntryId("demo_start");
                        m_nextScene = SceneType::NOTEBOOK;
                        m_isFinished = true;
                        break;
                    case 1:

                        if (GameStateManager::getInstance().hasSaveGame()) {

                            if (GameStateManager::getInstance().loadGame()) {

                                m_nextScene = SceneType::NOTEBOOK;
                                m_isFinished = true;
                            } else {
                                std::cerr << "Failed to load game, staying on main menu" << std::endl;
                            }
                        } else {
                            std::cout << "No save file found, cannot continue" << std::endl;
                        }
                        break;
                    case 2:

                        GameStateManager::getInstance().getPlayerState().setCurrentNotebookEntryId("test_thought_system");
                        m_nextScene = SceneType::NOTEBOOK;
                        m_isFinished = true;
                        break;
                    case 3:
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


void MainMenuScene::update(float deltaTime) {

    float yPositions[] = {350.0f, 420.0f, 490.0f, 560.0f};
    m_selectorBox.setPosition(sf::Vector2f(500.0f, yPositions[m_selectedIndex]));


    static float blinkTimer = 0.0f;
    blinkTimer += deltaTime;
    if (blinkTimer > 0.5f) {
        blinkTimer = 0.0f;

        sf::Color color = m_selectorBox.getFillColor();
        color.a = (color.a == 255) ? 100 : 255;
        m_selectorBox.setFillColor(color);
    }


    bool hasSave = GameStateManager::getInstance().hasSaveGame();


    if (m_newGameText) {
        m_newGameText->setFillColor((m_selectedIndex == 0) ? UI::Color::ACCENT_YELLOW : UI::Color::ACCENT_GREEN);
    }

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


void MainMenuScene::render(sf::RenderWindow& window) {

    window.clear(UI::Color::BACKGROUND_DARK);

    if (!m_fontLoaded) {
        return;
    }


    window.draw(m_titleBox);
    if (m_titleText) {
        window.draw(*m_titleText);
    }


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


    window.draw(m_selectorBox);
}


SceneType MainMenuScene::getNextScene() const {
    return m_nextScene;
}


bool MainMenuScene::isFinished() const {
    return m_isFinished;
}
