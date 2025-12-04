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
    
    // Загрузка шрифта с fallback опциями (SFML 3.x uses openFromFile)
    if (m_font.openFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "CharacterScene: WARNING - Could not load any font!" << std::endl;
    }
    
    setupUI();
    updateCharacterDisplay();
}

void CharacterScene::setupUI() {
    // Fixed layout for 1440x900 (no responsive scaling)
    const float SCREEN_WIDTH = 1440.0f;
    const float SCREEN_HEIGHT = 900.0f;

    // Фон - fixed sizing
    m_background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_background.setFillColor(sf::Color(30, 30, 46)); // Dark blue-gray
    m_background.setPosition(sf::Vector2f(0.0f, 0.0f));

    if (m_fontLoaded) {
        // Заголовок
        m_titleText.emplace(m_font, "CHARACTER INFORMATION", 36);
        m_titleText->setFillColor(sf::Color::White);
        m_titleText->setPosition(sf::Vector2f(100.0f, 40.0f));

        // Текст имени
        m_nameText.emplace(m_font, "", 24);
        m_nameText->setFillColor(sf::Color(255, 165, 0)); // Orange accent
        m_nameText->setPosition(sf::Vector2f(100.0f, 110.0f));

        // Текст предыстории
        m_backstoryText.emplace(m_font, "", 20);
        m_backstoryText->setFillColor(sf::Color(204, 204, 204)); // Light gray
        m_backstoryText->setPosition(sf::Vector2f(100.0f, 150.0f));

        // Текст уровня
        m_levelText.emplace(m_font, "", 22);
        m_levelText->setFillColor(sf::Color::White);
        m_levelText->setPosition(sf::Vector2f(100.0f, 195.0f));

        // Текст опыта
        m_experienceText.emplace(m_font, "", 18);
        m_experienceText->setFillColor(sf::Color(204, 204, 204));
        m_experienceText->setPosition(sf::Vector2f(100.0f, 230.0f));

        // Заголовок статистики
        m_statsHeaderText.emplace(m_font, "STATS:", 28);
        m_statsHeaderText->setFillColor(sf::Color::White);
        m_statsHeaderText->setPosition(sf::Vector2f(100.0f, 320.0f));

        // Заголовок эффектов
        m_effectsHeaderText.emplace(m_font, "ACTIVE EFFECTS:", 28);
        m_effectsHeaderText->setFillColor(sf::Color::White);
        m_effectsHeaderText->setPosition(sf::Vector2f(100.0f, 520.0f));

        // Подсказка управления
        m_controlsText.emplace(m_font, "ESC - Back to Node", 16);
        m_controlsText->setFillColor(sf::Color(150, 150, 150));
        m_controlsText->setPosition(sf::Vector2f(100.0f, 700.0f));
    }

    // Полоса опыта
    m_experienceBarBackground.setSize(sf::Vector2f(1000.0f, 30.0f));
    m_experienceBarBackground.setPosition(sf::Vector2f(100.0f, 265.0f));
    m_experienceBarBackground.setFillColor(sf::Color(62, 62, 78)); // Medium gray
    m_experienceBarBackground.setOutlineColor(sf::Color(94, 126, 160)); // Light blue
    m_experienceBarBackground.setOutlineThickness(2.0f);

    // Полоса опыта - заполнение
    m_experienceBarFill.setPosition(sf::Vector2f(100.0f, 265.0f));
    m_experienceBarFill.setFillColor(sf::Color(76, 175, 80)); // Success green
}

