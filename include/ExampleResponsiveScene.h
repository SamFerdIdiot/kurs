#ifndef EXAMPLE_RESPONSIVE_SCENE_H
#define EXAMPLE_RESPONSIVE_SCENE_H

#include "Scene.h"
#include "UILayout.h"
#include <SFML/Graphics.hpp>

// ============================================================================
// ПРИМЕР ИСПОЛЬЗОВАНИЯ RESPONSIVE LAYOUT SYSTEM
// Демонстрация как использовать адаптивную разметку в сценах
// ============================================================================

class ExampleResponsiveScene : public Scene {
public:
    ExampleResponsiveScene() 
        : m_finished(false),
          m_nextScene(SceneType::NODE) {
        
        // Получение layout для этой сцены
        UILayout& layout = GET_LAYOUT("ExampleScene");
        
        // Инициализация для MacBook Air M1
        layout.setTargetResolution(
            ScreenResolution::MAC_AIR_M1_WIDTH,
            ScreenResolution::MAC_AIR_M1_HEIGHT
        );
        
        setupUI(layout);
    }
    
    void setupUI(UILayout& layout) {
        // Пример 1: Масштабирование базовых координат
        // Вместо жестко заданных координат используем масштабирование
        sf::Vector2f titlePos = layout.scale(50.0f, 30.0f);
        m_titleText.setPosition(titlePos);
        
        // Пример 2: Масштабирование размера шрифта
        unsigned int scaledFontSize = layout.scaleFont(32);
        m_titleText.setCharacterSize(scaledFontSize);
        
        // Пример 3: Использование относительных координат (процентов)
        RelativePosition centerPos(0.5f, 0.5f);  // 50%, 50% - центр экрана
        sf::Vector2f centerAbsolute = layout.fromRelative(centerPos);
        m_centerElement.setPosition(centerAbsolute);
        
        // Пример 4: Выравнивание с отступами
        Padding padding(20.0f);  // 20px со всех сторон
        sf::Vector2f buttonPos = layout.getAlignedPosition(
            200.0f,  // ширина кнопки
            50.0f,   // высота кнопки
            Alignment::BOTTOM_RIGHT,
            padding
        );
        m_closeButton.setPosition(buttonPos);
        
        // Пример 5: Создание сетки (например, для inventory)
        auto gridCells = layout.createGrid(
            4,  // 4 ряда
            6,  // 6 колонок
            Padding(100.0f, 50.0f, 100.0f, 50.0f),  // отступы
            5.0f  // расстояние между ячейками
        );
        
        // Использование ячеек сетки
        for (size_t i = 0; i < gridCells.size() && i < m_gridRects.size(); ++i) {
            m_gridRects[i].setPosition(gridCells[i].position);
            m_gridRects[i].setSize(gridCells[i].size);
        }
        
        // Пример 6: Адаптивное масштабирование контейнера
        sf::Vector2f containerSize = layout.scaleSize(700.0f, 500.0f);
        m_container.setSize(containerSize);
        
        // Центрирование контейнера
        sf::Vector2f containerPos = layout.getAlignedPosition(
            containerSize.x,
            containerSize.y,
            Alignment::CENTER
        );
        m_container.setPosition(containerPos);
    }
    
    void handleInput(const sf::Event& event) override {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                m_finished = true;
                m_nextScene = SceneType::NODE;
            }
        }
    }
    
    void update(float deltaTime) override {
        // Обновление логики
    }
    
    void render(sf::RenderWindow& window) override {
        window.draw(m_container);
        window.draw(m_titleText);
        window.draw(m_centerElement);
        window.draw(m_closeButton);
        
        for (const auto& rect : m_gridRects) {
            window.draw(rect);
        }
    }
    
    SceneType getNextScene() const override {
        return m_nextScene;
    }
    
    bool isFinished() const override {
        return m_finished;
    }
    
private:
    bool m_finished;
    SceneType m_nextScene;
    
    sf::Text m_titleText;
    sf::RectangleShape m_centerElement;
    sf::RectangleShape m_closeButton;
    sf::RectangleShape m_container;
    std::vector<sf::RectangleShape> m_gridRects{24};  // 4x6 сетка
};

// ============================================================================
// ПРИМЕРЫ ИСПОЛЬЗОВАНИЯ В СУЩЕСТВУЮЩИХ СЦЕНАХ
// ============================================================================

/*
// В InventoryScene.cpp:

void InventoryScene::setupUI() {
    UILayout& layout = GET_LAYOUT("InventoryScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Вместо:
    // m_titleText.setPosition(100.0f, 25.0f);
    // m_titleText.setCharacterSize(32);
    
    // Используем:
    m_titleText.setPosition(SCALE_POS(layout, 100.0f, 25.0f));
    m_titleText.setCharacterSize(SCALE_FONT(layout, 32));
    
    // Создание адаптивной сетки инвентаря
    auto gridCells = layout.createGrid(4, 6, Padding(100.0f, 50.0f), 5.0f);
    for (size_t i = 0; i < gridCells.size(); ++i) {
        m_gridCells[i].setPosition(gridCells[i].position);
        m_gridCells[i].setSize(gridCells[i].size);
    }
}

// В CharacterScene.cpp:

void CharacterScene::setupUI() {
    UILayout& layout = GET_LAYOUT("CharacterScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Заголовок в верхней части с отступом
    sf::Vector2f titlePos = layout.getAlignedPosition(
        400.0f, 50.0f,
        Alignment::TOP_CENTER,
        Padding(20.0f)
    );
    m_titleText.setPosition(titlePos);
    
    // Информация о персонаже в центре слева
    sf::Vector2f infoPos = layout.getAlignedPosition(
        300.0f, 400.0f,
        Alignment::CENTER_LEFT,
        Padding(0.0f, 50.0f, 0.0f, 50.0f)
    );
    m_infoPanel.setPosition(infoPos);
}

// В CompanionsScene.cpp:

void CompanionsScene::setupUI() {
    UILayout& layout = GET_LAYOUT("CompanionsScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Автоматическое выравнивание слотов компаньонов
    float slotHeight = 100.0f;
    float spacing = 20.0f;
    
    for (int i = 0; i < m_maxCompanions; ++i) {
        float yOffset = i * (slotHeight + spacing);
        sf::Vector2f slotPos = layout.scale(50.0f, 150.0f + yOffset);
        m_companionSlots[i].setPosition(slotPos);
    }
}
*/

#endif // EXAMPLE_RESPONSIVE_SCENE_H
