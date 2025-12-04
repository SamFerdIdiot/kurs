#include "QuestsScene.h"
#include "QuestManager.h"
#include <iostream>

QuestsScene::QuestsScene()
    : m_finished(false),
      m_nextScene(SceneType::QUESTS),
      m_fontLoaded(false),
      m_selectedQuestIndex(0),
      m_currentFilter(QuestStatus::ACTIVE),
      m_scrollOffset(0) {
    
    // Загрузка шрифта с fallback опциями (SFML 3.x uses openFromFile)
    if (m_font.openFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "QuestsScene: WARNING - Could not load any font!" << std::endl;
    }
    
    setupUI();
    loadQuests();
}

void QuestsScene::setupUI() {
    // Fixed layout for 1440x900 (no responsive scaling)
    const float SCREEN_WIDTH = 1440.0f;
    const float SCREEN_HEIGHT = 900.0f;

    // Фон - fixed sizing
    m_background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    m_background.setFillColor(sf::Color(30, 30, 46)); // Dark blue-gray
    m_background.setPosition(sf::Vector2f(0.0f, 0.0f));

    if (m_fontLoaded) {
        // Заголовок
        m_titleText.emplace(m_font, "QUEST JOURNAL", 36);
        m_titleText->setFillColor(sf::Color::White);
        m_titleText->setPosition(sf::Vector2f(100.0f, 40.0f));

        // Фильтр
        m_filterText.emplace(m_font, "", 20);
        m_filterText->setFillColor(sf::Color(255, 165, 0)); // Orange
        m_filterText->setPosition(sf::Vector2f(100.0f, 100.0f));

        // Количество квестов
        m_questCountText.emplace(m_font, "", 18);
        m_questCountText->setFillColor(sf::Color(204, 204, 204));
        m_questCountText->setPosition(sf::Vector2f(600.0f, 103.0f));

        // Подсказка управления
        m_controlsText.emplace(m_font, "Up/Down: Navigate | Tab: Filter | ESC - Back to Node", 16);
        m_controlsText->setFillColor(sf::Color(150, 150, 150));
        m_controlsText->setPosition(sf::Vector2f(100.0f, 700.0f));
    }

    // Подсветка выбора
    m_selectionHighlight.setSize(sf::Vector2f(1100.0f, 100.0f));
    m_selectionHighlight.setFillColor(sf::Color(94, 126, 160, 100)); // Light blue with transparency
    m_selectionHighlight.setOutlineColor(sf::Color(94, 126, 160));
    m_selectionHighlight.setOutlineThickness(2.0f);
}

void QuestsScene::loadQuests() {
    // Загрузка квестов
    // В реальности здесь будет интеграция с QuestManager
    // Пока добавим примеры квестов для демонстрации
    
    m_quests.clear();
    
    // Пример активного квеста
    QuestInfo quest1("Reach Vladivostok", "Travel across Russia to reach Vladivostok", QuestStatus::ACTIVE, "500\xE2\x82\xBD, 100 XP");
    quest1.objectives.push_back(QuestObjective("Travel to next node", 1, 5));
    quest1.objectives.push_back(QuestObjective("Visit gas stations", 2, 3));
    m_quests.push_back(quest1);
    
    // Пример активного квеста 2
    QuestInfo quest2("Find Missing Trader", "Local trader has gone missing, ask around", QuestStatus::ACTIVE, "250\xE2\x82\xBD, 50 XP");
    quest2.objectives.push_back(QuestObjective("Ask locals", 0, 3));
    m_quests.push_back(quest2);
    
    // Пример завершенного квеста
    QuestInfo quest3("Fuel Run", "Collect enough fuel for the journey", QuestStatus::COMPLETED, "100\xE2\x82\xBD");
    quest3.objectives.push_back(QuestObjective("Collect 50L fuel", 50, 50));
    m_quests.push_back(quest3);

    // Note: Integration with QuestManager not yet implemented
    // Currently using hardcoded quest data for UI demonstration
    // QuestManager& questManager = QuestManager::getInstance();
    // Future: Load quests from QuestManager
}

