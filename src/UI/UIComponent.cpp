#include "UI/UIComponent.h"
#include <algorithm>

// ============================================================================
// UIComponent - Base class implementation
// ============================================================================

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
    
    // Handle mouse movement for hover state
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                             static_cast<float>(event.mouseMove.y));
        bool wasHovered = m_hovered;
        m_hovered = containsPoint(mousePos);
        
        if (m_hovered && !wasHovered) {
            onMouseEnter();
        } else if (!m_hovered && wasHovered) {
            onMouseLeave();
        }
    }
    
    // Let children handle input first
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
    // Handle children in reverse order (top to bottom)
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        if (*it && (*it)->isVisible() && (*it)->isEnabled()) {
            if ((*it)->handleInput(event)) {
                return true;
            }
        }
    }
    return false;
}

// ============================================================================
// UIPanel - Container panel implementation
// ============================================================================

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
    
    // Update shape properties
    m_shape.setPosition(m_position);
    m_shape.setSize(m_size);
    m_shape.setFillColor(m_backgroundColor);
    m_shape.setOutlineColor(m_borderColor);
    m_shape.setOutlineThickness(m_borderThickness);
    
    window.draw(m_shape);
    renderChildren(window);
}

// ============================================================================
// UILabel - Text label implementation
// ============================================================================

UILabel::UILabel(const std::string& text, const sf::Vector2f& position,
                 unsigned int fontSize)
    : UIComponent(position, {0.f, 0.f}) {
    
    m_text.setString(text);
    m_text.setCharacterSize(fontSize);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(position);
    
    // Update size based on text bounds
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_size = sf::Vector2f(bounds.width, bounds.height);
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
    
    // Update size based on new text bounds
    sf::FloatRect bounds = m_text.getLocalBounds();
    m_size = sf::Vector2f(bounds.width, bounds.height);
}

// ============================================================================
// UIButton - Interactive button implementation
// ============================================================================

UIButton::UIButton(const std::string& text, const sf::Vector2f& position,
                   const sf::Vector2f& size)
    : UIComponent(position, size),
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
    
    m_text.setString(text);
    m_text.setCharacterSize(18);
    m_text.setFillColor(m_textColor);
    
    // Center text in button
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setPosition(
        position.x + (size.x - textBounds.width) / 2.f - textBounds.left,
        position.y + (size.y - textBounds.height) / 2.f - textBounds.top
    );
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
    
    // Call base class to handle hover
    UIComponent::handleInput(event);
    
    // Handle mouse button events
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                                 static_cast<float>(event.mouseButton.y));
            if (containsPoint(mousePos)) {
                m_pressed = true;
                return true;
            }
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left && m_pressed) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                                 static_cast<float>(event.mouseButton.y));
            if (containsPoint(mousePos)) {
                // Button was clicked
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
    
    // Update button color based on state
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
    
    // Re-center text
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setPosition(
        m_position.x + (m_size.x - textBounds.width) / 2.f - textBounds.left,
        m_position.y + (m_size.y - textBounds.height) / 2.f - textBounds.top
    );
}

void UIButton::onMouseEnter() {
    UIComponent::onMouseEnter();
}

void UIButton::onMouseLeave() {
    UIComponent::onMouseLeave();
    m_pressed = false;
}

// ============================================================================
// UIProgressBar - Progress bar implementation
// ============================================================================

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
    
    // Calculate bar width based on progress
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

// ============================================================================
// UIImage - Image/Sprite implementation
// ============================================================================

UIImage::UIImage(const sf::Texture& texture, const sf::Vector2f& position,
                 const sf::Vector2f& size)
    : UIComponent(position, size) {
    
    m_sprite.setTexture(texture);
    m_sprite.setPosition(position);
    
    // If size is specified, scale sprite to fit
    if (size.x > 0.f && size.y > 0.f) {
        sf::Vector2u textureSize = texture.getSize();
        float scaleX = size.x / textureSize.x;
        float scaleY = size.y / textureSize.y;
        m_sprite.setScale(scaleX, scaleY);
    } else {
        // Use texture size
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
    
    // Update size if no custom size was set
    if (m_size.x == 0.f && m_size.y == 0.f) {
        sf::Vector2u textureSize = texture.getSize();
        m_size = sf::Vector2f(static_cast<float>(textureSize.x),
                             static_cast<float>(textureSize.y));
    }
}
