/**
 * @file Graphics.hpp
 * @brief Mock SFML Graphics types for testing without SFML
 * @date 2025-11-18
 * 
 * This file provides mock implementations of SFML Graphics types
 * to allow testing of code that depends on SFML without requiring
 * SFML to be installed.
 */

#ifndef MOCK_SFML_GRAPHICS_HPP
#define MOCK_SFML_GRAPHICS_HPP

#include "System.hpp"
#include "Window.hpp"
#include <string>
#include <cstdint>

namespace sf {

/**
 * Mock Color - RGBA color representation
 */
class Color {
public:
    uint8_t r, g, b, a;
    
    Color() : r(0), g(0), b(0), a(255) {}
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
    
    bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    
    bool operator!=(const Color& other) const {
        return !(*this == other);
    }
    
    // Common colors
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;
    static const Color Transparent;
};

// Define common colors
inline const Color Color::Black(0, 0, 0);
inline const Color Color::White(255, 255, 255);
inline const Color Color::Red(255, 0, 0);
inline const Color Color::Green(0, 255, 0);
inline const Color Color::Blue(0, 0, 255);
inline const Color Color::Yellow(255, 255, 0);
inline const Color Color::Magenta(255, 0, 255);
inline const Color Color::Cyan(0, 255, 255);
inline const Color Color::Transparent(0, 0, 0, 0);

/**
 * Mock RectangleShape - Rectangle drawable
 */
class RectangleShape {
public:
    RectangleShape() {}
    explicit RectangleShape(const Vector2f& size) : m_size(size) {}
    
    void setSize(const Vector2f& size) { m_size = size; }
    const Vector2f& getSize() const { return m_size; }
    
    void setPosition(const Vector2f& position) { m_position = position; }
    void setPosition(float x, float y) { m_position = Vector2f(x, y); }
    const Vector2f& getPosition() const { return m_position; }
    
    void setOrigin(const Vector2f& origin) { m_origin = origin; }
    void setOrigin(float x, float y) { m_origin = Vector2f(x, y); }
    const Vector2f& getOrigin() const { return m_origin; }
    
    void setRotation(float angle) { m_rotation = angle; }
    float getRotation() const { return m_rotation; }
    
    void setFillColor(const Color& color) { m_fillColor = color; }
    const Color& getFillColor() const { return m_fillColor; }
    
    void setOutlineColor(const Color& color) { m_outlineColor = color; }
    const Color& getOutlineColor() const { return m_outlineColor; }
    
    void setOutlineThickness(float thickness) { m_outlineThickness = thickness; }
    float getOutlineThickness() const { return m_outlineThickness; }
    
private:
    Vector2f m_size;
    Vector2f m_position;
    Vector2f m_origin;
    float m_rotation = 0.0f;
    Color m_fillColor;
    Color m_outlineColor;
    float m_outlineThickness = 0.0f;
};

/**
 * Mock CircleShape - Circle drawable
 */
class CircleShape {
public:
    CircleShape() : m_radius(0.0f) {}
    explicit CircleShape(float radius) : m_radius(radius) {}
    
    void setRadius(float radius) { m_radius = radius; }
    float getRadius() const { return m_radius; }
    
    void setPosition(const Vector2f& position) { m_position = position; }
    void setPosition(float x, float y) { m_position = Vector2f(x, y); }
    const Vector2f& getPosition() const { return m_position; }
    
    void setOrigin(const Vector2f& origin) { m_origin = origin; }
    void setOrigin(float x, float y) { m_origin = Vector2f(x, y); }
    const Vector2f& getOrigin() const { return m_origin; }
    
    void setFillColor(const Color& color) { m_fillColor = color; }
    const Color& getFillColor() const { return m_fillColor; }
    
    void setOutlineColor(const Color& color) { m_outlineColor = color; }
    const Color& getOutlineColor() const { return m_outlineColor; }
    
    void setOutlineThickness(float thickness) { m_outlineThickness = thickness; }
    float getOutlineThickness() const { return m_outlineThickness; }
    
    void setPointCount(size_t count) { m_pointCount = count; }
    size_t getPointCount() const { return m_pointCount; }
    
private:
    float m_radius;
    Vector2f m_position;
    Vector2f m_origin;
    Color m_fillColor;
    Color m_outlineColor;
    float m_outlineThickness = 0.0f;
    size_t m_pointCount = 30;
};

/**
 * Mock Font - Font for text rendering
 */
class Font {
public:
    struct Info {
        std::string family;
    };
    
    Font() {}

    bool loadFromFile(const std::string& filename) {
        // Mock: always succeeds
        m_filename = filename;
        m_info.family = "MockFont";
        return true;
    }

    // SFML 3.x API
    bool openFromFile(const std::string& filename) {
        return loadFromFile(filename);
    }
    
    const std::string& getFilename() const { return m_filename; }
    
    const Info& getInfo() const { return m_info; }
    
private:
    std::string m_filename;
    Info m_info;
};

/**
 * Mock Text - Text drawable
 */
class Text {
public:
    enum Style {
        Regular = 0,
        Bold = 1 << 0,
        Italic = 1 << 1,
        Underlined = 1 << 2,
        StrikeThrough = 1 << 3
    };
    
    Text() : m_characterSize(30), m_style(Regular) {}

    // SFML 2.x API
    Text(const std::string& string, const Font& font, unsigned int characterSize = 30)
        : m_string(string), m_font(&font), m_characterSize(characterSize), m_style(Regular) {}

