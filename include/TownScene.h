#ifndef TOWN_SCENE_H
#define TOWN_SCENE_H

#include "Scene.h"
#include "TownMenu.h"
#include "PlayerState.h"
#include <SFML/Graphics.hpp>
#include <memory>

// Town scene
// Purpose: Allow player to interact with town locations and NPCs
// Features:
//   - Display town overview/map
//   - Show available locations (shop, garage, restaurant, people)
//   - Navigate between locations
//   - Refuel car at gas station
//   - Buy supplies at shop
//   - Repair car at garage
//   - Rest and eat at restaurant
//   - Talk to NPCs for quests/information
//   - Exit town to continue driving
class TownScene : public Scene {
public:
    TownScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Placeholder rectangles for town elements
    sf::RectangleShape m_backgroundBox;     // Town background/skyline
    sf::RectangleShape m_shopBox;           // Shop building
    sf::RectangleShape m_garageBox;         // Garage building
    sf::RectangleShape m_restaurantBox;     // Restaurant building
    sf::RectangleShape m_gasStationBox;     // Gas station
    sf::RectangleShape m_npc1Box;           // NPC 1 (quest giver)
    sf::RectangleShape m_npc2Box;           // NPC 2 (information)
    sf::RectangleShape m_npc3Box;           // NPC 3 (trader)
    sf::RectangleShape m_playerBox;         // Player character
    sf::RectangleShape m_exitSignBox;       // Exit town sign
    sf::RectangleShape m_selectionBox;      // Location selection indicator
    
    // Font and text elements
    sf::Font m_font;                        // Font for text rendering
    sf::Text m_titleText;                   // Town title text
    sf::Text m_shopText;                    // Shop label
    sf::Text m_garageText;                  // Garage label
    sf::Text m_restaurantText;              // Restaurant label
    sf::Text m_gasStationText;              // Gas station label
    sf::Text m_npc1Text;                    // NPC 1 label
    sf::Text m_npc2Text;                    // NPC 2 label
    sf::Text m_npc3Text;                    // NPC 3 label
    sf::Text m_exitText;                    // Exit label
    sf::Text m_instructionsText;            // Instructions text
    bool m_fontLoaded;                      // Font loading status
    
    int m_selectedLocation;                 // Currently selected location
    bool m_isFinished;                      // Scene completion flag
    SceneType m_nextScene;                  // Next scene to transition to
    
    // Town menu and player state
    std::unique_ptr<TownMenu> m_townMenu;   // Town services menu
    PlayerState m_playerState;              // Player state and resources
    bool m_showMenu;                        // Flag to show town menu
};

#endif // TOWN_SCENE_H
