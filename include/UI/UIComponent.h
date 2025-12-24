#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include <functional>





















class UIComponent {
public:





    UIComponent(const sf::Vector2f& position = {0.f, 0.f},
                const sf::Vector2f& size = {0.f, 0.f});

    virtual ~UIComponent() = default;






    virtual bool handleInput(const sf::Event& event);





    virtual void update(float deltaTime);





    virtual void render(sf::RenderWindow& window) = 0;


    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    sf::Vector2f getPosition() const { return m_position; }
    sf::Vector2f getSize() const { return m_size; }
    sf::FloatRect getBounds() const;


    void setVisible(bool visible) { m_visible = visible; }
    bool isVisible() const { return m_visible; }
    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }


    void addChild(std::shared_ptr<UIComponent> child);
    void removeChild(std::shared_ptr<UIComponent> child);
    void clearChildren();
    const std::vector<std::shared_ptr<UIComponent>>& getChildren() const { return m_children; }


    void setId(const std::string& id) { m_id = id; }
    std::string getId() const { return m_id; }


    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onFocus();
    virtual void onBlur();

    bool isHovered() const { return m_hovered; }
    bool isFocused() const { return m_focused; }


    bool containsPoint(const sf::Vector2f& point) const;

protected:

    sf::Vector2f m_position;
    sf::Vector2f m_size;
    bool m_visible;
    bool m_enabled;
    bool m_hovered;
    bool m_focused;
    std::string m_id;


    std::vector<std::shared_ptr<UIComponent>> m_children;


    void renderChildren(sf::RenderWindow& window);
    void updateChildren(float deltaTime);
    bool handleChildrenInput(const sf::Event& event);
};




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


    sf::Color m_normalColor;
    sf::Color m_hoverColor;
    sf::Color m_pressedColor;
    sf::Color m_disabledColor;
    sf::Color m_textColor;
    sf::Color m_currentColor;

    bool m_pressed;
};




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

#endif
