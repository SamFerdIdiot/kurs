#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>
#include <memory>
#include <functional>


enum class PassiveAbilityAttachment {
    MAIN_HERO,
    NPC,
    CAR
};


struct PassiveAbility {
    std::string id;
    std::string name;
    std::string description;
    PassiveAbilityAttachment attachment;

    PassiveAbility() : attachment(PassiveAbilityAttachment::NPC) {}
    PassiveAbility(const std::string& abilityId, const std::string& abilityName,
                   const std::string& desc, PassiveAbilityAttachment attach)
        : id(abilityId), name(abilityName), description(desc), attachment(attach) {}
};


struct ActiveAbility {
    std::string id;
    std::string name;
    std::string description;
    float cooldown;

    ActiveAbility() : cooldown(0.0f) {}
    ActiveAbility(const std::string& abilityId, const std::string& abilityName,
                  const std::string& desc, float cd = 0.0f)
        : id(abilityId), name(abilityName), description(desc), cooldown(cd) {}
};







class NPC {
public:

    NPC(const std::string& id, const std::string& name,
        const std::string& bio, int initialLevel = 1);


    std::string getId() const { return m_id; }
    std::string getName() const { return m_name; }
    std::string getBio() const { return m_bio; }


    int getLevel() const { return m_level; }
    int getExperience() const { return m_experience; }
    int getExperienceForNextLevel() const;
    void addExperience(int exp);
    bool canLevelUp() const;
    void levelUp();


    void setPassiveAbility(const PassiveAbility& ability) { m_passiveAbility = ability; }
    void setActiveAbility(const ActiveAbility& ability) { m_activeAbility = ability; }
    const PassiveAbility& getPassiveAbility() const { return m_passiveAbility; }
    const ActiveAbility& getActiveAbility() const { return m_activeAbility; }
    bool hasPassiveAbility() const { return !m_passiveAbility.id.empty(); }
    bool hasActiveAbility() const { return !m_activeAbility.id.empty(); }


    void attachQuest(const std::string& questId);
    void detachQuest(const std::string& questId);
    bool hasQuest(const std::string& questId) const;
    const std::vector<std::string>& getAttachedQuests() const { return m_attachedQuests; }


    int getRelationship() const { return m_relationship; }
    void setRelationship(int value);
    void modifyRelationship(int delta);


    bool hasMetBefore() const { return m_metBefore; }
    void setMetBefore(bool met) { m_metBefore = met; }

    bool isInParty() const { return m_inParty; }
    void setInParty(bool inParty) { m_inParty = inParty; }

    std::string getCurrentLocation() const { return m_currentLocation; }
    void setCurrentLocation(const std::string& loc) { m_currentLocation = loc; }

private:
    std::string m_id;
    std::string m_name;
    std::string m_bio;


    int m_level;
    int m_experience;


    PassiveAbility m_passiveAbility;
    ActiveAbility m_activeAbility;


    std::vector<std::string> m_attachedQuests;


    int m_relationship;
    bool m_metBefore;
    bool m_inParty;
    std::string m_currentLocation;
};



class NPCManager {
public:

    static NPCManager& getInstance();


    bool addNPC(std::unique_ptr<NPC> npc);
    void removeNPC(const std::string& npcId);
    NPC* getNPC(const std::string& npcId);
    const std::vector<std::unique_ptr<NPC>>& getTeam() const { return m_team; }


    bool hasSpace() const;
    int getTeamSize() const { return static_cast<int>(m_team.size()); }
    int getMaxTeamSize() const { return m_maxTeamSize; }
    void setMaxTeamSize(int size) { m_maxTeamSize = size; }


    void addExperienceToTeam(int exp);


    void clear();

private:
    NPCManager();
    ~NPCManager() = default;


    NPCManager(const NPCManager&) = delete;
    NPCManager& operator=(const NPCManager&) = delete;

    std::vector<std::unique_ptr<NPC>> m_team;
    int m_maxTeamSize;

    static NPCManager* s_instance;
};


namespace NPCFactory {

    std::unique_ptr<NPC> createMechanic(const std::string& id = "mechanic_mike");
    std::unique_ptr<NPC> createTrader(const std::string& id = "trader_sarah");
    std::unique_ptr<NPC> createCompanion(const std::string& id = "companion_alex");
    std::unique_ptr<NPC> createMedic(const std::string& id = "medic_emma");
    std::unique_ptr<NPC> createNavigator(const std::string& id = "navigator_jack");


    std::unique_ptr<NPC> createRussianMechanic(const std::string& id = "mechanic_mikhalych");
    std::unique_ptr<NPC> createUnemployed(const std::string& id = "unemployed_vitaliy");
    std::unique_ptr<NPC> createPunk(const std::string& id = "punk_vasya");
    std::unique_ptr<NPC> createGranny(const std::string& id = "granny_galina");
    std::unique_ptr<NPC> createTrucker(const std::string& id = "trucker_petrovich");
    std::unique_ptr<NPC> createStudent(const std::string& id = "student_lyokha");
}

#endif
