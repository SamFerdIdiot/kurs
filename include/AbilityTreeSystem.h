#ifndef ABILITY_TREE_SYSTEM_H
#define ABILITY_TREE_SYSTEM_H

#include "AbilityNode.h"
#include <vector>
#include <memory>
#include <string>

// Заряды активного перка
struct ActivePerkCharges {
    std::string perkId;             // ID активного перка
    int currentCharges;             // Текущее количество доступных зарядов
    int maxCharges;                 // Максимальное количество зарядов
    
    ActivePerkCharges()
        : currentCharges(0), maxCharges(1) {}
        
    ActivePerkCharges(const std::string& id, int current, int max)
        : perkId(id), currentCharges(current), maxCharges(max) {}
};

// Система дерева способностей (Singleton)
class AbilityTreeSystem {
public:
    // Singleton
    static AbilityTreeSystem& getInstance();
    
    // === Инициализация ===
    void initialize();
    void reset();
    
    // === Управление способностями ===
    bool unlockAbility(const std::string& abilityId);
    bool isAbilityUnlocked(const std::string& abilityId) const;
    
    // === Получение информации ===
    const std::vector<AbilityNode>& getAllAbilities() const { return m_abilities; }
    std::vector<AbilityNode> getAbilitiesByCategory(AbilityCategory category) const;
    const AbilityNode* getAbility(const std::string& abilityId) const;
    AbilityNode* getAbilityMutable(const std::string& abilityId);
    
    // Получить активные перки по типу сохранения ресурса
    // Get active perks by resource preservation type
    std::vector<const AbilityNode*> getActivePerksForEvent(ResourcePreservationType preservationType) const;
    
    // === Очки способностей ===
    int getAvailableSkillPoints() const { return m_availableSkillPoints; }
    void addSkillPoints(int points);
    bool spendSkillPoints(int points);
    
    // === Активные перки (система зарядов) ===
    bool canUseActivePerk(const std::string& perkId) const;
    bool useActivePerk(const std::string& perkId);
    void updateCooldowns(float deltaTimeMinutes);  // Обновление кулдаунов (оставлено для совместимости)
    void restoreCharges();  // Восстанавливает все заряды через отдых
    int getCurrentCharges(const std::string& perkId) const;
    int getMaxCharges(const std::string& perkId) const;
    bool hasCharges(const std::string& perkId) const;
    
    // === Пассивные бонусы ===
    float getPassiveBonus(const std::string& effectType) const;
    int getPassiveBonusFlat(const std::string& effectType) const;
    
    // === Сохранение/загрузка ===
    void save() const;
    void load();
    
private:
    // Конструктор private для Singleton
    AbilityTreeSystem();
    ~AbilityTreeSystem() = default;
    
    // Запретить копирование
    AbilityTreeSystem(const AbilityTreeSystem&) = delete;
    AbilityTreeSystem& operator=(const AbilityTreeSystem&) = delete;
    
    // Внутренние методы
    void initializeAbilityTree();
    void addAbility(const AbilityNode& ability);
    void initializePerkCharges(const std::string& perkId, int maxCharges);
    void decrementCharge(const std::string& perkId);
    
    // Данные
    std::vector<AbilityNode> m_abilities;
    std::vector<ActivePerkCharges> m_activePerkCharges;  // Заряды активных перков
    int m_availableSkillPoints;
    
    static AbilityTreeSystem* s_instance;
};

#endif // ABILITY_TREE_SYSTEM_H
