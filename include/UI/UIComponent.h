#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include <functional>

/**
 * @brief Base class for all UI components
 * 
 * Provides a unified interface for UI elements across all scenes.
 * Implements composite pattern for nested UI hierarchies.
 * 
 * Design Principles:
 * - Single Responsibility: Each component handles one UI concern
 * - Open/Closed: Easy to extend with new component types
 * - Liskov Substitution: All components share common interface
 * - Interface Segregation: Minimal required interface
 * - Dependency Inversion: Depends on abstractions, not concrete types
 * 
 * Component Lifecycle:
 * 1. Construction - Initialize with position and size
 * 2. handleInput - Process user input events
 * 3. update - Update component state (animations, etc.)
 * 4. render - Draw component to window
 * 5. Destruction - Clean up resources
 */
class UIComponent {
public:
    /**
     * @brief Construct a UI component
     * @param position Screen position (x, y)
     * @param size Component size (width, height)
     */
    UIComponent(const sf::Vector2f& position = {0.f, 0.f}, 
                const sf::Vector2f& size = {0.f, 0.f});
    
    virtual ~UIComponent() = default;
    
    /**
     * @brief Handle input events
     * @param event SFML event to process
     * @return true if event was handled and should not propagate
     */
    virtual bool handleInput(const sf::Event& event);
    
    /**
     * @brief Update component state
     * @param deltaTime Time since last update in seconds
     */
    virtual void update(float deltaTime);
    
    /**
     * @brief Render component to window
     * @param window SFML render window
     */
    virtual void render(sf::RenderWindow& window) = 0;
    
    // Position and size management
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    sf::Vector2f getPosition() const { return m_position; }
    sf::Vector2f getSize() const { return m_size; }
    sf::FloatRect getBounds() const;
    
    // Visibility and interaction
    void setVisible(bool visible) { m_visible = visible; }
    bool isVisible() const { return m_visible; }
    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }
    
    // Hierarchy management
    void addChild(std::shared_ptr<UIComponent> child);
    void removeChild(std::shared_ptr<UIComponent> child);
    void clearChildren();
    const std::vector<std::shared_ptr<UIComponent>>& getChildren() const { return m_children; }
    
    // Component identification
    void setId(const std::string& id) { m_id = id; }
    std::string getId() const { return m_id; }
    
    // Hover and focus state
    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onFocus();
    virtual void onBlur();
    
    bool isHovered() const { return m_hovered; }
    bool isFocused() const { return m_focused; }
    
    // Helper for mouse position checking
    bool containsPoint(const sf::Vector2f& point) const;
    
protected:
    // Component state
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    bool m_visible;
    bool m_enabled;
    bool m_hovered;
    bool m_focused;
    std::string m_id;
    
    // Child components
    std::vector<std::shared_ptr<UIComponent>> m_children;
    
    // Helper methods for derived classes
    void renderChildren(sf::RenderWindow& window);
    void updateChildren(float deltaTime);
    bool handleChildrenInput(const sf::Event& event);
};

/**
 * @brief Panel component - container for other UI elements
 */
class UIPanel : public UIComponent {
public:
    UIPanel(const sf::Vector2f& position, const sf::Vector2f& size,
            const sf::Color& backgroundColor = sf::Color(40, 40, 50));
    
    void render(sf::RenderWindow& window) override;
    
    void setBackgroundColor(const sf::Color& color) { m_backgroundColor = color; }
    void setBorderColor(const sf::Color& color) { m_borderColor = color; }
    void setBorderThickness(float thickness) { m_borderThickness = thickness; }
    
private:
    sf::Color m_backgroundColor;
    sf::Color m_borderColor;
    float m_borderThickness;
    sf::RectangleShape m_shape;
};

/**
 * @brief Text label component
 */
class UILabel : public UIComponent {
public:
    UILabel(const sf::Font& font, const std::string& text, const sf::Vector2f& position,
            unsigned int fontSize = 16);

    void render(sf::RenderWindow& window) override;

    void setText(const std::string& text);
    std::string getText() const { return m_text.getString(); }

    void setFont(const sf::Font& font) { m_text.setFont(font); }
    void setFontSize(unsigned int size) { m_text.setCharacterSize(size); }
    void setTextColor(const sf::Color& color) { m_text.setFillColor(color); }
    void setStyle(uint32_t style) { m_text.setStyle(static_cast<sf::Text::Style>(style)); }

private:
    sf::Text m_text;
};

/**
 * @brief Button component with hover and click states
 */
class UIButton : public UIComponent {
public:
    using ClickCallback = std::function<void()>;

    UIButton(const sf::Font& font, const std::string& text, const sf::Vector2f& position,
             const sf::Vector2f& size);

    void render(sf::RenderWindow& window) override;
    bool handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    
    void setText(const std::string& text);
    void setCallback(ClickCallback callback) { m_callback = callback; }
    
    // Visual customization
    void setNormalColor(const sf::Color& color) { m_normalColor = color; }
    void setHoverColor(const sf::Color& color) { m_hoverColor = color; }
    void setPressedColor(const sf::Color& color) { m_pressedColor = color; }
    void setDisabledColor(const sf::Color& color) { m_disabledColor = color; }
    void setTextColor(const sf::Color& color) { m_textColor = color; }
    
    void onMouseEnter() override;
    void onMouseLeave() override;
    
private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    ClickCallback m_callback;
    
    // Visual states
    sf::Color m_normalColor;
    sf::Color m_hoverColor;
    sf::Color m_pressedColor;
    sf::Color m_disabledColor;
    sf::Color m_textColor;
    sf::Color m_currentColor;
    
    bool m_pressed;
};

/**
 * @brief Progress bar component
 */
class UIProgressBar : public UIComponent {
public:
    UIProgressBar(const sf::Vector2f& position, const sf::Vector2f& size,
                  float minValue = 0.f, float maxValue = 100.f);
    
    void render(sf::RenderWindow& window) override;
    
    void setValue(float value);
    float getValue() const { return m_value; }
    float getProgress() const { return (m_value - m_minValue) / (m_maxValue - m_minValue); }
    
    void setMinMax(float min, float max) { m_minValue = min; m_maxValue = max; }
    void setBarColor(const sf::Color& color) { m_barColor = color; }
    void setBackgroundColor(const sf::Color& color) { m_backgroundColor = color; }
    void setBorderColor(const sf::Color& color) { m_borderColor = color; }
    
private:
    float m_value;
    float m_minValue;
    float m_maxValue;
    
    sf::Color m_barColor;
    sf::Color m_backgroundColor;
    sf::Color m_borderColor;
    
    sf::RectangleShape m_background;
    sf::RectangleShape m_bar;
};

/**
 * @brief Image/Sprite component
 */
class UIImage : public UIComponent {
public:
    UIImage(const sf::Texture& texture, const sf::Vector2f& position,
            const sf::Vector2f& size = {0.f, 0.f});
    
    void render(sf::RenderWindow& window) override;
    
    void setTexture(const sf::Texture& texture);
    void setColor(const sf::Color& color) { m_sprite.setColor(color); }
    
private:
    sf::Sprite m_sprite;
};

#endif // UI_COMPONENT_H
