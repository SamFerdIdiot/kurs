#ifndef COMPANIONS_SCENE_H
#define COMPANIONS_SCENE_H

#include "Scene.h"
#include "UILayout.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// CompanionsScene - Управление группой и компаньонами
// Отображает слоты для компаньонов (зависит от вместимости машины)
// Показывает информацию о компаньонах, их мораль и способности
class CompanionsScene : public Scene {
public:
    CompanionsScene();
    ~CompanionsScene() override = default;
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
    bool isFinished() const override { return m_finished; }
    SceneType getNextScene() const override { return m_nextScene; }
    
private:
    // Структура информации о компаньоне
    struct CompanionInfo {
        std::string name;
        std::string role;
        std::string ability;
        float morale; // 0-100%
        bool isRecruited;
        
        CompanionInfo() : name("Empty"), role(""), ability(""), morale(0.0f), isRecruited(false) {}
        CompanionInfo(const std::string& n, const std::string& r, const std::string& a, float m)
            : name(n), role(r), ability(a), morale(m), isRecruited(true) {}
    };
    
    // Инициализация UI
    void setupUI();
    
    // Обновление данных компаньонов
    void updateCompanionsDisplay();
    
    // Навигация по слотам
    void selectPrevious();
    void selectNext();
    
    // Рендер индикатора морали в виде ячеек
    void renderMoraleCells(sf::RenderWindow& window, float x, float y, float morale);
    
    // Рендер одного слота компаньона
    void renderCompanionSlot(sf::RenderWindow& window, int slotIndex, float y, bool isSelected);
    
    // Состояние сцены
    bool m_finished;
    SceneType m_nextScene;
    
    // Шрифт
    sf::Font m_font;
    bool m_fontLoaded;
    
    // UI элементы
    sf::RectangleShape m_background;
    sf::Text m_titleText;
    sf::Text m_vehicleInfoText;
    sf::Text m_capacityText;
    sf::Text m_controlsText;
    sf::RectangleShape m_selectionHighlight;
    
    // Данные о компаньонах
    std::vector<CompanionInfo> m_companions;
    int m_maxCompanions;
    int m_selectedSlot;
    
    // Информация о машине
    std::string m_vehicleName;
};

#endif // COMPANIONS_SCENE_H
