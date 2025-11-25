#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>

// Enum to identify different scenes in the game
enum class SceneType {
    MAIN_MENU,
    CHARACTER_CREATION,
    ROUTE_SELECTION,
    WORLD_MAP,
    NODE,           // Node gameplay scene (city with car control)
    DRIVING,
    SIDE_VIEW,
    ROAD,
    TOWN,
    MOTEL,
    DIALOGUE,
    INVENTORY,
    CHARACTER,      // Character stats and progression scene
    COMPANIONS,     // Party and companions management scene
    QUESTS,         // Quest journal and tracking scene
    ABILITY_TREE,   // Ability tree skill selection scene
    PAUSE_MENU,
    GAME_OVER,
    CINEMATIC_EVENT,
    TRAVEL_CINEMATIC,  // Cinematic travel scene between nodes
    REST,
    LOCATION,       // Generic location scene for gas stations, stores, diners, etc.
    EXIT
};

// Базовый класс для всех игровых сцен
class Scene {
public:
    virtual ~Scene() = default;
    
    // Handle input events
    // This is where keyboard/mouse input should be processed
    virtual void handleInput(const sf::Event& event) = 0;
    
    // Update scene logic
    // This is where game state updates happen (delta time in seconds)
    virtual void update(float deltaTime) = 0;
    
    // Render the scene
    // This is where all drawing operations happen
    virtual void render(sf::RenderWindow& window) = 0;
    
    // Get the next scene to transition to
    // Returns the current scene type if no transition is needed
    virtual SceneType getNextScene() const = 0;
    
    // Проверить завершена ли сцена
    // Returns true when scene should transition
    virtual bool isFinished() const = 0;
};

#endif // SCENE_H
