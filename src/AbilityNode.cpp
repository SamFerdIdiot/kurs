#include "AbilityNode.h"
#include <algorithm>

AbilityNode::AbilityNode()
    : m_type(AbilityType::PASSIVE),
      m_category(AbilityCategory::DRIVING),
      m_unlocked(false) {
}

AbilityNode::AbilityNode(const std::string& id, const std::string& name,
                         const std::string& description, AbilityType type,
                         AbilityCategory category, const AbilityRequirement& req,
                         const AbilityEffect& effect)
    : m_id(id), m_name(name), m_description(description),
      m_type(type), m_category(category), m_unlocked(false),
      m_requirement(req), m_effect(effect) {
}

bool AbilityNode::canUnlock(int playerLevel, int availableSkillPoints,
                            const std::vector<std::string>& unlockedAbilities) const {
    // Шаг 1: Проверка, не разблокирована ли уже
    if (m_unlocked) {
        return false;
    }
    
    // Шаг 2: Проверка уровня
    if (playerLevel < m_requirement.requiredLevel) {
        return false;
    }
    
    // Шаг 3: Проверка очков способностей
    if (availableSkillPoints < m_requirement.skillPointCost) {
        return false;
    }
    
    // Шаг 4: Проверка предварительных способностей
    for (const std::string& prereqId : m_requirement.prerequisiteIds) {
        bool found = false;
        for (const std::string& unlockedId : unlockedAbilities) {
            if (unlockedId == prereqId) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;  // Не все предварительные способности разблокированы
        }
    }
    
    // Все проверки пройдены
    return true;
}
