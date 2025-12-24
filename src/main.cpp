#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include "UIConstants.h"
#include <iostream>

int main() {


    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(UI::SCREEN_WIDTH),
                                            static_cast<unsigned int>(UI::SCREEN_HEIGHT)}),
                           "KKURS");
    window.setFramerateLimit(60);

    try {

        SceneManager sceneManager(window);
        sceneManager.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
