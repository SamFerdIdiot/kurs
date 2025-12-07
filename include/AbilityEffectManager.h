#ifndef ABILITY_EFFECT_MANAGER_H
#define ABILITY_EFFECT_MANAGER_H

#include <string>
#include <map>
#include <memory>

// Forward declarations
class NPC;
class PlayerState;
class Car;

// Структура для отслеживания кулдаунов активных способностей
struct AbilityCooldown {
    std::string abilityId;
    float remainingTime;  // Время в часах до готовности

    AbilityCooldown() : remainingTime(0.0f) {}
    AbilityCooldown(const std::string& id, float time)
        : abilityId(id), remainingTime(time) {}

    bool isReady() const { return remainingTime <= 0.0f; }
    void tick(float hours) { remainingTime = std::max(0.0f, remainingTime - hours); }
};

// Менеджер эффектов способностей
// Отвечает за:
// - Применение пассивных способностей (ежедневно/постоянно)
// - Выполнение активных способностей
// - Отслеживание кулдаунов активных способностей
class AbilityEffectManager {
public:
    AbilityEffectManager();

    // ============================================================================
    // Пассивные способности
    // ============================================================================

    // Применить все пассивные эффекты от команды
    // Вызывается каждый день при отдыхе или в определённые моменты
    void applyPassiveEffects(PlayerState* playerState, Car* car);

    // Применить пассивный эффект конкретного NPC
    void applyNPCPassiveEffect(const NPC* npc, PlayerState* playerState, Car* car);

    // Получить модификатор экономии топлива от пассивных способностей команды
    // Возвращает множитель (например, 0.85 = -15% расхода)
    float getFuelConsumptionModifier() const;

    // Получить бонус к максимальной энергии от пассивных способностей
    float getMaxEnergyBonus() const;

    // ============================================================================
    // Активные способности
    // ============================================================================

    // Использовать активную способность NPC
    // Возвращает true если способность успешно использована
    bool useActiveAbility(const std::string& npcId, PlayerState* playerState, Car* car);

    // Проверить, доступна ли активная способность NPC
    bool isAbilityReady(const std::string& npcId) const;

    // Получить оставшееся время кулдауна способности (в часах)
    float getRemainingCooldown(const std::string& npcId) const;

    // Обновить кулдауны (вызывается при прохождении времени)
    void updateCooldowns(float hoursElapsed);

    // ============================================================================
    // Специфические эффекты способностей
    // ============================================================================

    // Механик Михалыч: Ремонт машины
    // Каждый день чинит машину (+5% состояния если <90%)
    void applyMechanicRepair(Car* car);

    // Безработный Виталий: Пособие по безработице
    // +100₽ каждую неделю (каждые 7 дней)
    void applyUnemployedBenefit(PlayerState* playerState, int daysPassed);

    // Панк Вася: Гитара
    // +10 к отношениям с молодыми NPC (применяется в диалогах)
    int getPunkRelationshipBonus() const;

    // Бабушка Галина: Пирожки
    // Восстанавливает 10 энергии каждый день
    void applyGrannyPies(PlayerState* playerState);

    // Дальнобойщик Петрович: Знание дорог
    // Экономия топлива -15% (учитывается в getFuelConsumptionModifier)

    // Студент Лёха: Молодость
    // +10 к максимальной энергии (учитывается в getMaxEnergyBonus)

    // ============================================================================
    // Активные способности (специфические)
    // ============================================================================

    // Механик: Полевой ремонт - восстановить 20% состояния машины
    bool useMechanicFieldRepair(Car* car);

    // Безработный: Знание цен - скидка -20% в магазинах (не реализовано здесь)
    bool useUnemployedDiscount();

    // Панк: Концерт - заработать 200₽ (риск штрафа - случайно)
    bool usePunkConcert(PlayerState* playerState);

    // Бабушка: Материнский совет - подсказка о событии (одноразовая)
    bool useGrannyAdvice();

    // Дальнобойщик: Связь с дальнобойщиками - информация о дороге
    bool useTruckerRadio();

    // Студент: Помощь в пути - восстановление энергии +20
    bool useStudentHelp(PlayerState* playerState);

    // ============================================================================
    // Утилиты
    // ============================================================================

    // Сбросить все кулдауны (для новой игры)
    void resetCooldowns();

    // Получить информацию о всех активных кулдаунах
    const std::map<std::string, AbilityCooldown>& getAllCooldowns() const {
        return m_cooldowns;
    }

private:
    // Кулдауны активных способностей (npcId -> cooldown)
    std::map<std::string, AbilityCooldown> m_cooldowns;

    // Флаг для одноразовой способности бабушки
    bool m_grannyAdviceUsed;

    // Счётчик дней для пособия безработного
    int m_daysSinceLastBenefit;

    // Установить кулдаун для способности
    void setCooldown(const std::string& npcId, float hours);

    // Проверить наличие NPC в команде по ID
    bool isNPCInParty(const std::string& npcId) const;

    // Получить указатель на NPC по ID
    NPC* getNPC(const std::string& npcId) const;
};

#endif // ABILITY_EFFECT_MANAGER_H
