#pragma once

#include "PlayerState.h"
#include "EventManager.h"
#include <string>
#include <vector>

/**
 * @brief Система событий, связанных с ресурсами
 *
 * Отслеживает уровни ресурсов (топливо, энергия, деньги, состояние машины)
 * и автоматически вызывает события при достижении критических порогов
 */
class ResourceEventSystem {
public:
    /**
     * @brief Пороги для триггера событий
     */
    struct ResourceThresholds {
        // Топливо
        float fuel_critical = 20.0f;    // <20L - критично
        float fuel_low = 35.0f;          // <35L - мало
        float fuel_very_low = 10.0f;     // <10L - очень мало

        // Энергия
        float energy_exhausted = 15.0f;  // <15% - истощение
        float energy_tired = 30.0f;      // <30% - усталость
        float energy_very_tired = 10.0f; // <10% - очень устал

        // Деньги
        float money_broke = 50.0f;       // <50₽ - почти без денег
        float money_low = 200.0f;        // <200₽ - мало денег

        // Состояние машины
        float vehicle_broken = 25.0f;    // <25% - критическое
        float vehicle_damaged = 50.0f;   // <50% - повреждена

        // Настроение
        float mood_depressed = 20.0f;    // <20% - депрессия
        float mood_low = 40.0f;          // <40% - плохое настроение
    };

    /**
     * @brief Конструктор
     * @param playerState Указатель на состояние игрока
     * @param eventManager Указатель на менеджер событий
     */
    ResourceEventSystem(PlayerState* playerState, EventManager* eventManager);

    /**
     * @brief Проверить все ресурсы и вызвать события при необходимости
     * @return ID события, которое нужно показать (или пустая строка)
     */
    std::string checkResourcesAndTriggerEvents();

    /**
     * @brief Проверить конкретный ресурс
     * @param resourceType Тип ресурса для проверки
     * @return ID события или пустая строка
     */
    std::string checkSpecificResource(const std::string& resourceType);

    /**
     * @brief Получить предупреждение о состоянии ресурсов
     * @return Текст предупреждения
     */
    std::string getResourceWarning() const;

    /**
     * @brief Проверить, есть ли критические ресурсы
     * @return true если хотя бы один ресурс в критическом состоянии
     */
    bool hasCriticalResources() const;

    /**
     * @brief Установить пороги
     */
    void setThresholds(const ResourceThresholds& thresholds);

private:
    PlayerState* m_playerState;
    EventManager* m_eventManager;
    ResourceThresholds m_thresholds;

    // Флаги для отслеживания уже показанных предупреждений
    bool m_fuelWarningShown = false;
    bool m_energyWarningShown = false;
    bool m_moneyWarningShown = false;
    bool m_vehicleWarningShown = false;

    /**
     * @brief Проверить топливо
     */
    std::string checkFuel();

    /**
     * @brief Проверить энергию
     */
    std::string checkEnergy();

    /**
     * @brief Проверить деньги
     */
    std::string checkMoney();

    /**
     * @brief Проверить состояние машины
     */
    std::string checkVehicle();

    /**
     * @brief Проверить настроение
     */
    std::string checkMood();

    /**
     * @brief Сбросить флаги предупреждений (когда ресурс восстановлен)
     */
    void resetWarningFlags();
};
