#include "ExperienceSystem.h"
#include <algorithm>
#include <cmath>

ExperienceSystem* ExperienceSystem::s_instance = nullptr;

ExperienceSystem::ExperienceSystem()
    : m_level(1), m_experience(0) {

}

ExperienceSystem& ExperienceSystem::getInstance() {
    if (!s_instance) {
        s_instance = new ExperienceSystem();
    }
    return *s_instance;
}

int ExperienceSystem::getExperienceForNextLevel() const {


    return static_cast<int>(100.0f * std::pow(m_level, 1.5f));
}

void ExperienceSystem::addExperience(int exp) {
    m_experience += exp;


    while (canLevelUp()) {
        levelUp();
    }
}

bool ExperienceSystem::canLevelUp() const {
    return m_experience >= getExperienceForNextLevel();
}

void ExperienceSystem::levelUp() {
    if (!canLevelUp()) {
        return;
    }

    m_experience -= getExperienceForNextLevel();
    m_level++;


}

void ExperienceSystem::unlockAbility(const std::string& abilityId) {
    for (auto& node : m_abilityTree) {
        if (node.id == abilityId && m_level >= node.requiredLevel) {
            node.unlocked = true;
            break;
        }
    }
}

bool ExperienceSystem::isAbilityUnlocked(const std::string& abilityId) const {
    for (const auto& node : m_abilityTree) {
        if (node.id == abilityId) {
            return node.unlocked;
        }
    }
    return false;
}

void ExperienceSystem::reset() {
    m_level = 1;
    m_experience = 0;
    initializeAbilityTree();
}

void ExperienceSystem::initializeAbilityTree() {
    m_abilityTree.clear();


    m_abilityTree.push_back(AbilityTreeNode(
        "ability_driving_1",
        "Driving I",
        "Basic driving skills",
        1
    ));

    m_abilityTree.push_back(AbilityTreeNode(
        "ability_driving_2",
        "Вождение II / Driving II",
        "Улучшенные навыки вождения / Improved driving skills",
        3
    ));

    m_abilityTree.push_back(AbilityTreeNode(
        "ability_trading_1",
        "Торговля I / Trading I",
        "Базовые навыки торговли / Basic trading skills",
        2
    ));

    m_abilityTree.push_back(AbilityTreeNode(
        "ability_survival_1",
        "Выживание I / Survival I",
        "Навыки выживания / Survival skills",
        2
    ));

    m_abilityTree.push_back(AbilityTreeNode(
        "ability_mechanics_1",
        "Механика I / Mechanics I",
        "Базовые навыки механики / Basic mechanics skills",
        4
    ));
}
