#include "UI/UIComponent.h"
#include <algorithm>





UIComponent::UIComponent(const sf::Vector2f& position, const sf::Vector2f& size)
    : m_position(position),
      m_size(size),
      m_visible(true),
      m_enabled(true),
      m_hovered(false),
      m_focused(false) {
}

bool UIComponent::handleInput(const sf::Event& event) {
    if (!m_visible || !m_enabled) {
        return false;
    }


    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x),
                             static_cast<float>(mouseMoved->position.y));
        bool wasHovered = m_hovered;
        m_hovered = containsPoint(mousePos);

        if (m_hovered && !wasHovered) {
            onMouseEnter();
        } else if (!m_hovered && wasHovered) {
            onMouseLeave();
        }
    }


    if (handleChildrenInput(event)) {
        return true;
    }

    return false;
}

void UIComponent::update(float deltaTime) {
    if (!m_visible) {
        return;
    }

    updateChildren(deltaTime);
}

void UIComponent::setPosition(const sf::Vector2f& position) {
    m_position = position;
}

void UIComponent::setSize(const sf::Vector2f& size) {
    m_size = size;
}

sf::FloatRect UIComponent::getBounds() const {
    return sf::FloatRect(m_position, m_size);
}

void UIComponent::addChild(std::shared_ptr<UIComponent> child) {
    if (child) {
        m_children.push_back(child);
    }
}

void UIComponent::removeChild(std::shared_ptr<UIComponent> child) {
    m_children.erase(
        std::remove(m_children.begin(), m_children.end(), child),
        m_children.end()
    );
}

void UIComponent::clearChildren() {
    m_children.clear();
}

void UIComponent::onMouseEnter() {
    m_hovered = true;
}

void UIComponent::onMouseLeave() {
    m_hovered = false;
}

void UIComponent::onFocus() {
    m_focused = true;
}

void UIComponent::onBlur() {
    m_focused = false;
}

bool UIComponent::containsPoint(const sf::Vector2f& point) const {
    return getBounds().contains(point);
}

void UIComponent::renderChildren(sf::RenderWindow& window) {
    for (auto& child : m_children) {
        if (child && child->isVisible()) {
            child->render(window);
        }
    }
}

void UIComponent::updateChildren(float deltaTime) {
    for (auto& child : m_children) {
        if (child && child->isVisible()) {
            child->update(deltaTime);
        }
    }
}

bool UIComponent::handleChildrenInput(const sf::Event& event) {

    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        if (*it && (*it)->isVisible() && (*it)->isEnabled()) {
            if ((*it)->handleInput(event)) {
                return true;
            }
        }
    }
    return false;
}





UIPanel::UIPanel(const sf::Vector2f& position, const sf::Vector2f& size,
                 const sf::Color& backgroundColor)
    : UIComponent(position, size),
      m_backgroundColor(backgroundColor),
      m_borderColor(sf::Color(100, 100, 110)),
      m_borderThickness(2.f) {

    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(backgroundColor);
    m_shape.setOutlineColor(m_borderColor);
    m_shape.setOutlineThickness(m_borderThickness);
}

void UIPanel::render(sf::RenderWindow& window) {
    if (!m_visible) {
        return;
    }


    m_shape.setPosition(m_position);
    m_shape.setSize(m_size);
    m_shape.setFillColor(m_backgroundColor);
    m_shape.setOutlineColor(m_borderColor);
    m_shape.setOutlineThickness(m_borderThickness);

    window.draw(m_shape);
    renderChildren(window);
}





UILabel::UILabel(const sf::Font& font, const std::string& text, const sf::Vector2f& position,
                 unsigned int fontSize)
    : UIComponent(position, {0.f, 0.f}),
      m_text(font, text, fontSize) {

    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(position);


    sf::FloatRect bounds = m_text.getLocalBounds();
    m_size = bounds.size;
}

void UILabel::render(sf::RenderWindow& window) {
    if (!m_visible) {
        return;
    }

    m_text.setPosition(m_position);
    window.draw(m_text);
    renderChildren(window);
}

void UILabel::setText(const std::string& text) {
    m_text.setString(text);


    sf::FloatRect bounds = m_text.getLocalBounds();
    m_size = bounds.size;
}





UIButton::UIButton(const sf::Font& font, const std::string& text, const sf::Vector2f& position,
                   const sf::Vector2f& size)
    : UIComponent(position, size),
      m_text(font, text, 18),
      m_normalColor(sf::Color(60, 60, 70)),
      m_hoverColor(sf::Color(80, 80, 100)),
      m_pressedColor(sf::Color(100, 150, 100)),
      m_disabledColor(sf::Color(40, 40, 50)),
      m_textColor(sf::Color::White),
      m_currentColor(sf::Color(60, 60, 70)),
      m_pressed(false) {

    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(m_normalColor);
    m_shape.setOutlineColor(sf::Color(100, 100, 110));
    m_shape.setOutlineThickness(2.f);

    m_text.setFillColor(m_textColor);


    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setPosition(sf::Vector2f(
        position.x + (size.x - textBounds.size.x) / 2.f - textBounds.position.x,
        position.y + (size.y - textBounds.size.y) / 2.f - textBounds.position.y
    ));
}

