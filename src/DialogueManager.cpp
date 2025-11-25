#include "DialogueManager.h"
#include "NPC.h"
#include <iostream>

// ============================================================================
// DialogueManager Class Implementation
// ============================================================================

DialogueManager* DialogueManager::s_instance = nullptr;

DialogueManager::DialogueManager()
    : m_active(false) {
}

DialogueManager& DialogueManager::getInstance() {
    if (!s_instance) {
        s_instance = new DialogueManager();
    }
    return *s_instance;
}

void DialogueManager::registerDialogue(const std::string& id, const DialogueTree& tree) {
    m_dialogues[id] = tree;
}

void DialogueManager::removeDialogue(const std::string& id) {
    m_dialogues.erase(id);
}

bool DialogueManager::hasDialogue(const std::string& id) const {
    return m_dialogues.find(id) != m_dialogues.end();
}

bool DialogueManager::startDialogue(const std::string& dialogueId) {
    // Check if dialogue exists
    auto it = m_dialogues.find(dialogueId);
    if (it == m_dialogues.end()) {
        std::cerr << "Dialogue not found: " << dialogueId << std::endl;
        return false;
    }
    
    // Check if start node exists
    const DialogueTree& tree = it->second;
    auto nodeIt = tree.nodes.find(tree.startNodeId);
    if (nodeIt == tree.nodes.end()) {
        std::cerr << "Start node not found in dialogue: " << dialogueId << std::endl;
        return false;
    }
    
    // Start dialogue
    m_currentDialogueId = dialogueId;
    m_currentNodeId = tree.startNodeId;
    m_active = true;
    
    return true;
}

void DialogueManager::selectChoice(int choiceIndex) {
    if (!m_active) {
        return;
    }
    
    const DialogueNode* currentNode = getCurrentNode();
    if (!currentNode) {
        return;
    }
    
    // Validate choice index
    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(currentNode->choices.size())) {
        std::cerr << "Invalid choice index: " << choiceIndex << std::endl;
        return;
    }
    
    const DialogueChoice& choice = currentNode->choices[choiceIndex];
    
    // Check if choice is available
    if (!isChoiceAvailable(choiceIndex)) {
        std::cerr << "Choice not available: " << getChoiceUnavailableReason(choiceIndex) << std::endl;
        return;
    }
    
    // Execute consequence
    if (choice.consequence.execute) {
        choice.consequence.execute();
    }
    
    // Add to history
    addToHistory(m_currentDialogueId, choice.text);
    
    // Move to next node
    if (choice.nextNodeId.empty()) {
        // End dialogue
        endDialogue();
    } else {
        if (!goToNode(choice.nextNodeId)) {
            // If node not found, end dialogue
            std::cerr << "Next node not found: " << choice.nextNodeId << std::endl;
            endDialogue();
        }
    }
}

bool DialogueManager::goToNode(const std::string& nodeId) {
    const DialogueTree* tree = getCurrentTree();
    if (!tree) {
        return false;
    }
    
    auto nodeIt = tree->nodes.find(nodeId);
    if (nodeIt == tree->nodes.end()) {
        return false;
    }
    
    m_currentNodeId = nodeId;
    
    // Check if this is an end node
    if (nodeIt->second.isEndNode) {
        endDialogue();
    }
    
    return true;
}

void DialogueManager::endDialogue() {
    m_active = false;
    m_currentDialogueId.clear();
    m_currentNodeId.clear();
}

const DialogueNode* DialogueManager::getCurrentNode() const {
    if (!m_active) {
        return nullptr;
    }
    
    const DialogueTree* tree = getCurrentTree();
    if (!tree) {
        return nullptr;
    }
    
    auto it = tree->nodes.find(m_currentNodeId);
    if (it == tree->nodes.end()) {
        return nullptr;
    }
    
    return &(it->second);
}

const DialogueTree* DialogueManager::getCurrentTree() const {
    if (m_currentDialogueId.empty()) {
        return nullptr;
    }
    
    auto it = m_dialogues.find(m_currentDialogueId);
    if (it == m_dialogues.end()) {
        return nullptr;
    }
    
    return &(it->second);
}

