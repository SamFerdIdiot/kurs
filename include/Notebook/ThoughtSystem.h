#ifndef THOUGHT_SYSTEM_H
#define THOUGHT_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

/**
 * @brief Система всплывающих мыслей во время выборов
 *
 * Простая система для оживления долгих раздумий.
 * Мысли появляются автоматически после определенного времени.
 */
class ThoughtSystem {
public:
    /**
     * @brief Конструктор
     */
    ThoughtSystem();

    /**
     * @brief Обновить систему мыслей
     * @param deltaTime Время с последнего кадра
     * @param timeSinceChoice Время с момента появления выбора (секунды)
     */
    void update(float deltaTime, float timeSinceChoice);

    /**
     * @brief Отрисовать мысли на экране
     * @param window Окно для рендера
     * @param font Шрифт для текста
     */
    void render(sf::RenderWindow& window, const sf::Font& font);

    /**
     * @brief Сбросить таймеры (при новом выборе)
     */
    void reset();

    /**
     * @brief Показать конкретную мысль
     * @param text Текст мысли
     */
    void spawnThought(const std::string& text);

    /**
     * @brief Включить тестовый режим (короткие задержки для быстрого тестирования)
     * @param enabled true - тестовый режим (5/10/15 сек), false - обычный (30/60/90 сек)
     */
    void enableTestMode(bool enabled = true);

private:
    /**
     * @brief Одна всплывающая мысль
     */
    struct ThoughtBubble {
        std::string text;
        float lifetime;           // Сколько уже живет (секунды)
        float maxLifetime;        // Максимальное время жизни
        float alpha;              // Прозрачность (0-255)
        sf::Vector2f position;    // Позиция на экране
    };

    // === СОСТОЯНИЕ ===
    std::vector<ThoughtBubble> m_activeThoughts;  // Активные мысли
    float m_thoughtTimer;                         // Таймер до следующей мысли
    float m_nextThoughtDelay;                     // Задержка до следующей мысли
    int m_thoughtsSpawned;                        // Сколько мыслей уже показано
    bool m_testMode;                              // Тестовый режим (короткие задержки)

    // === КОНФИГУРАЦИЯ ===
    static constexpr float THOUGHT_LIFETIME = 8.0f;       // Время жизни мысли
    static constexpr float FADE_IN_TIME = 0.5f;           // Время появления
    static constexpr float FADE_OUT_TIME = 1.0f;          // Время исчезновения

    // Обычный режим
    static constexpr float FIRST_THOUGHT_DELAY = 30.0f;   // Первая мысль после 30 сек
    static constexpr float SECOND_THOUGHT_DELAY = 60.0f;  // Вторая после 60 сек
    static constexpr float THIRD_THOUGHT_DELAY = 90.0f;   // Третья после 90 сек

    // Тестовый режим (для быстрой проверки)
    static constexpr float TEST_FIRST_THOUGHT_DELAY = 5.0f;   // Первая мысль после 5 сек
    static constexpr float TEST_SECOND_THOUGHT_DELAY = 10.0f;  // Вторая после 10 сек
    static constexpr float TEST_THIRD_THOUGHT_DELAY = 15.0f;   // Третья после 15 сек

    // === ПУЛЫ МЫСЛЕЙ ===
    std::vector<std::string> m_generalThoughts;   // Общие мысли

    /**
     * @brief Получить случайную мысль из пула
     */
    std::string getRandomThought() const;

    /**
     * @brief Обновить прозрачность мысли (fade in/out)
     */
    void updateThoughtAlpha(ThoughtBubble& thought) const;

    /**
     * @brief Инициализировать пулы мыслей
     */
    void initializeThoughts();
};

#endif // THOUGHT_SYSTEM_H
