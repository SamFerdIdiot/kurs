#ifndef EXPERIENCE_SYSTEM_H
#define EXPERIENCE_SYSTEM_H

#include <string>
#include <vector>
#include <memory>

// Дерево способностей (заглушка)
struct AbilityTreeNode {
    std::string id;
    std::string name;
    std::string description;
    int requiredLevel;
    bool unlocked;
    
    AbilityTreeNode()
        : requiredLevel(1), unlocked(false) {}
        
    AbilityTreeNode(const std::string& nodeId, const std::string& nodeName,
                    const std::string& desc, int level)
        : id(nodeId), name(nodeName), description(desc),
          requiredLevel(level), unlocked(false) {}
};

// Система опыта главного героя
// Синглтон для глобального доступа
class ExperienceSystem {
public:
    // Получить экземпляр синглтона
    static ExperienceSystem& getInstance();
    
    // Опыт и уровни
    int getLevel() const { return m_level; }
    int getExperience() const { return m_experience; }
    int getExperienceForNextLevel() const;
    void addExperience(int exp);
    bool canLevelUp() const;
    void levelUp();
    
    // Дерево способностей (заглушка)
    void unlockAbility(const std::string& abilityId);
    bool isAbilityUnlocked(const std::string& abilityId) const;
    const std::vector<AbilityTreeNode>& getAbilityTree() const { return m_abilityTree; }
    
    // Сброс системы
    void reset();
    
private:
    ExperienceSystem();
    ~ExperienceSystem() = default;
    
    // Запретить копирование
    ExperienceSystem(const ExperienceSystem&) = delete;
    ExperienceSystem& operator=(const ExperienceSystem&) = delete;
    
    void initializeAbilityTree();
    
    int m_level;
    int m_experience;
    std::vector<AbilityTreeNode> m_abilityTree;
    
    static ExperienceSystem* s_instance;
};

#endif // EXPERIENCE_SYSTEM_H