bool DialogueManager::isChoiceAvailable(int choiceIndex) const {
    const DialogueNode* currentNode = getCurrentNode();
    if (!currentNode) {
        return false;
    }
    
    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(currentNode->choices.size())) {
        return false;
    }
    
    const DialogueChoice& choice = currentNode->choices[choiceIndex];
    
    // Check condition
    if (choice.condition.check && !choice.condition.check()) {
        return false;
    }
    
    return true;
}

std::string DialogueManager::getChoiceUnavailableReason(int choiceIndex) const {
    const DialogueNode* currentNode = getCurrentNode();
    if (!currentNode) {
        return "No active dialogue";
    }
    
    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(currentNode->choices.size())) {
        return "Invalid choice index";
    }
    
    const DialogueChoice& choice = currentNode->choices[choiceIndex];
    
    // Check condition
    if (choice.condition.check && !choice.condition.check()) {
        if (!choice.condition.description.empty()) {
            return "Недоступно: " + choice.condition.description + 
                   " / Unavailable: " + choice.condition.description;
        }
        return "Требования не выполнены / Requirements not met";
    }
    
    return "Доступно / Available";
}

void DialogueManager::addToHistory(const std::string& dialogueId, const std::string& choiceId) {
    m_history[dialogueId].push_back(choiceId);
}

bool DialogueManager::wasChoiceMade(const std::string& dialogueId, const std::string& choiceId) const {
    auto it = m_history.find(dialogueId);
    if (it == m_history.end()) {
        return false;
    }
    
    const auto& choices = it->second;
    return std::find(choices.begin(), choices.end(), choiceId) != choices.end();
}

void DialogueManager::clearHistory() {
    m_history.clear();
}

void DialogueManager::clear() {
    m_dialogues.clear();
    m_history.clear();
    m_active = false;
    m_currentDialogueId.clear();
    m_currentNodeId.clear();
}

// ============================================================================
// DialogueFactory Implementation
// ============================================================================

namespace DialogueFactory {
    
    DialogueTree createMechanicDialogue() {
        DialogueTree tree;
        tree.id = "mechanic_repair";
        tree.title = "Диалог с механиком / Mechanic Dialogue";
        tree.startNodeId = "start";
        
        // Start node
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Механик / Mechanic";
        startNode.text = "Эй, приятель! Вижу, у тебя проблемы с машиной. "
                        "Могу помочь, но это будет стоить денег...\n\n"
                        "Hey, buddy! I see you have car problems. "
                        "I can help, but it will cost money...";
        startNode.isPlayerSpeaking = false;
        
        // Choice 1: Pay for repair
        DialogueChoice choice1;
        choice1.text = "Заплатить за ремонт ($80) / Pay for repair ($80)";
        choice1.nextNodeId = "repair_done";
        choice1.requiresMoney = true;
        choice1.moneyCost = 80.0f;
        
        // Choice 2: Ask for discount
        DialogueChoice choice2;
        choice2.text = "Попросить скидку / Ask for discount";
        choice2.nextNodeId = "ask_discount";
        
        // Choice 3: Leave
        DialogueChoice choice3;
        choice3.text = "Отказаться и уехать / Refuse and leave";
        choice3.nextNodeId = "";  // End dialogue
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        startNode.choices.push_back(choice3);
        
        // Repair done node
        DialogueNode repairNode;
        repairNode.id = "repair_done";
        repairNode.speakerName = "Механик / Mechanic";
        repairNode.text = "Готово! Машина как новая!\n\n"
                         "Done! Car is as good as new!";
        repairNode.isPlayerSpeaking = false;
        repairNode.isEndNode = true;
        
        // Ask discount node
        DialogueNode discountNode;
        discountNode.id = "ask_discount";
        discountNode.speakerName = "Механик / Mechanic";
        discountNode.text = "Ну ладно, для тебя сделаю скидку - $60.\n\n"
                           "Alright, I'll give you a discount - $60.";
        discountNode.isPlayerSpeaking = false;
        
        DialogueChoice discountChoice1;
        discountChoice1.text = "Согласиться ($60) / Agree ($60)";
        discountChoice1.nextNodeId = "repair_done";
        discountChoice1.requiresMoney = true;
        discountChoice1.moneyCost = 60.0f;
        
        DialogueChoice discountChoice2;
        discountChoice2.text = "Отказаться / Refuse";
        discountChoice2.nextNodeId = "";
        
        discountNode.choices.push_back(discountChoice1);
        discountNode.choices.push_back(discountChoice2);
        
        // Add nodes to tree
        tree.nodes[startNode.id] = startNode;
        tree.nodes[repairNode.id] = repairNode;
        tree.nodes[discountNode.id] = discountNode;
        
        return tree;
    }
    
