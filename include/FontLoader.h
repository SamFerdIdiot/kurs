#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <SFML/Graphics.hpp>
#include "UIConstants.h"
#include <optional>
#include <iostream>


class FontLoader {
public:

    static std::optional<sf::Font> load() {
        sf::Font font;


        if (font.openFromFile(UI::Fonts::PRIMARY)) {
            std::cout << "FontLoader: Loaded primary font: " << UI::Fonts::PRIMARY << std::endl;
            return font;
        }


        if (font.openFromFile(UI::Fonts::FALLBACK_1)) {
            std::cout << "FontLoader: Loaded fallback font 1: " << UI::Fonts::FALLBACK_1 << std::endl;
            return font;
        }


        if (font.openFromFile(UI::Fonts::FALLBACK_2)) {
            std::cout << "FontLoader: Loaded fallback font 2: " << UI::Fonts::FALLBACK_2 << std::endl;
            return font;
        }

        std::cerr << "FontLoader: ERROR - Could not load any font!" << std::endl;
        return std::nullopt;
    }


    static std::optional<sf::Font> loadFrom(const char* path) {
        sf::Font font;
        if (font.openFromFile(path)) {
            std::cout << "FontLoader: Loaded custom font: " << path << std::endl;
            return font;
        }
        std::cerr << "FontLoader: ERROR - Could not load font: " << path << std::endl;
        return std::nullopt;
    }
};

#endif
