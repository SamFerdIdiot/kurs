#include "NPC.h"
#include <algorithm>
#include <cmath>





NPC::NPC(const std::string& id, const std::string& name,
         const std::string& bio, int initialLevel)
    : m_id(id),
      m_name(name),
      m_bio(bio),
      m_level(std::max(1, std::min(3, initialLevel))),
      m_experience(0),
      m_relationship(0),
      m_metBefore(false),
      m_inParty(false),
      m_currentLocation("") {
}

int NPC::getExperienceForNextLevel() const {



    switch (m_level) {
        case 1:
            return 100;
        case 2:
            return 250;
        case 3:
        default:
            return 0;
    }
}

void NPC::addExperience(int exp) {
    if (m_level >= 3) {
        return;
    }

    m_experience += exp;


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


    if (m_level > 3) {
        m_level = 3;
        m_experience = 0;
    }
}


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


void NPC::setRelationship(int value) {

    m_relationship = std::max(-100, std::min(100, value));
}

void NPC::modifyRelationship(int delta) {
    setRelationship(m_relationship + delta);
}





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


    for (const auto& existing : m_team) {
        if (existing->getId() == npc->getId()) {
            return false;
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





namespace NPCFactory {
    std::unique_ptr<NPC> createMechanic(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Майк / Mike",
            "Опытный механик, знающий все о старых советских машинах. "
            "Experienced mechanic who knows everything about old Soviet cars.",
            1
        );


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





    std::unique_ptr<NPC> createRussianMechanic(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Механик Михалыч",
            "Опытный механик из деревни, всю жизнь проработал в автосервисе. "
            "Замасленная спецовка, седые виски, вечная сигарета в зубах.",
            1
        );

        PassiveAbility passive("mechanic_repair", "Ремонт машины",
                              "Каждый день чинит машину (+5% состояния если <90%)",
                              PassiveAbilityAttachment::CAR);
        npc->setPassiveAbility(passive);

        ActiveAbility active("mechanic_field_repair", "Полевой ремонт",
                            "Восстановить 20% состояния машины", 72.0f);
        npc->setActiveAbility(active);

        return npc;
    }

    std::unique_ptr<NPC> createUnemployed(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Безработный Виталий",
            "Бывший бухгалтер, уволенный при сокращении штата. "
            "Потрёпанный костюм, портфель с документами о безработице.",
            1
        );

        PassiveAbility passive("unemployed_benefit", "Пособие по безработице",
                              "Получает +100 рублей раз в неделю",
                              PassiveAbilityAttachment::MAIN_HERO);
        npc->setPassiveAbility(passive);

        ActiveAbility active("unemployed_discount", "Знание цен",
                            "Скидка -20% в магазинах", 168.0f);
        npc->setActiveAbility(active);

        return npc;
    }

    std::unique_ptr<NPC> createPunk(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Панк Вася",
            "Молодой панк-рокер, путешествует с гитарой по стране. "
            "Ирокез, кожаная куртка, множество значков.",
            1
        );

        PassiveAbility passive("punk_guitar", "Гитара",
                              "Имеет гитару (не продаётся), +10 к отношениям с молодыми NPC",
                              PassiveAbilityAttachment::NPC);
        npc->setPassiveAbility(passive);

        ActiveAbility active("punk_concert", "Концерт",
                            "Устроить концерт на заправке (+200₽, риск штрафа)", 120.0f);
        npc->setActiveAbility(active);

        return npc;
    }

    std::unique_ptr<NPC> createGranny(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Бабушка Галина",
            "Добрая старушка, везёт гостинцы внукам в другой город. "
            "Платочек, большая сумка с пирожками.",
            1
        );

        PassiveAbility passive("granny_pies", "Пирожки",
                              "Восстанавливает 10 энергии каждый день",
                              PassiveAbilityAttachment::MAIN_HERO);
        npc->setPassiveAbility(passive);

        ActiveAbility active("granny_advice", "Материнский совет",
                            "Даёт подсказку о событии заранее (1 раз за игру)", 0.0f);
        npc->setActiveAbility(active);

        return npc;
    }

    std::unique_ptr<NPC> createTrucker(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Дальнобойщик Петрович",
            "Опытный водитель-дальнобойщик, знает все дороги России. "
            "Кепка, термос с чаем, радиоприёмник.",
            1
        );

        PassiveAbility passive("trucker_fuel_economy", "Знание дорог",
                              "Экономия топлива -15%",
                              PassiveAbilityAttachment::CAR);
        npc->setPassiveAbility(passive);

        ActiveAbility active("trucker_radio", "Связь с дальнобойщиками",
                            "Найти дешёвую заправку или получить инфо о дороге", 72.0f);
        npc->setActiveAbility(active);

        return npc;
    }

    std::unique_ptr<NPC> createStudent(const std::string& id) {
        auto npc = std::make_unique<NPC>(
            id,
            "Студент Лёха",
            "Студент, путешествует автостопом на каникулах. "
            "Рюкзак, джинсы, кроссовки.",
            1
        );

        PassiveAbility passive("student_youth", "Молодость",
                              "+10 к максимальной энергии",
                              PassiveAbilityAttachment::MAIN_HERO);
        npc->setPassiveAbility(passive);

        ActiveAbility active("student_help_driving", "Помощь в пути",
                            "Может водить машину (восстановление энергии +20)", 48.0f);
        npc->setActiveAbility(active);

        return npc;
    }
}