    DialogueTree createTraderDialogue() {
        DialogueTree tree;
        tree.id = "trader_shop";
        tree.title = "Диалог с торговцем / Trader Dialogue";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Торговец / Trader";
        startNode.text = "Добро пожаловать! У меня есть отличные товары!\n\n"
                        "Welcome! I have great goods!";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Купить топливо ($50) / Buy fuel ($50)";
        choice1.nextNodeId = "buy_fuel";
        choice1.requiresMoney = true;
        choice1.moneyCost = 50.0f;
        
        DialogueChoice choice2;
        choice2.text = "Купить еду ($15) / Buy food ($15)";
        choice2.nextNodeId = "buy_food";
        choice2.requiresMoney = true;
        choice2.moneyCost = 15.0f;
        
        DialogueChoice choice3;
        choice3.text = "Уйти / Leave";
        choice3.nextNodeId = "";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        startNode.choices.push_back(choice3);
        
        // Buy fuel node
        DialogueNode fuelNode;
        fuelNode.id = "buy_fuel";
        fuelNode.speakerName = "Торговец / Trader";
        fuelNode.text = "Топливо залито! / Fuel filled!";
        fuelNode.isPlayerSpeaking = false;
        fuelNode.isEndNode = true;
        
        // Buy food node
        DialogueNode foodNode;
        foodNode.id = "buy_food";
        foodNode.speakerName = "Торговец / Trader";
        foodNode.text = "Приятного аппетита! / Bon appetit!";
        foodNode.isPlayerSpeaking = false;
        foodNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[fuelNode.id] = fuelNode;
        tree.nodes[foodNode.id] = foodNode;
        
        return tree;
    }
    
    DialogueTree createCompanionDialogue() {
        DialogueTree tree;
        tree.id = "companion_chat";
        tree.title = "Разговор с компаньоном / Chat with Companion";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Компаньон / Companion";
        startNode.text = "Отличная погода для путешествия!\n\n"
                        "Great weather for a trip!";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Согласиться / Agree";
        choice1.nextNodeId = "agree";
        
        DialogueChoice choice2;
        choice2.text = "Закончить разговор / End conversation";
        choice2.nextNodeId = "";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        
        DialogueNode agreeNode;
        agreeNode.id = "agree";
        agreeNode.speakerName = "Компаньон / Companion";
        agreeNode.text = "Вот и я так думаю! Продолжим путь?\n\n"
                        "That's what I think too! Shall we continue?";
        agreeNode.isPlayerSpeaking = false;
        agreeNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[agreeNode.id] = agreeNode;
        
        return tree;
    }
    
