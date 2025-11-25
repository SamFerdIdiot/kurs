#ifndef SIDE_VIEW_SCENE_H
#define SIDE_VIEW_SCENE_H

#include "Scene.h"
#include "PlayerState.h"
#include "InteractionMenu.h"
#include "UI/HUDPanel.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// Forward declaration
class StructurePlacement;

// SideViewScene - Full side-scrolling game scene with layered rendering
// Purpose: Main gameplay scene with side view perspective
// Features:
//   - Layered rendering system (background, structures, vehicles, road, player, HUD, menu)
//   - Parallax scrolling for depth effect
//   - Camera tracking following player movement
//   - Automatic structure generation via StructurePlacement
//   - Integration with HUD panel
//   - Support for oncoming and passing vehicles
//   - Popup interaction menu
class SideViewScene : public Scene {
public:
    SideViewScene();
    ~SideViewScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Scene state
    bool m_isFinished;
    SceneType m_nextScene;
    
    // Camera position (tracks player movement)
    float m_cameraX;
    
    // Player state and resources
    PlayerState m_playerState;
    
    // Player car properties
    float m_playerCarX;      // Player car X position in world space
    float m_playerCarY;      // Player car Y position (screen space, fixed)
    float m_playerSpeed;     // Current player speed
    
    // === Layer 1: Background (furthest, slowest parallax) ===
    sf::RectangleShape m_backgroundSky;
    sf::RectangleShape m_backgroundMountains;
    float m_backgroundParallaxSpeed;
    
    // === Layer 2: Second Background (medium parallax) ===
    sf::RectangleShape m_secondBackgroundHills;
    sf::RectangleShape m_secondBackgroundTrees;
    float m_secondBackgroundParallaxSpeed;
    
    // === Layer 3: Structures (slower than road, generated via StructurePlacement) ===
    std::unique_ptr<StructurePlacement> m_structurePlacement;
    
    // === Layer 4: Oncoming and Passing Vehicles ===
    struct Vehicle {
        float x;              // X position in world space
        float y;              // Y position in screen space
        float speed;          // Relative speed to player
        bool isOncoming;      // True if oncoming, false if passing
        sf::RectangleShape shape;
    };
    std::vector<Vehicle> m_vehicles;
    
    // === Layer 5: Road and Player Car ===
    sf::RectangleShape m_road;
    sf::RectangleShape m_roadLine1;
    sf::RectangleShape m_roadLine2;
    sf::RectangleShape m_playerCar;
    
    // === Layer 6: HUD ===
    std::unique_ptr<HUDPanel> m_hudPanel;
    
    // === Layer 7: Popup Menu ===
    std::unique_ptr<InteractionMenu> m_interactionMenu;
    
    // Font for debug text
    sf::Font m_font;
    bool m_fontLoaded;
    
    // Вспомогательные методы
    void initializeLayers();
    void initializePlayerCar();
    void initializeVehicles();
    
    void updateCamera(float deltaTime);
    void updatePlayerMovement(float deltaTime);
    void updateVehicles(float deltaTime);
    void updateHUD();
    
    void renderBackground(sf::RenderWindow& window);
    void renderSecondBackground(sf::RenderWindow& window);
    void renderStructures(sf::RenderWindow& window);
    void renderVehicles(sf::RenderWindow& window);
    void renderRoadAndPlayer(sf::RenderWindow& window);
    void renderHUD(sf::RenderWindow& window);
    void renderMenu(sf::RenderWindow& window);
    
    void checkNearbyStructures();
    void handleInteractionMenuAction(const std::string& action);
    
    // Calculate parallax offset for a layer
    float calculateParallaxOffset(float parallaxSpeed) const;
};

#endif // SIDE_VIEW_SCENE_H
