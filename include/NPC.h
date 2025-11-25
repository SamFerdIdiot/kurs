#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>
#include <memory>
#include <functional>

// Типы привязки пассивных способностей
enum class PassiveAbilityAttachment {
    MAIN_HERO,      // Привязано к главному герою
    NPC,            // Привязано к NPC
    CAR             // Привязано к машине
};

// Пассивная способность (заглушка)
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

// Активная способность (заглушка)
struct ActiveAbility {
    std::string id;
    std::string name;
    std::string description;
    float cooldown;  // Время восстановления
    
    ActiveAbility() : cooldown(0.0f) {}
    ActiveAbility(const std::string& abilityId, const std::string& abilityName,
                  const std::string& desc, float cd = 0.0f)
        : id(abilityId), name(abilityName), description(desc), cooldown(cd) {}
};

// Класс NPC - неигровой персонаж/попутчик
// Каждый NPC имеет уникальные характеристики:
// - Имя и биография
// - Система опыта и уровней (1-3)
// - Одна активная и одна пассивная способность
// - Возможность прикрепления квестов
class NPC {
public:
    // Конструктор
    NPC(const std::string& id, const std::string& name, 
        const std::string& bio, int initialLevel = 1);
    
    // Базовая информация
    std::string getId() const { return m_id; }
    std::string getName() const { return m_name; }
    std::string getBio() const { return m_bio; }
    
    // Система опыта и уровней
    int getLevel() const { return m_level; }
    int getExperience() const { return m_experience; }
    int getExperienceForNextLevel() const;
    void addExperience(int exp);
    bool canLevelUp() const;
    void levelUp();
    
    // Способности
    void setPassiveAbility(const PassiveAbility& ability) { m_passiveAbility = ability; }
    void setActiveAbility(const ActiveAbility& ability) { m_activeAbility = ability; }
    const PassiveAbility& getPassiveAbility() const { return m_passiveAbility; }
    const ActiveAbility& getActiveAbility() const { return m_activeAbility; }
    bool hasPassiveAbility() const { return !m_passiveAbility.id.empty(); }
    bool hasActiveAbility() const { return !m_activeAbility.id.empty(); }
    
    // Квесты (заглушка)
    void attachQuest(const std::string& questId);
    void detachQuest(const std::string& questId);
    bool hasQuest(const std::string& questId) const;
    const std::vector<std::string>& getAttachedQuests() const { return m_attachedQuests; }
    
private:
    std::string m_id;           // Уникальный идентификатор
    std::string m_name;         // Имя NPC
    std::string m_bio;          // Биография/описание
    
    // Система опыта и уровней
    int m_level;                // Уровень (1-3)
    int m_experience;           // Текущий опыт
    
    // Способности
    PassiveAbility m_passiveAbility;  // Пассивная способность
    ActiveAbility m_activeAbility;    // Активная способность
    
    // Квесты
    std::vector<std::string> m_attachedQuests;  // Привязанные квесты
};

// Менеджер NPC - управление командой попутчиков
// Синглтон для глобального доступа
class NPCManager {
public:
    // Получить экземпляр синглтона
    static NPCManager& getInstance();
    
    // Управление командой
    bool addNPC(std::unique_ptr<NPC> npc);
    void removeNPC(const std::string& npcId);
    NPC* getNPC(const std::string& npcId);
    const std::vector<std::unique_ptr<NPC>>& getTeam() const { return m_team; }
    
    // Проверки
    bool hasSpace() const;
    int getTeamSize() const { return static_cast<int>(m_team.size()); }
    int getMaxTeamSize() const { return m_maxTeamSize; }
    void setMaxTeamSize(int size) { m_maxTeamSize = size; }
    
    // Добавление опыта всей команде
    void addExperienceToTeam(int exp);
    
    // Очистка команды
    void clear();
    
private:
    NPCManager();
    ~NPCManager() = default;
    
    // Запретить копирование
    NPCManager(const NPCManager&) = delete;
    NPCManager& operator=(const NPCManager&) = delete;
    
    std::vector<std::unique_ptr<NPC>> m_team;
    int m_maxTeamSize;
    
    static NPCManager* s_instance;
};

// Фабрика для создания предопределённых NPC
namespace NPCFactory {
    std::unique_ptr<NPC> createMechanic(const std::string& id = "mechanic_mike");
    std::unique_ptr<NPC> createTrader(const std::string& id = "trader_sarah");
    std::unique_ptr<NPC> createCompanion(const std::string& id = "companion_alex");
    std::unique_ptr<NPC> createMedic(const std::string& id = "medic_emma");
    std::unique_ptr<NPC> createNavigator(const std::string& id = "navigator_jack");
}

#endif // NPC_H
