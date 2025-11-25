#ifndef MAP_SCENE_H
#define MAP_SCENE_H

#include "Scene.h"
#include "UI/HUDPanel.hpp"
#include "UILayout.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Точка на карте (нода)
struct MapNode {
    std::string id;
    std::string name;
    sf::Vector2f position;  // Позиция на UI карты
    bool isUnlocked;        // Доступна ли
    bool isCurrentNode;     // Текущая нода
    
    MapNode()
        : position(0.0f, 0.0f),
          isUnlocked(false),
          isCurrentNode(false) {}
          
    MapNode(const std::string& nodeId, const std::string& nodeName,
            const sf::Vector2f& pos, bool unlocked = false, bool current = false)
        : id(nodeId), name(nodeName), position(pos),
          isUnlocked(unlocked), isCurrentNode(current) {}
};

// MapScene - Сцена карты для выбора следующей локации
// Показывает доступные ноды и позволяет выбрать направление
class MapScene : public Scene {
public:
    MapScene();
    ~MapScene() override = default;
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
    // Установить текущую ноду
    void setCurrentNode(const std::string& nodeId);
    
    // Добавить доступную ноду
    void addAvailableNode(const MapNode& node);
    
private:
    void initializeMap();
    void renderNodes(sf::RenderWindow& window);
    void renderConnections(sf::RenderWindow& window);
    void handleNodeSelection(const sf::Vector2f& mousePos);
    
    // Map data
    std::vector<MapNode> m_nodes;
    std::string m_currentNodeId;
    std::string m_selectedNodeId;
    
    // UI elements
    sf::RectangleShape m_background;
    sf::Font m_font;
    sf::Text m_titleText;
    sf::Text m_instructionText;
    
    // Scene state
    bool m_finished;
    SceneType m_nextScene;
    
    // Mouse
    sf::Vector2i m_mousePosition;
    
    // HUD Panel for displaying resources
    HUDPanel m_hudPanel;
};

#endif // MAP_SCENE_H