void CharacterScene::updateCharacterDisplay() {
    // Получение данных из PlayerState и GameStateManager
    PlayerState& playerState = GameStateManager::getInstance().getPlayerState();
    
    // Имя игрока (пока заглушка, так как Character не хранит имя)
    m_playerName = "Player";
    if (m_nameText) {
        m_nameText->setString("Name: " + m_playerName);
    }

    // [MVP] Disabled - Origin type (uncomment to enable)
    /*
    // Предыстория из OriginType
    OriginType origin = playerState.getOrigin();
    m_backstory = getOriginTypeName(origin);
    if (m_backstoryText) {
        m_backstoryText->setString("Backstory: " + m_backstory);
    }
    */
    m_backstory = "Traveler";
    if (m_backstoryText) {
        m_backstoryText->setString("Backstory: " + m_backstory);
    }

    // Уровень и опыт из ExperienceSystem
    ExperienceSystem& expSystem = ExperienceSystem::getInstance();
    m_level = expSystem.getLevel();
    m_currentExp = expSystem.getExperience();
    m_maxExp = expSystem.getExperienceForNextLevel();

    if (m_levelText) {
        m_levelText->setString("Level: " + std::to_string(m_level));
    }
    if (m_experienceText) {
        m_experienceText->setString("XP: " + std::to_string(m_currentExp) + "/" + std::to_string(m_maxExp));
    }
    
    // Обновление полосы опыта - adjusted for wider bar
    float expPercentage = static_cast<float>(m_currentExp) / static_cast<float>(m_maxExp);
    m_experienceBarFill.setSize(sf::Vector2f(1000.0f * expPercentage, 30.0f));
    
    // [MVP] Disabled - Origin-based skills (uncomment to enable)
    /*
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
    */
    // [MVP] Default skills
    m_drivingSkill = 50.0f;
    m_charisma = 50.0f;
    m_luck = 50.0f;
    
    // [MVP] Disabled - Energy-based effects (uncomment to enable)
    /*
    // Активные эффекты (пример)
    m_activeEffects.clear();
    if (playerState.getEnergy() > 75.0f) {
        m_activeEffects.push_back({"+ Well Rested (+10% energy regen)", true});
    }
    if (playerState.getEnergy() < 30.0f) {
        m_activeEffects.push_back({"- Tired (-5% max energy)", false});
    }
    */
    m_activeEffects.clear();
}

void CharacterScene::handleInput(const sf::Event& event) {
    // SFML 3.x event handling
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
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
    if (m_fontLoaded) {
        sf::Text labelText(m_font, label, 20);
        labelText.setFillColor(sf::Color::White);
        labelText.setPosition(sf::Vector2f(x, y));
        window.draw(labelText);
    }

    // Отрисовка ячеек
    for (int i = 0; i < totalCells; ++i) {
        sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
        cell.setPosition(sf::Vector2f(x + 200.0f + i * (cellWidth + cellSpacing), y));
        
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
    if (m_titleText) {
        window.draw(*m_titleText);
    }

    // Отрисовка информации о персонаже
    if (m_nameText) {
        window.draw(*m_nameText);
    }
    if (m_backstoryText) {
        window.draw(*m_backstoryText);
    }
    if (m_levelText) {
        window.draw(*m_levelText);
    }
    if (m_experienceText) {
        window.draw(*m_experienceText);
    }

    // Отрисовка полосы опыта
    window.draw(m_experienceBarBackground);
    window.draw(m_experienceBarFill);

    // Отрисовка процента опыта
    if (m_fontLoaded) {
        float expPercentage = static_cast<float>(m_currentExp) / static_cast<float>(m_maxExp) * 100.0f;
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(0) << expPercentage << "%";
        sf::Text expPercentText(m_font, ss.str(), 18);
        expPercentText.setFillColor(sf::Color(204, 204, 204));
        expPercentText.setPosition(sf::Vector2f(1110.0f, 268.0f));
        window.draw(expPercentText);
    }

    // Отрисовка заголовка статистики
    if (m_statsHeaderText) {
        window.draw(*m_statsHeaderText);
    }

    // Отрисовка навыков в виде ячеек
    renderSkillCells(window, 100.0f, 365.0f, m_drivingSkill, "Driving Skill:");
    renderSkillCells(window, 100.0f, 410.0f, m_charisma, "Charisma:");
    renderSkillCells(window, 100.0f, 455.0f, m_luck, "Luck:");

    // Отрисовка заголовка эффектов
    if (m_effectsHeaderText) {
        window.draw(*m_effectsHeaderText);
    }

    // Отрисовка активных эффектов
    if (m_fontLoaded) {
        float effectY = 565.0f;
        for (const auto& effect : m_activeEffects) {
            sf::Text effectText(m_font, effect.first, 18);
            effectText.setFillColor(effect.second ? sf::Color(76, 175, 80) : sf::Color(244, 67, 54));
            effectText.setPosition(sf::Vector2f(100.0f, effectY));
            window.draw(effectText);
            effectY += 30.0f;
        }
    }

    // Отрисовка подсказки управления
    if (m_controlsText) {
        window.draw(*m_controlsText);
    }
}
