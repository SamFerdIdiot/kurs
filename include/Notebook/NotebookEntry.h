#ifndef NOTEBOOK_ENTRY_H
#define NOTEBOOK_ENTRY_H

#include <string>
#include <vector>
#include <functional>

// Forward declaration
class PlayerState;

/**
 * @brief Тип записи в блокноте
 */
enum class EntryType {
    PRESENT,   // Настоящее время: выбор здесь и сейчас
    PAST,      // Воспоминание: просто чтение, без выбора
    THOUGHT    // Внутренний монолог: размышления
};

/**
 * @brief Вариант выбора в записи
 */
struct NotebookChoice {
    std::string text;                                    // Текст выбора
    std::function<void(PlayerState*)> action;            // Действие при выборе (изменение ресурсов и т.д.)
    std::vector<std::string> nextEntryIds;               // Возможные следующие записи (ветвление!)
    float probability = 1.0f;                            // Вероятность перехода к каждой записи

    // Требования для активации выбора
    float energyRequired = 0.0f;                         // Нужна энергия
    float fuelRequired = 0.0f;                           // Нужен бензин
    float moneyRequired = 0.0f;                          // Нужны деньги
    bool isDisabled = false;                             // Выбор заблокирован
    std::string disabledReason;                          // Причина блокировки

    /**
     * @brief Проверить, доступен ли выбор для игрока
     */
    bool isAvailable(const PlayerState* player) const;
};

/**
 * @brief Запись в блокноте
 */
struct NotebookEntry {
    std::string id;                                      // Уникальный ID записи
    EntryType type;                                      // Тип записи
    std::string text;                                    // Основной текст записи
    std::vector<NotebookChoice> choices;                 // Варианты выбора

    // Анимация печати текста
    float printSpeed = 50.0f;                            // символов/секунду
    bool canSkip = true;                                 // Можно ли пропустить анимацию

    // Ветвление сюжета
    std::string branchId;                                // ID ветки сюжета ("highway", "backroad", etc.)

    // Метаданные
    std::string date;                                    // Дата записи (игровое время)
    std::string location;                                // Локация
    std::string mood;                                    // Настроение ("спокойный", "напряженный", etc.)

    /**
     * @brief Конструктор по умолчанию
     */
    NotebookEntry();

    /**
     * @brief Создать запись с основными параметрами
     */
    NotebookEntry(const std::string& id,
                  EntryType type,
                  const std::string& text);

    /**
     * @brief Добавить выбор к записи
     */
    void addChoice(const NotebookChoice& choice);

    /**
     * @brief Получить доступные выборы для игрока
     */
    std::vector<NotebookChoice> getAvailableChoices(const PlayerState* player) const;

    /**
     * @brief Проверить, есть ли у записи выборы
     */
    bool hasChoices() const;
};

#endif // NOTEBOOK_ENTRY_H
