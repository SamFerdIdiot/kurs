#ifndef NODE_SCENE_H
#define NODE_SCENE_H

#include "Scene.h"
#include "UI/HUDPanel.hpp"
#include "Structure.h"
#include "UILayout.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

// Структура знака для путешествия к следующей ноде
struct TravelSign {
    sf::Vector2f position;          // Позиция знака на карте
    std::string destinationNodeId;  // ID следующей ноды
    bool isPlayerNear;              // Рядом ли игрок
    float interactionDistance;      // Дистанция взаимодействия
    
    TravelSign()
        : position(0.0f, 0.0f),
          isPlayerNear(false),
          interactionDistance(50.0f) {}
};

// NodeScene - Menu navigation in a node (city)
// Simple menu interface with arrow key navigation
// Navigate with arrows, select with Enter
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
    // Menu page structure
    struct MenuPage {
        std::string title;
        std::string structureType;  // "gas_station", "store", "motel", "travel"
        LocationType locationType;   // For matching with structures
        std::vector<std::string> options;
        std::vector<std::string> descriptions;
    };
    
    void initializePages();
    void selectPrevious();
    void selectNext();
    void confirmSelection();
    void executeAction(int pageIndex, int optionIndex);
    
    // Node information
    std::string m_nodeId;
    std::string m_nodeName;
    
    // Page-based menu system
    std::vector<MenuPage> m_pages;
    int m_currentPageIndex;
    int m_selectedOptionIndex;
    
    // Font for rendering
    sf::Font m_font;
    bool m_fontLoaded;
    
    // Scene state
    bool m_finished;
    SceneType m_nextScene;
    
    // Visual elements
    sf::Text m_titleText;
    sf::Text m_pageIndicatorText;
    std::vector<sf::Text> m_optionTexts;
    std::vector<sf::Text> m_descriptionTexts;
    sf::RectangleShape m_selectionHighlight;
    sf::RectangleShape m_background;
};

#endif // NODE_SCENE_H
