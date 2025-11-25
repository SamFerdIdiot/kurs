# Техническая Спецификация Системы Дерева Способностей
# Technical Specification for Ability Tree System

**Версия / Version**: 1.0  
**Дата / Date**: 2025-11-16  
**Статус / Status**: Детальное описание / Detailed Description  
**Предыдущий документ / Previous**: [ABILITY_TREE_SYSTEM.md](./ABILITY_TREE_SYSTEM.md)

---

## 📋 Содержание / Table of Contents

1. [Структуры данных / Data Structures](#структуры-данных--data-structures)
2. [Архитектура классов / Class Architecture](#архитектура-классов--class-architecture)
3. [Алгоритмы / Algorithms](#алгоритмы--algorithms)
4. [Файловая структура / File Structure](#файловая-структура--file-structure)
5. [API спецификация / API Specification](#api-спецификация--api-specification)
6. [Форматы данных / Data Formats](#форматы-данных--data-formats)
7. [Интеграционные точки / Integration Points](#интеграционные-точки--integration-points)
8. [Тестовое покрытие / Test Coverage](#тестовое-покрытие--test-coverage)

---

## Структуры данных / Data Structures

### AbilityTypeEnum

```cpp
// include/AbilityTypes.h
enum class AbilityType {
    PASSIVE,  // Пассивная способность (постоянный бонус)
    ACTIVE    // Активный перк (используется вручную)
};
```

**Описание:**
- `PASSIVE`: Способность активна постоянно после разблокировки
- `ACTIVE`: Перк требует активации игроком, имеет стоимость и перезарядку

### AbilityCategory Enum

```cpp
// include/AbilityTypes.h
enum class AbilityCategory {
    DRIVING,      // 🚗 Вождение: экономия топлива, скорость, избежание аварий
    TRADING,      // 💰 Торговля: скидки, лучшие цены продажи, поиск товаров
    SURVIVAL,     // ⚡ Выживание: энергия, восстановление, стрессоустойчивость
    MECHANICS,    // 🔧 Механика: ремонт, обслуживание, тюнинг
    SOCIAL        // 👥 Социальные: харизма, лидерство, командная работа
};
```

**Использование:**
- Группировка способностей в UI
- Фильтрация при отображении дерева
- Балансировка (игрок не может максимально прокачать все категории)

### AbilityRequirement Structure

```cpp
// include/AbilityNode.h
struct AbilityRequirement {
    int requiredLevel;                          // Минимальный уровень персонажа (1-20)
    std::vector<std::string> prerequisiteIds;   // ID предварительных способностей
    int skillPointCost;                         // Стоимость в очках способностей (1-3)
    
    AbilityRequirement()
        : requiredLevel(1), skillPointCost(1) {}
        
    AbilityRequirement(int level, const std::vector<std::string>& prereqs, int cost)
        : requiredLevel(level), prerequisiteIds(prereqs), skillPointCost(cost) {}
};
```

**Правила:**
- `requiredLevel`: Игрок должен достичь этого уровня
- `prerequisiteIds`: Все указанные способности должны быть разблокированы
- `skillPointCost`: Обычные способности стоят 1 очко, мощные 2-3

**Примеры:**
```cpp
// Базовая способность уровня 1
AbilityRequirement req1(1, {}, 1);

// Продвинутая способность, требует предыдущую
AbilityRequirement req2(3, {"driving_economical_1"}, 1);

// Мощный перк
AbilityRequirement req3(5, {"driving_economical_1", "driving_experienced_1"}, 2);
```

### AbilityEffect Structure

```cpp
// include/AbilityNode.h
struct AbilityEffect {
    // === Для пассивных способностей ===
    std::string effectType;         // Тип эффекта: "fuel_efficiency", "trader_discount", etc.
    float bonusMultiplier;          // Множитель (1.1 = +10%, 0.9 = -10%)
    int bonusFlat;                  // Плоский бонус (+20 энергии, +50 здоровья)
    
    // === Для активных перков ===
    // Стоимость использования
    int energyCost;                 // Стоимость энергии (0-50)
    int moneyCost;                  // Стоимость денег в рублях (0-500)
    std::vector<std::string> itemRequirements;  // Требуемые предметы
    
    // Перезарядка и эффект
    float cooldownMinutes;          // Время перезарядки в минутах (1-360)
    std::string effectDescription;  // Подробное описание эффекта
    
    // Эффект перка при использовании
    std::string targetType;         // "self", "team", "vehicle", "event"
    float duration;                 // Длительность эффекта в минутах (0 = мгновенный)
    
    AbilityEffect()
        : bonusMultiplier(1.0f), bonusFlat(0),
          energyCost(0), moneyCost(0), cooldownMinutes(0.0f),
          duration(0.0f) {}
};
```

**Типы эффектов (effectType):**

**Для вождения:**
- `fuel_efficiency`: Экономия топлива (множитель)
- `crash_avoidance`: Шанс избежать аварии (множитель)
- `travel_speed`: Скорость путешествия (множитель)

**Для торговли:**
- `trader_discount`: Скидка в магазинах (множитель)
- `selling_bonus`: Бонус к продаже (множитель)
- `rare_item_chance`: Шанс найти редкий товар (множитель)

**Для выживания:**
- `max_energy`: Максимальная энергия (плоский бонус)
- `energy_regen`: Скорость восстановления энергии (множитель)
- `morale_decay`: Скорость падения морали (множитель)

**Для механики:**
- `wear_reduction`: Снижение износа автомобиля (множитель)
- `repair_discount`: Скидка на ремонт (множитель)
- `improvise_chance`: Шанс починить подручными средствами (множитель)

**Для социальных:**
- `charisma_bonus`: Успех социальных проверок (множитель)
- `morale_regen`: Скорость восстановления морали (множитель)
- `npc_efficiency`: Эффективность NPC в команде (множитель)

### AbilityNode Class

```cpp
// include/AbilityNode.h
#ifndef ABILITY_NODE_H
#define ABILITY_NODE_H

#include <string>
#include <vector>

enum class AbilityType;
enum class AbilityCategory;
struct AbilityRequirement;
struct AbilityEffect;

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
```

### ActivePerkCooldown Structure

```cpp
// include/AbilityTreeSystem.h
struct ActivePerkCooldown {
    std::string perkId;             // ID активного перка
    float remainingMinutes;         // Оставшееся время перезарядки в минутах
    
    ActivePerkCooldown()
        : remainingMinutes(0.0f) {}
        
    ActivePerkCooldown(const std::string& id, float minutes)
        : perkId(id), remainingMinutes(minutes) {}
};
```

---

## Архитектура классов / Class Architecture

### AbilityTreeSystem Class (Singleton)

```cpp
// include/AbilityTreeSystem.h
#ifndef ABILITY_TREE_SYSTEM_H
#define ABILITY_TREE_SYSTEM_H

#include "AbilityNode.h"
#include <vector>
#include <memory>
#include <string>

struct ActivePerkCooldown;

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
    
    // === Очки способностей ===
    int getAvailableSkillPoints() const { return m_availableSkillPoints; }
    void addSkillPoints(int points);
    bool spendSkillPoints(int points);
    
    // === Активные перки ===
    bool canUseActivePerk(const std::string& perkId) const;
    bool useActivePerk(const std::string& perkId);
    void updateCooldowns(float deltaTimeMinutes);
    float getCooldownRemaining(const std::string& perkId) const;
    bool isOnCooldown(const std::string& perkId) const;
    
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
    void startCooldown(const std::string& perkId, float minutes);
    
    // Данные
    std::vector<AbilityNode> m_abilities;
    std::vector<ActivePerkCooldown> m_activeCooldowns;
    int m_availableSkillPoints;
    
    static AbilityTreeSystem* s_instance;
};

#endif // ABILITY_TREE_SYSTEM_H
```

### AbilityTreeScene Class

```cpp
// include/AbilityTreeScene.h
#ifndef ABILITY_TREE_SCENE_H
#define ABILITY_TREE_SCENE_H

#include "Scene.h"
#include "AbilityNode.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class AbilityTreeSystem;
class ExperienceSystem;

class AbilityTreeScene : public Scene {
public:
    AbilityTreeScene();
    ~AbilityTreeScene() override;
    
    // Scene interface
    void handleInput(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    SceneType getNextScene() const override;
    bool isFinished() const override;
    
private:
    // === Rendering methods ===
    void renderHeader(sf::RenderWindow& window);
    void renderCategoryTabs(sf::RenderWindow& window);
    void renderAbilityTree(sf::RenderWindow& window);
    void renderDetailsPanel(sf::RenderWindow& window);
    void renderFooter(sf::RenderWindow& window);
    
    void renderAbilityNode(sf::RenderWindow& window, const AbilityNode& node,
                          float x, float y, bool isSelected);
    void renderConnectionLine(sf::RenderWindow& window,
                             float x1, float y1, float x2, float y2);
    
    // === Input handling ===
    void handleMouseClick(int mouseX, int mouseY);
    void handleKeyPress(sf::Keyboard::Key key);
    
    // === Logic methods ===
    void selectAbility(const std::string& abilityId);
    void unlockSelectedAbility();
    void changeCategory(AbilityCategory category);
    
    // === Helper methods ===
    bool isAbilityAvailable(const AbilityNode& node) const;
    sf::Color getNodeColor(const AbilityNode& node, bool isSelected) const;
    std::vector<const AbilityNode*> getNodesForCategory(AbilityCategory category) const;
    
    // === State ===
    bool m_isFinished;
    SceneType m_nextScene;
    
    AbilityTreeSystem* m_abilitySystem;
    ExperienceSystem* m_experienceSystem;
    
    std::string m_selectedAbilityId;
    AbilityCategory m_currentCategory;
    
    // === UI Resources ===
    sf::Font m_font;
    sf::RectangleShape m_background;
    sf::RectangleShape m_detailsPanel;
    
    // UI текст
    sf::Text m_titleText;
    sf::Text m_levelText;
    sf::Text m_skillPointsText;
    sf::Text m_detailsText;
    sf::Text m_helpText;
    
    // Константы позиционирования
    static constexpr float HEADER_HEIGHT = 80.0f;
    static constexpr float TABS_HEIGHT = 50.0f;
    static constexpr float DETAILS_WIDTH = 400.0f;
    static constexpr float NODE_SIZE = 60.0f;
    static constexpr float NODE_SPACING = 120.0f;
};

#endif // ABILITY_TREE_SCENE_H
```

---

## Алгоритмы / Algorithms

### Алгоритм 1: Проверка возможности разблокировки

```cpp
// В AbilityNode.cpp
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
```

**Сложность:** O(P × U), где P - количество предварительных способностей, U - количество разблокированных способностей

**Оптимизация:** Использовать `std::unordered_set` для unlockedAbilities → O(P)

### Алгоритм 2: Разблокировка способности

```cpp
// В AbilityTreeSystem.cpp
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
    
    // Шаг 5: Списать очки способностей
    m_availableSkillPoints -= ability->getRequirement().skillPointCost;
    
    // Шаг 6: Сохранить изменения
    save();
    
    return true;
}
```

**Сложность:** O(N), где N - количество способностей в дереве

### Алгоритм 3: Расчёт пассивных бонусов

```cpp
// В AbilityTreeSystem.cpp
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
    
    // Возвращаем итоговый множитель
    // Пример: 1.265 означает +26.5% бонус
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
```

**Сложность:** O(N), где N - количество способностей

**Оптимизация:** Кэшировать результат и пересчитывать только при разблокировке новой способности

### Алгоритм 4: Использование активного перка

```cpp
// В AbilityTreeSystem.cpp
bool AbilityTreeSystem::useActivePerk(const std::string& perkId) {
    // Шаг 1: Найти перк
    const AbilityNode* perk = getAbility(perkId);
    if (!perk || !perk->isUnlocked() || perk->getType() != AbilityType::ACTIVE) {
        return false;
    }
    
    // Шаг 2: Проверить, не на перезарядке ли
    if (isOnCooldown(perkId)) {
        return false;
    }
    
    // Шаг 3: Проверить и списать ресурсы
    const AbilityEffect& effect = perk->getEffect();
    auto& playerState = PlayerState::getInstance();
    
    if (effect.energyCost > 0) {
        if (playerState.getEnergy() < effect.energyCost) {
            return false;  // Недостаточно энергии
        }
        playerState.modifyEnergy(-effect.energyCost);
    }
    
    if (effect.moneyCost > 0) {
        if (playerState.getMoney() < effect.moneyCost) {
            return false;  // Недостаточно денег
        }
        playerState.modifyMoney(-effect.moneyCost);
    }
    
    // Проверка предметов
    for (const std::string& itemId : effect.itemRequirements) {
        if (!InventorySystem::getInstance().hasItem(itemId)) {
            return false;  // Нет нужного предмета
        }
        InventorySystem::getInstance().removeItem(itemId);
    }
    
    // Шаг 4: Применить эффект перка
    // (Это будет зависеть от контекста использования)
    // Например, через событие или прямое изменение состояния
    
    // Шаг 5: Запустить перезарядку
    startCooldown(perkId, effect.cooldownMinutes);
    
    return true;
}
```

### Алгоритм 5: Обновление перезарядок

```cpp
// В AbilityTreeSystem.cpp
void AbilityTreeSystem::updateCooldowns(float deltaTimeMinutes) {
    // Проходим по всем активным перезарядкам
    for (auto it = m_activeCooldowns.begin(); it != m_activeCooldowns.end(); ) {
        // Уменьшаем оставшееся время
        it->remainingMinutes -= deltaTimeMinutes;
        
        // Если перезарядка завершена - удаляем
        if (it->remainingMinutes <= 0.0f) {
            it = m_activeCooldowns.erase(it);
        } else {
            ++it;
        }
    }
}
```

**Сложность:** O(C), где C - количество активных перезарядок (обычно 1-5)

---

## Файловая структура / File Structure

```
kkurs/
├── include/
│   ├── AbilityTypes.h           # Enum'ы (AbilityType, AbilityCategory)
│   ├── AbilityNode.h            # Класс AbilityNode + структуры Requirement/Effect
│   ├── AbilityTreeSystem.h      # Singleton система управления деревом
│   ├── AbilityTreeScene.h       # UI сцена дерева способностей
│   └── ExperienceSystem.h       # [Расширение] Интеграция с AbilityTreeSystem
│
├── src/
│   ├── AbilityNode.cpp
│   ├── AbilityTreeSystem.cpp
│   ├── AbilityTreeScene.cpp
│   └── ExperienceSystem.cpp     # [Обновление] Добавление методов для способностей
│
├── tests/
│   ├── test_AbilityNode.cpp
│   ├── test_AbilityTreeSystem.cpp
│   └── test_AbilityIntegration.cpp
│
└── docs/
    ├── ABILITY_TREE_SYSTEM.md              # Фаза 1: Документация
    ├── ABILITY_TREE_TECHNICAL_SPEC.md      # Фаза 2: Техническое описание (этот файл)
    └── progress/
        └── ability_tree_implementation.md  # Фаза 3: Отчёт о реализации
```

---

## API спецификация / API Specification

### AbilityTreeSystem API

#### Инициализация

```cpp
// Инициализация системы (вызывается при запуске игры)
void initialize();

// Сброс системы (новая игра)
void reset();
```

#### Управление способностями

```cpp
// Разблокировать способность
// Возвращает: true если успешно, false если не выполнены требования
bool unlockAbility(const std::string& abilityId);

// Проверить, разблокирована ли способность
// Возвращает: true если разблокирована
bool isAbilityUnlocked(const std::string& abilityId) const;
```

#### Получение информации

```cpp
// Получить все способности
const std::vector<AbilityNode>& getAllAbilities() const;

// Получить способности определённой категории
std::vector<AbilityNode> getAbilitiesByCategory(AbilityCategory category) const;

// Получить конкретную способность по ID
const AbilityNode* getAbility(const std::string& abilityId) const;
```

#### Очки способностей

```cpp
// Получить количество доступных очков
int getAvailableSkillPoints() const;

// Добавить очки (вызывается при повышении уровня)
void addSkillPoints(int points);

// Потратить очки (вызывается при разблокировке)
bool spendSkillPoints(int points);
```

#### Активные перки

```cpp
// Проверить, можно ли использовать перк сейчас
// (разблокирован, не на перезарядке, достаточно ресурсов)
bool canUseActivePerk(const std::string& perkId) const;

// Использовать активный перк
// Возвращает: true если успешно использован
bool useActivePerk(const std::string& perkId);

// Обновить таймеры перезарядок (вызывается каждый кадр)
void updateCooldowns(float deltaTimeMinutes);

// Получить оставшееся время перезарядки в минутах
// Возвращает: 0.0f если перезарядка завершена
float getCooldownRemaining(const std::string& perkId) const;

// Проверить, на перезарядке ли перк
bool isOnCooldown(const std::string& perkId) const;
```

#### Пассивные бонусы

```cpp
// Получить множитель пассивного бонуса
// effectType: "fuel_efficiency", "trader_discount", и т.д.
// Возвращает: множитель (1.0 = нет бонуса, 1.2 = +20%, 0.8 = -20%)
float getPassiveBonus(const std::string& effectType) const;

// Получить плоский бонус
// Возвращает: суммарный бонус (например, +50 максимальной энергии)
int getPassiveBonusFlat(const std::string& effectType) const;
```

### ExperienceSystem API (расширение)

```cpp
// В ExperienceSystem.h (новые методы)

// Вызывается автоматически при повышении уровня
void onLevelUp();

// Дать игроку очки способностей напрямую (для квестов)
void grantBonusSkillPoints(int points);
```

### AbilityTreeScene API

```cpp
// Конструктор
AbilityTreeScene();

// Scene interface
void handleInput(const sf::Event& event) override;
void update(float deltaTime) override;
void render(sf::RenderWindow& window) override;
SceneType getNextScene() const override;
bool isFinished() const override;
```

---

## Форматы данных / Data Formats

### JSON формат сохранения

```json
{
  "abilityTree": {
    "version": 1,
    "availableSkillPoints": 2,
    "unlockedAbilities": [
      "driving_economical_1",
      "driving_economical_2",
      "driving_experienced_1",
      "survival_stamina_1",
      "survival_stamina_2",
      "social_charisma_1",
      "perk_relax",
      "perk_boost",
      "perk_encourage"
    ],
    "activeCooldowns": [
      {
        "perkId": "perk_boost",
        "remainingMinutes": 3.5
      },
      {
        "perkId": "perk_emergency_repair",
        "remainingMinutes": 25.0
      }
    ]
  }
}
```

### Формат конфигурации способностей (для будущего расширения)

```json
{
  "abilities": [
    {
      "id": "driving_economical_1",
      "name": "Экономичное вождение I",
      "description": "Снижение расхода топлива на 10%",
      "type": "PASSIVE",
      "category": "DRIVING",
      "requirement": {
        "requiredLevel": 1,
        "prerequisites": [],
        "skillPointCost": 1
      },
      "effect": {
        "effectType": "fuel_efficiency",
        "bonusMultiplier": 1.1,
        "bonusFlat": 0
      },
      "iconPath": "assets/icons/abilities/driving_economical_1.png"
    },
    {
      "id": "perk_boost",
      "name": "Форсаж",
      "description": "Увеличение скорости на 50% на 30 секунд",
      "type": "ACTIVE",
      "category": "DRIVING",
      "requirement": {
        "requiredLevel": 3,
        "prerequisites": ["driving_economical_1"],
        "skillPointCost": 2
      },
      "effect": {
        "effectType": "temporary_speed_boost",
        "energyCost": 0,
        "moneyCost": 0,
        "itemRequirements": [],
        "cooldownMinutes": 5.0,
        "effectDescription": "Увеличение скорости путешествия на 50% на 30 секунд. Расход топлива: 10%.",
        "targetType": "vehicle",
        "duration": 0.5
      },
      "iconPath": "assets/icons/abilities/perk_boost.png"
    }
  ]
}
```

---

## Интеграционные точки / Integration Points

### 1. Интеграция с ExperienceSystem

**Файл:** `src/ExperienceSystem.cpp`

**Изменения:**

```cpp
void ExperienceSystem::levelUp() {
    if (!canLevelUp()) {
        return;
    }
    
    m_experience -= getExperienceForNextLevel();
    m_level++;
    
    // ===== НОВОЕ: Добавить очко способности =====
    AbilityTreeSystem::getInstance().addSkillPoints(1);
    
    // ===== НОВОЕ: Показать уведомление или открыть дерево способностей =====
    // (Можно добавить флаг или событие для показа UI)
}
```

### 2. Интеграция с EventManager

**Файл:** `src/EventManager.cpp`

**Добавление опций с перками:**

```cpp
void EventManager::processEvent(const std::string& eventId) {
    Event event = getEvent(eventId);
    std::vector<EventChoice> choices = event.choices;
    
    // ===== НОВОЕ: Добавить опции с активными перками =====
    auto& abilitySystem = AbilityTreeSystem::getInstance();
    
    // Проходим по всем разблокированным активным перкам
    for (const auto& ability : abilitySystem.getAllAbilities()) {
        if (ability.getType() == AbilityType::ACTIVE &&
            ability.isUnlocked() &&
            abilitySystem.canUseActivePerk(ability.getId())) {
            
            // Проверяем, применим ли перк к этому событию
            if (isPerkApplicableToEvent(ability.getId(), event)) {
                EventChoice perkChoice = createPerkChoiceForEvent(ability, event);
                choices.push_back(perkChoice);
            }
        }
    }
    
    // Показать событие с расширенными опциями
    openCinematicEventScene(event, choices);
}
```

**Обработка выбора с перком:**

```cpp
void EventManager::onChoiceSelected(const EventChoice& choice) {
    // ===== НОВОЕ: Проверить, это выбор с перком или обычный =====
    if (!choice.perkId.empty()) {
        // Использовать перк
        if (AbilityTreeSystem::getInstance().useActivePerk(choice.perkId)) {
            // Перк успешно использован, применить специальные последствия
            applyPerkEffectToEvent(choice.perkId, choice);
        }
    }
    
    // Применить стандартные последствия
    applyOutcome(choice.outcome);
}
```

### 3. Интеграция с PlayerState

**Файл:** `src/PlayerState.cpp`

**Применение пассивных бонусов:**

```cpp
float PlayerState::getFuelConsumptionRate() const {
    float baseRate = m_car.fuelConsumption;
    
    // ===== НОВОЕ: Применить пассивный бонус экономии топлива =====
    float fuelBonus = AbilityTreeSystem::getInstance()
        .getPassiveBonus("fuel_efficiency");
    
    // fuelBonus = 1.2 означает на 20% эффективнее
    return baseRate / fuelBonus;
}

int PlayerState::getMaxEnergy() const {
    int baseEnergy = 100;
    
    // ===== НОВОЕ: Применить пассивный плоский бонус =====
    int energyBonus = AbilityTreeSystem::getInstance()
        .getPassiveBonusFlat("max_energy");
    
    return baseEnergy + energyBonus;
}
```

### 4. Интеграция с TravelCinematicScene

**Файл:** `src/TravelCinematicScene.cpp`

**Обновление перезарядок:**

```cpp
void TravelCinematicScene::update(float deltaTime) {
    // ... существующий код ...
    
    // ===== НОВОЕ: Обновить перезарядки перков =====
    float deltaMinutes = deltaTime / 60.0f;  // Конвертировать секунды в минуты
    AbilityTreeSystem::getInstance().updateCooldowns(deltaMinutes);
    
    // ... остальной код ...
}
```

### 5. Интеграция с HUDPanel

**Файл:** `src/HUDPanel.cpp`

**Отображение иконок активных перков:**

```cpp
void HUDPanel::renderActivePerkIcons(sf::RenderWindow& window) {
    // Список быстрого доступа (F1-F5)
    const std::vector<std::string> quickSlots = {
        "perk_relax",
        "perk_boost",
        "perk_emergency_repair",
        "perk_negotiate",
        "perk_encourage"
    };
    
    auto& abilitySystem = AbilityTreeSystem::getInstance();
    float x = 10.0f;
    float y = window.getSize().y - 80.0f;
    
    for (size_t i = 0; i < quickSlots.size(); ++i) {
        const std::string& perkId = quickSlots[i];
        
        if (abilitySystem.isAbilityUnlocked(perkId)) {
            float cooldown = abilitySystem.getCooldownRemaining(perkId);
            
            // Отрисовка иконки с таймером перезарядки (если есть)
            renderPerkIcon(window, perkId, x + i * 70.0f, y, cooldown);
        }
    }
}
```

---

## Тестовое покрытие / Test Coverage

### Unit тесты

**Файл:** `tests/test_AbilityNode.cpp`

```cpp
TEST(AbilityNode, CanUnlockWithRequirementsMet) {
    AbilityRequirement req(3, {"ability_1"}, 1);
    AbilityEffect effect;
    AbilityNode node("test_ability", "Test", "Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    std::vector<std::string> unlocked = {"ability_1"};
    ASSERT_TRUE(node.canUnlock(3, 1, unlocked));
}

TEST(AbilityNode, CannotUnlockWithoutPrerequisites) {
    AbilityRequirement req(3, {"ability_1", "ability_2"}, 1);
    AbilityEffect effect;
    AbilityNode node("test_ability", "Test", "Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    std::vector<std::string> unlocked = {"ability_1"};  // ability_2 отсутствует
    ASSERT_FALSE(node.canUnlock(3, 1, unlocked));
}

TEST(AbilityNode, CannotUnlockWithInsufficientLevel) {
    AbilityRequirement req(5, {}, 1);
    AbilityEffect effect;
    AbilityNode node("test_ability", "Test", "Description",
                    AbilityType::PASSIVE, AbilityCategory::DRIVING,
                    req, effect);
    
    std::vector<std::string> unlocked;
    ASSERT_FALSE(node.canUnlock(3, 1, unlocked));  // Уровень 3 < 5
}
```

**Файл:** `tests/test_AbilityTreeSystem.cpp`

```cpp
TEST(AbilityTreeSystem, InitializeCreatesAbilities) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    system.initialize();
    
    ASSERT_GT(system.getAllAbilities().size(), 0);
}

TEST(AbilityTreeSystem, UnlockAbilitySuccess) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    system.initialize();
    system.addSkillPoints(5);
    
    // Установить уровень игрока
    ExperienceSystem::getInstance().reset();
    ExperienceSystem::getInstance().addExperience(300);  // Поднять уровень
    
    bool result = system.unlockAbility("driving_economical_1");
    ASSERT_TRUE(result);
    ASSERT_TRUE(system.isAbilityUnlocked("driving_economical_1"));
}

TEST(AbilityTreeSystem, CannotUnlockWithoutSkillPoints) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    system.initialize();
    // Не добавляем очки способностей
    
    bool result = system.unlockAbility("driving_economical_1");
    ASSERT_FALSE(result);
}

TEST(AbilityTreeSystem, PassiveBonusCalculation) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    system.initialize();
    system.addSkillPoints(10);
    
    // Разблокировать несколько способностей с бонусом к fuel_efficiency
    system.unlockAbility("driving_economical_1");  // +10% (1.1)
    system.unlockAbility("driving_economical_2");  // +15% (1.15)
    
    float bonus = system.getPassiveBonus("fuel_efficiency");
    ASSERT_FLOAT_EQ(bonus, 1.1f * 1.15f);  // 1.265
}

TEST(AbilityTreeSystem, ActivePerkCooldown) {
    auto& system = AbilityTreeSystem::getInstance();
    system.reset();
    system.initialize();
    
    // Разблокировать и использовать перк
    system.addSkillPoints(5);
    system.unlockAbility("perk_boost");
    
    ASSERT_TRUE(system.canUseActivePerk("perk_boost"));
    ASSERT_TRUE(system.useActivePerk("perk_boost"));
    
    // Проверить, что перк на перезарядке
    ASSERT_TRUE(system.isOnCooldown("perk_boost"));
    ASSERT_GT(system.getCooldownRemaining("perk_boost"), 0.0f);
    
    // Обновить перезарядки
    system.updateCooldowns(5.0f);  // 5 минут прошло
    
    // Если cooldown был 5 минут, теперь должен быть готов
    ASSERT_FALSE(system.isOnCooldown("perk_boost"));
}
```

### Интеграционные тесты

**Файл:** `tests/test_AbilityIntegration.cpp`

```cpp
TEST(AbilityIntegration, LevelUpGrantsSkillPoints) {
    auto& expSystem = ExperienceSystem::getInstance();
    auto& abilitySystem = AbilityTreeSystem::getInstance();
    
    expSystem.reset();
    abilitySystem.reset();
    
    int initialPoints = abilitySystem.getAvailableSkillPoints();
    
    // Дать достаточно опыта для повышения уровня
    expSystem.addExperience(200);
    
    int finalPoints = abilitySystem.getAvailableSkillPoints();
    ASSERT_GT(finalPoints, initialPoints);
}

TEST(AbilityIntegration, PassiveBonusAffectsPlayerState) {
    auto& abilitySystem = AbilityTreeSystem::getInstance();
    auto& playerState = PlayerState::getInstance();
    
    abilitySystem.reset();
    playerState.reset();
    
    float baseFuel = playerState.getFuelConsumptionRate();
    
    // Разблокировать способность экономии топлива
    abilitySystem.addSkillPoints(2);
    abilitySystem.unlockAbility("driving_economical_1");
    
    float modifiedFuel = playerState.getFuelConsumptionRate();
    
    // Расход топлива должен уменьшиться
    ASSERT_LT(modifiedFuel, baseFuel);
}

TEST(AbilityIntegration, ActivePerkInEvent) {
    auto& abilitySystem = AbilityTreeSystem::getInstance();
    auto& eventManager = EventManager::getInstance();
    
    abilitySystem.reset();
    abilitySystem.addSkillPoints(5);
    abilitySystem.unlockAbility("perk_emergency_repair");
    
    // Симулировать событие поломки
    Event event = eventManager.getEvent("car_breakdown");
    
    // Проверить, что опция с перком доступна
    bool perkOptionAvailable = false;
    for (const auto& choice : event.choices) {
        if (choice.perkId == "perk_emergency_repair") {
            perkOptionAvailable = true;
            break;
        }
    }
    
    ASSERT_TRUE(perkOptionAvailable);
}
```

### Целевое покрытие

- **Unit тесты**: 80%+ покрытие кода
- **Интеграционные тесты**: Все критические пути взаимодействия
- **Минимум тестов**: 15 unit + 5 интеграционных = 20 тестов

---

## Контрольный список реализации / Implementation Checklist

### Фаза 3: Реализация

- [ ] **Шаг 1**: Создать файлы заголовков
  - [ ] `include/AbilityTypes.h`
  - [ ] `include/AbilityNode.h`
  - [ ] `include/AbilityTreeSystem.h`
  - [ ] `include/AbilityTreeScene.h`

- [ ] **Шаг 2**: Реализовать базовые классы
  - [ ] `src/AbilityNode.cpp`
  - [ ] `src/AbilityTreeSystem.cpp` (базовая функциональность)

- [ ] **Шаг 3**: Инициализировать дерево способностей
  - [ ] Добавить все 35+ способностей в `initializeAbilityTree()`
  - [ ] Настроить зависимости и требования

- [ ] **Шаг 4**: Интеграция с ExperienceSystem
  - [ ] Обновить `ExperienceSystem::levelUp()`
  - [ ] Добавить выдачу очков способностей

- [ ] **Шаг 5**: Реализовать UI сцену
  - [ ] `src/AbilityTreeScene.cpp` (рендеринг)
  - [ ] Навигация и взаимодействие
  - [ ] Детальная панель информации

- [ ] **Шаг 6**: Интеграция с игровыми системами
  - [ ] PlayerState (пассивные бонусы)
  - [ ] EventManager (активные перки в событиях)
  - [ ] TravelCinematicScene (обновление перезарядок)

- [ ] **Шаг 7**: Система сохранения/загрузки
  - [ ] Реализовать `save()` и `load()`
  - [ ] JSON сериализация

- [ ] **Шаг 8**: Тестирование
  - [ ] Unit тесты (15+)
  - [ ] Интеграционные тесты (5+)
  - [ ] Ручное тестирование UI

- [ ] **Шаг 9**: Документация и отчёт
  - [ ] Обновить `docs/progress/ability_tree_implementation.md`
  - [ ] Добавить примеры использования
  - [ ] Обновить PROJECT_STATUS.md

---

## Следующий шаг / Next Step

✅ **Фаза 1**: Документация завершена ([ABILITY_TREE_SYSTEM.md](./ABILITY_TREE_SYSTEM.md))  
✅ **Фаза 2**: Техническое описание завершено (этот документ)  
🔄 **Фаза 3**: Реализация (следующий шаг)

**Команда для начала реализации:**
```bash
# Создать новые файлы
touch include/AbilityTypes.h
touch include/AbilityNode.h
touch include/AbilityTreeSystem.h
touch include/AbilityTreeScene.h

touch src/AbilityNode.cpp
touch src/AbilityTreeSystem.cpp
touch src/AbilityTreeScene.cpp

touch tests/test_AbilityNode.cpp
touch tests/test_AbilityTreeSystem.cpp
touch tests/test_AbilityIntegration.cpp
```

---

**Статус / Status**: ✅ Детальное описание завершено / Detailed Description Complete  
**Следующий документ / Next Document**: `docs/progress/ability_tree_implementation.md` (после реализации)
