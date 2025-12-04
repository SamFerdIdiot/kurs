#ifndef NODE_SCENE_H
#define NODE_SCENE_H

#include "Scene.h"
#include "Structure.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>

// NodeScene - Tab-based menu navigation in a node (city)
// Features:
// - Top navigation bar with abbreviated tab names (3 letters)
// - Dynamic structure tabs based on available structures
// - A/D for page navigation, W/S for selection (if applicable)
// - Enter to confirm, Esc to exit
//
// Tab structure:
// [CHR] [INV] [COM] [QST] [ABL] [MAP] [GAS] [STR] [MOT] [  ] [  ]
//
// Pages:
// 1. CHR - Character screen
// 2. INV - Inventory
// 3. COM - Companions
// 4. QST - Quests
// 5. ABL - Abilities
// 6. MAP - World Map
// 7+ GAS/STR/MOT/etc - Structures (dynamically loaded)
class NodeScene : public Scene {
public:
    NodeScene(const std::string& nodeId, const std::string& nodeName);
    ~NodeScene() override = default;

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    SceneType getNextScene() const override;
    bool isFinished() const override;

private:
    // Tab structure
    struct Tab {
        std::string name;           // 3-letter abbreviation (CHR, INV, etc.)
        std::string fullName;       // Full name (Character, Inventory, etc.)
        std::string description;    // Description shown on page
        SceneType targetScene;      // Scene to transition to
        LocationType locationType;  // For structure tabs (GAS_STATION, etc.)
        bool isStructure;           // True if this is a structure tab

        Tab() : targetScene(SceneType::NODE), locationType(LocationType::UNKNOWN), isStructure(false) {}
    };

    // Initialize tabs (fixed + dynamic structures)
    void initializeTabs();

    // Load available structures from current node
    void loadStructures();

    // Navigation
    void selectPreviousTab();
    void selectNextTab();
    void confirmSelection();

    // Rendering
    void renderTopBar(sf::RenderWindow& window);
    void renderPageContent(sf::RenderWindow& window);
    void renderStatusBar(sf::RenderWindow& window);
    void renderControls(sf::RenderWindow& window);

    // Node information
    std::string m_nodeId;
    std::string m_nodeName;

    // Tab system
    std::vector<Tab> m_tabs;
    int m_currentTabIndex;
    const int MAX_TABS = 12;  // Maximum tabs to display

    // Font for rendering
    sf::Font m_font;
    bool m_fontLoaded;

    // Scene state
    bool m_finished;
    SceneType m_nextScene;

    // Visual elements
    sf::RectangleShape m_background;

    // Layout constants (1300x800)
    static constexpr float TOP_BAR_HEIGHT = 60.0f;
    static constexpr float STATUS_BAR_HEIGHT = 80.0f;
    static constexpr float TAB_WIDTH = 80.0f;
    static constexpr float TAB_SPACING = 10.0f;
    static constexpr float CONTENT_PADDING = 40.0f;
};

#endif // NODE_SCENE_H
