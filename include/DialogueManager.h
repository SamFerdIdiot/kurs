#ifndef DIALOGUE_MANAGER_H
#define DIALOGUE_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

// Forward declarations
class NPC;

// Условие для отображения варианта диалога
struct DialogueCondition {
    std::function<bool()> check;    // Функция проверки условия
    std::string description;         // Описание условия для игрока
    
    DialogueCondition() : check([]() { return true; }), description("") {}
    explicit DialogueCondition(std::function<bool()> checkFunc, const std::string& desc = "")
        : check(checkFunc), description(desc) {}
};

// Последствие выбора в диалоге
struct DialogueConsequence {
    std::function<void()> execute;   // Функция выполнения последствия
    std::string description;         // Описание последствия
    
    DialogueConsequence() : execute([]() {}), description("") {}
    explicit DialogueConsequence(std::function<void()> execFunc, const std::string& desc = "")
        : execute(execFunc), description(desc) {}
};

// Вариант выбора в диалоге
struct DialogueChoice {
    std::string text;                       // Текст варианта выбора
    std::string nextNodeId;                 // ID следующего узла диалога
    DialogueCondition condition;            // Условие доступности (по умолчанию всегда доступен)
    DialogueConsequence consequence;        // Последствие выбора (по умолчанию нет)
    bool requiresMoney;                     // Требуется ли деньги
    float moneyCost;                        // Стоимость выбора
    bool requiresItem;                      // Требуется ли предмет
    std::string requiredItemId;             // ID требуемого предмета
    
    DialogueChoice()
        : requiresMoney(false), moneyCost(0.0f),
          requiresItem(false), requiredItemId("") {}
};

// Узел диалогового дерева
struct DialogueNode {
    std::string id;                         // Уникальный ID узла
    std::string speakerName;                // Имя говорящего (NPC или игрок)
    std::string text;                       // Текст диалога
    std::vector<DialogueChoice> choices;    // Варианты ответов
    bool isPlayerSpeaking;                  // Говорит ли игрок (false = NPC)
    bool isEndNode;                         // Конечный узел диалога
    
    DialogueNode()
        : isPlayerSpeaking(false), isEndNode(false) {}
};

// Дерево диалога
struct DialogueTree {
    std::string id;                         // ID диалога
    std::string title;                      // Название диалога
    std::map<std::string, DialogueNode> nodes; // Узлы диалога по ID
    std::string startNodeId;                // ID начального узла
    
    DialogueTree() : startNodeId("start") {}
};

// Менеджер диалогов
// Управляет всеми диалогами в игре
class DialogueManager {
public:
    // Получить экземпляр синглтона
    static DialogueManager& getInstance();
    
    // Управление диалогами
    void registerDialogue(const std::string& id, const DialogueTree& tree);
    void removeDialogue(const std::string& id);
    bool hasDialogue(const std::string& id) const;
    
    // Запуск диалога
    bool startDialogue(const std::string& dialogueId);
    
    // Навигация по диалогу
    void selectChoice(int choiceIndex);
    bool goToNode(const std::string& nodeId);
    void endDialogue();
    
    // Получение информации о текущем состоянии
    bool isActive() const { return m_active; }
    const DialogueNode* getCurrentNode() const;
    const DialogueTree* getCurrentTree() const;
    std::string getCurrentDialogueId() const { return m_currentDialogueId; }
    
    // Проверка доступности варианта
    bool isChoiceAvailable(int choiceIndex) const;
    std::string getChoiceUnavailableReason(int choiceIndex) const;
    
    // История диалогов
    void addToHistory(const std::string& dialogueId, const std::string& choiceId);
    bool wasChoiceMade(const std::string& dialogueId, const std::string& choiceId) const;
    void clearHistory();
    
    // Очистка
    void clear();
    
private:
    DialogueManager();
    ~DialogueManager() = default;
    
    // Запретить копирование
    DialogueManager(const DialogueManager&) = delete;
    DialogueManager& operator=(const DialogueManager&) = delete;
    
    std::map<std::string, DialogueTree> m_dialogues;    // Все диалоги
    std::string m_currentDialogueId;                    // ID текущего диалога
    std::string m_currentNodeId;                        // ID текущего узла
    bool m_active;                                      // Активен ли диалог
    
    // История выборов: dialogueId -> список выбранных choiceIds
    std::map<std::string, std::vector<std::string>> m_history;
    
    static DialogueManager* s_instance;
};

// Фабрика для создания предопределённых диалогов
namespace DialogueFactory {
    // Диалог с механиком
    DialogueTree createMechanicDialogue();
    
    // Диалог с торговцем
    DialogueTree createTraderDialogue();
    
    // Диалог с компаньоном
    DialogueTree createCompanionDialogue();
    
    // Диалог найма NPC
    DialogueTree createRecruitDialogue(const std::string& npcId, 
                                       const std::string& npcName,
                                       float hireCost);
    
    // Диалог конфликта в команде
    DialogueTree createConflictDialogue();
    
    // Простой информационный диалог
    DialogueTree createInfoDialogue(const std::string& id,
                                   const std::string& speakerName,
                                   const std::string& message);
    
    // Новые диалоги для рекрутинга конкретных NPC
    // Dialogue for recruiting Mechanic Mikhail
    DialogueTree createRecruitMechanicDialogue();
    
    // Dialogue for recruiting Trader Sarah
    DialogueTree createRecruitTraderDialogue();
    
    // Dialogue for recruiting Navigator Jack
    DialogueTree createRecruitNavigatorDialogue();
    
    // Dialogue for recruiting Medic Emma
    DialogueTree createRecruitMedicDialogue();
    
    // Dialogue for recruiting Companion Alex
    DialogueTree createRecruitCompanionAlexDialogue();
}

#endif // DIALOGUE_MANAGER_H
