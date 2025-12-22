#include <gtest/gtest.h>
#include <iostream>
#include "../include/EventFactory.h"
#include "../include/NPC.h"
#include "../include/EventManager.h"

// ====================================================================
// ТЕСТ 1: Создание русских NPC
// ====================================================================
TEST(CompanionSystem, CreateRussianNPCs) {
    std::cout << "\n=== TEST: Creating Russian NPCs ===" << std::endl;

    auto mechanic = NPCFactory::createRussianMechanic();
    auto unemployed = NPCFactory::createUnemployed();
    auto punk = NPCFactory::createPunk();
    auto granny = NPCFactory::createGranny();
    auto trucker = NPCFactory::createTrucker();
    auto student = NPCFactory::createStudent();

    ASSERT_NE(mechanic, nullptr);
    EXPECT_EQ(mechanic->getName(), "Механик Михалыч");
    EXPECT_EQ(mechanic->getLevel(), 1);
    EXPECT_EQ(mechanic->getRelationship(), 0);
    EXPECT_FALSE(mechanic->isInParty());
    EXPECT_FALSE(mechanic->hasMetBefore());

    std::cout << "  ✓ Механик Михалыч создан" << std::endl;
    std::cout << "    - ID: " << mechanic->getId() << std::endl;
    std::cout << "    - Уровень: " << mechanic->getLevel() << std::endl;
    std::cout << "    - Отношения: " << mechanic->getRelationship() << std::endl;

    if (mechanic->hasPassiveAbility()) {
        std::cout << "    - Пассивная способность: " << mechanic->getPassiveAbility().name << std::endl;
    }
    if (mechanic->hasActiveAbility()) {
        std::cout << "    - Активная способность: " << mechanic->getActiveAbility().name << std::endl;
    }

    EXPECT_EQ(unemployed->getName(), "Безработный Виталий");
    EXPECT_EQ(punk->getName(), "Панк Вася");
    EXPECT_EQ(granny->getName(), "Бабушка Галина");
    EXPECT_EQ(trucker->getName(), "Дальнобойщик Петрович");
    EXPECT_EQ(student->getName(), "Студент Лёха");

    std::cout << "  ✓ Все 6 русских NPC созданы успешно" << std::endl;
}

// ====================================================================
// ТЕСТ 2: NPCManager
// ====================================================================
TEST(CompanionSystem, NPCManager) {
    std::cout << "\n=== TEST: NPCManager ===" << std::endl;

    NPCManager& npcManager = NPCManager::getInstance();
    npcManager.clear(); // Очистить перед тестом

    EXPECT_EQ(npcManager.getTeamSize(), 0);
    EXPECT_TRUE(npcManager.hasSpace());

    auto mechanic = NPCFactory::createRussianMechanic();
    bool added = npcManager.addNPC(std::move(mechanic));
    EXPECT_TRUE(added);
    EXPECT_EQ(npcManager.getTeamSize(), 1);

    NPC* foundNPC = npcManager.getNPC("mechanic_mikhalych");
    ASSERT_NE(foundNPC, nullptr);
    EXPECT_EQ(foundNPC->getName(), "Механик Михалыч");

    std::cout << "  ✓ NPCManager работает корректно" << std::endl;
    std::cout << "    - Размер команды: " << npcManager.getTeamSize() << "/" << npcManager.getMaxTeamSize() << std::endl;
}

