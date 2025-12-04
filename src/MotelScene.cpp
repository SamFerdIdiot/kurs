#include "MotelScene.h"
#include "FontLoader.h"
#include "UIConstants.h"
#include <iostream>

MotelScene::MotelScene()
    : m_isFinished(false),
      m_nextScene(SceneType::NODE),
      m_selectedOption(RestOption::QUICK_REST),
      m_fontLoaded(false) {
    setupUI();
}

void MotelScene::setupUI() {
    // Load font
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "MotelScene: Failed to load font" << std::endl;
    }

    // Background
    m_background.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    m_background.setFillColor(sf::Color(20, 20, 30));

    if (m_fontLoaded) {
        // Title
        m_titleText.emplace(m_font, "MOTEL - Rest Here", UI::FONT_SIZE_HUGE);
        m_titleText->setFillColor(UI::Color::TEXT_PRIMARY);
        m_titleText->setPosition(sf::Vector2f(400.0f, 100.0f));

        // Instructions
        m_instructionsText.emplace(m_font, "[W/S] Navigate  [Enter] Select  [Esc] Leave", UI::FONT_SIZE_SMALL);
        m_instructionsText->setFillColor(UI::Color::TEXT_SECONDARY);
        m_instructionsText->setPosition(sf::Vector2f(400.0f, UI::SCREEN_HEIGHT - 80.0f));

        // Status
        m_statusText.emplace(m_font, "", UI::FONT_SIZE_NORMAL);
        m_statusText->setFillColor(UI::Color::TEXT_PRIMARY);
        m_statusText->setPosition(sf::Vector2f(400.0f, 200.0f));
    }
}

void MotelScene::handleInput(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::W:
            case sf::Keyboard::Key::Up:
                if (m_selectedOption == RestOption::FULL_REST) {
                    m_selectedOption = RestOption::QUICK_REST;
                } else if (m_selectedOption == RestOption::LEAVE) {
                    m_selectedOption = RestOption::FULL_REST;
                }
                break;

            case sf::Keyboard::Key::S:
            case sf::Keyboard::Key::Down:
                if (m_selectedOption == RestOption::QUICK_REST) {
                    m_selectedOption = RestOption::FULL_REST;
                } else if (m_selectedOption == RestOption::FULL_REST) {
                    m_selectedOption = RestOption::LEAVE;
                }
                break;

            case sf::Keyboard::Key::Enter:
            case sf::Keyboard::Key::Space:
                performRest(m_selectedOption);
                break;

            case sf::Keyboard::Key::Escape:
                m_isFinished = true;
                m_nextScene = SceneType::NODE;
                break;

            default:
                break;
        }
    }
}

void MotelScene::update(float deltaTime) {
    // Update status display
    if (m_fontLoaded && m_statusText) {
        PlayerState& player = GameStateManager::getInstance().getPlayerState();

        std::string status = "Current Status:\n";
        status += "Money: $" + std::to_string(static_cast<int>(player.getMoney())) + "\n";
        status += "Energy: " + std::to_string(static_cast<int>(player.getEnergy())) + "%\n";

        m_statusText->setString(status);
    }
}

void MotelScene::render(sf::RenderWindow& window) {
    window.draw(m_background);

    if (m_fontLoaded) {
        if (m_titleText) window.draw(*m_titleText);
        if (m_statusText) window.draw(*m_statusText);
        if (m_instructionsText) window.draw(*m_instructionsText);
    }

    renderMenu(window);
}

