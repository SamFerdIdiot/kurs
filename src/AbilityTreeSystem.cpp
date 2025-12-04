#include "AbilityTreeSystem.h"
#include "ExperienceSystem.h"
#include "PlayerState.h"
#include "GameStateManager.h"
#include "InventorySystem.h"
#include <algorithm>
#include <fstream>
#include <iostream>

AbilityTreeSystem* AbilityTreeSystem::s_instance = nullptr;

AbilityTreeSystem::AbilityTreeSystem()
    : m_availableSkillPoints(0) {
}

AbilityTreeSystem& AbilityTreeSystem::getInstance() {
    if (!s_instance) {
        s_instance = new AbilityTreeSystem();
    }
    return *s_instance;
}

void AbilityTreeSystem::initialize() {
    m_abilities.clear();
    m_activePerkCharges.clear();
    m_availableSkillPoints = 0;
    initializeAbilityTree();
}

void AbilityTreeSystem::reset() {
    m_abilities.clear();
    m_activePerkCharges.clear();
    m_availableSkillPoints = 0;
    initializeAbilityTree();
}

void AbilityTreeSystem::addAbility(const AbilityNode& ability) {
    m_abilities.push_back(ability);
}

// === Управление способностями ===

bool AbilityTreeSystem::unlockAbility(const std::string& abilityId) {
    // Шаг 1: Найти способность
    AbilityNode* ability = getAbilityMutable(abilityId);
    if (!ability) {
        return false;  // Способность не найдена
    }
    
    // Шаг 2: Собрать список разблокированных способностей
    std::vector<std::string> unlockedIds;
    for (const auto& node : m_abilities) {
        if (node.isUnlocked()) {
            unlockedIds.push_back(node.getId());
        }
    }
    
    // Шаг 3: Проверить возможность разблокировки
    int playerLevel = ExperienceSystem::getInstance().getLevel();
    if (!ability->canUnlock(playerLevel, m_availableSkillPoints, unlockedIds)) {
        return false;  // Не выполнены требования
    }
    
    // Шаг 4: Разблокировать способность
    ability->setUnlocked(true);
    
    // Шаг 5: Инициализировать заряды для активного перка
    if (ability->getType() == AbilityType::ACTIVE) {
        initializePerkCharges(abilityId, ability->getEffect().maxCharges);
    }
    
    // Шаг 6: Списать очки способностей
    m_availableSkillPoints -= ability->getRequirement().skillPointCost;
    
    // Шаг 7: Сохранить изменения
    save();
    
    return true;
}

bool AbilityTreeSystem::isAbilityUnlocked(const std::string& abilityId) const {
    const AbilityNode* ability = getAbility(abilityId);
    return ability && ability->isUnlocked();
}

// === Получение информации ===

std::vector<AbilityNode> AbilityTreeSystem::getAbilitiesByCategory(AbilityCategory category) const {
    std::vector<AbilityNode> result;
    for (const auto& ability : m_abilities) {
        if (ability.getCategory() == category) {
            result.push_back(ability);
        }
    }
    return result;
}

const AbilityNode* AbilityTreeSystem::getAbility(const std::string& abilityId) const {
    for (const auto& ability : m_abilities) {
        if (ability.getId() == abilityId) {
            return &ability;
        }
    }
    return nullptr;
}

AbilityNode* AbilityTreeSystem::getAbilityMutable(const std::string& abilityId) {
    for (auto& ability : m_abilities) {
        if (ability.getId() == abilityId) {
            return &ability;
        }
    }
    return nullptr;
}

// Получить активные перки для события по типу сохранения ресурса
std::vector<const AbilityNode*> AbilityTreeSystem::getActivePerksForEvent(ResourcePreservationType preservationType) const {
    std::vector<const AbilityNode*> result;
    
    for (const auto& ability : m_abilities) {
        // Проверяем: активный перк, разблокирован, есть заряды, подходящий тип сохранения
        if (ability.getType() == AbilityType::ACTIVE &&
            ability.isUnlocked() &&
            hasCharges(ability.getId()) &&
            ability.getEffect().preservationType == preservationType) {
            
            result.push_back(&ability);
        }
    }
    
    return result;
}

// === Очки способностей ===

void AbilityTreeSystem::addSkillPoints(int points) {
    m_availableSkillPoints += points;
}

bool AbilityTreeSystem::spendSkillPoints(int points) {
    if (m_availableSkillPoints >= points) {
        m_availableSkillPoints -= points;
        return true;
    }
    return false;
}

