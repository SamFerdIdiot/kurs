#include "EndGameScene.h"
#include "UIConstants.h"
#include <sstream>
#include <iomanip>
#include <iostream>

EndGameScene::EndGameScene(float totalTime, int citiesVisited)
    : m_fontLoaded(false),
      m_totalTime(totalTime),
      m_citiesVisited(citiesVisited),
      m_menuButtonHovered(false),
      m_isFinished(false),
      m_nextScene(SceneType::MAIN_MENU),
      m_fadeAlpha(0.0f),
      m_animationTime(0.0f) {
    setupUI();
}

void EndGameScene::setupUI() {
    // Try to load font
    if (m_font.openFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "EndGameScene: WARNING - Could not load any font!" << std::endl;
    }

    // Background - gradient-like dark blue
    m_background.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    m_background.setFillColor(sf::Color(20, 30, 50));

    if (m_fontLoaded) {
        // Title text
        m_titleText.emplace(m_font, "", 48);
        m_titleText->setString("ПУТЕШЕСТВИЕ ЗАВЕРШЕНО!\nJOURNEY COMPLETE!");
        m_titleText->setFillColor(sf::Color(255, 215, 0)); // Gold
        m_titleText->setStyle(sf::Text::Bold);

        // Center title
        sf::FloatRect titleBounds = m_titleText->getGlobalBounds();
        m_titleText->setPosition(sf::Vector2f(
            UI::SCREEN_WIDTH / 2.0f - titleBounds.size.x / 2.0f,
            150.0f
        ));

        // Stats text
        std::stringstream stats;
        stats << "Пройдено городов: " << m_citiesVisited << " / 5\n\n";
        stats << "Время в пути: " << formatTime(m_totalTime) << "\n\n";
        stats << "Вы успешно пересекли всю страну!\n";
        stats << "От Москвы до Владивостока!\n\n";
        stats << "Спасибо за игру!";

        m_statsText.emplace(m_font, stats.str(), 24);
        m_statsText->setFillColor(sf::Color(200, 200, 200));

        // Center stats
        sf::FloatRect statsBounds = m_statsText->getGlobalBounds();
        m_statsText->setPosition(sf::Vector2f(
            UI::SCREEN_WIDTH / 2.0f - statsBounds.size.x / 2.0f,
            300.0f
        ));

        // Menu button
        m_menuButton.setSize(sf::Vector2f(400.0f, 70.0f));
        m_menuButton.setPosition(sf::Vector2f(
            UI::SCREEN_WIDTH / 2.0f - 200.0f,
            700.0f
        ));
        m_menuButton.setFillColor(sf::Color(60, 60, 80));
        m_menuButton.setOutlineColor(sf::Color(150, 150, 150));
        m_menuButton.setOutlineThickness(3.0f);

        // Menu button text
        m_menuButtonText.emplace(m_font, "Вернуться в меню\nReturn to Menu", 22);
        m_menuButtonText->setFillColor(sf::Color::White);

        sf::FloatRect buttonTextBounds = m_menuButtonText->getGlobalBounds();
        m_menuButtonText->setPosition(sf::Vector2f(
            UI::SCREEN_WIDTH / 2.0f - buttonTextBounds.size.x / 2.0f,
            715.0f
        ));
    }
}

void EndGameScene::handleInput(const sf::Event& event) {
    // Mouse movement for button hover
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x),
                             static_cast<float>(mouseMoved->position.y));

        m_menuButtonHovered = m_menuButton.getGlobalBounds().contains(mousePos);
    }

    // Mouse click
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            if (m_menuButtonHovered) {
                m_isFinished = true;
                m_nextScene = SceneType::MAIN_MENU;
            }
        }
    }

    // Keyboard shortcuts
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Enter ||
            keyPressed->code == sf::Keyboard::Key::Space ||
            keyPressed->code == sf::Keyboard::Key::Escape) {
            m_isFinished = true;
            m_nextScene = SceneType::MAIN_MENU;
        }
    }
}

void EndGameScene::update(float deltaTime) {
    m_animationTime += deltaTime;

    // Fade in animation
    if (m_fadeAlpha < 255.0f) {
        m_fadeAlpha = std::min(255.0f, m_fadeAlpha + deltaTime * 300.0f);
    }

    // Update button hover effect
    if (m_menuButtonHovered) {
        m_menuButton.setFillColor(sf::Color(80, 80, 100));
        m_menuButton.setOutlineColor(sf::Color(200, 200, 200));
    } else {
        m_menuButton.setFillColor(sf::Color(60, 60, 80));
        m_menuButton.setOutlineColor(sf::Color(150, 150, 150));
    }
}

void EndGameScene::render(sf::RenderWindow& window) {
    // Draw background
    window.draw(m_background);

    if (m_fontLoaded) {
        // Apply fade to all elements
        std::uint8_t alpha = static_cast<std::uint8_t>(m_fadeAlpha);

        // Draw title with fade
        if (m_titleText) {
            sf::Color titleColor = m_titleText->getFillColor();
            titleColor.a = alpha;
            m_titleText->setFillColor(titleColor);
            window.draw(*m_titleText);
        }

        // Draw stats with fade
        if (m_statsText) {
            sf::Color statsColor = m_statsText->getFillColor();
            statsColor.a = alpha;
            m_statsText->setFillColor(statsColor);
            window.draw(*m_statsText);
        }

        // Draw button
        sf::Color buttonColor = m_menuButton.getFillColor();
        buttonColor.a = alpha;
        m_menuButton.setFillColor(buttonColor);
        window.draw(m_menuButton);

        // Draw button text
        if (m_menuButtonText) {
            sf::Color buttonTextColor = m_menuButtonText->getFillColor();
            buttonTextColor.a = alpha;
            m_menuButtonText->setFillColor(buttonTextColor);
            window.draw(*m_menuButtonText);
        }
    }
}

SceneType EndGameScene::getNextScene() const {
    return m_nextScene;
}

bool EndGameScene::isFinished() const {
    return m_isFinished;
}

std::string EndGameScene::formatTime(float seconds) const {
    int hours = static_cast<int>(seconds) / 3600;
    int minutes = (static_cast<int>(seconds) % 3600) / 60;
    int secs = static_cast<int>(seconds) % 60;

    std::stringstream ss;
    if (hours > 0) {
        ss << hours << "ч " << minutes << "м " << secs << "с";
    } else if (minutes > 0) {
        ss << minutes << "м " << secs << "с";
    } else {
        ss << secs << "с";
    }

    return ss.str();
}
