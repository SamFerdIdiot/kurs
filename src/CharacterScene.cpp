#include "CharacterScene.h"
#include "GameStateManager.h"
#include "GameTypes.h"
#include "ExperienceSystem.h"
#include <iostream>
#include <sstream>
#include <iomanip>

CharacterScene::CharacterScene()
    : m_finished(false),
      m_nextScene(SceneType::CHARACTER),
      m_fontLoaded(false),
      m_level(1),
      m_currentExp(0),
      m_maxExp(100),
      m_drivingSkill(50.0f),
      m_charisma(50.0f),
      m_luck(50.0f) {
    
    // Загрузка шрифта с fallback опциями
    if (m_font.loadFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.loadFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "CharacterScene: WARNING - Could not load any font!" << std::endl;
    }
    
    setupUI();
    updateCharacterDisplay();
}

void CharacterScene::setupUI() {
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("CharacterScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Фон with responsive sizing
    m_background.setSize(sf::Vector2f(layout.getTargetWidth(), layout.getTargetHeight()));
    m_background.setFillColor(sf::Color(30, 30, 46)); // Dark blue-gray (#1E1E2E)
    m_background.setPosition(0.0f, 0.0f);
    
    // Заголовок with responsive scaling
    m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(SCALE_FONT(layout, 36));
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(SCALE_POS(layout, 100.0f, 40.0f));
    m_titleText.setString("CHARACTER INFORMATION");
    
    // Текст имени with responsive scaling
    m_nameText.setFont(m_font);
    m_nameText.setCharacterSize(SCALE_FONT(layout, 24));
    m_nameText.setFillColor(sf::Color(255, 165, 0)); // Orange accent
    m_nameText.setPosition(SCALE_POS(layout, 100.0f, 110.0f));
    
    // Текст предыстории with responsive scaling
    m_backstoryText.setFont(m_font);
    m_backstoryText.setCharacterSize(SCALE_FONT(layout, 20));
    m_backstoryText.setFillColor(sf::Color(204, 204, 204)); // Light gray
    m_backstoryText.setPosition(SCALE_POS(layout, 100.0f, 150.0f));
    
    // Текст уровня with responsive scaling
    m_levelText.setFont(m_font);
    m_levelText.setCharacterSize(SCALE_FONT(layout, 22));
    m_levelText.setFillColor(sf::Color::White);
    m_levelText.setPosition(SCALE_POS(layout, 100.0f, 195.0f));
    
    // Текст опыта with responsive scaling
    m_experienceText.setFont(m_font);
    m_experienceText.setCharacterSize(SCALE_FONT(layout, 18));
    m_experienceText.setFillColor(sf::Color(204, 204, 204));
    m_experienceText.setPosition(SCALE_POS(layout, 100.0f, 230.0f));
    
    // Полоса опыта with responsive scaling
    m_experienceBarBackground.setSize(SCALE_SIZE(layout, 1000.0f, 30.0f));
    m_experienceBarBackground.setPosition(SCALE_POS(layout, 100.0f, 265.0f));
    m_experienceBarBackground.setFillColor(sf::Color(62, 62, 78)); // Medium gray
    m_experienceBarBackground.setOutlineColor(sf::Color(94, 126, 160)); // Light blue
    m_experienceBarBackground.setOutlineThickness(2.0f);
    
    // Полоса опыта - заполнение - adjusted
    m_experienceBarFill.setPosition(100.0f, 265.0f);
    m_experienceBarFill.setFillColor(sf::Color(76, 175, 80)); // Success green
    
    // Заголовок статистики - repositioned
    m_statsHeaderText.setFont(m_font);  // Always set font
    m_statsHeaderText.setCharacterSize(28);
    m_statsHeaderText.setFillColor(sf::Color::White);
    m_statsHeaderText.setPosition(100.0f, 320.0f);
    m_statsHeaderText.setString("STATS:");
    
    // Заголовок эффектов - repositioned
    m_effectsHeaderText.setFont(m_font);  // Always set font
    m_effectsHeaderText.setCharacterSize(28);
    m_effectsHeaderText.setFillColor(sf::Color::White);
    m_effectsHeaderText.setPosition(100.0f, 520.0f);
    m_effectsHeaderText.setString("ACTIVE EFFECTS:");
    
    // Подсказка управления - repositioned for full window
    m_controlsText.setFont(m_font);  // Always set font
    m_controlsText.setCharacterSize(16);
    m_controlsText.setFillColor(sf::Color(150, 150, 150));
    m_controlsText.setPosition(100.0f, 700.0f);
    m_controlsText.setString("ESC - Back to Node");
}

void CharacterScene::updateCharacterDisplay() {
    // Получение данных из PlayerState и GameStateManager
    PlayerState& playerState = GameStateManager::getInstance().getPlayerState();
    
    // Имя игрока (пока заглушка, так как Character не хранит имя)
    m_playerName = "Player";
    m_nameText.setString("Name: " + m_playerName);
    
    // Предыстория из OriginType
    OriginType origin = playerState.getOrigin();
    m_backstory = getOriginTypeName(origin);
    m_backstoryText.setString("Backstory: " + m_backstory);
    
    // Уровень и опыт из ExperienceSystem
    ExperienceSystem& expSystem = ExperienceSystem::getInstance();
    m_level = expSystem.getLevel();
    m_currentExp = expSystem.getExperience();
    m_maxExp = expSystem.getExperienceForNextLevel();
    
    m_levelText.setString("Level: " + std::to_string(m_level));
    m_experienceText.setString("XP: " + std::to_string(m_currentExp) + "/" + std::to_string(m_maxExp));
    
    // Обновление полосы опыта - adjusted for wider bar
    float expPercentage = static_cast<float>(m_currentExp) / static_cast<float>(m_maxExp);
    m_experienceBarFill.setSize(sf::Vector2f(1000.0f * expPercentage, 30.0f));
    
    // Навыки - зависят от предыстории
    switch (origin) {
        case OriginType::EX_RACER:
            m_drivingSkill = 80.0f;
            m_charisma = 50.0f;
            m_luck = 70.0f;
            break;
        case OriginType::HITCHHIKER:
            m_drivingSkill = 50.0f;
            m_charisma = 80.0f;
            m_luck = 60.0f;
            break;
        case OriginType::NOMAD:
            m_drivingSkill = 60.0f;
            m_charisma = 40.0f;
            m_luck = 80.0f;
            break;
        default:
            m_drivingSkill = 50.0f;
            m_charisma = 50.0f;
            m_luck = 50.0f;
            break;
    }
    
    // Активные эффекты (пример)
    m_activeEffects.clear();
    if (playerState.getEnergy() > 75.0f) {
        m_activeEffects.push_back({"+ Well Rested (+10% energy regen)", true});
    }
    if (playerState.getEnergy() < 30.0f) {
        m_activeEffects.push_back({"- Tired (-5% max energy)", false});
    }
}

void CharacterScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            m_finished = true;
            m_nextScene = SceneType::NODE;
        }
    }
}