// === Активные перки (система зарядов) ===

bool AbilityTreeSystem::canUseActivePerk(const std::string& perkId) const {
    // Проверка 1: Разблокирован ли перк
    const AbilityNode* perk = getAbility(perkId);
    if (!perk || !perk->isUnlocked() || perk->getType() != AbilityType::ACTIVE) {
        return false;
    }
    
    // Проверка 2: Есть ли заряды
    if (!hasCharges(perkId)) {
        return false;
    }
    
    // [MVP] Disabled - Resource checks (uncomment to enable)
    /*
    // Проверка 3: Достаточно ли ресурсов
    const AbilityEffect& effect = perk->getEffect();
    auto& playerState = GameStateManager::getInstance().getPlayerState();

    if (effect.energyCost > 0 && playerState.getEnergy() < effect.energyCost) {
        return false;
    }

    if (effect.moneyCost > 0 && playerState.getMoney() < effect.moneyCost) {
        return false;
    }

    // Проверка предметов через InventorySystem
    const auto& inventory = GameStateManager::getInstance().getPlayerState().getInventory();
    for (const auto& itemName : effect.itemRequirements) {
        if (!inventory.hasItem(itemName, 1)) {
            return false;  // Не хватает требуемого предмета
        }
    }
    */

    return true;
}

bool AbilityTreeSystem::useActivePerk(const std::string& perkId) {
    // Шаг 1: Найти перк
    const AbilityNode* perk = getAbility(perkId);
    if (!perk || !perk->isUnlocked() || perk->getType() != AbilityType::ACTIVE) {
        return false;
    }
    
    // Шаг 2: Проверить наличие зарядов
    if (!hasCharges(perkId)) {
        return false;  // Нет зарядов
    }
    
    // [MVP] Disabled - Resource deduction (uncomment to enable)
    /*
    // Шаг 3: Проверить и списать ресурсы
    const AbilityEffect& effect = perk->getEffect();
    auto& playerState = GameStateManager::getInstance().getPlayerState();

    if (effect.energyCost > 0) {
        if (playerState.getEnergy() < effect.energyCost) {
            return false;  // Недостаточно энергии
        }
        playerState.addEnergy(-effect.energyCost);
    }

    if (effect.moneyCost > 0) {
        if (playerState.getMoney() < effect.moneyCost) {
            return false;  // Недостаточно денег
        }
        playerState.addMoney(-effect.moneyCost);
    }

    // Проверка и удаление предметов через InventorySystem
    auto& inventory = GameStateManager::getInstance().getPlayerState().getInventory();
    for (const auto& itemName : effect.itemRequirements) {
        if (!inventory.hasItem(itemName, 1)) {
            return false;  // Не хватает требуемого предмета
        }
    }

    // Удаление использованных предметов
    for (const auto& itemName : effect.itemRequirements) {
        inventory.removeItemByName(itemName, 1);
    }
    */

    // Шаг 4: Применить эффект перка
    // (Это будет зависеть от контекста использования)
    // Например, через событие или прямое изменение состояния
    
    // Шаг 5: Уменьшить количество зарядов
    decrementCharge(perkId);
    
    return true;
}

void AbilityTreeSystem::updateCooldowns(float deltaTimeMinutes) {
    // NOTE: Система зарядов больше не использует кулдауны
    // Заряды восстанавливаются только через отдых (restoreCharges)
    // Этот метод оставлен пустым для обратной совместимости
}

// === Пассивные бонусы ===

float AbilityTreeSystem::getPassiveBonus(const std::string& effectType) const {
    float totalMultiplier = 1.0f;
    
    // Проходим по всем разблокированным пассивным способностям
    for (const auto& ability : m_abilities) {
        if (ability.isUnlocked() &&
            ability.getType() == AbilityType::PASSIVE &&
            ability.getEffect().effectType == effectType) {
            
            // Умножаем бонусы (1.1 × 1.15 = 1.265)
            totalMultiplier *= ability.getEffect().bonusMultiplier;
        }
    }
    
    return totalMultiplier;
}

int AbilityTreeSystem::getPassiveBonusFlat(const std::string& effectType) const {
    int totalBonus = 0;
    
    // Суммируем плоские бонусы
    for (const auto& ability : m_abilities) {
        if (ability.isUnlocked() &&
            ability.getType() == AbilityType::PASSIVE &&
            ability.getEffect().effectType == effectType) {
            
            totalBonus += ability.getEffect().bonusFlat;
        }
    }
    
    return totalBonus;
}

