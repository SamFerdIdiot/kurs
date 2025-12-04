#include "MapScene.h"
#include "UIConstants.h"
#include "FontLoader.h"
#include <iostream>

MapScene::MapScene()
    : m_mapOffset(0.0f, 0.0f),
      m_isDragging(false),
      m_lastMousePos(0, 0),
      m_finished(false),
      m_nextScene(SceneType::MAP),
      m_returnToScene(SceneType::NODE),
      m_fontLoaded(false) {

    // Load font
    if (auto fontOpt = FontLoader::load()) {
        m_font = std::move(*fontOpt);
        m_fontLoaded = true;
    } else {
        std::cerr << "MapScene: Failed to load font" << std::endl;
    }

    // Setup background
    m_background.setSize(sf::Vector2f(UI::SCREEN_WIDTH, UI::SCREEN_HEIGHT));
    m_background.setFillColor(UI::Color::BACKGROUND_DARK);

    // [MVP] Setup map placeholder (large draggable area representing world map)
    // In production, this would be replaced with actual sf::Texture/sf::Sprite
    m_mapPlaceholder.setSize(sf::Vector2f(2880.0f, 1800.0f));  // 2x screen size for panning demo
    m_mapPlaceholder.setPosition(sf::Vector2f(0.0f, 0.0f));
    m_mapPlaceholder.setFillColor(sf::Color(40, 40, 50));  // Dark gray placeholder
    m_mapPlaceholder.setOutlineColor(UI::Color::ACCENT_BLUE);
    m_mapPlaceholder.setOutlineThickness(3.0f);

    // SFML 3.x: Text initialization
    if (m_fontLoaded) {
        // Title
        m_titleText.emplace(m_font, "WORLD MAP", UI::FONT_SIZE_HUGE);
        m_titleText->setFillColor(UI::Color::ACCENT_YELLOW);
        m_titleText->setPosition(sf::Vector2f(50.0f, 30.0f));

        // Instructions
        m_instructionText.emplace(m_font, "[Mouse Drag] Pan Map  [Esc] Close", UI::FONT_SIZE_NORMAL);
        m_instructionText->setFillColor(UI::Color::TEXT_SECONDARY);
        m_instructionText->setPosition(sf::Vector2f(50.0f, 820.0f));

        // [MVP] Placeholder text in center of map
        m_placeholderText.emplace(m_font, "[MVP] World Map Image Goes Here\n\nDrag with mouse to pan", UI::FONT_SIZE_SUBTITLE);
        m_placeholderText->setFillColor(UI::Color::TEXT_SECONDARY);
        m_placeholderText->setPosition(sf::Vector2f(600.0f, 400.0f));
    }

    std::cout << "[MVP] MapScene created - static world map viewer" << std::endl;
}

void MapScene::handleInput(const sf::Event& event) {
    // ESC to close
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            m_finished = true;
            m_nextScene = m_returnToScene;
        }
    }

    // Mouse button pressed - start dragging
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            m_isDragging = true;
            m_lastMousePos = sf::Mouse::getPosition();
        }
    }

    // Mouse button released - stop dragging
    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            m_isDragging = false;
        }
    }

    // Mouse moved - update pan if dragging
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        if (m_isDragging) {
            sf::Vector2i currentMousePos = sf::Mouse::getPosition();
            sf::Vector2i delta = currentMousePos - m_lastMousePos;

            // Update map offset
            m_mapOffset.x += static_cast<float>(delta.x);
            m_mapOffset.y += static_cast<float>(delta.y);

            // Clamp offset to prevent scrolling too far
            // Map size: 2880x1800, Screen: 1440x900
            float minX = -(2880.0f - UI::SCREEN_WIDTH);
            float maxX = 0.0f;
            float minY = -(1800.0f - UI::SCREEN_HEIGHT);
            float maxY = 0.0f;

            if (m_mapOffset.x < minX) m_mapOffset.x = minX;
            if (m_mapOffset.x > maxX) m_mapOffset.x = maxX;
            if (m_mapOffset.y < minY) m_mapOffset.y = minY;
            if (m_mapOffset.y > maxY) m_mapOffset.y = maxY;

            m_lastMousePos = currentMousePos;
        }
    }
}

void MapScene::update(float deltaTime) {
    // Update map placeholder position based on offset
    m_mapPlaceholder.setPosition(m_mapOffset);

    // Update placeholder text position to follow map
    if (m_fontLoaded && m_placeholderText) {
        m_placeholderText->setPosition(sf::Vector2f(
            600.0f + m_mapOffset.x,
            400.0f + m_mapOffset.y
        ));
    }
}

void MapScene::render(sf::RenderWindow& window) {
    // Draw background
    window.draw(m_background);

    // Draw map placeholder (would be actual map texture in production)
    window.draw(m_mapPlaceholder);

    // Draw placeholder text
    if (m_fontLoaded && m_placeholderText) {
        window.draw(*m_placeholderText);
    }

    // Draw UI overlay (fixed position, not affected by pan)
    if (m_fontLoaded) {
        if (m_titleText) window.draw(*m_titleText);
        if (m_instructionText) window.draw(*m_instructionText);
    }
}

SceneType MapScene::getNextScene() const {
    return m_nextScene;
}

bool MapScene::isFinished() const {
    return m_finished;
}
