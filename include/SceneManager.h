#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "Scene.h"

// SceneManager handles scene transitions and lifecycle
// It acts as a dispatcher between different game scenes
// The game ALWAYS starts with the main menu scene
class SceneManager {
public:
    // Конструктор инициализирует игру сценой главного меню
    // Throws runtime_error if main menu scene cannot be created
    SceneManager(sf::RenderWindow& window);
    
    // Run the game loop with scene management
    void run();
    
private:
    // Create and return a new scene based on the scene type
    std::unique_ptr<Scene> createScene(SceneType type);
    
    // Reference to the game window
    sf::RenderWindow& m_window;
    
    // Текущая активная сцена
    std::unique_ptr<Scene> m_currentScene;
    
    // Тип текущей сцены
    SceneType m_currentSceneType;
};

#endif // SCENE_MANAGER_H
