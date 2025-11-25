#ifndef CHARACTER_CREATION_SCENE_H
#define CHARACTER_CREATION_SCENE_H

#include "Scene.h"
#include "CharacterCreationMenu.h"
#include "UILayout.h"
#include <SFML/Graphics.hpp>

// Character creation scene
// Purpose: Allow player to create and customize their character
// Features:
//   - Select character background/class (Nomad, Hitchhiker, Ex-Racer)
//   - Select starting vehicle (Old Sedan, Offroad Van, Sports Coupe)
//   - ASCII/Notebook style menu with typewriter effect
//   - Blinking orange arrow for selection
class CharacterCreationScene : public Scene {
public:
    CharacterCreationScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    CharacterCreationMenu m_menu;           // Character creation menu
    bool m_isFinished;                      // Scene completion flag
    SceneType m_nextScene;                  // Next scene to transition to
};

#endif // CHARACTER_CREATION_SCENE_H
