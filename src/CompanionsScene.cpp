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
    
    // Загрузка шрифта с fallback опциями (SFML 3.x uses openFromFile)
    if (m_font.openFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "CompanionsScene: WARNING - Could not load any font!" << std::endl;
    }
    
    setupUI();
    updateCompanionsDisplay();
}

void CompanionsScene::setupUI() {
    // Fixed layout for 1440x900 (no responsive scaling)
    const float SCREEN_WIDTH = 1440.0f;
    const float SCREEN_HEIGHT = 900.0f;

    // Фон - fixed sizing
    m_background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_background.setFillColor(sf::Color(30, 30, 46)); // Dark blue-gray
    m_background.setPosition(sf::Vector2f(0.0f, 0.0f));

    if (m_fontLoaded) {
        // Заголовок
        m_titleText.emplace(m_font, "PARTY & COMPANIONS", 36);
        m_titleText->setFillColor(sf::Color::White);
        m_titleText->setPosition(sf::Vector2f(100.0f, 40.0f));

        // Информация о машине
        m_vehicleInfoText.emplace(m_font, "", 22);
        m_vehicleInfoText->setFillColor(sf::Color(204, 204, 204));
        m_vehicleInfoText->setPosition(sf::Vector2f(100.0f, 100.0f));

        // Вместимость
        m_capacityText.emplace(m_font, "", 20);
        m_capacityText->setFillColor(sf::Color(255, 165, 0)); // Orange
        m_capacityText->setPosition(sf::Vector2f(100.0f, 135.0f));

        // Подсказка управления
        m_controlsText.emplace(m_font, "Up/Down: Navigate | Enter: Select | ESC - Back to Node", 16);
        m_controlsText->setFillColor(sf::Color(150, 150, 150));
        m_controlsText->setPosition(sf::Vector2f(100.0f, 700.0f));
    }

    // Подсветка выбора
    m_selectionHighlight.setSize(sf::Vector2f(1100.0f, 80.0f));
    m_selectionHighlight.setFillColor(sf::Color(94, 126, 160, 100)); // Light blue with transparency
    m_selectionHighlight.setOutlineColor(sf::Color(94, 126, 160));
    m_selectionHighlight.setOutlineThickness(2.0f);
}

void CompanionsScene::updateCompanionsDisplay() {
    // [MVP] Disabled - Car type system (uncomment to enable)
    /*
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
    if (m_vehicleInfoText) {
        m_vehicleInfoText->setString("Vehicle: " + m_vehicleName);
    }
    */

    // [MVP] Default values
    m_maxCompanions = 3;
    m_vehicleName = "Car";
    if (m_vehicleInfoText) {
        m_vehicleInfoText->setString("Vehicle: " + m_vehicleName);
    }

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
    if (m_capacityText) {
        m_capacityText->setString("Capacity: " + std::to_string(recruitedCount) + "/" + std::to_string(m_maxCompanions) + " companions");
    }
}

void CompanionsScene::handleInput(const sf::Event& event) {
    // SFML 3.x event handling
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Up:
                selectPrevious();
                break;
            case sf::Keyboard::Key::Down:
                selectNext();
                break;
            case sf::Keyboard::Key::Enter:
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
            case sf::Keyboard::Key::Escape:
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
    if (m_fontLoaded) {
        sf::Text moraleLabel(m_font, "Morale:", 16);
        moraleLabel.setFillColor(sf::Color::White);
        moraleLabel.setPosition(sf::Vector2f(x, y));
        window.draw(moraleLabel);
    }
    
    // Отрисовка ячеек
    for (int i = 0; i < totalCells; ++i) {
        sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
        cell.setPosition(sf::Vector2f(x + 80.0f + i * (cellWidth + cellSpacing), y + 3.0f));
        
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
        m_selectionHighlight.setPosition(sf::Vector2f(100.0f, y - 5.0f));
        window.draw(m_selectionHighlight);
    }

    if (m_fontLoaded) {
        // Заголовок слота - repositioned
        std::string slotTitleStr = "SLOT " + std::to_string(slotIndex + 1) + ": " +
                            (companion.isRecruited ? "[" + companion.name + " - " + companion.role + "]" : "[Empty]");
        sf::Text slotTitle(m_font, slotTitleStr, 20);
        slotTitle.setFillColor(companion.isRecruited ? sf::Color::White : sf::Color(150, 150, 150));
        slotTitle.setPosition(sf::Vector2f(110.0f, y));
        window.draw(slotTitle);

        if (companion.isRecruited) {
            // Мораль - repositioned
            renderMoraleCells(window, 110.0f, y + 30.0f, companion.morale);

            // Способность - repositioned
            sf::Text abilityText(m_font, "Ability: " + companion.ability, 16);
            abilityText.setFillColor(sf::Color(255, 193, 7)); // Amber/yellow
            abilityText.setPosition(sf::Vector2f(110.0f, y + 50.0f));
            window.draw(abilityText);
        } else {
            // Пустой слот - repositioned
            sf::Text emptyText(m_font, "> Recruit Companion", 16);
            emptyText.setFillColor(sf::Color(150, 150, 150));
            emptyText.setPosition(sf::Vector2f(110.0f, y + 30.0f));
            window.draw(emptyText);
        }
    }
}

void CompanionsScene::render(sf::RenderWindow& window) {
    // Отрисовка фона
    window.draw(m_background);

    // Отрисовка заголовка
    if (m_titleText) {
        window.draw(*m_titleText);
    }

    // Отрисовка информации о машине
    if (m_vehicleInfoText) {
        window.draw(*m_vehicleInfoText);
    }
    if (m_capacityText) {
        window.draw(*m_capacityText);
    }

    // Отрисовка слотов компаньонов - repositioned for full window
    float slotY = 180.0f;
    const float slotHeight = 90.0f;

    for (size_t i = 0; i < m_companions.size(); ++i) {
        renderCompanionSlot(window, i, slotY, i == m_selectedSlot);
        slotY += slotHeight;
    }

    // Отрисовка подсказки управления
    if (m_controlsText) {
        window.draw(*m_controlsText);
    }
}