// === Сохранение/загрузка ===

void AbilityTreeSystem::save() const {
    std::ofstream file("saves/ability_tree.sav");
    if (!file.is_open()) {
        std::cerr << "Failed to save ability tree" << std::endl;
        return;
    }

    file << "{\n";
    file << "  \"availableSkillPoints\": " << m_availableSkillPoints << ",\n";

    // Сохранить разблокированные способности
    file << "  \"unlockedAbilities\": [\n";
    bool firstAbility = true;
    for (const auto& ability : m_abilities) {
        if (ability.isUnlocked()) {
            if (!firstAbility) file << ",\n";
            file << "    \"" << ability.getId() << "\"";
            firstAbility = false;
        }
    }
    file << "\n  ],\n";

    // Сохранить заряды активных перков
    file << "  \"perkCharges\": [\n";
    bool firstCharge = true;
    for (const auto& charges : m_activePerkCharges) {
        if (!firstCharge) file << ",\n";
        file << "    {\n";
        file << "      \"perkId\": \"" << charges.perkId << "\",\n";
        file << "      \"currentCharges\": " << charges.currentCharges << ",\n";
        file << "      \"maxCharges\": " << charges.maxCharges << "\n";
        file << "    }";
        firstCharge = false;
    }
    file << "\n  ]\n";

    file << "}\n";
    file.close();
}

