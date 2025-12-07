#include <iostream>
#include <cassert>
#include "../include/EventFactory.h"
#include "../include/NPC.h"
#include "../include/EventManager.h"

// Простой тест системы компаньонов и событий
int main() {
    std::cout << "=================================================\n";
    std::cout << "ТЕСТ СИСТЕМЫ КОМПАНЬОНОВ И СОБЫТИЙ\n";
    std::cout << "=================================================\n\n";

    // ====================================================================
    // ТЕСТ 1: Создание русских NPC
    // ====================================================================
    std::cout << "[ТЕСТ 1] Создание русских NPC...\n";

    auto mechanic = NPCFactory::createRussianMechanic();
    auto unemployed = NPCFactory::createUnemployed();
    auto punk = NPCFactory::createPunk();
    auto granny = NPCFactory::createGranny();
    auto trucker = NPCFactory::createTrucker();
    auto student = NPCFactory::createStudent();

    assert(mechanic != nullptr);
    assert(mechanic->getName() == "Механик Михалыч");
    assert(mechanic->getLevel() == 1);
    assert(mechanic->getRelationship() == 0);
    assert(!mechanic->isInParty());
    assert(!mechanic->hasMetBefore());

    std::cout << "  ✓ Механик Михалыч создан\n";
    std::cout << "    - ID: " << mechanic->getId() << "\n";
    std::cout << "    - Уровень: " << mechanic->getLevel() << "\n";
    std::cout << "    - Отношения: " << mechanic->getRelationship() << "\n";

    if (mechanic->hasPassiveAbility()) {
        std::cout << "    - Пассивная способность: " << mechanic->getPassiveAbility().name << "\n";
    }
    if (mechanic->hasActiveAbility()) {
        std::cout << "    - Активная способность: " << mechanic->getActiveAbility().name << "\n";
    }

    assert(unemployed->getName() == "Безработный Виталий");
    assert(punk->getName() == "Панк Вася");
    assert(granny->getName() == "Бабушка Галина");
    assert(trucker->getName() == "Дальнобойщик Петрович");
    assert(student->getName() == "Студент Лёха");

    std::cout << "  ✓ Все 6 русских NPC созданы успешно\n\n";

    // ====================================================================
    // ТЕСТ 2: NPCManager
    // ====================================================================
    std::cout << "[ТЕСТ 2] Тестирование NPCManager...\n";

    NPCManager& npcManager = NPCManager::getInstance();
    npcManager.clear(); // Очистить перед тестом

    assert(npcManager.getTeamSize() == 0);
    assert(npcManager.hasSpace());

    bool added = npcManager.addNPC(std::move(mechanic));
    assert(added);
    assert(npcManager.getTeamSize() == 1);

    NPC* foundNPC = npcManager.getNPC("mechanic_mikhalych");
    assert(foundNPC != nullptr);
    assert(foundNPC->getName() == "Механик Михалыч");

    std::cout << "  ✓ NPCManager работает корректно\n";
    std::cout << "    - Размер команды: " << npcManager.getTeamSize() << "/" << npcManager.getMaxTeamSize() << "\n\n";

    // ====================================================================
    // ТЕСТ 3: Система отношений
    // ====================================================================
    std::cout << "[ТЕСТ 3] Тестирование системы отношений...\n";

    foundNPC->setRelationship(50);
    assert(foundNPC->getRelationship() == 50);

    foundNPC->modifyRelationship(30);
    assert(foundNPC->getRelationship() == 80);

    foundNPC->modifyRelationship(-200);
    assert(foundNPC->getRelationship() == -100); // Минимум -100

    foundNPC->setRelationship(150);
    assert(foundNPC->getRelationship() == 100); // Максимум +100

    foundNPC->setInParty(true);
    foundNPC->setMetBefore(true);
    assert(foundNPC->isInParty());
    assert(foundNPC->hasMetBefore());

    std::cout << "  ✓ Система отношений работает корректно\n";
    std::cout << "    - Отношения ограничены диапазоном [-100, +100]\n\n";

    // ====================================================================
    // ТЕСТ 4: EventFactory
    // ====================================================================
    std::cout << "[ТЕСТ 4] Создание событий через EventFactory...\n";

    EventManager eventManager;
    EventFactory::initializeAllEvents(eventManager);

    const auto& allEvents = eventManager.getAllEvents();
    std::cout << "  ✓ Создано событий: " << allEvents.size() << "\n";

    // Проверим типы событий
    int encounterCount = 0;
    int companionCount = 0;
    int roadCount = 0;
    int resourceCount = 0;

    for (const auto& event : allEvents) {
        switch (event.type) {
            case EventType::ENCOUNTER: encounterCount++; break;
            case EventType::COMPANION: companionCount++; break;
            case EventType::ROAD: roadCount++; break;
            case EventType::RESOURCE: resourceCount++; break;
            default: break;
        }
    }

    std::cout << "    - Встречи с NPC (ENCOUNTER): " << encounterCount << "\n";
    std::cout << "    - События с компаньонами (COMPANION): " << companionCount << "\n";
    std::cout << "    - Дорожные события (ROAD): " << roadCount << "\n";
    std::cout << "    - Ресурсные события (RESOURCE): " << resourceCount << "\n";

    assert(encounterCount == 6); // 6 встреч с NPC
    assert(companionCount == 4); // 4 события с компаньонами
    assert(roadCount == 4);      // 4 дорожных события
    assert(resourceCount == 2);  // 2 ресурсных события

    std::cout << "  ✓ Все события созданы корректно\n\n";

    // ====================================================================
    // ТЕСТ 5: Проверка конкретного события
    // ====================================================================
    std::cout << "[ТЕСТ 5] Проверка события 'Встреча с Механиком'...\n";

    GameEvent* meetMechanicEvent = eventManager.getEvent("meet_mechanic");
    assert(meetMechanicEvent != nullptr);
    assert(meetMechanicEvent->title == "Сломанная машина на обочине");
    assert(meetMechanicEvent->type == EventType::ENCOUNTER);
    assert(meetMechanicEvent->oneTimeOnly == true);
    assert(meetMechanicEvent->associatedNPC == "mechanic_mikhalych");
    assert(meetMechanicEvent->choices.size() == 3); // 3 варианта выбора

    std::cout << "  ✓ Событие найдено\n";
    std::cout << "    - Название: " << meetMechanicEvent->title << "\n";
    std::cout << "    - Описание: " << meetMechanicEvent->description.substr(0, 60) << "...\n";
    std::cout << "    - Вариантов выбора: " << meetMechanicEvent->choices.size() << "\n";

    // Проверим первый выбор (помочь и пригласить)
    const EventChoice& firstChoice = meetMechanicEvent->choices[0];
    assert(firstChoice.recruitNPC == "mechanic_mikhalych");
    assert(firstChoice.relationshipChanges.count("mechanic_mikhalych") > 0);
    assert(firstChoice.relationshipChanges.at("mechanic_mikhalych") == 20);

    std::cout << "    - Первый выбор: " << firstChoice.text << "\n";
    std::cout << "    - Результат: рекрутинг '" << firstChoice.recruitNPC << "'\n";
    std::cout << "    - Изменение отношений: +" << firstChoice.relationshipChanges.at("mechanic_mikhalych") << "\n\n";

    // ====================================================================
    // ТЕСТ 6: EventManager::checkCondition
    // ====================================================================
    std::cout << "[ТЕСТ 6] Тестирование проверки условий EventManager...\n";

    npcManager.clear();

    // Событие требует максимум 2 NPC в команде (есть место)
    bool conditionMet = eventManager.checkCondition(
        meetMechanicEvent->condition,
        50.0f,  // fuel
        70.0f,  // energy
        1000    // money
    );
    assert(conditionMet); // Должно пройти, т.к. команда пустая (0 < 3)

    std::cout << "  ✓ Условие проверено (команда пустая): " << (conditionMet ? "PASS" : "FAIL") << "\n";

    // Добавим 3 NPC - места не должно быть
    npcManager.addNPC(NPCFactory::createRussianMechanic("test1"));
    npcManager.addNPC(NPCFactory::createUnemployed("test2"));
    npcManager.addNPC(NPCFactory::createPunk("test3"));

    conditionMet = eventManager.checkCondition(
        meetMechanicEvent->condition,
        50.0f, 70.0f, 1000
    );
    assert(!conditionMet); // Не должно пройти, команда полная (3 = max)

    std::cout << "  ✓ Условие проверено (команда полная): " << (!conditionMet ? "PASS" : "FAIL") << "\n\n";

    // ====================================================================
    // ТЕСТ 7: applyChoice
    // ====================================================================
    std::cout << "[ТЕСТ 7] Применение эффектов выбора...\n";

    npcManager.clear();
    npcManager.addNPC(NPCFactory::createRussianMechanic());

    float fuel = 50.0f;
    float energy = 70.0f;
    int money = 1000;

    // Применим первый выбор (рекрутинг механика)
    eventManager.applyChoice(firstChoice, fuel, energy, money);

    assert(energy == 65.0f); // -5 энергии по событию

    NPC* recruitedMechanic = npcManager.getNPC("mechanic_mikhalych");
    assert(recruitedMechanic != nullptr);
    assert(recruitedMechanic->isInParty());
    assert(recruitedMechanic->hasMetBefore());
    assert(recruitedMechanic->getRelationship() == 20);

    std::cout << "  ✓ Эффекты применены\n";
    std::cout << "    - Энергия изменена: 70 -> " << energy << "\n";
    std::cout << "    - NPC рекрутирован: " << recruitedMechanic->getName() << "\n";
    std::cout << "    - Отношения: " << recruitedMechanic->getRelationship() << "\n\n";

    // ====================================================================
    // ИТОГИ
    // ====================================================================
    std::cout << "=================================================\n";
    std::cout << "ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО! ✓\n";
    std::cout << "=================================================\n";

    return 0;
}
