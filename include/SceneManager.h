#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <memory>
#include <SFML/Graphics.hpp>
#include "Scene.h"




class SceneManager {
public:


    SceneManager(sf::RenderWindow& window);


    void run();

private:

    std::unique_ptr<Scene> createScene(SceneType type);


    sf::RenderWindow& m_window;


    std::unique_ptr<Scene> m_currentScene;


    SceneType m_currentSceneType;
};

#endif
