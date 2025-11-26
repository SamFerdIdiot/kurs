#ifndef UI_CONSTANTS_H
#define UI_CONSTANTS_H

#include <SFML/Graphics.hpp>

// Fixed UI Constants for MacBook Air M1 (1440x900)
// No scaling, no responsive design - everything is hardcoded

namespace UI {

// Screen dimensions (MacBook Air M1)
constexpr float SCREEN_WIDTH = 1440.0f;
constexpr float SCREEN_HEIGHT = 900.0f;

// Colors
namespace Color {
    const sf::Color BACKGROUND_DARK(40, 40, 50);
    const sf::Color BACKGROUND_LIGHT(60, 60, 70);
    const sf::Color ACCENT_BLUE(70, 130, 180);
    const sf::Color ACCENT_GREEN(60, 180, 75);
    const sf::Color ACCENT_RED(220, 50, 50);
    const sf::Color ACCENT_YELLOW(255, 200, 50);
    const sf::Color BUTTON_NORMAL(80, 80, 90);
    const sf::Color BUTTON_HOVER(100, 100, 110);
    const sf::Color BUTTON_PRESSED(60, 60, 70);
    const sf::Color TEXT_PRIMARY(255, 255, 255);
    const sf::Color TEXT_SECONDARY(200, 200, 200);
    const sf::Color BORDER_COLOR(150, 150, 160);
    const sf::Color SELECTION_HIGHLIGHT(70, 130, 180, 100);
}

// Font sizes
constexpr unsigned int FONT_SIZE_HUGE = 48;
constexpr unsigned int FONT_SIZE_TITLE = 32;
constexpr unsigned int FONT_SIZE_SUBTITLE = 24;
constexpr unsigned int FONT_SIZE_NORMAL = 18;
constexpr unsigned int FONT_SIZE_SMALL = 14;
constexpr unsigned int FONT_SIZE_TINY = 12;

// Spacing and padding
constexpr float PADDING_HUGE = 40.0f;
constexpr float PADDING_LARGE = 20.0f;
constexpr float PADDING_MEDIUM = 10.0f;
constexpr float PADDING_SMALL = 5.0f;

constexpr float SPACING_HUGE = 80.0f;
constexpr float SPACING_LARGE = 60.0f;
constexpr float SPACING_MEDIUM = 40.0f;
constexpr float SPACING_SMALL = 20.0f;

// Button dimensions
constexpr float BUTTON_HEIGHT = 50.0f;
constexpr float BUTTON_WIDTH = 300.0f;
constexpr float BUTTON_WIDTH_SMALL = 150.0f;

// Menu positioning (NodeScene)
namespace Menu {
    constexpr float START_X = 100.0f;
    constexpr float START_Y = 150.0f;
    constexpr float OPTION_SPACING = 70.0f;
    constexpr float SELECTION_HIGHLIGHT_WIDTH = 1000.0f;
    constexpr float SELECTION_HIGHLIGHT_HEIGHT = 60.0f;
    constexpr float SELECTION_HIGHLIGHT_OFFSET = -10.0f;
}

// HUD positioning
namespace HUD {
    constexpr float TOP_RIGHT_X = 1200.0f;
    constexpr float TOP_RIGHT_Y = 20.0f;
    constexpr float STATUS_Y = 660.0f;
    constexpr float CONTROLS_Y = 820.0f;
}

// Font paths
namespace Fonts {
    const char* const PRIMARY = "assets/fonts/font.ttf";
    const char* const FALLBACK_1 = "images/Press_Start_2P/PressStart2P-Regular.ttf";
    const char* const FALLBACK_2 = "/System/Library/Fonts/Helvetica.ttc";
}

} // namespace UI

#endif // UI_CONSTANTS_H