void UIButton::render(sf::RenderWindow& window) {
    if (!m_visible) {
        return;
    }

    m_shape.setPosition(m_position);
    m_shape.setSize(m_size);
    m_shape.setFillColor(m_currentColor);

    window.draw(m_shape);
    window.draw(m_text);
    renderChildren(window);
}

bool UIButton::handleInput(const sf::Event& event) {
    if (!m_visible || !m_enabled) {
        return false;
    }


    UIComponent::handleInput(event);


    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x),
                                 static_cast<float>(mousePressed->position.y));
            if (containsPoint(mousePos)) {
                m_pressed = true;
                return true;
            }
        }
    } else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left && m_pressed) {
            sf::Vector2f mousePos(static_cast<float>(mouseReleased->position.x),
                                 static_cast<float>(mouseReleased->position.y));
            if (containsPoint(mousePos)) {

                if (m_callback) {
                    m_callback();
                }
                m_pressed = false;
                return true;
            }
            m_pressed = false;
        }
    }

    return false;
}

void UIButton::update(float deltaTime) {
    UIComponent::update(deltaTime);


    if (!m_enabled) {
        m_currentColor = m_disabledColor;
    } else if (m_pressed) {
        m_currentColor = m_pressedColor;
    } else if (m_hovered) {
        m_currentColor = m_hoverColor;
    } else {
        m_currentColor = m_normalColor;
    }
}

void UIButton::setText(const std::string& text) {
    m_text.setString(text);


    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setPosition(sf::Vector2f(
        m_position.x + (m_size.x - textBounds.size.x) / 2.f - textBounds.position.x,
        m_position.y + (m_size.y - textBounds.size.y) / 2.f - textBounds.position.y
    ));
}

void UIButton::onMouseEnter() {
    UIComponent::onMouseEnter();
}

void UIButton::onMouseLeave() {
    UIComponent::onMouseLeave();
    m_pressed = false;
}





UIProgressBar::UIProgressBar(const sf::Vector2f& position, const sf::Vector2f& size,
                             float minValue, float maxValue)
    : UIComponent(position, size),
      m_value(minValue),
      m_minValue(minValue),
      m_maxValue(maxValue),
      m_barColor(sf::Color(100, 200, 100)),
      m_backgroundColor(sf::Color(40, 40, 40)),
      m_borderColor(sf::Color(80, 80, 80)) {

    m_background.setPosition(position);
    m_background.setSize(size);
    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineColor(m_borderColor);
    m_background.setOutlineThickness(2.f);

    m_bar.setPosition(position);
    m_bar.setSize(sf::Vector2f(0.f, size.y));
    m_bar.setFillColor(m_barColor);
}

void UIProgressBar::render(sf::RenderWindow& window) {
    if (!m_visible) {
        return;
    }

    m_background.setPosition(m_position);
    m_background.setSize(m_size);


    float progress = getProgress();
    float barWidth = m_size.x * progress;
    m_bar.setPosition(m_position);
    m_bar.setSize(sf::Vector2f(barWidth, m_size.y));

    window.draw(m_background);
    window.draw(m_bar);
    renderChildren(window);
}

void UIProgressBar::setValue(float value) {
    m_value = std::max(m_minValue, std::min(value, m_maxValue));
}





UIImage::UIImage(const sf::Texture& texture, const sf::Vector2f& position,
                 const sf::Vector2f& size)
    : UIComponent(position, size),
      m_sprite(texture) {

    m_sprite.setPosition(position);


    if (size.x > 0.f && size.y > 0.f) {
        sf::Vector2u textureSize = texture.getSize();
        float scaleX = size.x / textureSize.x;
        float scaleY = size.y / textureSize.y;
        m_sprite.setScale(sf::Vector2f(scaleX, scaleY));
    } else {

        sf::Vector2u textureSize = texture.getSize();
        m_size = sf::Vector2f(static_cast<float>(textureSize.x),
                             static_cast<float>(textureSize.y));
    }
}

void UIImage::render(sf::RenderWindow& window) {
    if (!m_visible) {
        return;
    }

    m_sprite.setPosition(m_position);
    window.draw(m_sprite);
    renderChildren(window);
}

void UIImage::setTexture(const sf::Texture& texture) {
    m_sprite.setTexture(texture);


    if (m_size.x == 0.f && m_size.y == 0.f) {
        sf::Vector2u textureSize = texture.getSize();
        m_size = sf::Vector2f(static_cast<float>(textureSize.x),
                             static_cast<float>(textureSize.y));
    }
}
