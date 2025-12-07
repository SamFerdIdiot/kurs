#ifndef JOURNAL_SCENE_H
#define JOURNAL_SCENE_H

#include "Scene.h"
#include "Journal/Note.h"
#include "Journal/CityNote.h"
#include "PlayerState.h"
#include "FontLoader.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include <vector>

/**
 * @brief JournalScene - единая сцена для всего геймплея
 *
 * Заменяет множество отдельных сцен (NodeScene, TravelCinematicScene,
 * LocationScene и т.д.) одной универсальной сценой, которая показывает
 * разные типы записок (Note).
 *
 * Архитектура:
 * ┌─────────────────────────────┐
 * │     JOURNALSCENE            │
 * │  (единый контроллер игры)   │
 * └─────────────┬───────────────┘
 *               │
 *   ┌───────────┼───────────┐
 *   │           │           │
 * CityNote  TravelNote  EventNote
 *
 * Все записки отображаются в едином стиле "старая бумага".
 */
class JournalScene : public Scene {
public:
    /**
     * @brief Конструктор
     * @param playerState Состояние игрока
     * @param startCityId ID стартового города (по умолчанию "moscow")
     */
    explicit JournalScene(PlayerState* playerState,
                         const std::string& startCityId = "moscow");

    ~JournalScene() override = default;

    // === SCENE INTERFACE ===

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    SceneType getNextScene() const override;
    bool isFinished() const override;

    // === УПРАВЛЕНИЕ ЗАПИСКАМИ ===

    /**
     * @brief Показать записку города
     * @param cityId ID города
     */
    void showCityNote(const std::string& cityId);

    /**
     * @brief Показать записку путешествия
     * @param roadData Данные о маршруте
     */
    // void showTravelNote(const RoadData& roadData);  // TODO: Phase 2

    /**
     * @brief Показать событийную записку
     * @param event Событие
     */
    // void showEventNote(GameEvent* event);  // TODO: Phase 2

    /**
     * @brief Показать записку локации
     * @param type Тип локации (заправка, мотель и т.д.)
     */
    // void showLocationNote(LocationType type);  // TODO: Phase 2

    /**
     * @brief Получить текущую записку
     */
    Note* getCurrentNote() const { return m_currentNote.get(); }

private:
    // === СОСТОЯНИЕ ===
    PlayerState* m_playerState;
    bool m_isFinished;
    SceneType m_nextScene;

    // === СИСТЕМА ЗАПИСОК ===
    std::unique_ptr<Note> m_currentNote;                // Текущая отображаемая записка
    std::stack<std::unique_ptr<Note>> m_noteStack;      // Стек записок (для возврата назад)

    // === ВИЗУАЛИЗАЦИЯ ===
    sf::Font m_font;
    bool m_fontLoaded;

    // === УПРАВЛЕНИЕ ===
    int m_selectedChoiceIndex;      // Выбранный вариант (0-N)

    // === ПРИВАТНЫЕ МЕТОДЫ ===

    /**
     * @brief Обработать выбор игрока
     * @param choiceIndex Индекс выбранного варианта
     */
    void handleChoice(int choiceIndex);

    /**
     * @brief Переключить выбор вверх
     */
    void moveSelectionUp();

    /**
     * @brief Переключить выбор вниз
     */
    void moveSelectionDown();

    /**
     * @brief Обработать завершение записки
     */
    void handleNoteFinished();

    /**
     * @brief Отрендерить минимальный HUD (топливо, энергия, деньги)
     */
    void renderHUD(sf::RenderWindow& window);
};

#endif // JOURNAL_SCENE_H
