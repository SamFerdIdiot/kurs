#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>


enum class SceneType {
    MAIN_MENU,
    MOTEL,
    INVENTORY,
    CHARACTER,
    COMPANIONS,
    QUESTS,
    ABILITY_TREE,
    NOTEBOOK,
    EXIT
};


class Scene {
public:
    virtual ~Scene() = default;



    virtual void handleInput(const sf::Event& event) = 0;



    virtual void update(float deltaTime) = 0;



    virtual void render(sf::RenderWindow& window) = 0;



    virtual SceneType getNextScene() const = 0;



    virtual bool isFinished() const = 0;
};

#endif
