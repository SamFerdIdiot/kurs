#ifndef QUESTS_SCENE_H
#define QUESTS_SCENE_H

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <optional>

// QuestsScene - Журнал квестов и отслеживание целей
// Отображает активные, завершенные и проваленные квесты
// Показывает цели квестов, прогресс и награды
class QuestsScene : public Scene {
public:
    QuestsScene();
    ~QuestsScene() override = default;
    
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    
    bool isFinished() const override { return m_finished; }
    SceneType getNextScene() const override { return m_nextScene; }
    
private:
    // Статус квеста
    enum class QuestStatus {
        ACTIVE,
        COMPLETED,
        FAILED
    };
    
    // Цель квеста
    struct QuestObjective {
        std::string description;
        int current;
        int required;
        bool completed;
        
        QuestObjective(const std::string& desc, int curr, int req)
            : description(desc), current(curr), required(req), completed(curr >= req) {}
    };
    
    // Информация о квесте
    struct QuestInfo {
        std::string title;
        std::string description;
        QuestStatus status;
        std::vector<QuestObjective> objectives;
        std::string reward;
        
        QuestInfo(const std::string& t, const std::string& d, QuestStatus s, const std::string& r)
            : title(t), description(d), status(s), reward(r) {}
    };
    
    // Инициализация UI
    void setupUI();
    
    // Загрузка квестов из QuestManager
    void loadQuests();
    
    // Навигация
    void selectPrevious();
    void selectNext();
    void changeFilter();
    
    // Получение отфильтрованных квестов
    std::vector<int> getFilteredQuestIndices() const;
    
    // Рендер одного квеста
    void renderQuest(sf::RenderWindow& window, const QuestInfo& quest, float y, bool isSelected);
    
    // Состояние сцены
    bool m_finished;
    SceneType m_nextScene;
    
    // Шрифт
    sf::Font m_font;
    bool m_fontLoaded;
    
    // UI элементы
    sf::RectangleShape m_background;
    std::optional<sf::Text> m_titleText;
    std::optional<sf::Text> m_filterText;
    std::optional<sf::Text> m_questCountText;
    std::optional<sf::Text> m_controlsText;
    sf::RectangleShape m_selectionHighlight;
    
    // Данные о квестах
    std::vector<QuestInfo> m_quests;
    int m_selectedQuestIndex;
    QuestStatus m_currentFilter;
    
    // Прокрутка
    int m_scrollOffset;
};

#endif // QUESTS_SCENE_H
