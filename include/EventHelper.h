#ifndef EVENT_HELPER_H
#define EVENT_HELPER_H

#include <SFML/Graphics.hpp>

// EventHelper - Helper for handling SFML 3.x events
// Simplifies event checking with type-safe methods
class EventHelper {
public:
    // Check if specific key was pressed
    static bool isKeyPressed(const sf::Event& event, sf::Keyboard::Key key) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
            return keyPressed->code == key;
        }
        return false;
    }

    // Check if specific key was released
    static bool isKeyReleased(const sf::Event& event, sf::Keyboard::Key key) {
        if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
            return keyReleased->code == key;
        }
        return false;
    }

    // Check if window close requested
    static bool isClosed(const sf::Event& event) {
        return event.is<sf::Event::Closed>();
    }

    // Check if mouse button was pressed
    static bool isMousePressed(const sf::Event& event, sf::Mouse::Button button) {
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            return mousePressed->button == button;
        }
        return false;
    }

    // Check if mouse button was released
    static bool isMouseReleased(const sf::Event& event, sf::Mouse::Button button) {
        if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
            return mouseReleased->button == button;
        }
        return false;
    }

    // Get mouse position from event
    static std::optional<sf::Vector2i> getMousePosition(const sf::Event& event) {
        if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
            return sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y);
        }
        return std::nullopt;
    }

    // Check if text was entered
    static std::optional<char32_t> getTextEntered(const sf::Event& event) {
        if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
            return textEntered->unicode;
        }
        return std::nullopt;
    }
};

#endif // EVENT_HELPER_H
