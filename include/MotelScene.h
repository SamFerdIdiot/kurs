#ifndef MOTEL_SCENE_H
#define MOTEL_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>

// Motel scene
// Purpose: Allow player to rest and manage inventory/status
// Features:
//   - Display motel room interior
//   - Rest to restore energy
//   - Save game progress
//   - Access inventory
//   - Review character stats and progress
//   - Plan next destination on map
//   - Check car status
//   - Leave motel to continue journey
class MotelScene : public Scene {
public:
    MotelScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Placeholder rectangles for motel elements
    sf::RectangleShape m_roomBackground;    // Motel room interior
    sf::RectangleShape m_bedBox;            // Bed for resting
    sf::RectangleShape m_tvBox;             // TV (news/information)
    sf::RectangleShape m_tableBox;          // Table with map
    sf::RectangleShape m_mapBox;            // Map for planning route
    sf::RectangleShape m_inventoryBox;      // Inventory access button
    sf::RectangleShape m_statsBox;          // Character stats panel
    sf::RectangleShape m_saveBox;           // Save game button
    sf::RectangleShape m_exitBox;           // Exit motel door
    sf::RectangleShape m_playerBox;         // Player character
    
    // Font and text elements
    sf::Font m_font;                        // Font for text rendering
    sf::Text m_titleText;                   // "MOTEL ROOM" title
    sf::Text m_bedText;                     // Bed action text
    sf::Text m_tvText;                      // TV action text
    sf::Text m_mapText;                     // Map action text
    sf::Text m_inventoryText;               // Inventory action text
    sf::Text m_statsText;                   // Stats display text
    sf::Text m_saveText;                    // Save action text
    sf::Text m_exitText;                    // Exit action text
    sf::Text m_instructionsText;            // Instructions text
    bool m_fontLoaded;                      // Font loading status
    
    bool m_hasRested;                       // Whether player has rested
    bool m_isFinished;                      // Scene completion flag
    SceneType m_nextScene;                  // Next scene to transition to
};

#endif // MOTEL_SCENE_H
