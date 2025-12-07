#ifndef JOURNAL_CITY_NOTE_H
#define JOURNAL_CITY_NOTE_H

#include "Journal/Note.h"
#include "PlayerState.h"
#include <string>

/**
 * @brief CityNote - записка города
 *
 * Отображается при прибытии в город.
 * Показывает описание города и доступные сервисы:
 * - Заправка
 * - Работа
 * - Мотель
 * - Магазин
 * - Продолжить путь
 *
 * Пример:
 * ┌────────────────────────────────┐
 * │ 📍 МОСКВА                      │
 * │                                │
 * │ Вы въезжаете в столицу...      │
 * │                                │
 * │ [1] Заправка (500₽)           │
 * │ [2] Работа                     │
 * │ [3] Отправиться в путь         │
 * └────────────────────────────────┘
 */
class CityNote : public Note {
public:
    /**
     * @brief Конструктор записки города
     * @param cityId ID города ("moscow", "spb", "vladivostok" и т.д.)
     * @param playerState Состояние игрока
     */
    CityNote(const std::string& cityId, PlayerState* playerState);

    // === ИНТЕРФЕЙС NOTE ===

    std::string getTitle() const override;
    std::string getDescription() const override;
    std::vector<NoteChoice> getChoices() const override;
    Type getType() const override { return Type::CITY; }

    void executeChoice(int choiceIndex, PlayerState* playerState) override;

    // === СПЕЦИФИЧНЫЕ ДЛЯ CITYNOTE ===

    /**
     * @brief Получить название города
     * @return Название (например, "Москва", "Санкт-Петербург")
     */
    std::string getCityName() const;

    /**
     * @brief Получить выбранное действие (для перехода к следующей сцене)
     * @return Тип действия
     */
    enum class Action {
        NONE,           // Ничего не выбрано
        GAS_STATION,    // Заправка
        WORK,           // Работа
        MOTEL,          // Мотель
        SHOP,           // Магазин
        TRAVEL          // Отправиться в путь
    };

    Action getSelectedAction() const { return m_selectedAction; }

private:
    std::string m_cityId;               // ID города
    PlayerState* m_playerState;         // Состояние игрока
    Action m_selectedAction;            // Выбранное действие

    /**
     * @brief Генерировать динамическое описание города
     * @return Описание на основе времени суток, погоды, состояния игрока
     */
    std::string generateCityDescription() const;

    /**
     * @brief Создать варианты выбора на основе текущего состояния
     */
    void buildChoices(std::vector<NoteChoice>& choices) const;
};

#endif // JOURNAL_CITY_NOTE_H