void QuestsScene::handleInput(const sf::Event& event) {
    // SFML 3.x event handling
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        switch (keyPressed->code) {
            case sf::Keyboard::Key::Up:
                selectPrevious();
                break;
            case sf::Keyboard::Key::Down:
                selectNext();
                break;
            case sf::Keyboard::Key::Tab:
                changeFilter();
                break;
            case sf::Keyboard::Key::Enter:
                {
                    // Показать детали квеста
                    auto filteredIndices = getFilteredQuestIndices();
                    if (m_selectedQuestIndex >= 0 && m_selectedQuestIndex < filteredIndices.size()) {
                        int questIndex = filteredIndices[m_selectedQuestIndex];
                        const QuestInfo& quest = m_quests[questIndex];
                        std::cout << "Quest: " << quest.title << std::endl;
                        std::cout << "Description: " << quest.description << std::endl;
                        std::cout << "Reward: " << quest.reward << std::endl;
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

void QuestsScene::update(float deltaTime) {
    // Обновление фильтра
    std::string filterName;
    switch (m_currentFilter) {
        case QuestStatus::ACTIVE:
            filterName = "[Active]";
            break;
        case QuestStatus::COMPLETED:
            filterName = "[Completed]";
            break;
        case QuestStatus::FAILED:
            filterName = "[Failed]";
            break;
    }
    if (m_filterText) {
        m_filterText->setString(filterName + " Quests");
    }

    // Обновление счетчика квестов
    auto filteredIndices = getFilteredQuestIndices();
    if (m_questCountText) {
        m_questCountText->setString("(" + std::to_string(filteredIndices.size()) + ")");
    }
}

void QuestsScene::selectPrevious() {
    auto filteredIndices = getFilteredQuestIndices();
    if (filteredIndices.size() > 0) {
        m_selectedQuestIndex = (m_selectedQuestIndex - 1 + filteredIndices.size()) % filteredIndices.size();
    }
}

void QuestsScene::selectNext() {
    auto filteredIndices = getFilteredQuestIndices();
    if (filteredIndices.size() > 0) {
        m_selectedQuestIndex = (m_selectedQuestIndex + 1) % filteredIndices.size();
    }
}

void QuestsScene::changeFilter() {
    // Переключение между фильтрами
    int filterInt = static_cast<int>(m_currentFilter);
    filterInt = (filterInt + 1) % 3;
    m_currentFilter = static_cast<QuestStatus>(filterInt);
    m_selectedQuestIndex = 0;
    m_scrollOffset = 0;
}

std::vector<int> QuestsScene::getFilteredQuestIndices() const {
    std::vector<int> indices;
    for (size_t i = 0; i < m_quests.size(); ++i) {
        if (m_quests[i].status == m_currentFilter) {
            indices.push_back(i);
        }
    }
    return indices;
}

void QuestsScene::renderQuest(sf::RenderWindow& window, const QuestInfo& quest, float y, bool isSelected) {
    // Подсветка выбора - repositioned
    if (isSelected) {
        m_selectionHighlight.setPosition(sf::Vector2f(100.0f, y - 5.0f));  // Fixed: Vector2f wrapper
        window.draw(m_selectionHighlight);
    }

    // Заголовок квеста - SFML 3.x: Text requires font in constructor
    sf::Text titleText(m_font, (isSelected ? "> " : "  ") + quest.title, 20);

    // Цвет в зависимости от статуса
    if (quest.status == QuestStatus::ACTIVE) {
        titleText.setFillColor(sf::Color::White);
    } else if (quest.status == QuestStatus::COMPLETED) {
        titleText.setFillColor(sf::Color(76, 175, 80)); // Green
    } else {
        titleText.setFillColor(sf::Color(244, 67, 54)); // Red
    }

    titleText.setPosition(sf::Vector2f(110.0f, y));  // Fixed: Vector2f wrapper
    window.draw(titleText);
    
    // Цели квеста - repositioned
    float objectiveY = y + 25.0f;
    for (const auto& objective : quest.objectives) {
        std::string checkmark = objective.completed ? "\xE2\x9C\x93" : ""; // Unicode checkmark
        std::string objectiveStr = "  \xE2\x94\x94 " + objective.description + " (" +
                                    std::to_string(objective.current) + "/" +
                                    std::to_string(objective.required) + ") " + checkmark;

        // SFML 3.x: Text requires font in constructor
        sf::Text objectiveText(m_font, objectiveStr, 16);
        objectiveText.setFillColor(objective.completed ? sf::Color(76, 175, 80) : sf::Color(204, 204, 204));
        objectiveText.setPosition(sf::Vector2f(110.0f, objectiveY));  // Fixed: Vector2f wrapper
        window.draw(objectiveText);

        objectiveY += 20.0f;
    }

    // Награда - SFML 3.x: Text requires font in constructor
    sf::Text rewardText(m_font, "Reward: " + quest.reward, 16);
    rewardText.setFillColor(sf::Color(255, 193, 7)); // Amber
    rewardText.setPosition(sf::Vector2f(110.0f, objectiveY));  // Fixed: Vector2f wrapper
    window.draw(rewardText);
}

void QuestsScene::render(sf::RenderWindow& window) {
    // Отрисовка фона
    window.draw(m_background);

    // Отрисовка заголовка
    if (m_titleText) {
        window.draw(*m_titleText);
    }

    // Отрисовка фильтра
    if (m_filterText) {
        window.draw(*m_filterText);
    }
    if (m_questCountText) {
        window.draw(*m_questCountText);
    }

    // Отрисовка квестов
    auto filteredIndices = getFilteredQuestIndices();

    if (filteredIndices.empty()) {
        // Нет квестов в этой категории - SFML 3.x: Text requires font in constructor
        if (m_fontLoaded) {
            sf::Text emptyText(m_font, "No quests in this category", 18);
            emptyText.setFillColor(sf::Color(150, 150, 150));
            emptyText.setPosition(sf::Vector2f(100.0f, 180.0f));
            window.draw(emptyText);
        }
    } else {
        float questY = 150.0f;
        const float questHeight = 120.0f;

        // Отрисовка только видимых квестов (максимум 4 for larger window)
        int maxVisible = 4;
        for (size_t i = m_scrollOffset; i < filteredIndices.size() && i < m_scrollOffset + maxVisible; ++i) {
            int questIndex = filteredIndices[i];
            renderQuest(window, m_quests[questIndex], questY, i == m_selectedQuestIndex);
            questY += questHeight;
        }
    }

    // Отрисовка подсказки управления
    if (m_controlsText) {
        window.draw(*m_controlsText);
    }
}