    // SFML 3.x API (different parameter order)
    Text(const Font& font, const std::string& string, unsigned int characterSize = 30)
        : m_string(string), m_font(&font), m_characterSize(characterSize), m_style(Regular) {}
    
    void setString(const std::string& string) { m_string = string; }
    const std::string& getString() const { return m_string; }
    
    void setFont(const Font& font) { m_font = &font; }
    const Font* getFont() const { return m_font; }
    
    void setCharacterSize(unsigned int size) { m_characterSize = size; }
    unsigned int getCharacterSize() const { return m_characterSize; }
    
    void setStyle(uint32_t style) { m_style = style; }
    uint32_t getStyle() const { return m_style; }
    
    void setFillColor(const Color& color) { m_fillColor = color; }
    const Color& getFillColor() const { return m_fillColor; }
    
    void setOutlineColor(const Color& color) { m_outlineColor = color; }
    const Color& getOutlineColor() const { return m_outlineColor; }
    
    void setOutlineThickness(float thickness) { m_outlineThickness = thickness; }
    float getOutlineThickness() const { return m_outlineThickness; }
    
    void setPosition(const Vector2f& position) { m_position = position; }
    void setPosition(float x, float y) { m_position = Vector2f(x, y); }
    const Vector2f& getPosition() const { return m_position; }
    
    FloatRect getLocalBounds() const {
        // Mock: return approximate bounds
        return FloatRect(0, 0, m_string.length() * m_characterSize * 0.6f, m_characterSize * 1.2f);
    }
    
    FloatRect getGlobalBounds() const {
        FloatRect bounds = getLocalBounds();
        bounds.left = m_position.x;
        bounds.top = m_position.y;
        return bounds;
    }
    
private:
    std::string m_string;
    const Font* m_font = nullptr;
    unsigned int m_characterSize;
    uint32_t m_style;
    Color m_fillColor;
    Color m_outlineColor;
    float m_outlineThickness = 0.0f;
    Vector2f m_position;
};

/**
 * Mock Vertex - A vertex with position and color
 */
class Vertex {
public:
    Vector2f position;
    Color color;
    
    Vertex() {}
    Vertex(const Vector2f& pos) : position(pos), color(Color::White) {}
    Vertex(const Vector2f& pos, const Color& col) : position(pos), color(col) {}
};

/**
 * Mock PrimitiveType - Type of primitives to draw
 */
enum PrimitiveType {
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
    Quads
};

/**
 * Mock Texture - Image texture
 */
class Texture {
public:
    Texture() : m_size(0, 0) {}
    
    bool loadFromFile(const std::string& filename) {
        m_filename = filename;
        m_size = Vector2u(100, 100); // Mock size
        return true;
    }
    
    Vector2u getSize() const { return m_size; }
    
private:
    std::string m_filename;
    Vector2u m_size;
};

/**
 * Mock Sprite - Drawable texture
 */
class Sprite {
public:
    Sprite() : m_texture(nullptr) {}
    explicit Sprite(const Texture& texture) : m_texture(&texture) {}
    
    void setTexture(const Texture& texture) { m_texture = &texture; }
    const Texture* getTexture() const { return m_texture; }
    
    void setPosition(const Vector2f& position) { m_position = position; }
    void setPosition(float x, float y) { m_position = Vector2f(x, y); }
    const Vector2f& getPosition() const { return m_position; }
    
    void setScale(const Vector2f& scale) { m_scale = scale; }
    void setScale(float x, float y) { m_scale = Vector2f(x, y); }
    const Vector2f& getScale() const { return m_scale; }
    
    void setColor(const Color& color) { m_color = color; }
    const Color& getColor() const { return m_color; }
    
    FloatRect getLocalBounds() const {
        if (m_texture) {
            Vector2u size = m_texture->getSize();
            return FloatRect(0, 0, static_cast<float>(size.x), static_cast<float>(size.y));
        }
        return FloatRect(0, 0, 0, 0);
    }
    
    FloatRect getGlobalBounds() const {
        FloatRect bounds = getLocalBounds();
        bounds.left = m_position.x;
        bounds.top = m_position.y;
        bounds.width *= m_scale.x;
        bounds.height *= m_scale.y;
        return bounds;
    }
    
private:
    const Texture* m_texture;
    Vector2f m_position;
    Vector2f m_scale{1.0f, 1.0f};
    Color m_color{Color::White};
};

/**
 * Mock RenderWindow - Window for rendering
 */
class RenderWindow {
public:
    RenderWindow() : m_isOpen(true) {}
    
    void draw(const RectangleShape& shape) {
        // Mock: do nothing
    }
    
    void draw(const CircleShape& shape) {
        // Mock: do nothing
    }
    
    void draw(const Text& text) {
        // Mock: do nothing
    }
    
    void draw(const Sprite& sprite) {
        // Mock: do nothing
    }
    
    void draw(const Vertex* vertices, size_t vertexCount, PrimitiveType type) {
        // Mock: do nothing
    }
    
    void clear(const Color& color = Color::Black) {
        // Mock: do nothing
    }
    
    void display() {
        // Mock: do nothing
    }
    
    bool isOpen() const { return m_isOpen; }
    
    void close() { m_isOpen = false; }
    
    bool pollEvent(Event& event) {
        // Mock: no events
        return false;
    }
    
    Vector2u getSize() const { return Vector2u(800, 600); }
    
private:
    bool m_isOpen;
};

} // namespace sf

#endif // MOCK_SFML_GRAPHICS_HPP
