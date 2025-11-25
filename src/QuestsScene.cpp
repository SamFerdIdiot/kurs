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
    
    // Загрузка шрифта с fallback опциями
    if (m_font.loadFromFile("assets/fonts/font.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.loadFromFile("images/Press_Start_2P/PressStart2P-Regular.ttf")) {
        m_fontLoaded = true;
    } else if (m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        m_fontLoaded = true;
    } else {
        m_fontLoaded = false;
        std::cerr << "QuestsScene: WARNING - Could not load any font!" << std::endl;
    }
    
    setupUI();
    loadQuests();
}

void QuestsScene::setupUI() {
    // Создание адаптивной разметки для MacBook Air M1
    // Create responsive layout for MacBook Air M1
    UILayout& layout = GET_LAYOUT("QuestsScene");
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
    m_titleText.setString("QUEST JOURNAL");
    
    // Фильтр with responsive scaling
    m_filterText.setFont(m_font);
    m_filterText.setCharacterSize(SCALE_FONT(layout, 20));
    m_filterText.setFillColor(sf::Color(255, 165, 0)); // Orange
    m_filterText.setPosition(SCALE_POS(layout, 100.0f, 100.0f));
    
    // Количество квестов with responsive scaling
    m_questCountText.setFont(m_font);
    m_questCountText.setCharacterSize(SCALE_FONT(layout, 18));
    m_questCountText.setFillColor(sf::Color(204, 204, 204));
    m_questCountText.setPosition(SCALE_POS(layout, 600.0f, 103.0f));
    
    // Подсказка управления with responsive scaling
    m_controlsText.setFont(m_font);
    m_controlsText.setCharacterSize(SCALE_FONT(layout, 16));
    m_controlsText.setFillColor(sf::Color(150, 150, 150));
    m_controlsText.setPosition(SCALE_POS(layout, 100.0f, 700.0f));
    m_controlsText.setString("Up/Down: Navigate | Tab: Filter | ESC - Back to Node");
    
    // Подсветка выбора with responsive scaling
    m_selectionHighlight.setSize(SCALE_SIZE(layout, 1100.0f, 100.0f));
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
    
    // TODO: Интеграция с реальным QuestManager
    // QuestManager& questManager = QuestManager::getInstance();
    // Получить список квестов из менеджера
}

void QuestsScene::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
                selectPrevious();
                break;
            case sf::Keyboard::Down:
                selectNext();
                break;
            case sf::Keyboard::Tab:
                changeFilter();
                break;
            case sf::Keyboard::Enter:
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
            case sf::Keyboard::Escape:
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
    m_filterText.setString(filterName + " Quests");
    
    // Обновление счетчика квестов
    auto filteredIndices = getFilteredQuestIndices();
    m_questCountText.setString("(" + std::to_string(filteredIndices.size()) + ")");
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
        m_selectionHighlight.setPosition(100.0f, y - 5.0f);
        window.draw(m_selectionHighlight);
    }
    
    // Заголовок квеста - repositioned
    sf::Text titleText;
    titleText.setFont(m_font);  // Always set font
    titleText.setString((isSelected ? "> " : "  ") + quest.title);
    titleText.setCharacterSize(20);
    
    // Цвет в зависимости от статуса
    if (quest.status == QuestStatus::ACTIVE) {
        titleText.setFillColor(sf::Color::White);
    } else if (quest.status == QuestStatus::COMPLETED) {
        titleText.setFillColor(sf::Color(76, 175, 80)); // Green
    } else {
        titleText.setFillColor(sf::Color(244, 67, 54)); // Red
    }
    
    titleText.setPosition(110.0f, y);
    window.draw(titleText);
    
    // Цели квеста - repositioned
    float objectiveY = y + 25.0f;
    for (const auto& objective : quest.objectives) {
        sf::Text objectiveText;
        objectiveText.setFont(m_font);  // Always set font
        
        std::string checkmark = objective.completed ? "\xE2\x9C\x93" : ""; // Unicode checkmark
        std::string objectiveStr = "  \xE2\x94\x94 " + objective.description + " (" + 
                                    std::to_string(objective.current) + "/" + 
                                    std::to_string(objective.required) + ") " + checkmark;
        
        objectiveText.setString(objectiveStr);
        objectiveText.setCharacterSize(16);
        objectiveText.setFillColor(objective.completed ? sf::Color(76, 175, 80) : sf::Color(204, 204, 204));
        objectiveText.setPosition(110.0f, objectiveY);
        window.draw(objectiveText);
        
        objectiveY += 20.0f;
    }
    
    // Награда - repositioned
    sf::Text rewardText;
    rewardText.setFont(m_font);  // Always set font
    rewardText.setString("Reward: " + quest.reward);
    rewardText.setCharacterSize(16);
    rewardText.setFillColor(sf::Color(255, 193, 7)); // Amber
    rewardText.setPosition(110.0f, objectiveY);
    window.draw(rewardText);
}

void QuestsScene::render(sf::RenderWindow& window) {
    // Отрисовка фона
    window.draw(m_background);
    
    // Отрисовка заголовка
    window.draw(m_titleText);
    
    // Отрисовка фильтра
    window.draw(m_filterText);
    window.draw(m_questCountText);
    
    // Отрисовка квестов
    auto filteredIndices = getFilteredQuestIndices();
    
    if (filteredIndices.empty()) {
        // Нет квестов в этой категории
        sf::Text emptyText;
        emptyText.setFont(m_font);  // Always set font
        emptyText.setString("No quests in this category");
        emptyText.setCharacterSize(18);
        emptyText.setFillColor(sf::Color(150, 150, 150));
        emptyText.setPosition(100.0f, 180.0f);
        window.draw(emptyText);
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
    window.draw(m_controlsText);
}
