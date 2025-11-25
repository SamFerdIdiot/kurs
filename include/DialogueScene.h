#ifndef DIALOGUE_SCENE_H
#define DIALOGUE_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>

// Сцена диалога
// Purpose: Display conversations with NPCs and story events
// Features:
//   - Display NPC portrait/character
//   - Show dialogue text with typewriter effect
//   - Present dialogue choices (multiple choice responses)
//   - Track conversation branches
//   - Display player's dialogue history
//   - Show relationship/reputation effects
//   - Quest acceptance/completion dialogues
//   - Return to previous scene after dialogue ends
class DialogueScene : public Scene {
public:
    DialogueScene();
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Placeholder rectangles for dialogue elements
    sf::RectangleShape m_backgroundBox;     // Dialogue background/backdrop
    sf::RectangleShape m_npcPortraitBox;    // NPC portrait/sprite
    sf::RectangleShape m_dialogueBox;       // Main dialogue text box
    sf::RectangleShape m_nameBox;           // NPC name display
    sf::RectangleShape m_choice1Box;        // Dialogue choice 1
    sf::RectangleShape m_choice2Box;        // Dialogue choice 2
    sf::RectangleShape m_choice3Box;        // Dialogue choice 3
    sf::RectangleShape m_relationshipBox;   // Relationship indicator
    sf::RectangleShape m_continueBox;       // Continue/next button
    
    // Font and text elements
    sf::Font m_font;                        // Font for text rendering
    sf::Text m_npcNameText;                 // NPC name text
    sf::Text m_dialogueText;                // Main dialogue text
    sf::Text m_choice1Text;                 // Choice 1 text
    sf::Text m_choice2Text;                 // Choice 2 text
    sf::Text m_choice3Text;                 // Choice 3 text
    sf::Text m_relationshipText;            // Relationship text
    sf::Text m_continueText;                // Continue prompt text
    bool m_fontLoaded;                      // Font loading status
    
    int m_dialogueState;                    // Current dialogue state/node
    int m_selectedChoice;                   // Selected dialogue choice
    bool m_showingChoices;                  // Whether showing choice options
    bool m_isFinished;                      // Scene completion flag
    SceneType m_nextScene;                  // Next scene to transition to
};

#endif // DIALOGUE_SCENE_H