void MotelScene::renderMenu(sf::RenderWindow& window) {
    if (!m_fontLoaded) return;

    PlayerState& player = GameStateManager::getInstance().getPlayerState();
    float yPos = 350.0f;

    // Quick Rest option
    {
        std::string text = (m_selectedOption == RestOption::QUICK_REST ? "> " : "  ");
        text += "[1] Quick Rest - $20";

        sf::Text optionText(m_font, text, UI::FONT_SIZE_SUBTITLE);
        optionText.setFillColor(m_selectedOption == RestOption::QUICK_REST ?
            UI::Color::ACCENT_YELLOW : UI::Color::TEXT_PRIMARY);
        optionText.setPosition(sf::Vector2f(450.0f, yPos));
        window.draw(optionText);

        sf::Text descText(m_font, "Restore 50% energy (30 minutes)", UI::FONT_SIZE_SMALL);
        descText.setFillColor(UI::Color::TEXT_SECONDARY);
        descText.setPosition(sf::Vector2f(480.0f, yPos + 30.0f));
        window.draw(descText);

        // Check if affordable
        if (player.getMoney() < 20.0f) {
            sf::Text warningText(m_font, "Not enough money!", UI::FONT_SIZE_SMALL);
            warningText.setFillColor(sf::Color(255, 0, 0));
            warningText.setPosition(sf::Vector2f(480.0f, yPos + 55.0f));
            window.draw(warningText);
        }
    }

    yPos += 100.0f;

    // Full Rest option
    {
        std::string text = (m_selectedOption == RestOption::FULL_REST ? "> " : "  ");
        text += "[2] Full Rest - $40";

        sf::Text optionText(m_font, text, UI::FONT_SIZE_SUBTITLE);
        optionText.setFillColor(m_selectedOption == RestOption::FULL_REST ?
            UI::Color::ACCENT_YELLOW : UI::Color::TEXT_PRIMARY);
        optionText.setPosition(sf::Vector2f(450.0f, yPos));
        window.draw(optionText);

        sf::Text descText(m_font, "Restore 100% energy + Auto-save (8 hours)", UI::FONT_SIZE_SMALL);
        descText.setFillColor(UI::Color::TEXT_SECONDARY);
        descText.setPosition(sf::Vector2f(480.0f, yPos + 30.0f));
        window.draw(descText);

        // Check if affordable
        if (player.getMoney() < 40.0f) {
            sf::Text warningText(m_font, "Not enough money!", UI::FONT_SIZE_SMALL);
            warningText.setFillColor(sf::Color(255, 0, 0));
            warningText.setPosition(sf::Vector2f(480.0f, yPos + 55.0f));
            window.draw(warningText);
        }
    }

    yPos += 100.0f;

    // Leave option
    {
        std::string text = (m_selectedOption == RestOption::LEAVE ? "> " : "  ");
        text += "[3] Leave";

        sf::Text optionText(m_font, text, UI::FONT_SIZE_SUBTITLE);
        optionText.setFillColor(m_selectedOption == RestOption::LEAVE ?
            UI::Color::ACCENT_YELLOW : UI::Color::TEXT_PRIMARY);
        optionText.setPosition(sf::Vector2f(450.0f, yPos));
        window.draw(optionText);
    }
}

void MotelScene::performRest(RestOption option) {
    PlayerState& player = GameStateManager::getInstance().getPlayerState();

    switch (option) {
        case RestOption::QUICK_REST: {
            if (player.getMoney() < 20.0f) {
                std::cout << "Not enough money for quick rest!" << std::endl;
                return;
            }

            player.addMoney(-20.0f);
            float currentEnergy = player.getEnergy();
            player.setEnergy(std::min(100.0f, currentEnergy + 50.0f));

            std::cout << "Quick rest complete: +50 energy" << std::endl;

            m_isFinished = true;
            m_nextScene = SceneType::NODE;
            break;
        }

        case RestOption::FULL_REST: {
            if (player.getMoney() < 40.0f) {
                std::cout << "Not enough money for full rest!" << std::endl;
                return;
            }

            player.addMoney(-40.0f);
            player.setEnergy(100.0f);

            // Note: Auto-save will be implemented in Phase 5.3
            std::cout << "Full rest complete: energy restored to 100%, game saved" << std::endl;

            m_isFinished = true;
            m_nextScene = SceneType::NODE;
            break;
        }

        case RestOption::LEAVE:
            m_isFinished = true;
            m_nextScene = SceneType::NODE;
            break;
    }
}

SceneType MotelScene::getNextScene() const {
    return m_nextScene;
}

bool MotelScene::isFinished() const {
    return m_isFinished;
}
