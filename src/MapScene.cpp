#include "MapScene.h"
#include <iostream>
#include <cmath>

MapScene::MapScene()
    : m_finished(false),
      m_nextScene(SceneType::WORLD_MAP),
      m_hudPanel(1366.0f, 768.0f) {  // Initialize HUD with full window size
    
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("MapScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Setup background with responsive sizing
    m_background.setSize(sf::Vector2f(layout.getTargetWidth(), layout.getTargetHeight()));
    m_background.setFillColor(sf::Color(40, 40, 60));
    
    // Setup UI text with responsive scaling
    m_titleText.setString("Map");
    m_titleText.setCharacterSize(SCALE_FONT(layout, 32));
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(SCALE_POS(layout, 600.0f, 50.0f));
    
    m_instructionText.setString("Select destination\nESC - Return");
    m_instructionText.setCharacterSize(SCALE_FONT(layout, 20));
    m_instructionText.setFillColor(sf::Color::White);
    m_instructionText.setPosition(SCALE_POS(layout, 400.0f, 650.0f));
    
    initializeMap();
}

void MapScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            m_finished = true;
            m_nextScene = SceneType::NODE;  // Return to node scene
        }
    } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), 
                                 static_cast<float>(event.mouseButton.y));
            handleNodeSelection(mousePos);
        }
    } else if (event.type == sf::Event::MouseMoved) {
        m_mousePosition = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    }
}

void MapScene::update(float deltaTime) {
    // Update HUD with example values
    m_hudPanel.setMoney(1250.0f);
    m_hudPanel.setFuel(65.0f, 100.0f);
    m_hudPanel.setEnergy(75.0f, 100.0f);
    m_hudPanel.setCargo(20.0f, 64.0f);
    m_hudPanel.updateTopRight(deltaTime);
}

void MapScene::render(sf::RenderWindow& window) {
    window.draw(m_background);
    
    renderConnections(window);
    renderNodes(window);
    
    window.draw(m_titleText);
    window.draw(m_instructionText);
    
    // Render HUD
    m_hudPanel.render(window);
}

SceneType MapScene::getNextScene() const {
    return m_nextScene;
}

bool MapScene::isFinished() const {
    return m_finished;
}

void MapScene::setCurrentNode(const std::string& nodeId) {
    m_currentNodeId = nodeId;
    
    // Update current node flag
    for (auto& node : m_nodes) {
        node.isCurrentNode = (node.id == nodeId);
    }
}

void MapScene::addAvailableNode(const MapNode& node) {
    m_nodes.push_back(node);
}

void MapScene::initializeMap() {
    // Initialize with one test node (current location)
    MapNode startNode("node_start", "Начальная точка / Start", 
                     sf::Vector2f(683.0f, 384.0f), true, true);
    m_nodes.push_back(startNode);
    
    // Add one destination node (for future travel)
    MapNode nextNode("node_next", "Следующая точка / Next", 
                    sf::Vector2f(900.0f, 300.0f), true, false);
    m_nodes.push_back(nextNode);
    
    m_currentNodeId = "node_start";
}

void MapScene::renderNodes(sf::RenderWindow& window) {
    for (const auto& node : m_nodes) {
        sf::CircleShape nodeShape(20.0f);
        nodeShape.setOrigin(20.0f, 20.0f);
        nodeShape.setPosition(node.position);
        
        // Color based on state
        if (node.isCurrentNode) {
            nodeShape.setFillColor(sf::Color::Green);  // Current location
        } else if (node.isUnlocked) {
            nodeShape.setFillColor(sf::Color::Yellow);  // Available
        } else {
            nodeShape.setFillColor(sf::Color(100, 100, 100));  // Locked
        }
        
        window.draw(nodeShape);
        
        // Draw node name
        sf::Text nodeText;
        nodeText.setString(node.name);
        nodeText.setCharacterSize(16);
        nodeText.setFillColor(sf::Color::White);
        nodeText.setPosition(node.position.x - 40.0f, node.position.y + 25.0f);
        window.draw(nodeText);
    }
}

void MapScene::renderConnections(sf::RenderWindow& window) {
    // Draw lines between current node and available nodes
    if (m_nodes.size() < 2) return;
    
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        if (m_nodes[i].isCurrentNode) {
            for (size_t j = 0; j < m_nodes.size(); ++j) {
                if (i != j && m_nodes[j].isUnlocked) {
                    sf::Vertex line[] = {
                        sf::Vertex(m_nodes[i].position, sf::Color(150, 150, 150)),
                        sf::Vertex(m_nodes[j].position, sf::Color(150, 150, 150))
                    };
                    window.draw(line, 2, sf::Lines);
                }
            }
            break;
        }
    }
}

void MapScene::handleNodeSelection(const sf::Vector2f& mousePos) {
    // Check if clicked on any available node
    for (const auto& node : m_nodes) {
        float dx = mousePos.x - node.position.x;
        float dy = mousePos.y - node.position.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance <= 20.0f && node.isUnlocked && !node.isCurrentNode) {
            std::cout << "Selected destination: " << node.name << std::endl;
            m_selectedNodeId = node.id;
            
            // Transition to cinematic mode (placeholder)
            m_finished = true;
            m_nextScene = SceneType::MAIN_MENU;  // Temp: return to menu
            break;
        }
    }
}
