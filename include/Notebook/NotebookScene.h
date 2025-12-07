#ifndef NOTEBOOK_SCENE_H
#define NOTEBOOK_SCENE_H

#include "Scene.h"
#include "PlayerState.h"
#include "FontLoader.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief NotebookScene - Сцена блокнота путешественника
 *
 * Основа всего геймплея. Весь контент - это записи в блокноте.
 * Текст печатается символ за символом, создавая эффект "писания от руки".
 *
 * Архитектура:
 * ┌──────────────────────────────────────┐
 * │           NOTEBOOK SCENE             │
 * │   (Блокнот путешественника)          │
 * └────────────┬─────────────────────────┘
 *              │
 *   ┌──────────┼──────────┐
 *   │          │          │
 * Entry    Thought   Choice
 * (Запись) (Мысли)  (Выбор)
 */
class NotebookScene : public Scene {
public:
    /**
     * @brief Конструктор
     * @param playerState Состояние игрока
     * @param initialEntryId ID стартовой записи
     */
    explicit NotebookScene(PlayerState* playerState,
                          const std::string& initialEntryId = "tutorial_start");

    ~NotebookScene() override = default;

    // === SCENE INTERFACE ===

    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    SceneType getNextScene() const override;
    bool isFinished() const override;

    // === NOTEBOOK УПРАВЛЕНИЕ ===

    /**
     * @brief Показать запись по ID
     * @param entryId ID записи
     */
    void showEntry(const std::string& entryId);

    /**
     * @brief Пропустить анимацию печати текста
     */
    void skipTextAnimation();

private:
    // === СОСТОЯНИЕ ===
    PlayerState* m_playerState;
    bool m_isFinished;
    SceneType m_nextScene;

    // === ТЕКУЩАЯ ЗАПИСЬ ===
    std::string m_currentEntryId;
    std::string m_fullText;                     // Полный текст записи
    std::string m_revealedText;                 // Уже показанный текст

    // === АНИМАЦИЯ ПЕЧАТИ ТЕКСТА ===
    float m_textRevealTimer;                    // Таймер для печати
    float m_charactersPerSecond;                // Скорость печати (символов/сек)
    bool m_textFullyRevealed;                   // Текст полностью показан?
    bool m_canSkipText;                         // Можно пропустить текст?

    // === ВЫБОРЫ ===
    struct SimpleChoice {
        std::string text;
        std::string nextEntryId;
        bool isDisabled;
        std::string disabledReason;
    };

    std::vector<SimpleChoice> m_choices;
    int m_selectedChoiceIndex;

    // === ВИЗУАЛИЗАЦИЯ ===
    sf::Font m_font;
    bool m_fontLoaded;

    // === ЗВУК (для будущего) ===
    // sf::Sound m_typingSound;
    // sf::SoundBuffer m_typingSoundBuffer;

    // === ПРИВАТНЫЕ МЕТОДЫ ===

    /**
     * @brief Обновить анимацию печати текста
     * @param deltaTime Время с последнего кадра
     */
    void updateTextReveal(float deltaTime);

    /**
     * @brief Отрендерить текст записи
     * @param window Окно для рендера
     */
    void renderText(sf::RenderWindow& window);

    /**
     * @brief Отрендерить варианты выбора
     * @param window Окно для рендера
     */
    void renderChoices(sf::RenderWindow& window);

    /**
     * @brief Отрендерить HUD (ресурсы)
     * @param window Окно для рендера
     */
    void renderHUD(sf::RenderWindow& window);

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
     * @brief Загрузить тестовую запись (для прототипа)
     */
    void loadTestEntry();

    /**
     * @brief Загрузить запись о выезде из города (день 1)
     */
    void loadDayOneDeparture();
};

#endif // NOTEBOOK_SCENE_H