// ====================================================================
// ТЕСТ 3: Система отношений
// ====================================================================
TEST(CompanionSystem, RelationshipSystem) {
    std::cout << "\n=== TEST: Relationship System ===" << std::endl;

    NPCManager& npcManager = NPCManager::getInstance();
    npcManager.clear();
    npcManager.addNPC(NPCFactory::createRussianMechanic());

    NPC* foundNPC = npcManager.getNPC("mechanic_mikhalych");
    ASSERT_NE(foundNPC, nullptr);

    foundNPC->setRelationship(50);
    EXPECT_EQ(foundNPC->getRelationship(), 50);

    foundNPC->modifyRelationship(30);
    EXPECT_EQ(foundNPC->getRelationship(), 80);

    foundNPC->modifyRelationship(-200);
    EXPECT_EQ(foundNPC->getRelationship(), -100); // Минимум -100

    foundNPC->setRelationship(150);
    EXPECT_EQ(foundNPC->getRelationship(), 100); // Максимум +100

    foundNPC->setInParty(true);
    foundNPC->setMetBefore(true);
    EXPECT_TRUE(foundNPC->isInParty());
    EXPECT_TRUE(foundNPC->hasMetBefore());

    std::cout << "  ✓ Система отношений работает корректно" << std::endl;
    std::cout << "    - Отношения ограничены диапазоном [-100, +100]" << std::endl;
}

// ====================================================================
// ТЕСТ 4: EventFactory
// ====================================================================
TEST(CompanionSystem, EventFactory) {
    std::cout << "\n=== TEST: EventFactory ===" << std::endl;

    EventManager eventManager;
    EventFactory::initializeAllEvents(eventManager);

    const auto& allEvents = eventManager.getAllEvents();
    std::cout << "  ✓ Создано событий: " << allEvents.size() << std::endl;

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

    std::cout << "    - Встречи с NPC (ENCOUNTER): " << encounterCount << std::endl;
    std::cout << "    - События с компаньонами (COMPANION): " << companionCount << std::endl;
    std::cout << "    - Дорожные события (ROAD): " << roadCount << std::endl;
    std::cout << "    - Ресурсные события (RESOURCE): " << resourceCount << std::endl;

    // Обновлённые значения после добавления 26 новых событий + 10 ресурсных:
    // 6 базовых + 1 случайное (random_hitchhiker) = 7 ENCOUNTER
    // 4 базовых = 4 COMPANION
    // 4 базовых + 15 для веток A/B/C + 1 для ветки D (shortcut) + 4 случайных = 24 ROAD
    // 2 базовых + 4 для ветки D + 1 случайное (random_hungry) + 10 новых ресурсных = 17 RESOURCE
    EXPECT_EQ(encounterCount, 7);   // 7 встреч с NPC
    EXPECT_EQ(companionCount, 4);   // 4 события с компаньонами
    EXPECT_EQ(roadCount, 24);       // 24 дорожных события
    EXPECT_EQ(resourceCount, 17);   // 17 ресурсных событий

    std::cout << "  ✓ Все события созданы корректно" << std::endl;
}

// ====================================================================
// ТЕСТ 5: Проверка конкретного события
// ====================================================================
TEST(CompanionSystem, SpecificEvent) {
    std::cout << "\n=== TEST: Specific Event (Meet Mechanic) ===" << std::endl;

    EventManager eventManager;
    EventFactory::initializeAllEvents(eventManager);

    GameEvent* meetMechanicEvent = eventManager.getEvent("meet_mechanic");
    ASSERT_NE(meetMechanicEvent, nullptr);
    EXPECT_EQ(meetMechanicEvent->title, "Сломанная машина на обочине");
    EXPECT_EQ(meetMechanicEvent->type, EventType::ENCOUNTER);
    EXPECT_TRUE(meetMechanicEvent->oneTimeOnly);
    EXPECT_EQ(meetMechanicEvent->associatedNPC, "mechanic_mikhalych");
    EXPECT_EQ(meetMechanicEvent->choices.size(), 3); // 3 варианта выбора

    std::cout << "  ✓ Событие найдено" << std::endl;
    std::cout << "    - Название: " << meetMechanicEvent->title << std::endl;
    std::cout << "    - Описание: " << meetMechanicEvent->description.substr(0, 60) << "..." << std::endl;
    std::cout << "    - Вариантов выбора: " << meetMechanicEvent->choices.size() << std::endl;

    // Проверим первый выбор (помочь и пригласить)
    const EventChoice& firstChoice = meetMechanicEvent->choices[0];
    EXPECT_EQ(firstChoice.recruitNPC, "mechanic_mikhalych");
    EXPECT_GT(firstChoice.relationshipChanges.count("mechanic_mikhalych"), 0);
    EXPECT_EQ(firstChoice.relationshipChanges.at("mechanic_mikhalych"), 20);

    std::cout << "    - Первый выбор: " << firstChoice.text << std::endl;
    std::cout << "    - Результат: рекрутинг '" << firstChoice.recruitNPC << "'" << std::endl;
    std::cout << "    - Изменение отношений: +" << firstChoice.relationshipChanges.at("mechanic_mikhalych") << std::endl;
}

