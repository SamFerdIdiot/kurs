#include "AbilityEffectManager.h"
#include "NPC.h"
#include "PlayerState.h"
#include "Car.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

// ============================================================================
// Constructor
// ============================================================================

AbilityEffectManager::AbilityEffectManager()
    : m_grannyAdviceUsed(false),
      m_daysSinceLastBenefit(0) {
    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

// ============================================================================
// Пассивные способности
// ============================================================================

void AbilityEffectManager::applyPassiveEffects(PlayerState* playerState, Car* car) {
    if (!playerState || !car) {
        return;
    }

    // Получить всех NPC из команды
    NPCManager& npcManager = NPCManager::getInstance();
    const auto& team = npcManager.getTeam();

    // Применить пассивные эффекты каждого NPC
    for (const auto& npc : team) {
        if (npc && npc->isInParty()) {
            applyNPCPassiveEffect(npc.get(), playerState, car);
        }
    }
}

void AbilityEffectManager::applyNPCPassiveEffect(const NPC* npc, PlayerState* playerState, Car* car) {
    if (!npc || !playerState || !car) {
        return;
    }

    // Проверить есть ли пассивная способность
    if (!npc->hasPassiveAbility()) {
        return;
    }

    const PassiveAbility& ability = npc->getPassiveAbility();

    // Применить эффекты на основе ID способности
    if (ability.id == "mechanic_repair") {
        // Механик Михалыч: ремонт машины +5% каждый день если <90%
        applyMechanicRepair(car);
    }
    else if (ability.id == "unemployed_benefit") {
        // Безработный Виталий: пособие по безработице (обрабатывается отдельно)
        // См. applyUnemployedBenefit()
    }
    else if (ability.id == "punk_guitar") {
        // Панк Вася: гитара (бонус к отношениям применяется в диалогах)
        // См. getPunkRelationshipBonus()
    }
    else if (ability.id == "granny_pies") {
        // Бабушка Галина: пирожки +10 энергии каждый день
        applyGrannyPies(playerState);
    }
    else if (ability.id == "trucker_fuel_economy") {
        // Дальнобойщик Петрович: экономия топлива
        // Обрабатывается через getFuelConsumptionModifier()
    }
    else if (ability.id == "student_youth") {
        // Студент Лёха: +10 к максимальной энергии
        // Обрабатывается через getMaxEnergyBonus()
    }
}

float AbilityEffectManager::getFuelConsumptionModifier() const {
    float modifier = 1.0f;

    // Проверить команду на наличие дальнобойщика
    NPCManager& npcManager = NPCManager::getInstance();
    const auto& team = npcManager.getTeam();

    for (const auto& npc : team) {
        if (npc && npc->isInParty() && npc->hasPassiveAbility()) {
            const PassiveAbility& ability = npc->getPassiveAbility();
            if (ability.id == "trucker_fuel_economy") {
                // -15% расход топлива
                modifier *= 0.85f;
            }
        }
    }

    return modifier;
}

float AbilityEffectManager::getMaxEnergyBonus() const {
    float bonus = 0.0f;

    // Проверить команду на наличие студента
    NPCManager& npcManager = NPCManager::getInstance();
    const auto& team = npcManager.getTeam();

    for (const auto& npc : team) {
        if (npc && npc->isInParty() && npc->hasPassiveAbility()) {
            const PassiveAbility& ability = npc->getPassiveAbility();
            if (ability.id == "student_youth") {
                // +10 к максимальной энергии
                bonus += 10.0f;
            }
        }
    }

    return bonus;
}

// ============================================================================
// Активные способности
// ============================================================================

bool AbilityEffectManager::useActiveAbility(const std::string& npcId, PlayerState* playerState, Car* car) {
    if (!playerState || !car) {
        return false;
    }

    // Проверить доступность способности
    if (!isAbilityReady(npcId)) {
        return false;
    }

    // Получить NPC
    NPC* npc = getNPC(npcId);
    if (!npc || !npc->hasActiveAbility()) {
        return false;
    }

    const ActiveAbility& ability = npc->getActiveAbility();
    bool success = false;

    // Выполнить способность на основе ID
    if (ability.id == "mechanic_field_repair") {
        success = useMechanicFieldRepair(car);
    }
    else if (ability.id == "unemployed_discount") {
        success = useUnemployedDiscount();
    }
    else if (ability.id == "punk_concert") {
        success = usePunkConcert(playerState);
    }
    else if (ability.id == "granny_advice") {
        success = useGrannyAdvice();
    }
    else if (ability.id == "trucker_radio") {
        success = useTruckerRadio();
    }
    else if (ability.id == "student_help_driving") {
        success = useStudentHelp(playerState);
    }

    // Если успешно, установить кулдаун
    if (success) {
        setCooldown(npcId, ability.cooldown);
    }

    return success;
}

bool AbilityEffectManager::isAbilityReady(const std::string& npcId) const {
    auto it = m_cooldowns.find(npcId);
    if (it == m_cooldowns.end()) {
        return true; // Нет кулдауна = готово
    }
    return it->second.isReady();
}

float AbilityEffectManager::getRemainingCooldown(const std::string& npcId) const {
    auto it = m_cooldowns.find(npcId);
    if (it == m_cooldowns.end()) {
        return 0.0f;
    }
    return it->second.remainingTime;
}

void AbilityEffectManager::updateCooldowns(float hoursElapsed) {
    for (auto& pair : m_cooldowns) {
        pair.second.tick(hoursElapsed);
    }
}

// ============================================================================
// Специфические эффекты пассивных способностей
// ============================================================================

void AbilityEffectManager::applyMechanicRepair(Car* car) {
    if (!car) {
        return;
    }

    float condition = car->getCondition();
    if (condition < 90.0f) {
        // Ремонт +5% состояния
        car->setCondition(std::min(100.0f, condition + 5.0f));
    }
}

void AbilityEffectManager::applyUnemployedBenefit(PlayerState* playerState, int daysPassed) {
    if (!playerState) {
        return;
    }

    // Проверить наличие безработного в команде
    NPCManager& npcManager = NPCManager::getInstance();
    NPC* unemployed = npcManager.getNPC("unemployed_vitaliy");

    if (!unemployed || !unemployed->isInParty()) {
        return;
    }

    m_daysSinceLastBenefit += daysPassed;

    // Каждые 7 дней выдавать пособие
    if (m_daysSinceLastBenefit >= 7) {
        playerState->addMoney(100.0f);
        m_daysSinceLastBenefit = 0;
    }
}

int AbilityEffectManager::getPunkRelationshipBonus() const {
    // Проверить наличие панка в команде
    NPCManager& npcManager = NPCManager::getInstance();
    NPC* punk = npcManager.getNPC("punk_vasya");

    if (!punk || !punk->isInParty()) {
        return 0;
    }

    // +10 к отношениям с молодыми NPC
    return 10;
}

void AbilityEffectManager::applyGrannyPies(PlayerState* playerState) {
    if (!playerState) {
        return;
    }

    // Восстановить 10 энергии
    playerState->addEnergy(10.0f);
}

// ============================================================================
// Активные способности (специфические)
// ============================================================================

bool AbilityEffectManager::useMechanicFieldRepair(Car* car) {
    if (!car) {
        return false;
    }

    // Восстановить 20% состояния машины
    float condition = car->getCondition();
    car->setCondition(std::min(100.0f, condition + 20.0f));
    return true;
}

bool AbilityEffectManager::useUnemployedDiscount() {
    // Эта способность применяется в магазинах
    // Здесь просто возвращаем true чтобы установился кулдаун
    // Реальная логика скидки должна быть в SceneShop или аналогичной сцене
    return true;
}

bool AbilityEffectManager::usePunkConcert(PlayerState* playerState) {
    if (!playerState) {
        return false;
    }

    // Заработать 200₽
    playerState->addMoney(200.0f);

    // Риск штрафа (30% шанс)
    int random = std::rand() % 100;
    if (random < 30) {
        // Штраф 50₽
        playerState->addMoney(-50.0f);
    }

    return true;
}

bool AbilityEffectManager::useGrannyAdvice() {
    // Одноразовая способность
    if (m_grannyAdviceUsed) {
        return false;
    }

    // Эта способность должна давать подсказку о событии
    // Реализация зависит от системы событий
    // Здесь просто помечаем как использованную
    m_grannyAdviceUsed = true;
    return true;
}

bool AbilityEffectManager::useTruckerRadio() {
    // Эта способность дает информацию о дороге
    // Реализация зависит от системы дорог и событий
    // Здесь просто возвращаем true чтобы установился кулдаун
    return true;
}

bool AbilityEffectManager::useStudentHelp(PlayerState* playerState) {
    if (!playerState) {
        return false;
    }

    // Восстановление энергии +20
    playerState->addEnergy(20.0f);
    return true;
}

// ============================================================================
// Утилиты
// ============================================================================

void AbilityEffectManager::resetCooldowns() {
    m_cooldowns.clear();
    m_grannyAdviceUsed = false;
    m_daysSinceLastBenefit = 0;
}

void AbilityEffectManager::setCooldown(const std::string& npcId, float hours) {
    m_cooldowns[npcId] = AbilityCooldown(npcId, hours);
}

bool AbilityEffectManager::isNPCInParty(const std::string& npcId) const {
    NPCManager& npcManager = NPCManager::getInstance();
    NPC* npc = npcManager.getNPC(npcId);
    return npc && npc->isInParty();
}

NPC* AbilityEffectManager::getNPC(const std::string& npcId) const {
    NPCManager& npcManager = NPCManager::getInstance();
    return npcManager.getNPC(npcId);
}
