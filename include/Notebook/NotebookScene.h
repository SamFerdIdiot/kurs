#ifndef NOTEBOOK_SCENE_H
#define NOTEBOOK_SCENE_H

#include "Scene.h"
#include "PlayerState.h"
#include "FontLoader.h"
#include "Notebook/NotebookEntry.h"
#include "EventManager.h"  // Добавлено для интеграции событий
#include "ResourceEventSystem.h"  // Система событий по ресурсам
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>

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
    NotebookEntry m_currentEntry;               // Текущая запись
    std::string m_fullText;                     // Полный текст записи (кэш)
    std::string m_revealedText;                 // Уже показанный текст

    // === АНИМАЦИЯ ПЕЧАТИ ТЕКСТА ===
    float m_textRevealTimer;                    // Таймер для печати
    float m_charactersPerSecond;                // Скорость печати (символов/сек)
    bool m_textFullyRevealed;                   // Текст полностью показан?
    bool m_canSkipText;                         // Можно пропустить текст?

    // === ВЫБОРЫ ===
    std::vector<NotebookChoice> m_choices;      // Выборы для текущей записи
    int m_selectedChoiceIndex;
    float m_choiceTimer;                        // Время с момента появления выбора

    // === СИСТЕМА СОБЫТИЙ ===
    EventManager m_eventManager;                // Менеджер случайных событий
    ResourceEventSystem m_resourceEventSystem;  // Система событий по ресурсам
    float m_eventTriggerChance;                 // Шанс случайного события (0.0-1.0)
    std::string m_pendingEventId;               // ID ожидающего события (если есть)
    std::string m_pendingNextEntryId;           // ID следующей записи после события

    // === ХРАНИЛИЩЕ ЗАПИСЕЙ ===
    std::map<std::string, NotebookEntry> m_entries;  // Все загруженные записи

    // === ВИЗУАЛИЗАЦИЯ ===
    sf::Font m_fontPresent;   // Шрифт для PRESENT (настоящее время)
    sf::Font m_fontPast;      // Шрифт для PAST (воспоминания)
    sf::Font m_fontThought;   // Шрифт для THOUGHT (мысли)
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
     * @brief Получить шрифт для текущего типа записи
     */
    const sf::Font& getCurrentFont() const;

    /**
     * @brief Получить цвет текста для текущего типа записи
     */
    sf::Color getCurrentTextColor() const;

    /**
     * @brief Получить размер шрифта для текущего типа записи
     */
    unsigned int getCurrentFontSize() const;

    /**
     * @brief Перенести текст по словам с учетом максимальной ширины
     */
    std::string wrapText(const std::string& text, const sf::Font& font, unsigned int fontSize, float maxWidth) const;

    // === ИНТЕГРАЦИЯ СОБЫТИЙ ===

    /**
     * @brief Попытаться вызвать случайное событие
     * @return true если событие сработало
     */
    bool tryTriggerRandomEvent();

    /**
     * @brief Конвертировать GameEvent в NotebookEntry
     * @param event Игровое событие
     * @return Запись блокнота
     */
    NotebookEntry convertEventToEntry(const GameEvent& event);

    /**
     * @brief Показать событие как запись блокнота
     * @param eventId ID события для показа
     */
    void showEventAsEntry(const std::string& eventId);

    // === ДЕНЬ 0: УРОКИ ДОРОГИ (будущая реализация) ===

    /**
     * @brief ЗАПИСЬ 1.1: Стук в дверь (PAST)
     */
    void loadDay0_KnockOnDoor();

    /**
     * @brief ЗАПИСЬ 1.2: Волга (PAST)
     */
    void loadDay0_Volga();

    /**
     * @brief ЗАПИСЬ 2: Первая задача - выбор подхода (PAST)
     */
    void loadDay0_FirstTask();

    // Ветка А: Системный анализ (Понимание контекста)
    void loadDay0_BranchA_MapAndTower();     // Запись 3А: Карта из ничего
    void loadDay0_BranchA_Geologist();       // Запись 4А: Геолог
    void loadDay0_BranchA_3();               // Запись 5А: Системный анализ
    void loadDay0_BranchA_4();               // Запись 6А: Практика диагностики
    void loadDay0_BranchA_5();               // Запись 7А: Закрепление принципа

    // Ветка Б: Слушать материал (Чувство машины)
    void loadDay0_BranchB_Engine();          // Запись 3Б: Ритм двигателя
    void loadDay0_BranchB_Blacksmith();      // Запись 4Б: Кузнец
    void loadDay0_BranchB_3();               // Запись 5Б: Чувство вибрации
    void loadDay0_BranchB_4();               // Запись 6Б: Запахи и температура
    void loadDay0_BranchB_5();               // Запись 7Б: Чувство материала
    void loadDay0_BranchB_Driver();          // Старое имя (для совместимости)

    // Ветка В: Спроси бывалого (Командная работа)
    void loadDay0_BranchC_Truck();           // Запись 3В: Помощь на обочине
    void loadDay0_BranchC_Teahouse();        // Запись 4В: Чай и разговоры
    void loadDay0_BranchC_3();               // Запись 5В: Обмен опытом
    void loadDay0_BranchC_4();               // Запись 6В: Коллективный разум
    void loadDay0_BranchC_5();               // Запись 7В: Ценность команды
    void loadDay0_BranchC_Package();         // Старое имя (для совместимости)

    // Ветка Г: Ресурсоэффективность (Быстро и по делу)
    void loadDay0_BranchD_Route();           // Запись 3Г: Расчёт маршрута
    void loadDay0_BranchD_Boys();            // Запись 4Г: Мальчишки и металл
    void loadDay0_BranchD_3();               // Запись 5Г: Оптимизация расходов
    void loadDay0_BranchD_4();               // Запись 6Г: Время — тоже ресурс
    void loadDay0_BranchD_5();               // Запись 7Г: Философия эффективности
    void loadDay0_BranchD_Choice();          // Старое имя (для совместимости)

    // Общие записи (для всех веток)
    /**
     * @brief ЗАПИСЬ 5: Провокация (после всех веток)
     */
    void loadDay0_Provocation();

    /**
     * @brief ЗАПИСЬ 6: Финал в гараже (PAST, общий для всех веток)
     */
    void loadDay0_GarageFinale();

    /**
     * @brief ЗАПИСЬ 7: Переход в настоящее (PRESENT)
     */
    void loadDay0_TransitionPresent();

    // День 1: Переход границы
    void loadDay1_BorderCrossing();
    void loadDay1_CustomsCheck();

    // === ТЕСТОВЫЕ ЗАПИСИ ===

    /**
     * @brief Тестовая запись для проверки ThoughtSystem (короткие задержки 5/10/15 сек)
     */
    void loadTestThoughtSystem();

    // === ДЕМО-КОНТЕНТ (простой пример для быстрого тестирования) ===

    /**
     * @brief ДЕМО: Запись 1 - Начало (PAST, без выбора)
     */
    void loadDemo_Start();

    /**
     * @brief ДЕМО: Запись 2 - Дорога (PAST, без выбора)
     */
    void loadDemo_Road();

    /**
     * @brief ДЕМО: Запись 3 - Развилка (PRESENT, 2 выбора)
     */
    void loadDemo_Choice();

    /**
     * @brief ДЕМО: Запись 4A - Трасса (PAST)
     */
    void loadDemo_Highway();

    /**
     * @brief ДЕМО: Запись 4B - Проселок (PAST)
     */
    void loadDemo_Backroad();

    /**
     * @brief ДЕМО: Запись 5 - Финал (THOUGHT)
     */
    void loadDemo_Finale();

    /**
     * @brief ДЕМО: Придорожное кафе
     */
    void loadDemo_RestStop();

    /**
     * @brief ДЕМО: Встреча с механиком
     */
    void loadDemo_MechanicEncounter();

    /**
     * @brief ДЕМО: Низкий уровень топлива
     */
    void loadDemo_LowFuel();

    /**
     * @brief ДЕМО: Финальный выбор пути
     */
    void loadDemo_FinalChoice();

    /**
     * @brief ДЕМО: Путь через лес
     */
    void loadDemo_ForestPath();

    /**
     * @brief ДЕМО: Путь через город
     */
    void loadDemo_CityPath();

    /**
     * @brief ДЕМО: Конец демонстрации
     */
    void loadDemo_End();
};

#endif // NOTEBOOK_SCENE_H