    DialogueTree createRecruitDialogue(const std::string& npcId, 
                                      const std::string& npcName,
                                      float hireCost) {
        DialogueTree tree;
        tree.id = "recruit_" + npcId;
        tree.title = "Найм NPC / Recruit NPC";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = npcName;
        startNode.text = "Ищете напарника? Я готов присоединиться за $" + 
                        std::to_string(static_cast<int>(hireCost)) + ".\n\n"
                        "Looking for a partner? I'm ready to join for $" + 
                        std::to_string(static_cast<int>(hireCost)) + ".";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Нанять ($" + std::to_string(static_cast<int>(hireCost)) + 
                      ") / Hire ($" + std::to_string(static_cast<int>(hireCost)) + ")";
        choice1.nextNodeId = "hired";
        choice1.requiresMoney = true;
        choice1.moneyCost = hireCost;
        
        DialogueChoice choice2;
        choice2.text = "Отказать / Refuse";
        choice2.nextNodeId = "";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        
        DialogueNode hiredNode;
        hiredNode.id = "hired";
        hiredNode.speakerName = npcName;
        hiredNode.text = "Отлично! Поехали!\n\nGreat! Let's go!";
        hiredNode.isPlayerSpeaking = false;
        hiredNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[hiredNode.id] = hiredNode;
        
        return tree;
    }
    
    DialogueTree createConflictDialogue() {
        DialogueTree tree;
        tree.id = "team_conflict";
        tree.title = "Конфликт в команде / Team Conflict";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Система / System";
        startNode.text = "Ваша команда поссорилась! Нужно что-то делать.\n\n"
                        "Your team had a fight! Something needs to be done.";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Вмешаться и разрешить конфликт / Intervene and resolve";
        choice1.nextNodeId = "resolve";
        
        DialogueChoice choice2;
        choice2.text = "Игнорировать / Ignore";
        choice2.nextNodeId = "ignore";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        
        DialogueNode resolveNode;
        resolveNode.id = "resolve";
        resolveNode.speakerName = "Система / System";
        resolveNode.text = "Вы успешно разрешили конфликт! Мораль команды +10%.\n\n"
                          "You successfully resolved the conflict! Team morale +10%.";
        resolveNode.isPlayerSpeaking = false;
        resolveNode.isEndNode = true;
        
        DialogueNode ignoreNode;
        ignoreNode.id = "ignore";
        ignoreNode.speakerName = "Система / System";
        ignoreNode.text = "Конфликт продолжается. Мораль команды -10%.\n\n"
                         "The conflict continues. Team morale -10%.";
        ignoreNode.isPlayerSpeaking = false;
        ignoreNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[resolveNode.id] = resolveNode;
        tree.nodes[ignoreNode.id] = ignoreNode;
        
        return tree;
    }
    
    DialogueTree createInfoDialogue(const std::string& id,
                                   const std::string& speakerName,
                                   const std::string& message) {
        DialogueTree tree;
        tree.id = id;
        tree.title = "Информация / Information";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = speakerName;
        startNode.text = message;
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice;
        choice.text = "ОК / OK";
        choice.nextNodeId = "";
        
        startNode.choices.push_back(choice);
        tree.nodes[startNode.id] = startNode;
        
        return tree;
    }
    
    // Dialogue for recruiting Mechanic Mikhail
    DialogueTree createRecruitMechanicDialogue() {
        DialogueTree tree;
        tree.id = "recruit_mechanic_mikhail";
        tree.title = "Найм механика / Recruit Mechanic";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Михаил / Mikhail";
        startNode.text = "Привет! Я Михаил, механик с 15-летним стажем. Могу чинить любую машину на ходу.\n"
                        "Возьмёшь меня в команду? Обещаю, твоя машина всегда будет в отличном состоянии!\n\n"
                        "Hi! I'm Mikhail, mechanic with 15 years experience. I can fix any car on the go.\n"
                        "Will you take me on the team? I promise your car will always be in excellent condition!";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Поехали вместе! / Let's go together!";
        choice1.nextNodeId = "hired";
        
        DialogueChoice choice2;
        choice2.text = "Расскажи о своих навыках / Tell me about your skills";
        choice2.nextNodeId = "skills";
        
        DialogueChoice choice3;
        choice3.text = "Может позже / Maybe later";
        choice3.nextNodeId = "";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        startNode.choices.push_back(choice3);
        
        DialogueNode skillsNode;
        skillsNode.id = "skills";
        skillsNode.speakerName = "Михаил / Mikhail";
        skillsNode.text = "Я умею:\n- Быстрый ремонт двигателя прямо в дороге\n- Снижение расхода топлива на 10%\n"
                         "- Предупреждение серьёзных поломок\n\nВозьмёшь меня?\n\n"
                         "I can:\n- Quick engine repair right on the road\n- Reduce fuel consumption by 10%\n"
                         "- Prevent serious breakdowns\n\nWill you take me?";
        skillsNode.isPlayerSpeaking = false;
        
        DialogueChoice skillsChoice1;
        skillsChoice1.text = "Хорошо, поехали! / Alright, let's go!";
        skillsChoice1.nextNodeId = "hired";
        
        DialogueChoice skillsChoice2;
        skillsChoice2.text = "Дай подумать / Let me think";
        skillsChoice2.nextNodeId = "";
        
        skillsNode.choices.push_back(skillsChoice1);
        skillsNode.choices.push_back(skillsChoice2);
        
        DialogueNode hiredNode;
        hiredNode.id = "hired";
        hiredNode.speakerName = "Михаил / Mikhail";
        hiredNode.text = "Отлично! Не пожалеешь! Поехали!\n\nGreat! You won't regret it! Let's go!";
        hiredNode.isPlayerSpeaking = false;
        hiredNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[skillsNode.id] = skillsNode;
        tree.nodes[hiredNode.id] = hiredNode;
        
        return tree;
    }
    
    // Dialogue for recruiting Trader Sarah
    DialogueTree createRecruitTraderDialogue() {
        DialogueTree tree;
        tree.id = "recruit_trader_sarah";
        tree.title = "Найм торговца / Recruit Trader";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Сара / Sarah";
        startNode.text = "Здравствуйте! Меня зовут Сара. Я торговка с большим опытом.\n"
                        "Я знаю, где купить дёшево и продать дорого. Возьмёте меня с собой?\n\n"
                        "Hello! My name is Sarah. I'm a trader with lots of experience.\n"
                        "I know where to buy cheap and sell high. Will you take me along?";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Поехали вместе! / Let's go together!";
        choice1.nextNodeId = "hired";
        
        DialogueChoice choice2;
        choice2.text = "Что ты умеешь? / What can you do?";
        choice2.nextNodeId = "skills";
        
        DialogueChoice choice3;
        choice3.text = "Нет, спасибо / No, thanks";
        choice3.nextNodeId = "";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        startNode.choices.push_back(choice3);
        
        DialogueNode skillsNode;
        skillsNode.id = "skills";
        skillsNode.speakerName = "Сара / Sarah";
        skillsNode.text = "Со мной вы будете:\n- Покупать товары на 15% дешевле\n- Продавать на 20% дороже\n"
                         "- Находить редкие предметы\n\nВозьмёшь меня?\n\n"
                         "With me you will:\n- Buy goods 15% cheaper\n- Sell 20% more expensive\n"
                         "- Find rare items\n\nWill you take me?";
        skillsNode.isPlayerSpeaking = false;
        
        DialogueChoice skillsChoice1;
        skillsChoice1.text = "Убедила! Поехали! / Convinced! Let's go!";
        skillsChoice1.nextNodeId = "hired";
        
        DialogueChoice skillsChoice2;
        skillsChoice2.text = "Подумаю / I'll think about it";
        skillsChoice2.nextNodeId = "";
        
        skillsNode.choices.push_back(skillsChoice1);
        skillsNode.choices.push_back(skillsChoice2);
        
        DialogueNode hiredNode;
        hiredNode.id = "hired";
        hiredNode.speakerName = "Сара / Sarah";
        hiredNode.text = "Замечательно! Мы заработаем кучу денег!\n\nWonderful! We'll make lots of money!";
        hiredNode.isPlayerSpeaking = false;
        hiredNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[skillsNode.id] = skillsNode;
        tree.nodes[hiredNode.id] = hiredNode;
        
        return tree;
    }
    
    // Dialogue for recruiting Navigator Jack
    DialogueTree createRecruitNavigatorDialogue() {
        DialogueTree tree;
        tree.id = "recruit_navigator_jack";
        tree.title = "Найм штурмана / Recruit Navigator";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Джек / Jack";
        startNode.text = "Эй, приятель! Я Джек, бывший дальнобойщик. 20 лет объездил всю страну.\n"
                        "Знаю каждую дорогу, каждый поворот. Нужен навигатор?\n\n"
                        "Hey, buddy! I'm Jack, former truck driver. 20 years traveled the whole country.\n"
                        "I know every road, every turn. Need a navigator?";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Поехали вместе! / Let's go together!";
        choice1.nextNodeId = "hired";
        
        DialogueChoice choice2;
        choice2.text = "Чем ты полезен? / How are you useful?";
        choice2.nextNodeId = "skills";
        
        DialogueChoice choice3;
        choice3.text = "Справлюсь сам / I'll manage myself";
        choice3.nextNodeId = "";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        startNode.choices.push_back(choice3);
        
        DialogueNode skillsNode;
        skillsNode.id = "skills";
        skillsNode.speakerName = "Джек / Jack";
        skillsNode.text = "С моей помощью:\n- Сократишь расстояние на 15%\n- Избежишь пробок и плохих дорог\n"
                         "- Найдёшь лучшие места для отдыха\n\nПоехали?\n\n"
                         "With my help:\n- Reduce distance by 15%\n- Avoid traffic and bad roads\n"
                         "- Find best places to rest\n\nLet's go?";
        skillsNode.isPlayerSpeaking = false;
        
        DialogueChoice skillsChoice1;
        skillsChoice1.text = "Давай, поехали! / Let's go!";
        skillsChoice1.nextNodeId = "hired";
        
        DialogueChoice skillsChoice2;
        skillsChoice2.text = "Спасибо, но нет / Thanks, but no";
        skillsChoice2.nextNodeId = "";
        
        skillsNode.choices.push_back(skillsChoice1);
        skillsNode.choices.push_back(skillsChoice2);
        
        DialogueNode hiredNode;
        hiredNode.id = "hired";
        hiredNode.speakerName = "Джек / Jack";
        hiredNode.text = "Отлично, браток! Вперёд к приключениям!\n\nGreat, buddy! Forward to adventures!";
        hiredNode.isPlayerSpeaking = false;
        hiredNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[skillsNode.id] = skillsNode;
        tree.nodes[hiredNode.id] = hiredNode;
        
        return tree;
    }
    
    // Dialogue for recruiting Medic Emma
    DialogueTree createRecruitMedicDialogue() {
        DialogueTree tree;
        tree.id = "recruit_medic_emma";
        tree.title = "Найм врача / Recruit Medic";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Эмма / Emma";
        startNode.text = "Добрый день. Я Эмма, бывший военный врач. Путешествую и помогаю людям.\n"
                        "В дороге всякое случается. Хотите, чтобы я присоединилась?\n\n"
                        "Good afternoon. I'm Emma, former military doctor. I travel and help people.\n"
                        "Anything can happen on the road. Want me to join?";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Нанять за 600 руб / Hire for 600 rub";
        choice1.nextNodeId = "hired";
        
        DialogueChoice choice2;
        choice2.text = "Какой у тебя опыт? / What's your experience?";
        choice2.nextNodeId = "skills";
        
        DialogueChoice choice3;
        choice3.text = "Мы здоровы / We're healthy";
        choice3.nextNodeId = "";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        startNode.choices.push_back(choice3);
        
        DialogueNode skillsNode;
        skillsNode.id = "skills";
        skillsNode.speakerName = "Эмма / Emma";
        skillsNode.text = "Я могу:\n- Лечить команду в пути\n- Повышать выносливость на 20%\n"
                         "- Предотвращать болезни\n- Быстро восстанавливать силы\n\nВ дороге врач - на вес золота!\n\n"
                         "I can:\n- Heal team on the go\n- Increase stamina by 20%\n"
                         "- Prevent diseases\n- Quickly restore strength\n\nA doctor on the road is worth their weight in gold!";
        skillsNode.isPlayerSpeaking = false;
        
        DialogueChoice skillsChoice1;
        skillsChoice1.text = "Присоединяйся! / Join us!";
        skillsChoice1.nextNodeId = "hired";
        
        DialogueChoice skillsChoice2;
        skillsChoice2.text = "Дорого / Too expensive";
        skillsChoice2.nextNodeId = "";
        
        skillsNode.choices.push_back(skillsChoice1);
        skillsNode.choices.push_back(skillsChoice2);
        
        DialogueNode hiredNode;
        hiredNode.id = "hired";
        hiredNode.speakerName = "Эмма / Emma";
        hiredNode.text = "Рада помочь! Берегите здоровье!\n\nGlad to help! Take care of your health!";
        hiredNode.isPlayerSpeaking = false;
        hiredNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[skillsNode.id] = skillsNode;
        tree.nodes[hiredNode.id] = hiredNode;
        
        return tree;
    }
    
    // Dialogue for recruiting Companion Alex
    DialogueTree createRecruitCompanionAlexDialogue() {
        DialogueTree tree;
        tree.id = "recruit_companion_alex";
        tree.title = "Найм попутчика / Recruit Companion";
        tree.startNodeId = "start";
        
        DialogueNode startNode;
        startNode.id = "start";
        startNode.speakerName = "Алекс / Alex";
        startNode.text = "Привет! Я Алекс. Путешествую автостопом уже два года. Видел всю страну!\n"
                        "Одному скучно ехать? Я весёлый парень, не заскучаем!\n\n"
                        "Hi! I'm Alex. Been hitchhiking for two years. Seen the whole country!\n"
                        "Boring to travel alone? I'm a fun guy, we won't be bored!";
        startNode.isPlayerSpeaking = false;
        
        DialogueChoice choice1;
        choice1.text = "Поехали вместе! / Let's go together!";
        choice1.nextNodeId = "hired";
        
        DialogueChoice choice2;
        choice2.text = "Расскажи о себе / Tell me about yourself";
        choice2.nextNodeId = "skills";
        
        DialogueChoice choice3;
        choice3.text = "Извини, места нет / Sorry, no room";
        choice3.nextNodeId = "";
        
        startNode.choices.push_back(choice1);
        startNode.choices.push_back(choice2);
        startNode.choices.push_back(choice3);
        
        DialogueNode skillsNode;
        skillsNode.id = "skills";
        skillsNode.speakerName = "Алекс / Alex";
        skillsNode.text = "Я хороший компаньон:\n- Поднимаю настроение команды\n- Знаю много интересных историй\n"
                         "- Помогу не уснуть за рулём\n- Увеличиваю мораль на 15%\n\nВ путешествии нужны друзья!\n\n"
                         "I'm a good companion:\n- Lift team spirits\n- Know many interesting stories\n"
                         "- Help stay awake while driving\n- Increase morale by 15%\n\nYou need friends on a journey!";
        skillsNode.isPlayerSpeaking = false;
        
        DialogueChoice skillsChoice1;
        skillsChoice1.text = "Ты прав! Поехали! / You're right! Let's go!";
        skillsChoice1.nextNodeId = "hired";
        
        DialogueChoice skillsChoice2;
        skillsChoice2.text = "Может в другой раз / Maybe another time";
        skillsChoice2.nextNodeId = "";
        
        skillsNode.choices.push_back(skillsChoice1);
        skillsNode.choices.push_back(skillsChoice2);
        
        DialogueNode hiredNode;
        hiredNode.id = "hired";
        hiredNode.speakerName = "Алекс / Alex";
        hiredNode.text = "Ура! Будет весело! В путь!\n\nHooray! It'll be fun! Let's hit the road!";
        hiredNode.isPlayerSpeaking = false;
        hiredNode.isEndNode = true;
        
        tree.nodes[startNode.id] = startNode;
        tree.nodes[skillsNode.id] = skillsNode;
        tree.nodes[hiredNode.id] = hiredNode;
        
        return tree;
    }
}
