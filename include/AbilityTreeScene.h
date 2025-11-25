#ifndef ABILITY_TREE_SCENE_H
#define ABILITY_TREE_SCENE_H

#include "Scene.h"
#include "AbilityTreeSystem.h"
#include "AbilityTypes.h"
#include "ExperienceSystem.h"
#include "UILayout.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// AbilityTreeScene - Pip-Boy style terminal interface for ability tree
// Features:
// - Monochrome green terminal aesthetic (#00FF00 on #000000)
// - ASCII art tree structure
// - Category-based navigation (1-5 keys)
// - Shows locked/available/unlocked states with symbols [✓][ ][✗]
// - Minimal assets: 1 font + simple sounds
class AbilityTreeScene : public Scene {
public:
    AbilityTreeScene();
    ~AbilityTreeScene() override = default;
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // Setup methods
    void setupUI();
    void setupTerminalStyle();
    void loadAbilityData();
    
    // Navigation
    void switchCategory(AbilityCategory category);
    void navigateUp();
    void navigateDown();
    void selectAbility();
    void attemptUnlock();
    void goBack();
    
    // Rendering helpers
    void renderTerminalBorder(sf::RenderWindow& window);
    void renderHeader(sf::RenderWindow& window);
    void renderCategoryList(sf::RenderWindow& window);
    void renderAbilityTree(sf::RenderWindow& window);
    void renderDetailPanel(sf::RenderWindow& window);
    void renderFooter(sf::RenderWindow& window);
    
    // Tree rendering
    std::string getAbilityStateSymbol(const AbilityNode* ability) const;
    std::string getAbilityLine(const AbilityNode* ability, int indentLevel) const;
    sf::Color getAbilityColor(const AbilityNode* ability) const;
    
    // State
    bool m_isFinished;
    SceneType m_nextScene;
    AbilityCategory m_currentCategory;
    int m_selectedIndex;
    std::vector<const AbilityNode*> m_displayedAbilities;
    const AbilityNode* m_selectedAbility;
    
    // UI elements - Terminal style
    sf::Font m_terminalFont;
    bool m_fontLoaded;
    
    // Colors - Pip-Boy monochrome green
    sf::Color m_colorBackground;    // #000000 Black
    sf::Color m_colorText;          // #00FF00 Bright green
    sf::Color m_colorTextDim;       // #007700 Dim green
    sf::Color m_colorHighlight;     // #00FF00 Bright green
    sf::Color m_colorLocked;        // #004400 Very dim green
    sf::Color m_colorBorder;        // #00AA00 Medium green
    
    // Layout
    const float WINDOW_WIDTH = 1366.0f;
    const float WINDOW_HEIGHT = 768.0f;
    const float BORDER_THICKNESS = 3.0f;
    const float PADDING = 20.0f;
    const float LINE_HEIGHT = 25.0f;
    const unsigned int CHAR_SIZE = 18;
    
    // Terminal blink effect
    float m_blinkTimer;
    bool m_blinkState;
    
    // Cached data
    int m_playerLevel;
    int m_availableSkillPoints;
};

#endif // ABILITY_TREE_SCENE_H
