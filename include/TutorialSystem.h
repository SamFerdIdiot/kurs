#ifndef TUTORIAL_SYSTEM_H
#define TUTORIAL_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// TutorialStep represents a single tutorial message/hint
struct TutorialStep {
    std::string message;
    float displayDuration;  // Time to display (0 = until dismissed)
    bool dismissed;
    
    TutorialStep(const std::string& msg, float duration = 0.0f)
        : message(msg), displayDuration(duration), dismissed(false) {}
};

// TutorialSystem - Manages first-day tutorial and contextual hints
// Purpose: Guide new players through initial gameplay mechanics
// Features:
//   - Sequential tutorial steps
//   - Contextual hints based on game state
//   - Non-intrusive message display
//   - Skip/dismiss functionality
class TutorialSystem {
public:
    TutorialSystem();
    
    // Start the tutorial sequence
    void startTutorial();
    
    // Check if tutorial is active
    bool isTutorialActive() const { return m_tutorialActive; }
    
    // Update tutorial state
    void update(float deltaTime);
    
    // Render current tutorial message
    void render(sf::RenderWindow& window);
    
    // Handle input (dismiss, skip)
    bool handleInput(const sf::Event& event);
    
    // Mark tutorial step as complete and advance
    void completeCurrentStep();
    
    // Skip entire tutorial
    void skipTutorial();
    
    // Show a contextual hint (outside of tutorial)
    void showHint(const std::string& message, float duration = 5.0f);
    
    // Load font for text rendering
    bool loadFont(const std::string& fontPath);
    
private:
    bool m_tutorialActive;
    size_t m_currentStepIndex;
    std::vector<TutorialStep> m_tutorialSteps;
    float m_currentStepTimer;
    
    // Contextual hint system
    std::string m_currentHint;
    float m_hintTimer;
    
    // Visual elements
    sf::Font m_font;
    bool m_fontLoaded;
    sf::RectangleShape m_messageBox;
    sf::Text m_messageText;
    sf::Text m_dismissText;
    
    // Helper functions
    void initializeTutorialSteps();
    void updateVisuals();
};

#endif // TUTORIAL_SYSTEM_H