void CharacterScene::update(float deltaTime) {
    // Можно добавить анимации или обновление данных в реальном времени
}

void CharacterScene::renderSkillCells(sf::RenderWindow& window, float x, float y, float percentage, const std::string& label) {
    // Рендер навыка в виде ячеек (10 ячеек всего)
    const int totalCells = 10;
    const float cellWidth = 25.0f;
    const float cellHeight = 20.0f;
    const float cellSpacing = 5.0f;
    
    // Подсчет заполненных ячеек
    int filledCells = static_cast<int>((percentage / 100.0f) * totalCells);
    
    // Отрисовка метки навыка
    sf::Text labelText;
    labelText.setFont(m_font);  // Always set font
    labelText.setString(label);
    labelText.setCharacterSize(20);
    labelText.setFillColor(sf::Color::White);
    labelText.setPosition(x, y);
    window.draw(labelText);
    
    // Отрисовка ячеек
    for (int i = 0; i < totalCells; ++i) {
        sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
        cell.setPosition(x + 200.0f + i * (cellWidth + cellSpacing), y);
        
        if (i < filledCells) {
            // Заполненная ячейка (горящая)
            cell.setFillColor(sf::Color(76, 175, 80)); // Green
            cell.setOutlineColor(sf::Color(94, 126, 160)); // Light blue
        } else {
            // Пустая ячейка (потухшая)
            cell.setFillColor(sf::Color(40, 40, 40)); // Very dark
            cell.setOutlineColor(sf::Color(62, 62, 78)); // Medium gray
        }
        cell.setOutlineThickness(1.0f);
        
        window.draw(cell);
    }
}

void CharacterScene::render(sf::RenderWindow& window) {
    // Отрисовка фона
    window.draw(m_background);
    
    // Отрисовка заголовка
    window.draw(m_titleText);
    
    // Отрисовка информации о персонаже
    window.draw(m_nameText);
    window.draw(m_backstoryText);
    window.draw(m_levelText);
    window.draw(m_experienceText);
    
    // Отрисовка полосы опыта
    window.draw(m_experienceBarBackground);
    window.draw(m_experienceBarFill);
    
    // Отрисовка процента опыта - adjusted position
    float expPercentage = static_cast<float>(m_currentExp) / static_cast<float>(m_maxExp) * 100.0f;
    sf::Text expPercentText;
    expPercentText.setFont(m_font);  // Always set font
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(0) << expPercentage << "%";
    expPercentText.setString(ss.str());
    expPercentText.setCharacterSize(18);
    expPercentText.setFillColor(sf::Color(204, 204, 204));
    expPercentText.setPosition(1110.0f, 268.0f);
    window.draw(expPercentText);
    
    // Отрисовка заголовка статистики
    window.draw(m_statsHeaderText);
    
    // Отрисовка навыков в виде ячеек - repositioned
    renderSkillCells(window, 100.0f, 365.0f, m_drivingSkill, "Driving Skill:");
    renderSkillCells(window, 100.0f, 410.0f, m_charisma, "Charisma:");
    renderSkillCells(window, 100.0f, 455.0f, m_luck, "Luck:");
    
    // Отрисовка заголовка эффектов
    window.draw(m_effectsHeaderText);
    
    // Отрисовка активных эффектов - repositioned
    float effectY = 565.0f;
    for (const auto& effect : m_activeEffects) {
        sf::Text effectText;
        effectText.setFont(m_font);  // Always set font
        effectText.setString(effect.first);
        effectText.setCharacterSize(18);
        effectText.setFillColor(effect.second ? sf::Color(76, 175, 80) : sf::Color(244, 67, 54)); // Green for positive, red for negative
        effectText.setPosition(100.0f, effectY);
        window.draw(effectText);
        effectY += 30.0f;
    }
    
    // Отрисовка подсказки управления
    window.draw(m_controlsText);
}