// ====================================================================
// ТЕСТ 6: EventManager::checkCondition
// ====================================================================
TEST(CompanionSystem, EventConditions) {
    std::cout << "\n=== TEST: Event Conditions ===" << std::endl;

    EventManager eventManager;
    EventFactory::initializeAllEvents(eventManager);
    NPCManager& npcManager = NPCManager::getInstance();
    npcManager.clear();

    GameEvent* meetMechanicEvent = eventManager.getEvent("meet_mechanic");
    ASSERT_NE(meetMechanicEvent, nullptr);

    // Событие требует максимум 2 NPC в команде (есть место)
    bool conditionMet = eventManager.checkCondition(
        meetMechanicEvent->condition,
        50.0f,  // fuel
        70.0f,  // energy
        1000    // money
    );
    EXPECT_TRUE(conditionMet); // Должно пройти, т.к. команда пустая (0 < 3)

    std::cout << "  ✓ Условие проверено (команда пустая): " << (conditionMet ? "PASS" : "FAIL") << std::endl;

    // Добавим 3 NPC - места не должно быть
    npcManager.addNPC(NPCFactory::createRussianMechanic("test1"));
    npcManager.addNPC(NPCFactory::createUnemployed("test2"));
    npcManager.addNPC(NPCFactory::createPunk("test3"));

    conditionMet = eventManager.checkCondition(
        meetMechanicEvent->condition,
        50.0f, 70.0f, 1000
    );
    EXPECT_FALSE(conditionMet); // Не должно пройти, команда полная (3 = max)

    std::cout << "  ✓ Условие проверено (команда полная): " << (!conditionMet ? "PASS" : "FAIL") << std::endl;
}

// ====================================================================
// ТЕСТ 7: applyChoice
// ====================================================================
TEST(CompanionSystem, ApplyChoice) {
    std::cout << "\n=== TEST: Apply Choice ===" << std::endl;

    EventManager eventManager;
    EventFactory::initializeAllEvents(eventManager);
    NPCManager& npcManager = NPCManager::getInstance();
    npcManager.clear();
    npcManager.addNPC(NPCFactory::createRussianMechanic());

    GameEvent* meetMechanicEvent = eventManager.getEvent("meet_mechanic");
    ASSERT_NE(meetMechanicEvent, nullptr);
    const EventChoice& firstChoice = meetMechanicEvent->choices[0];

    float fuel = 50.0f;
    float energy = 70.0f;
    int money = 1000;

    // Применим первый выбор (рекрутинг механика)
    eventManager.applyChoice(firstChoice, fuel, energy, money);

    EXPECT_EQ(energy, 65.0f); // -5 энергии по событию

    NPC* recruitedMechanic = npcManager.getNPC("mechanic_mikhalych");
    ASSERT_NE(recruitedMechanic, nullptr);
    EXPECT_TRUE(recruitedMechanic->isInParty());
    EXPECT_TRUE(recruitedMechanic->hasMetBefore());
    EXPECT_EQ(recruitedMechanic->getRelationship(), 20);

    std::cout << "  ✓ Эффекты применены" << std::endl;
    std::cout << "    - Энергия изменена: 70 -> " << energy << std::endl;
    std::cout << "    - NPC рекрутирован: " << recruitedMechanic->getName() << std::endl;
    std::cout << "    - Отношения: " << recruitedMechanic->getRelationship() << std::endl;
}
