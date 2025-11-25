#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include <iostream>

int main() {
    // Create window with Russian 90s theme
    // Using ASCII-only title to avoid encoding issues on macOS
    sf::RenderWindow window(sf::VideoMode(1366, 768), "69 Crossroads - Russia 90s");
    window.setFramerateLimit(60);
    
    try {
        // Create and run scene manager
        SceneManager sceneManager(window);
        sceneManager.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
