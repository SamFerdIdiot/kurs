#include "CompanionsScene.h"
#include "GameStateManager.h"
#include "GameTypes.h"
#include "NPC.h"
#include <iostream>

CompanionsScene::CompanionsScene()
    : m_finished(false),
      m_nextScene(SceneType::COMPANIONS),
      m_fontLoaded(false),
      m_maxCompanions(2),
      m_selectedSlot(0) {
    
    // Загрузка шрифта с fallback опциями
    if (m_font.loadFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.loadFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "CompanionsScene: WARNING - Could not load any font!" << std::endl;
    }
    
    setupUI();
    updateCompanionsDisplay();
}

void CompanionsScene::setupUI() {
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("CompanionsScene");
    layout.setTargetResolution(
        ScreenResolution::MAC_AIR_M1_WIDTH,
        ScreenResolution::MAC_AIR_M1_HEIGHT
    );
    
    // Фон with responsive sizing
    m_background.setSize(sf::Vector2f(layout.getTargetWidth(), layout.getTargetHeight()));
    m_background.setFillColor(sf::Color(30, 30, 46)); // Dark blue-gray
    m_background.setPosition(0.0f, 0.0f);
    
    // Заголовок with responsive scaling
    m_titleText.setFont(m_font);
    m_titleText.setCharacterSize(SCALE_FONT(layout, 36));
    m_titleText.setFillColor(sf::Color::White);
    m_titleText.setPosition(SCALE_POS(layout, 100.0f, 40.0f));
    m_titleText.setString("PARTY & COMPANIONS");
    
    // Информация о машине with responsive scaling
    m_vehicleInfoText.setFont(m_font);
    m_vehicleInfoText.setCharacterSize(SCALE_FONT(layout, 22));
    m_vehicleInfoText.setFillColor(sf::Color(204, 204, 204));
    m_vehicleInfoText.setPosition(SCALE_POS(layout, 100.0f, 100.0f));
    
    // Вместимость with responsive scaling
    m_capacityText.setFont(m_font);
    m_capacityText.setCharacterSize(SCALE_FONT(layout, 20));
    m_capacityText.setFillColor(sf::Color(255, 165, 0)); // Orange
    m_capacityText.setPosition(SCALE_POS(layout, 100.0f, 135.0f));
    
    // Подсказка управления with responsive scaling
    m_controlsText.setFont(m_font);
    m_controlsText.setCharacterSize(SCALE_FONT(layout, 16));
    m_controlsText.setFillColor(sf::Color(150, 150, 150));
    m_controlsText.setPosition(SCALE_POS(layout, 100.0f, 700.0f));
    m_controlsText.setString("Up/Down: Navigate | Enter: Select | ESC - Back to Node");
    
    // Подсветка выбора with responsive scaling
    m_selectionHighlight.setSize(SCALE_SIZE(layout, 1100.0f, 80.0f));
    m_selectionHighlight.setFillColor(sf::Color(94, 126, 160, 100)); // Light blue with transparency
    m_selectionHighlight.setOutlineColor(sf::Color(94, 126, 160));
    m_selectionHighlight.setOutlineThickness(2.0f);
}

void CompanionsScene::updateCompanionsDisplay() {
    // Получение данных из PlayerState
    PlayerState& playerState = GameStateManager::getInstance().getPlayerState();
    CarType carType = playerState.getCarType();
    
    // Определение вместимости в зависимости от типа машины
    if (carType == CarType::OFFROAD_VAN) {
        m_maxCompanions = 4; // Фургон - много места
    } else if (carType == CarType::OLD_SEDAN) {
        m_maxCompanions = 3; // Седан - среднее количество
    } else if (carType == CarType::SPORTS_COUPE) {
        m_maxCompanions = 2; // Купе - мало места
    } else {
        m_maxCompanions = 2; // По умолчанию
    }
    
    // Название машины
    m_vehicleName = getCarTypeName(carType);
    m_vehicleInfoText.setString("Vehicle: " + m_vehicleName);
    
    // Получение реальных компаньонов из NPCManager
    NPCManager& npcManager = NPCManager::getInstance();
    const std::vector<std::unique_ptr<NPC>>& team = npcManager.getTeam();
    
    // Инициализация слотов компаньонов
    m_companions.clear();
    for (int i = 0; i < m_maxCompanions; ++i) {
        if (i < team.size() && team[i]) {
            // Реальный компаньон из NPCManager
            NPC* npc = team[i].get();
            std::string ability = npc->hasPassiveAbility() 
                ? npc->getPassiveAbility().description 
                : "No special ability";
            
            // Мораль пока 75% по умолчанию (может быть добавлена в NPC позже)
            m_companions.push_back(CompanionInfo(
                npc->getName(),
                npc->getBio(),  // Используем биографию как роль
                ability,
                75.0f
            ));
        } else {
            // Пустой слот
            m_companions.push_back(CompanionInfo());
        }
    }
    
    // Подсчет набранных компаньонов
    int recruitedCount = static_cast<int>(team.size());
    m_capacityText.setString("Capacity: " + std::to_string(recruitedCount) + "/" + std::to_string(m_maxCompanions) + " companions");
}

void CompanionsScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
                selectPrevious();
                break;
            case sf::Keyboard::Down:
                selectNext();
                break;
            case sf::Keyboard::Enter:
                // Выбор слота компаньона
                if (m_selectedSlot >= 0 && m_selectedSlot < m_companions.size()) {
                    const CompanionInfo& companion = m_companions[m_selectedSlot];
                    if (companion.isRecruited) {
                        std::cout << "Viewing companion: " << companion.name << std::endl;
                        // Здесь может быть переход к детальному просмотру компаньона
                    } else {
                        std::cout << "Empty slot - no companion recruited" << std::endl;
                    }
                }
                break;
            case sf::Keyboard::Escape:
                m_finished = true;
                m_nextScene = SceneType::NODE;
                break;
            default:
                break;
        }
    }
}

void CompanionsScene::update(float deltaTime) {
    // Можно добавить обновление морали со временем
}

void CompanionsScene::selectPrevious() {
    if (m_companions.size() > 0) {
        m_selectedSlot = (m_selectedSlot - 1 + m_companions.size()) % m_companions.size();
    }
}

void CompanionsScene::selectNext() {
    if (m_companions.size() > 0) {
        m_selectedSlot = (m_selectedSlot + 1) % m_companions.size();
    }
}

void CompanionsScene::renderMoraleCells(sf::RenderWindow& window, float x, float y, float morale) {
    // Рендер морали в виде ячеек (10 ячеек)
    const int totalCells = 10;
    const float cellWidth = 20.0f;
    const float cellHeight = 15.0f;
    const float cellSpacing = 3.0f;
    
    // Подсчет заполненных ячеек
    int filledCells = static_cast<int>((morale / 100.0f) * totalCells);
    
    // Метка
    sf::Text moraleLabel;
    moraleLabel.setFont(m_font);  // Always set font
    moraleLabel.setString("Morale:");
    moraleLabel.setCharacterSize(16);
    moraleLabel.setFillColor(sf::Color::White);
    moraleLabel.setPosition(x, y);
    window.draw(moraleLabel);
    
    // Отрисовка ячеек
    for (int i = 0; i < totalCells; ++i) {
        sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
        cell.setPosition(x + 80.0f + i * (cellWidth + cellSpacing), y + 3.0f);
        
        if (i < filledCells) {
            // Заполненная ячейка
            cell.setFillColor(sf::Color(76, 175, 80)); // Green
            cell.setOutlineColor(sf::Color(94, 126, 160));
        } else {
            // Пустая ячейка
            cell.setFillColor(sf::Color(40, 40, 40));
            cell.setOutlineColor(sf::Color(62, 62, 78));
        }
        cell.setOutlineThickness(1.0f);
        
        window.draw(cell);
    }
}

void CompanionsScene::renderCompanionSlot(sf::RenderWindow& window, int slotIndex, float y, bool isSelected) {
    if (slotIndex >= m_companions.size()) return;
    
    const CompanionInfo& companion = m_companions[slotIndex];
    
    // Подсветка выбора - repositioned
    if (isSelected) {
        m_selectionHighlight.setPosition(100.0f, y - 5.0f);
        window.draw(m_selectionHighlight);
    }
    
    // Заголовок слота - repositioned
    sf::Text slotTitle;
    slotTitle.setFont(m_font);  // Always set font
    slotTitle.setString("SLOT " + std::to_string(slotIndex + 1) + ": " + 
                        (companion.isRecruited ? "[" + companion.name + " - " + companion.role + "]" : "[Empty]"));
    slotTitle.setCharacterSize(20);
    slotTitle.setFillColor(companion.isRecruited ? sf::Color::White : sf::Color(150, 150, 150));
    slotTitle.setPosition(110.0f, y);
    window.draw(slotTitle);
    
    if (companion.isRecruited) {
        // Мораль - repositioned
        renderMoraleCells(window, 110.0f, y + 30.0f, companion.morale);
        
        // Способность - repositioned
        sf::Text abilityText;
        abilityText.setFont(m_font);  // Always set font
        abilityText.setString("Ability: " + companion.ability);
        abilityText.setCharacterSize(16);
        abilityText.setFillColor(sf::Color(255, 193, 7)); // Amber/yellow
        abilityText.setPosition(110.0f, y + 50.0f);
        window.draw(abilityText);
    } else {
        // Пустой слот - repositioned
        sf::Text emptyText;
        emptyText.setFont(m_font);  // Always set font
        emptyText.setString("> Recruit Companion");
        emptyText.setCharacterSize(16);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition(110.0f, y + 30.0f);
        window.draw(emptyText);
    }
}

void CompanionsScene::render(sf::RenderWindow& window) {
    // Отрисовка фона
    window.draw(m_background);
    
    // Отрисовка заголовка
    window.draw(m_titleText);
    
    // Отрисовка информации о машине
    window.draw(m_vehicleInfoText);
    window.draw(m_capacityText);
    
    // Отрисовка слотов компаньонов - repositioned for full window
    float slotY = 180.0f;
    const float slotHeight = 90.0f;
    
    for (size_t i = 0; i < m_companions.size(); ++i) {
        renderCompanionSlot(window, i, slotY, i == m_selectedSlot);
        slotY += slotHeight;
    }
    
    // Отрисовка подсказки управления
    window.draw(m_controlsText);
}
