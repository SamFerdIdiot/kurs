#include "UI/ResourceBar.h"
#include <algorithm>
#include <cmath>

ResourceBar::ResourceBar(sf::Vector2f pos, sf::Vector2f size)
    : m_position(pos),
      m_size(size),
      m_currentValue(0.0f),
      m_maxValue(100.0f),
      m_displayValue(0.0f) {


    m_border.setSize(size);
    m_border.setPosition(pos);
    m_border.setFillColor(sf::Color::Transparent);
    m_border.setOutlineColor(getBorderColor());
    m_border.setOutlineThickness(BORDER_THICKNESS);


    m_background.setSize(sf::Vector2f(size.x - BORDER_THICKNESS * 2, size.y - BORDER_THICKNESS * 2));
    m_background.setPosition(sf::Vector2f(pos.x + BORDER_THICKNESS, pos.y + BORDER_THICKNESS));
    m_background.setFillColor(sf::Color(40, 40, 40, 200));


    m_fill.setSize(sf::Vector2f(0.0f, size.y - BORDER_THICKNESS * 2));
    m_fill.setPosition(sf::Vector2f(pos.x + BORDER_THICKNESS, pos.y + BORDER_THICKNESS));
    m_fill.setFillColor(getFillColor(0.0f));
}

void ResourceBar::setValue(float current, float max) {

    m_currentValue = std::max(0.0f, current);
    m_maxValue = std::max(1.0f, max);


    m_currentValue = std::min(m_currentValue, m_maxValue);
}

void ResourceBar::setPosition(sf::Vector2f pos) {
    m_position = pos;


    m_border.setPosition(pos);
    m_background.setPosition(sf::Vector2f(pos.x + BORDER_THICKNESS, pos.y + BORDER_THICKNESS));
    m_fill.setPosition(sf::Vector2f(pos.x + BORDER_THICKNESS, pos.y + BORDER_THICKNESS));
}

void ResourceBar::update(float deltaTime) {

    float targetValue = (m_maxValue > 0.0f) ? (m_currentValue / m_maxValue) * 100.0f : 0.0f;



    float lerpFactor = TRANSITION_SPEED * deltaTime;
    lerpFactor = std::min(lerpFactor, 1.0f);

    m_displayValue = m_displayValue + (targetValue - m_displayValue) * lerpFactor;


    updateVisuals();
}

void ResourceBar::draw(sf::RenderWindow& window) {

    window.draw(m_background);
    window.draw(m_fill);
    window.draw(m_border);
}

float ResourceBar::getPercentage() const {
    if (m_maxValue <= 0.0f) {
        return 0.0f;
    }
    return (m_currentValue / m_maxValue) * 100.0f;
}

sf::Color ResourceBar::getBorderColor() const {

    return sf::Color::White;
}

sf::Color ResourceBar::getFillColor(float percentage) const {



    if (percentage < 40.0f) {

        return sf::Color(255, 0, 0);
    } else if (percentage < 70.0f) {

        return sf::Color(255, 255, 0);
    } else {

        return sf::Color(0, 255, 0);
    }
}

void ResourceBar::updateVisuals() {

    float fillPercentage = m_displayValue / 100.0f;
    fillPercentage = std::max(0.0f, std::min(fillPercentage, 1.0f));

    float maxFillWidth = m_size.x - BORDER_THICKNESS * 2;
    float fillWidth = maxFillWidth * fillPercentage;


    m_fill.setSize(sf::Vector2f(fillWidth, m_size.y - BORDER_THICKNESS * 2));


    m_fill.setFillColor(getFillColor(m_displayValue));
}
