#ifndef CHARACTER_SCENE_H
#define CHARACTER_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>

// CharacterScene - Отображение характеристик персонажа и прогрессии
// Показывает имя, предысторию, уровень, опыт, навыки, активные эффекты
class CharacterScene : public Scene {
public:
    CharacterScene();
    ~CharacterScene() override = default;
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
    bool isFinished() const override { return m_finished; }
    SceneType getNextScene() const override { return m_nextScene; }
    
private:
    // Инициализация UI элементов
    void setupUI();
    
    // Обновление отображаемых данных персонажа
    void updateCharacterDisplay();
    
    // Рендер полосы прогресса опыта
    void renderExperienceBar(sf::RenderWindow& window, float x, float y, float width, float height, float current, float max);
    
    // Рендер индикатора навыка в виде ячеек (5/10 стиль)
    void renderSkillCells(sf::RenderWindow& window, float x, float y, float percentage, const std::string& label);
    
    // Состояние сцены
    bool m_finished;
    SceneType m_nextScene;
    
    // Шрифт
    sf::Font m_font;
    bool m_fontLoaded;
    
    // UI элементы
    sf::RectangleShape m_background;
    std::optional<sf::Text> m_titleText;
    std::optional<sf::Text> m_nameText;
    std::optional<sf::Text> m_backstoryText;
    std::optional<sf::Text> m_levelText;
    std::optional<sf::Text> m_experienceText;
    std::optional<sf::Text> m_statsHeaderText;
    std::optional<sf::Text> m_effectsHeaderText;
    std::optional<sf::Text> m_controlsText;
    
    // Полоса опыта
    sf::RectangleShape m_experienceBarBackground;
    sf::RectangleShape m_experienceBarFill;
    
    // Данные персонажа
    std::string m_playerName;
    std::string m_backstory;
    int m_level;
    int m_currentExp;
    int m_maxExp;
    float m_drivingSkill;
    float m_charisma;
    float m_luck;
    
    // Активные эффекты
    std::vector<std::pair<std::string, bool>> m_activeEffects; // название эффекта, положительный/отрицательный
};

#endif // CHARACTER_SCENE_H