void AbilityTreeSystem::load() {
    std::ifstream file("saves/ability_tree.sav");
    if (!file.is_open()) {
        // Файл не существует - это нормально для новой игры
        return;
    }

    std::string line;
    std::vector<std::string> unlockedIds;

    // Простой парсер (как в SaveSystem)
    while (std::getline(file, line)) {
        // Пропустить пустые строки и скобки
        if (line.find("{") != std::string::npos ||
            line.find("}") != std::string::npos ||
            line.empty()) {
            continue;
        }

        // Парсинг availableSkillPoints
        if (line.find("\"availableSkillPoints\"") != std::string::npos) {
            size_t colonPos = line.find(":");
            if (colonPos != std::string::npos) {
                std::string value = line.substr(colonPos + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t,") + 1);
                m_availableSkillPoints = std::stoi(value);
            }
        }

        // Парсинг ID разблокированных способностей
        if (line.find("\"") != std::string::npos &&
            line.find("availableSkillPoints") == std::string::npos &&
            line.find("unlockedAbilities") == std::string::npos &&
            line.find("perkCharges") == std::string::npos &&
            line.find("perkId") == std::string::npos &&
            line.find("currentCharges") == std::string::npos &&
            line.find("maxCharges") == std::string::npos) {

            size_t firstQuote = line.find("\"");
            size_t lastQuote = line.rfind("\"");
            if (firstQuote != std::string::npos && lastQuote != firstQuote) {
                std::string abilityId = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
                unlockedIds.push_back(abilityId);
            }
        }
    }

    file.close();

    // Применить разблокированные способности
    for (const auto& id : unlockedIds) {
        AbilityNode* ability = getAbilityMutable(id);
        if (ability) {
            ability->setUnlocked(true);

            // Инициализировать заряды для активных перков
            if (ability->getType() == AbilityType::ACTIVE) {
                initializePerkCharges(id, ability->getEffect().maxCharges);
            }
        }
    }

    // Примечание: загрузка зарядов перков требует более сложного парсера
    // Для полной функциональности рекомендуется использовать JSON библиотеку
    // Сейчас заряды будут восстановлены до максимума при загрузке
}

// === Система зарядов ===

void AbilityTreeSystem::initializePerkCharges(const std::string& perkId, int maxCharges) {
    // Проверяем, не добавлены ли уже заряды для этого перка
    for (const auto& charges : m_activePerkCharges) {
        if (charges.perkId == perkId) {
            return;  // Уже существует
        }
    }
    
    // Добавляем новую запись зарядов
    m_activePerkCharges.push_back(ActivePerkCharges(perkId, maxCharges, maxCharges));
}

void AbilityTreeSystem::decrementCharge(const std::string& perkId) {
    for (auto& charges : m_activePerkCharges) {
        if (charges.perkId == perkId) {
            if (charges.currentCharges > 0) {
                charges.currentCharges--;
            }
            return;
        }
    }
}

void AbilityTreeSystem::restoreCharges() {
    // Восстанавливаем все заряды до максимума
    for (auto& charges : m_activePerkCharges) {
        charges.currentCharges = charges.maxCharges;
    }
}

int AbilityTreeSystem::getCurrentCharges(const std::string& perkId) const {
    for (const auto& charges : m_activePerkCharges) {
        if (charges.perkId == perkId) {
            return charges.currentCharges;
        }
    }
    return 0;
}

int AbilityTreeSystem::getMaxCharges(const std::string& perkId) const {
    for (const auto& charges : m_activePerkCharges) {
        if (charges.perkId == perkId) {
            return charges.maxCharges;
        }
    }
    
    // Если заряды не инициализированы, вернуть значение из способности
    const AbilityNode* perk = getAbility(perkId);
    if (perk && perk->getType() == AbilityType::ACTIVE) {
        return perk->getEffect().maxCharges;
    }
    
    return 0;
}

bool AbilityTreeSystem::hasCharges(const std::string& perkId) const {
    return getCurrentCharges(perkId) > 0;
}

// === Инициализация дерева способностей ===

void AbilityTreeSystem::initializeAbilityTree() {
    // ========== КАТЕГОРИЯ: ВОЖДЕНИЕ (DRIVING) ==========
    
    // Пассивные способности вождения (сбалансированы: 3-5% за уровень)
    {
        AbilityRequirement req(1, {}, 1);
        AbilityEffect effect;
        effect.effectType = "fuel_efficiency";
        effect.bonusMultiplier = 1.03f;  // +3% экономия топлива
        addAbility(AbilityNode("driving_economical_1", 
                              "Экономичное вождение I / Economical Driving I",
                              "Снижение расхода топлива на 3% / Reduces fuel consumption by 3%",
                              AbilityType::PASSIVE, AbilityCategory::DRIVING, req, effect));
    }
    
    {
        AbilityRequirement req(3, {"driving_economical_1"}, 1);
        AbilityEffect effect;
        effect.effectType = "fuel_efficiency";
        effect.bonusMultiplier = 1.04f;  // +4% экономия топлива
        addAbility(AbilityNode("driving_economical_2",
                              "Экономичное вождение II / Economical Driving II",
                              "Дополнительное снижение расхода топлива на 4% / Additional 4% fuel efficiency",
                              AbilityType::PASSIVE, AbilityCategory::DRIVING, req, effect));
    }
    
    {
        AbilityRequirement req(5, {"driving_economical_2"}, 1);
        AbilityEffect effect;
        effect.effectType = "fuel_efficiency";
        effect.bonusMultiplier = 1.05f;  // +5% экономия топлива
        addAbility(AbilityNode("driving_economical_3",
                              "Экономичное вождение III / Economical Driving III",
                              "Максимальная экономия топлива +5% / Maximum fuel efficiency +5%",
                              AbilityType::PASSIVE, AbilityCategory::DRIVING, req, effect));
    }
    
    // Активный перк с пассивным компаньоном: Топливная эффективность
    {
        // Пассивная способность для перка сохранения топлива
        AbilityRequirement req(2, {}, 1);
        AbilityEffect effect;
        effect.effectType = "fuel_reserve_bonus";
        effect.bonusFlat = 5;  // +5 литров запас топлива
        addAbility(AbilityNode("driving_fuel_reserve",
                              "Топливный резерв / Fuel Reserve",
                              "Увеличивает запас топлива на 5л / Increases fuel reserve by 5L",
                              AbilityType::PASSIVE, AbilityCategory::DRIVING, req, effect));
    }
    
    {
        // Перк сохранения топлива / Fuel preservation perk
        AbilityRequirement req(3, {"driving_fuel_reserve"}, 1);
        AbilityEffect effect;
        effect.effectType = "fuel_preservation";
        effect.preservationType = ResourcePreservationType::FUEL;
        effect.preserveAmount = 8;  // Сохраняет 8 литров топлива (уменьшено с 10)
        effect.energyCost = 0;
        effect.moneyCost = 0;
        effect.maxCharges = 3;  // 3 использования до отдыха
        effect.effectDescription = "Сохранить 8л топлива в событии / Preserve 8L fuel in event";
        effect.targetType = "event";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_fuel_saver",
                              "Экономия топлива / Fuel Saver",
                              "Позволяет сохранить топливо в событии / Allows preserving fuel in events",
                              AbilityType::ACTIVE, AbilityCategory::DRIVING, req, effect));
    }
    
    // Пассивно-активная пара: Защита машины
    {
        // Пассивная способность для защиты машины
        AbilityRequirement req(3, {}, 1);
        AbilityEffect effect;
        effect.effectType = "vehicle_durability";
        effect.bonusMultiplier = 1.05f;  // +5% прочность машины
        addAbility(AbilityNode("driving_vehicle_durability",
                              "Прочность машины / Vehicle Durability",
                              "Машина на 5% прочнее / Vehicle 5% more durable",
                              AbilityType::PASSIVE, AbilityCategory::DRIVING, req, effect));
    }
    
    {
        // Перк сохранения состояния машины / Vehicle condition preservation perk
        AbilityRequirement req(4, {"driving_vehicle_durability"}, 1);
        AbilityEffect effect;
        effect.effectType = "vehicle_preservation";
        effect.preservationType = ResourcePreservationType::VEHICLE_CONDITION;
        effect.preserveAmount = 15;  // Сохраняет 15% состояния (уменьшено с 20%)
        effect.energyCost = 5;
        effect.maxCharges = 2;  // 2 использования до отдыха
        effect.effectDescription = "Сохранить 15% состояния машины / Preserve 15% vehicle condition";
        effect.targetType = "event";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_vehicle_protector",
                              "Защита машины / Vehicle Protector",
                              "Позволяет избежать износа машины в событии / Prevents vehicle wear in events",
                              AbilityType::ACTIVE, AbilityCategory::DRIVING, req, effect));
    }
    
    // ========== КАТЕГОРИЯ: ТОРГОВЛЯ (TRADING) ==========
    
    // Пассивные способности торговли (сбалансированы)
    {
        AbilityRequirement req(1, {}, 1);
        AbilityEffect effect;
        effect.effectType = "trader_discount";
        effect.bonusMultiplier = 0.97f;  // -3% цены
        addAbility(AbilityNode("trading_discount_1",
                              "Торговец I / Trader I",
                              "Скидка 3% во всех магазинах / 3% discount in all shops",
                              AbilityType::PASSIVE, AbilityCategory::TRADING, req, effect));
    }
    
    {
        AbilityRequirement req(3, {"trading_discount_1"}, 1);
        AbilityEffect effect;
        effect.effectType = "trader_discount";
        effect.bonusMultiplier = 0.96f;  // -4% цены
        addAbility(AbilityNode("trading_discount_2",
                              "Торговец II / Trader II",
                              "Скидка 4% во всех магазинах / 4% discount in all shops",
                              AbilityType::PASSIVE, AbilityCategory::TRADING, req, effect));
    }
    
    // Пассивно-активная пара: Экономия денег
    {
        // Пассивная способность для денежного перка
        AbilityRequirement req(2, {}, 1);
        AbilityEffect effect;
        effect.effectType = "money_sense";
        effect.bonusFlat = 50;  // +50₽ при нахождении денег
        addAbility(AbilityNode("trading_money_sense",
                              "Чутьё на деньги / Money Sense",
                              "Находите на 50₽ больше / Find 50₽ more",
                              AbilityType::PASSIVE, AbilityCategory::TRADING, req, effect));
    }
    
    {
        // Перк сохранения денег / Money preservation perk
        AbilityRequirement req(3, {"trading_money_sense"}, 1);
        AbilityEffect effect;
        effect.effectType = "money_preservation";
        effect.preservationType = ResourcePreservationType::MONEY;
        effect.preserveAmount = 150;  // Сохраняет 150₽ (уменьшено с 200)
        effect.energyCost = 0;
        effect.maxCharges = 4;  // 4 использования до отдыха
        effect.effectDescription = "Сохранить 150₽ в событии / Preserve 150₽ in event";
        effect.targetType = "event";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_money_saver",
                              "Экономия денег / Money Saver",
                              "Позволяет избежать трат денег в событии / Allows preserving money in events",
                              AbilityType::ACTIVE, AbilityCategory::TRADING, req, effect));
    }
    
    {
        // Пассивная способность для продвинутого денежного перка
        AbilityRequirement req(5, {"perk_money_saver"}, 1);
        AbilityEffect effect;
        effect.effectType = "money_keeper";
        effect.bonusMultiplier = 1.03f;  // +3% к сохранённым деньгам
        addAbility(AbilityNode("trading_money_keeper",
                              "Хранитель денег / Money Keeper",
                              "Деньги приносят 3% пассивный доход / Money brings 3% passive income",
                              AbilityType::PASSIVE, AbilityCategory::TRADING, req, effect));
    }
    
    {
        // Продвинутый перк сохранения денег / Advanced money preservation perk
        AbilityRequirement req(6, {"trading_money_keeper"}, 2);
        AbilityEffect effect;
        effect.effectType = "money_preservation_advanced";
        effect.preservationType = ResourcePreservationType::MONEY;
        effect.preserveAmount = 350;  // Сохраняет 350₽ (уменьшено с 500)
        effect.energyCost = 0;
        effect.maxCharges = 2;  // 2 использования до отдыха
        effect.effectDescription = "Сохранить 350₽ в событии / Preserve 350₽ in event";
        effect.targetType = "event";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_money_protector",
                              "Защита денег / Money Protector",
                              "Позволяет сохранить больше денег в событии / Preserves more money in events",
                              AbilityType::ACTIVE, AbilityCategory::TRADING, req, effect));
    }
    
    // ========== КАТЕГОРИЯ: ВЫЖИВАНИЕ (SURVIVAL) ==========
    
    // Пассивные способности выживания (сбалансированы)
    {
        AbilityRequirement req(1, {}, 1);
        AbilityEffect effect;
        effect.effectType = "max_energy";
        effect.bonusFlat = 5;  // +5 максимальной энергии (уменьшено с 10)
        addAbility(AbilityNode("survival_stamina_1",
                              "Выносливость I / Stamina I",
                              "Максимальная энергия +5 / Max energy +5",
                              AbilityType::PASSIVE, AbilityCategory::SURVIVAL, req, effect));
    }
    
    {
        AbilityRequirement req(3, {"survival_stamina_1"}, 1);
        AbilityEffect effect;
        effect.effectType = "max_energy";
        effect.bonusFlat = 8;  // +8 максимальной энергии (уменьшено с 20)
        addAbility(AbilityNode("survival_stamina_2",
                              "Выносливость II / Stamina II",
                              "Максимальная энергия +8 / Max energy +8",
                              AbilityType::PASSIVE, AbilityCategory::SURVIVAL, req, effect));
    }
    
    // Пассивно-активная пара: Экономия энергии
    {
        // Пассивная способность для энергетического перка
        AbilityRequirement req(2, {}, 1);
        AbilityEffect effect;
        effect.effectType = "energy_efficiency";
        effect.bonusMultiplier = 1.03f;  // +3% эффективность использования энергии
        addAbility(AbilityNode("survival_energy_efficiency",
                              "Эффективность / Efficiency",
                              "Действия тратят на 3% меньше энергии / Actions cost 3% less energy",
                              AbilityType::PASSIVE, AbilityCategory::SURVIVAL, req, effect));
    }
    
    {
        // Перк сохранения энергии персонажа / Character energy preservation perk
        AbilityRequirement req(3, {"survival_energy_efficiency"}, 1);
        AbilityEffect effect;
        effect.effectType = "energy_preservation";
        effect.preservationType = ResourcePreservationType::CHARACTER_ENERGY;
        effect.preserveAmount = 10;  // Сохраняет 10 энергии (уменьшено с 15)
        effect.energyCost = 0;  // Не требует энергии для использования
        effect.maxCharges = 5;  // 5 использований до отдыха
        effect.effectDescription = "Сохранить 10 энергии в событии / Preserve 10 energy in event";
        effect.targetType = "event";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_energy_saver",
                              "Экономия энергии / Energy Saver",
                              "Позволяет сохранить энергию персонажа в событии / Allows preserving character energy in events",
                              AbilityType::ACTIVE, AbilityCategory::SURVIVAL, req, effect));
    }
    
    {
        // Пассивная способность для продвинутого энергетического перка
        AbilityRequirement req(4, {"perk_energy_saver"}, 1);
        AbilityEffect effect;
        effect.effectType = "energy_reserve";
        effect.bonusFlat = 10;  // +10 резерв энергии
        addAbility(AbilityNode("survival_energy_reserve",
                              "Энергетический резерв / Energy Reserve",
                              "Дополнительный резерв энергии +10 / Additional energy reserve +10",
                              AbilityType::PASSIVE, AbilityCategory::SURVIVAL, req, effect));
    }
    
    {
        // Продвинутый перк сохранения энергии / Advanced energy preservation perk
        AbilityRequirement req(5, {"survival_energy_reserve"}, 2);
        AbilityEffect effect;
        effect.effectType = "energy_preservation_advanced";
        effect.preservationType = ResourcePreservationType::CHARACTER_ENERGY;
        effect.preserveAmount = 20;  // Сохраняет 20 энергии (уменьшено с 30)
        effect.energyCost = 0;
        effect.maxCharges = 3;  // 3 использования до отдыха
        effect.effectDescription = "Сохранить 20 энергии в событии / Preserve 20 energy in event";
        effect.targetType = "event";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_energy_protector",
                              "Защита энергии / Energy Protector",
                              "Позволяет сохранить больше энергии в событии / Preserves more energy in events",
                              AbilityType::ACTIVE, AbilityCategory::SURVIVAL, req, effect));
    }
    
    // ========== КАТЕГОРИЯ: МЕХАНИКА (MECHANICS) ==========
    
    // Пассивные способности механики (сбалансированы)
    {
        AbilityRequirement req(1, {}, 1);
        AbilityEffect effect;
        effect.effectType = "wear_reduction";
        effect.bonusMultiplier = 0.97f;  // -3% износа (уменьшено с -15%)
        addAbility(AbilityNode("mechanics_prevention_1",
                              "Профилактика I / Preventive Care I",
                              "Снижение износа автомобиля на 3% / Reduce vehicle wear by 3%",
                              AbilityType::PASSIVE, AbilityCategory::MECHANICS, req, effect));
    }
    
    {
        AbilityRequirement req(3, {"mechanics_prevention_1"}, 1);
        AbilityEffect effect;
        effect.effectType = "wear_reduction";
        effect.bonusMultiplier = 0.95f;  // -5% износа (уменьшено с -30%)
        addAbility(AbilityNode("mechanics_prevention_2",
                              "Профилактика II / Preventive Care II",
                              "Дополнительное снижение износа на 5% / Additional 5% wear reduction",
                              AbilityType::PASSIVE, AbilityCategory::MECHANICS, req, effect));
    }
    
    {
        AbilityRequirement req(2, {}, 1);
        AbilityEffect effect;
        effect.effectType = "repair_discount";
        effect.bonusMultiplier = 0.96f;  // -4% стоимость ремонта (уменьшено с -10%)
        addAbility(AbilityNode("mechanics_selflearner_1",
                              "Самоучка I / Self-Learner I",
                              "Скидка 4% на услуги механика / 4% discount on mechanic services",
                              AbilityType::PASSIVE, AbilityCategory::MECHANICS, req, effect));
    }
    
    {
        AbilityRequirement req(4, {"mechanics_selflearner_1"}, 1);
        AbilityEffect effect;
        effect.effectType = "repair_discount";
        effect.bonusMultiplier = 0.93f;  // -7% стоимость ремонта (уменьшено с -20%)
        addAbility(AbilityNode("mechanics_selflearner_2",
                              "Самоучка II / Self-Learner II",
                              "Скидка 7% на услуги механика / 7% discount on mechanic services",
                              AbilityType::PASSIVE, AbilityCategory::MECHANICS, req, effect));
    }
    
    // Активные перки механики
    {
        AbilityRequirement req(2, {}, 2);
        AbilityEffect effect;
        effect.effectType = "emergency_repair";
        effect.energyCost = 0;
        effect.itemRequirements = {"tool_kit"};
        effect.maxCharges = 3;  // 3 использования до отдыха
        effect.effectDescription = "Восстановление 30% состояния автомобиля. Требует инструменты / Restore 30% vehicle condition. Requires tools";
        effect.targetType = "vehicle";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_emergency_repair",
                              "Экстренный ремонт / Emergency Repair",
                              "Быстрый ремонт в дороге / Quick roadside repair",
                              AbilityType::ACTIVE, AbilityCategory::MECHANICS, req, effect));
    }
    
    {
        AbilityRequirement req(5, {"perk_emergency_repair"}, 2);
        AbilityEffect effect;
        effect.effectType = "full_diagnostics";
        effect.energyCost = 5;
        effect.maxCharges = 2;  // 2 использования до отдыха
        effect.effectDescription = "Показывает все скрытые проблемы автомобиля / Shows all hidden vehicle problems";
        effect.targetType = "vehicle";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_diagnostics",
                              "Полная диагностика / Full Diagnostics",
                              "Выявление всех неисправностей / Detect all malfunctions",
                              AbilityType::ACTIVE, AbilityCategory::MECHANICS, req, effect));
    }
    
    {
        AbilityRequirement req(8, {"perk_diagnostics"}, 3);
        AbilityEffect effect;
        effect.effectType = "vehicle_tuning";
        effect.energyCost = 0;
        effect.moneyCost = 50;
        effect.itemRequirements = {"spare_parts"};
        effect.maxCharges = 1;  // 1 использование до отдыха
        effect.effectDescription = "Временно улучшает характеристики автомобиля на 1 день / Temporarily improves vehicle stats for 1 day";
        effect.targetType = "vehicle";
        effect.duration = 1440.0f;
        addAbility(AbilityNode("perk_tuning",
                              "Тюнинг / Tuning",
                              "Временное улучшение автомобиля / Temporary vehicle enhancement",
                              AbilityType::ACTIVE, AbilityCategory::MECHANICS, req, effect));
    }
    
    // ========== КАТЕГОРИЯ: СОЦИАЛЬНЫЕ НАВЫКИ (SOCIAL) ==========
    
    // Пассивные социальные способности
    {
        AbilityRequirement req(1, {}, 1);
        AbilityEffect effect;
        effect.effectType = "charisma_bonus";
        effect.bonusMultiplier = 1.05f;  // +5% успех социальных проверок
        addAbility(AbilityNode("social_charisma_1",
                              "Харизма I / Charisma I",
                              "+5% к успеху социальных проверок / +5% social check success",
                              AbilityType::PASSIVE, AbilityCategory::SOCIAL, req, effect));
    }
    
    {
        AbilityRequirement req(3, {"social_charisma_1"}, 1);
        AbilityEffect effect;
        effect.effectType = "charisma_bonus";
        effect.bonusMultiplier = 1.10f;  // +10% успех
        addAbility(AbilityNode("social_charisma_2",
                              "Харизма II / Charisma II",
                              "+10% к успеху социальных проверок / +10% social check success",
                              AbilityType::PASSIVE, AbilityCategory::SOCIAL, req, effect));
    }
    
    {
        AbilityRequirement req(2, {}, 1);
        AbilityEffect effect;
        effect.effectType = "morale_regen";
        effect.bonusMultiplier = 1.05f;  // +5% скорость восстановления морали
        addAbility(AbilityNode("social_leader_1",
                              "Лидер I / Leader I",
                              "Мораль команды повышается на 5% быстрее / Team morale increases 5% faster",
                              AbilityType::PASSIVE, AbilityCategory::SOCIAL, req, effect));
    }
    
    {
        AbilityRequirement req(4, {"social_leader_1"}, 1);
        AbilityEffect effect;
        effect.effectType = "morale_regen";
        effect.bonusMultiplier = 1.10f;  // +10% скорость
        addAbility(AbilityNode("social_leader_2",
                              "Лидер II / Leader II",
                              "Мораль команды повышается на 10% быстрее / Team morale increases 10% faster",
                              AbilityType::PASSIVE, AbilityCategory::SOCIAL, req, effect));
    }
    
    // Активные социальные перки
    {
        AbilityRequirement req(1, {}, 1);
        AbilityEffect effect;
        effect.effectType = "boost_morale";
        effect.energyCost = 10;
        effect.maxCharges = 4;  // 4 использования до отдыха
        effect.effectDescription = "+15 морали всей команды / +15 morale to entire team";
        effect.targetType = "team";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_encourage",
                              "Подбодрить / Encourage",
                              "Подъём морали команды / Boost team morale",
                              AbilityType::ACTIVE, AbilityCategory::SOCIAL, req, effect));
    }
    
    {
        AbilityRequirement req(4, {"perk_encourage"}, 2);
        AbilityEffect effect;
        effect.effectType = "motivational_speech";
        effect.energyCost = 20;
        effect.maxCharges = 2;  // 2 использования до отдыха
        effect.effectDescription = "+30 морали + временный бафф команде (+10% эффективности на 1 час) / +30 morale + temp team buff (+10% efficiency for 1 hour)";
        effect.targetType = "team";
        effect.duration = 60.0f;
        addAbility(AbilityNode("perk_motivate",
                              "Мотивационная речь / Motivational Speech",
                              "Мощный подъём морали с баффом / Powerful morale boost with buff",
                              AbilityType::ACTIVE, AbilityCategory::SOCIAL, req, effect));
    }
    
    {
        AbilityRequirement req(6, {"perk_motivate"}, 2);
        AbilityEffect effect;
        effect.effectType = "resolve_conflict";
        effect.energyCost = 15;
        effect.maxCharges = 1;  // 1 использование до отдыха
        effect.effectDescription = "Решение конфликта между NPC в команде / Resolve conflict between team NPCs";
        effect.targetType = "team";
        effect.duration = 0.0f;
        addAbility(AbilityNode("perk_resolve_conflict",
                              "Улаживание конфликта / Resolve Conflict",
                              "Прекращение конфликта в команде / Stop team conflict",
                              AbilityType::ACTIVE, AbilityCategory::SOCIAL, req, effect));
    }
}
