#ifndef ABILITY_NODE_H
#define ABILITY_NODE_H

#include "AbilityTypes.h"
#include <string>
#include <vector>

// Требования для разблокировки способности
struct AbilityRequirement {
    int requiredLevel;                          // Минимальный уровень персонажа (1-20)
    std::vector<std::string> prerequisiteIds;   // ID предварительных способностей
    int skillPointCost;                         // Стоимость в очках способностей (1-3)
    
    AbilityRequirement()
        : requiredLevel(1), skillPointCost(1) {}
        
    AbilityRequirement(int level, const std::vector<std::string>& prereqs, int cost)
        : requiredLevel(level), prerequisiteIds(prereqs), skillPointCost(cost) {}
};

// Эффект способности
struct AbilityEffect {
    // === Для пассивных способностей ===
    std::string effectType;         // Тип эффекта: "fuel_efficiency", "trader_discount", etc.
    float bonusMultiplier;          // Множитель (1.1 = +10%, 0.9 = -10%)
    int bonusFlat;                  // Плоский бонус (+20 энергии, +50 здоровья)
    
    // === Для активных перков ===
    // Тип сохранения ресурса в событиях
    ResourcePreservationType preservationType;  // Какой ресурс сохраняет перк
    
    // Стоимость использования
    int energyCost;                 // Стоимость энергии (0-50)
    int moneyCost;                  // Стоимость денег в рублях (0-500)
    std::vector<std::string> itemRequirements;  // Требуемые предметы
    
    // Заряды использования
    int maxCharges;                 // Максимальное количество использований (1-10)
    std::string effectDescription;  // Подробное описание эффекта
    
    // Эффект перка при использовании
    std::string targetType;         // "self", "team", "vehicle", "event"
    float duration;                 // Длительность эффекта в минутах (0 = мгновенный)
    
    // Значения сохранения ресурсов (для событий)
    int preserveAmount;             // Количество сохраняемого ресурса
    
    AbilityEffect()
        : bonusMultiplier(1.0f), bonusFlat(0),
          preservationType(ResourcePreservationType::NONE),
          energyCost(0), moneyCost(0), maxCharges(1),
          duration(0.0f), preserveAmount(0) {}
};

// Узел дерева способностей
class AbilityNode {
public:
    // Конструкторы
    AbilityNode();
    AbilityNode(const std::string& id, const std::string& name,
                const std::string& description, AbilityType type,
                AbilityCategory category, const AbilityRequirement& req,
                const AbilityEffect& effect);
    
    // Геттеры
    std::string getId() const { return m_id; }
    std::string getName() const { return m_name; }
    std::string getDescription() const { return m_description; }
    AbilityType getType() const { return m_type; }
    AbilityCategory getCategory() const { return m_category; }
    
    bool isUnlocked() const { return m_unlocked; }
    void setUnlocked(bool unlocked) { m_unlocked = unlocked; }
    
    const AbilityRequirement& getRequirement() const { return m_requirement; }
    const AbilityEffect& getEffect() const { return m_effect; }
    
    // Методы проверки
    bool canUnlock(int playerLevel, int availableSkillPoints,
                   const std::vector<std::string>& unlockedAbilities) const;
    
    // UI метаданные
    std::string getIconPath() const { return m_iconPath; }
    void setIconPath(const std::string& path) { m_iconPath = path; }
    
private:
    std::string m_id;
    std::string m_name;
    std::string m_description;
    AbilityType m_type;
    AbilityCategory m_category;
    bool m_unlocked;
    
    AbilityRequirement m_requirement;
    AbilityEffect m_effect;
    
    std::string m_iconPath;  // Путь к иконке для UI
};

#endif // ABILITY_NODE_H
