#ifndef DIALOGUE_MANAGER_H
#define DIALOGUE_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>


class NPC;


struct DialogueCondition {
    std::function<bool()> check;
    std::string description;

    DialogueCondition() : check([]() { return true; }), description("") {}
    explicit DialogueCondition(std::function<bool()> checkFunc, const std::string& desc = "")
        : check(checkFunc), description(desc) {}
};


struct DialogueConsequence {
    std::function<void()> execute;
    std::string description;

    DialogueConsequence() : execute([]() {}), description("") {}
    explicit DialogueConsequence(std::function<void()> execFunc, const std::string& desc = "")
        : execute(execFunc), description(desc) {}
};


struct DialogueChoice {
    std::string text;
    std::string nextNodeId;
    DialogueCondition condition;
    DialogueConsequence consequence;
    bool requiresMoney;
    float moneyCost;
    bool requiresItem;
    std::string requiredItemId;

    DialogueChoice()
        : requiresMoney(false), moneyCost(0.0f),
          requiresItem(false), requiredItemId("") {}
};


struct DialogueNode {
    std::string id;
    std::string speakerName;
    std::string text;
    std::vector<DialogueChoice> choices;
    bool isPlayerSpeaking;
    bool isEndNode;

    DialogueNode()
        : isPlayerSpeaking(false), isEndNode(false) {}
};


struct DialogueTree {
    std::string id;
    std::string title;
    std::map<std::string, DialogueNode> nodes;
    std::string startNodeId;

    DialogueTree() : startNodeId("start") {}
};



class DialogueManager {
public:

    static DialogueManager& getInstance();


    void registerDialogue(const std::string& id, const DialogueTree& tree);
    void removeDialogue(const std::string& id);
    bool hasDialogue(const std::string& id) const;


    bool startDialogue(const std::string& dialogueId);


    void selectChoice(int choiceIndex);
    bool goToNode(const std::string& nodeId);
    void endDialogue();


    bool isActive() const { return m_active; }
    const DialogueNode* getCurrentNode() const;
    const DialogueTree* getCurrentTree() const;
    std::string getCurrentDialogueId() const { return m_currentDialogueId; }


    bool isChoiceAvailable(int choiceIndex) const;
    std::string getChoiceUnavailableReason(int choiceIndex) const;


    void addToHistory(const std::string& dialogueId, const std::string& choiceId);
    bool wasChoiceMade(const std::string& dialogueId, const std::string& choiceId) const;
    void clearHistory();


    void clear();

private:
    DialogueManager();
    ~DialogueManager() = default;


    DialogueManager(const DialogueManager&) = delete;
    DialogueManager& operator=(const DialogueManager&) = delete;

    std::map<std::string, DialogueTree> m_dialogues;
    std::string m_currentDialogueId;
    std::string m_currentNodeId;
    bool m_active;


    std::map<std::string, std::vector<std::string>> m_history;

    static DialogueManager* s_instance;
};


namespace DialogueFactory {

    DialogueTree createMechanicDialogue();


    DialogueTree createTraderDialogue();


    DialogueTree createCompanionDialogue();


    DialogueTree createRecruitDialogue(const std::string& npcId,
                                       const std::string& npcName,
                                       float hireCost);


    DialogueTree createConflictDialogue();


    DialogueTree createInfoDialogue(const std::string& id,
                                   const std::string& speakerName,
                                   const std::string& message);



    DialogueTree createRecruitMechanicDialogue();


    DialogueTree createRecruitTraderDialogue();


    DialogueTree createRecruitNavigatorDialogue();


    DialogueTree createRecruitMedicDialogue();


    DialogueTree createRecruitCompanionAlexDialogue();
}

#endif
