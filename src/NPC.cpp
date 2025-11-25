#include "NPC.h"
#include <algorithm>
#include <cmath>

// ============================================================================
// NPC Class Implementation
// ============================================================================

NPC::NPC(const std::string& id, const std::string& name, 
         const std::string& bio, int initialLevel)
    : m_id(id),
      m_name(name),
      m_bio(bio),
      m_level(std::max(1, std::min(3, initialLevel))),
      m_experience(0) {
}

int NPC::getExperienceForNextLevel() const {
    // Опыт для следующего уровня
    // Уровень 1->2: 100 опыта
    // Уровень 2->3: 250 опыта
    switch (m_level) {
        case 1:
            return 100;
        case 2:
            return 250;
        case 3:
        default:
            return 0; // Максимальный уровень
    }
}

void NPC::addExperience(int exp) {
    if (m_level >= 3) {
        return; // Максимальный уровень достигнут
    }
    
    m_experience += exp;
    
    // Автоматическое повышение уровня
    while (canLevelUp()) {
        levelUp();
    }
}

bool NPC::canLevelUp() const {
    if (m_level >= 3) {
        return false;
    }
    return m_experience >= getExperienceForNextLevel();
}

void NPC::levelUp() {
    if (!canLevelUp()) {
        return;
    }
    
    m_experience -= getExperienceForNextLevel();
    m_level++;
    
    // Не допускаем уровень выше 3
    if (m_level > 3) {
        m_level = 3;
        m_experience = 0;
    }
}

// Квесты (заглушка)
void NPC::attachQuest(const std::string& questId) {
    if (!hasQuest(questId)) {
        m_attachedQuests.push_back(questId);
    }
}

void NPC::detachQuest(const std::string& questId) {
    m_attachedQuests.erase(
        std::remove(m_attachedQuests.begin(), m_attachedQuests.end(), questId),
        m_attachedQuests.end()
    );
}

bool NPC::hasQuest(const std::string& questId) const {
    return std::find(m_attachedQuests.begin(), m_attachedQuests.end(), questId) 
           != m_attachedQuests.end();
}

// ============================================================================
// NPCManager Class Implementation
// ============================================================================

NPCManager* NPCManager::s_instance = nullptr;

NPCManager::NPCManager()
    : m_maxTeamSize(3) {
}

NPCManager& NPCManager::getInstance() {
    if (!s_instance) {
        s_instance = new NPCManager();
    }
    return *s_instance;
}

bool NPCManager::addNPC(std::unique_ptr<NPC> npc) {
    if (!npc) {
        return false;
    }
    
    if (!hasSpace()) {
        return false;
    }
    
    // Check if NPC with same ID already exists
    for (const auto& existing : m_team) {
        if (existing->getId() == npc->getId()) {
            return false; // Duplicate ID
        }
    }
    
    m_team.push_back(std::move(npc));
    return true;
}

void NPCManager::removeNPC(const std::string& npcId) {
    m_team.erase(
        std::remove_if(m_team.begin(), m_team.end(),
            [&npcId](const std::unique_ptr<NPC>& npc) {
                return npc->getId() == npcId;
            }),
        m_team.end()
    );
}

NPC* NPCManager::getNPC(const std::string& npcId) {
    for (auto& npc : m_team) {
        if (npc->getId() == npcId) {
            return npc.get();
        }
    }
    return nullptr;
}

bool NPCManager::hasSpace() const {
    return static_cast<int>(m_team.size()) < m_maxTeamSize;
}

void NPCManager::addExperienceToTeam(int exp) {
    for (auto& npc : m_team) {
        npc->addExperience(exp);
    }
}

void NPCManager::clear() {
    m_team.clear();
}

// ============================================================================
// NPCFactory Implementation
// ============================================================================

namespace NPCFactory {
    std::unique_ptr<NPC> createMechanic(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Майк / Mike",
            "Опытный механик, знающий все о старых советских машинах. "
            "Experienced mechanic who knows everything about old Soviet cars.",
            1
        );
        
        // Установка способностей (заглушка)
        PassiveAbility passive("mechanic_passive", "Repair Expert",
                              "Reduces repair cost",
                              PassiveAbilityAttachment::CAR);
        npc->setPassiveAbility(passive);
        
        ActiveAbility active("mechanic_active", "Emergency Repair",
                            "Quick repair on the road", 60.0f);
        npc->setActiveAbility(active);
        
        return npc;
    }
    
    std::unique_ptr<NPC> createTrader(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Сара / Sarah",
            "Бывший торговец, знает все о ценах и скидках. "
            "Former trader, knows all about prices and discounts.",
            1
        );
        
        PassiveAbility passive("trader_passive", "Торговец / Trader",
                              "Скидки в магазинах / Shop discounts",
                              PassiveAbilityAttachment::MAIN_HERO);
        npc->setPassiveAbility(passive);
        
        ActiveAbility active("trader_active", "Торг / Bargain",
                            "Дополнительная скидка / Extra discount", 120.0f);
        npc->setActiveAbility(active);
        
        return npc;
    }
    
    std::unique_ptr<NPC> createCompanion(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Алекс / Alex",
            "Душа компании, поднимает настроение всей команде. "
            "Life of the party, lifts the spirits of the whole team.",
            1
        );
        
        PassiveAbility passive("companion_passive", "Friendliness",
                              "Dialogue bonuses",
                              PassiveAbilityAttachment::NPC);
        npc->setPassiveAbility(passive);
        
        ActiveAbility active("companion_active", "Inspiration",
                            "Temporary team bonus", 180.0f);
        npc->setActiveAbility(active);
        
        return npc;
    }
    
    std::unique_ptr<NPC> createMedic(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Эмма / Emma",
            "Бывший фельдшер, умеет оказывать первую помощь. "
            "Former paramedic, knows how to provide first aid.",
            1
        );
        
        PassiveAbility passive("medic_passive", "Лечение / Healing",
                              "Эффективное лечение / Effective healing",
                              PassiveAbilityAttachment::MAIN_HERO);
        npc->setPassiveAbility(passive);
        
        ActiveAbility active("medic_active", "Первая помощь / First Aid",
                            "Быстрое лечение / Quick heal", 90.0f);
        npc->setActiveAbility(active);
        
        return npc;
    }
    
    std::unique_ptr<NPC> createNavigator(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Джек / Jack",
            "Опытный путешественник, знает все дороги как свои пять пальцев. "
            "Experienced traveler, knows all the roads like the back of his hand.",
            1
        );
        
        PassiveAbility passive("navigator_passive", "Навигация / Navigation",
                              "Экономия топлива / Fuel economy",
                              PassiveAbilityAttachment::CAR);
        npc->setPassiveAbility(passive);
        
        ActiveAbility active("navigator_active", "Короткий путь / Shortcut",
                            "Находит короткий путь / Finds shortcut", 240.0f);
        npc->setActiveAbility(active);
        
        return npc;
    }
}
