/**
 * @file test_day0_complete.cpp
 * @brief Полное тестирование Дня 0: Уроки дороги
 *
 * Тестирует:
 * 1. Систему принципов (Principles)
 * 2. Систему черт (Traits)
 * 3. Систему артефактов (Story Items)
 * 4. Условные (скрытые) выборы
 * 5. Все 4 ветки обучения (А, Б, В, Г)
 * 6. Корректность ветвления сюжета
 */

#include <gtest/gtest.h>
#include "PlayerState.h"
#include "Notebook/NotebookEntry.h"
#include <iostream>

// ====================================================================
// ТЕСТ 1: Система принципов (Principles)
// ====================================================================
TEST(Day0System, PrinciplesSystem) {
    std::cout << "\n=== TEST: Principles System ===" << std::endl;

    PlayerState player;

    // Добавить принципы
    player.addPrinciple("understanding_context");
    player.addPrinciple("listen_material");
    player.addPrinciple("ask_experienced");
    player.addPrinciple("resource_efficient");

    // Проверить наличие
    EXPECT_TRUE(player.hasPrinciple("understanding_context"));
    EXPECT_TRUE(player.hasPrinciple("listen_material"));
    EXPECT_TRUE(player.hasPrinciple("ask_experienced"));
    EXPECT_TRUE(player.hasPrinciple("resource_efficient"));
    EXPECT_FALSE(player.hasPrinciple("nonexistent"));

    // Проверить количество
    EXPECT_EQ(player.getPrinciples().size(), 4);

    // Проверить что дублирование не добавляет лишние
    player.addPrinciple("understanding_context");
    EXPECT_EQ(player.getPrinciples().size(), 4);  // Должно остаться 4

    std::cout << "✅ Principles system works correctly" << std::endl;
}

// ====================================================================
// ТЕСТ 2: Система черт (Traits)
// ====================================================================
TEST(Day0System, TraitsSystem) {
    std::cout << "\n=== TEST: Traits System ===" << std::endl;

    PlayerState player;

    // Добавить черты
    player.addTrait("analytical");    // Из ветки А
    player.addTrait("intuitive");     // Из ветки Б
    player.addTrait("social");        // Из ветки В
    player.addTrait("practical");     // Из ветки Г
    player.addTrait("impulsive");     // Из провокации
    player.addTrait("patient");       // Из провокации

    // Проверить наличие
    EXPECT_TRUE(player.hasTrait("analytical"));
    EXPECT_TRUE(player.hasTrait("intuitive"));
    EXPECT_TRUE(player.hasTrait("social"));
    EXPECT_TRUE(player.hasTrait("practical"));
    EXPECT_TRUE(player.hasTrait("impulsive"));
    EXPECT_TRUE(player.hasTrait("patient"));
    EXPECT_FALSE(player.hasTrait("nonexistent"));

    // Проверить количество
    EXPECT_EQ(player.getTraits().size(), 6);

    std::cout << "✅ Traits system works correctly" << std::endl;
}

// ====================================================================
// ТЕСТ 3: Система артефактов (Story Items)
// ====================================================================
TEST(Day0System, StoryItemsSystem) {
    std::cout << "\n=== TEST: Story Items System ===" << std::endl;

    PlayerState player;

    // Добавить артефакты
    player.addStoryItem("technical_journal");  // Ветка А
    player.addStoryItem("burned_finger");      // Ветка Б
    player.addStoryItem("photo_at_machine");   // Ветка В
    player.addStoryItem("spare_starter");      // Ветка Г

    // Проверить наличие
    EXPECT_TRUE(player.hasStoryItem("technical_journal"));
    EXPECT_TRUE(player.hasStoryItem("burned_finger"));
    EXPECT_TRUE(player.hasStoryItem("photo_at_machine"));
    EXPECT_TRUE(player.hasStoryItem("spare_starter"));
    EXPECT_FALSE(player.hasStoryItem("nonexistent"));

    // Проверить количество
    EXPECT_EQ(player.getStoryItems().size(), 4);

    std::cout << "✅ Story items system works correctly" << std::endl;
}

// ====================================================================
// ТЕСТ 4: Условные (скрытые) выборы
// ====================================================================
TEST(Day0System, ConditionalChoices) {
    std::cout << "\n=== TEST: Conditional (Hidden) Choices ===" << std::endl;

    PlayerState player;

    // === Сценарий 1: Выбор без условий (всегда доступен) ===
    NotebookChoice basicChoice;
    basicChoice.text = "Basic choice";
    basicChoice.nextEntryIds = {"next"};

    EXPECT_TRUE(basicChoice.isAvailable(&player));
    std::cout << "✅ Basic choice is available" << std::endl;

    // === Сценарий 2: Скрытый выбор требует принцип ===
    NotebookChoice principleChoice;
    principleChoice.text = "[PRINCIPLE] Special choice";
    principleChoice.requiredPrinciples = {"understanding_context"};

    EXPECT_FALSE(principleChoice.isAvailable(&player));  // Нет принципа
    std::cout << "✅ Hidden choice is hidden without principle" << std::endl;

    player.addPrinciple("understanding_context");
    EXPECT_TRUE(principleChoice.isAvailable(&player));  // Теперь доступен
    std::cout << "✅ Hidden choice appears after acquiring principle" << std::endl;

    // === Сценарий 3: Скрытый выбор требует черту ===
    NotebookChoice traitChoice;
    traitChoice.text = "[TRAIT] Analytical choice";
    traitChoice.requiredTraits = {"analytical"};

    EXPECT_FALSE(traitChoice.isAvailable(&player));  // Нет черты
    player.addTrait("analytical");
    EXPECT_TRUE(traitChoice.isAvailable(&player));  // Теперь доступен
    std::cout << "✅ Hidden choice appears after acquiring trait" << std::endl;

    // === Сценарий 4: Скрытый выбор требует артефакт ===
    NotebookChoice itemChoice;
    itemChoice.text = "[ITEM] Use journal";
    itemChoice.requiredStoryItems = {"technical_journal"};

    EXPECT_FALSE(itemChoice.isAvailable(&player));  // Нет артефакта
    player.addStoryItem("technical_journal");
    EXPECT_TRUE(itemChoice.isAvailable(&player));  // Теперь доступен
    std::cout << "✅ Hidden choice appears after acquiring item" << std::endl;

    // === Сценарий 5: Выбор требует несколько условий ===
    NotebookChoice complexChoice;
    complexChoice.text = "[COMPLEX] Multiple requirements";
    complexChoice.requiredPrinciples = {"listen_material"};
    complexChoice.requiredTraits = {"intuitive"};
    complexChoice.requiredStoryItems = {"burned_finger"};

    EXPECT_FALSE(complexChoice.isAvailable(&player));  // Не все условия

    player.addPrinciple("listen_material");
    EXPECT_FALSE(complexChoice.isAvailable(&player));  // Еще не все

    player.addTrait("intuitive");
    EXPECT_FALSE(complexChoice.isAvailable(&player));  // Еще не все

    player.addStoryItem("burned_finger");
    EXPECT_TRUE(complexChoice.isAvailable(&player));  // Все условия выполнены!
    std::cout << "✅ Complex choice appears after all conditions met" << std::endl;
}

// ====================================================================
// ТЕСТ 5: Ветка А - Системный анализ (Understanding Context)
// ====================================================================
TEST(Day0Branches, BranchA_UnderstandingContext) {
    std::cout << "\n=== TEST: Branch A - Understanding Context ===" << std::endl;

    PlayerState player;

    // Симуляция выбора ветки А
    player.addTrait("analytical");  // Из первого выбора

    // Симуляция выбора "Взять журнал"
    player.addStoryItem("technical_journal");
    player.addPrinciple("understanding_context");

    // Проверить результат
    EXPECT_TRUE(player.hasTrait("analytical"));
    EXPECT_TRUE(player.hasStoryItem("technical_journal"));
    EXPECT_TRUE(player.hasPrinciple("understanding_context"));

    std::cout << "✅ Branch A completed successfully" << std::endl;
    std::cout << "   - Trait: analytical" << std::endl;
    std::cout << "   - Item: technical_journal" << std::endl;
    std::cout << "   - Principle: understanding_context" << std::endl;
}

// ====================================================================
// ТЕСТ 6: Ветка Б - Слушать материал (Listen to Material)
// ====================================================================
TEST(Day0Branches, BranchB_ListenToMaterial) {
    std::cout << "\n=== TEST: Branch B - Listen to Material ===" << std::endl;

    PlayerState player;

    // Симуляция выбора ветки Б
    player.addTrait("intuitive");  // Из первого выбора

    // Симуляция выбора "Прикоснуться к металлу"
    player.addStoryItem("burned_finger");
    player.modifyEnergy(-5.0f);  // Урон от ожога
    player.addPrinciple("listen_material");

    // Проверить результат
    EXPECT_TRUE(player.hasTrait("intuitive"));
    EXPECT_TRUE(player.hasStoryItem("burned_finger"));
    EXPECT_TRUE(player.hasPrinciple("listen_material"));
    EXPECT_EQ(player.getEnergy(), 95.0f);  // 100 - 5

    std::cout << "✅ Branch B completed successfully" << std::endl;
    std::cout << "   - Trait: intuitive" << std::endl;
    std::cout << "   - Item: burned_finger" << std::endl;
    std::cout << "   - Principle: listen_material" << std::endl;
    std::cout << "   - Energy: " << player.getEnergy() << " (penalty from burn)" << std::endl;
}

// ====================================================================
// ТЕСТ 7: Ветка В - Спроси бывалого (Ask Experienced)
// ====================================================================
TEST(Day0Branches, BranchC_AskExperienced) {
    std::cout << "\n=== TEST: Branch C - Ask Experienced ===" << std::endl;

    PlayerState player;

    // Симуляция выбора ветки В
    player.addTrait("social");  // Из первого выбора

    // Симуляция выбора "Взять фотографию"
    player.addStoryItem("photo_at_machine");
    player.addPrinciple("ask_experienced");

    // Проверить результат
    EXPECT_TRUE(player.hasTrait("social"));
    EXPECT_TRUE(player.hasStoryItem("photo_at_machine"));
    EXPECT_TRUE(player.hasPrinciple("ask_experienced"));

    std::cout << "✅ Branch C completed successfully" << std::endl;
    std::cout << "   - Trait: social" << std::endl;
    std::cout << "   - Item: photo_at_machine" << std::endl;
    std::cout << "   - Principle: ask_experienced" << std::endl;
}

// ====================================================================
// ТЕСТ 8: Ветка Г - Ресурсоэффективность (Resource Efficient)
// ====================================================================
TEST(Day0Branches, BranchD_ResourceEfficient) {
    std::cout << "\n=== TEST: Branch D - Resource Efficient ===" << std::endl;

    PlayerState player;

    // Симуляция выбора ветки Г
    player.addTrait("practical");  // Из первого выбора

    // Симуляция выбора "Взять стартер"
    player.addStoryItem("spare_starter");
    player.addPrinciple("resource_efficient");

    // Проверить результат
    EXPECT_TRUE(player.hasTrait("practical"));
    EXPECT_TRUE(player.hasStoryItem("spare_starter"));
    EXPECT_TRUE(player.hasPrinciple("resource_efficient"));

    std::cout << "✅ Branch D completed successfully" << std::endl;
    std::cout << "   - Trait: practical" << std::endl;
    std::cout << "   - Item: spare_starter" << std::endl;
    std::cout << "   - Principle: resource_efficient" << std::endl;
}

// ====================================================================
// ТЕСТ 9: Провокация (Impulsive vs Patient)
// ====================================================================
TEST(Day0Events, Provocation) {
    std::cout << "\n=== TEST: Provocation Event ===" << std::endl;

    // Сценарий 1: Огрызнуться (Impulsive)
    {
        PlayerState player;
        player.addTrait("impulsive");

        EXPECT_TRUE(player.hasTrait("impulsive"));
        EXPECT_FALSE(player.hasTrait("patient"));
        std::cout << "✅ Impulsive choice works" << std::endl;
    }

    // Сценарий 2: Подумать (Patient)
    {
        PlayerState player;
        player.addTrait("patient");

        EXPECT_TRUE(player.hasTrait("patient"));
        EXPECT_FALSE(player.hasTrait("impulsive"));
        std::cout << "✅ Patient choice works" << std::endl;
    }
}

// ====================================================================
// ТЕСТ 10: Полное прохождение (Full Playthrough)
// ====================================================================
TEST(Day0Integration, FullPlaythrough) {
    std::cout << "\n=== TEST: Full Day 0 Playthrough ===" << std::endl;

    PlayerState player;

    // === Этап 1: Первый выбор (Ветка А) ===
    player.addTrait("analytical");
    std::cout << "Step 1: Chose analytical approach (Branch A)" << std::endl;

    // === Этап 2: Карта из ничего ===
    std::cout << "Step 2: Map from nothing" << std::endl;

    // === Этап 3: Геолог (взять журнал) ===
    player.addStoryItem("technical_journal");
    player.addPrinciple("understanding_context");
    std::cout << "Step 3: Geologist - took journal" << std::endl;

    // === Этап 4: Провокация (терпеливый ответ) ===
    player.addTrait("patient");
    std::cout << "Step 4: Provocation - patient response" << std::endl;

    // === Финальная проверка ===
    EXPECT_EQ(player.getTraits().size(), 2);           // analytical + patient
    EXPECT_EQ(player.getPrinciples().size(), 1);       // understanding_context
    EXPECT_EQ(player.getStoryItems().size(), 1);       // technical_journal

    std::cout << "\n=== Final State ===" << std::endl;
    std::cout << "Traits (" << player.getTraits().size() << "): ";
    for (const auto& t : player.getTraits()) {
        std::cout << t << " ";
    }
    std::cout << std::endl;

    std::cout << "Principles (" << player.getPrinciples().size() << "): ";
    for (const auto& p : player.getPrinciples()) {
        std::cout << p << " ";
    }
    std::cout << std::endl;

    std::cout << "Story Items (" << player.getStoryItems().size() << "): ";
    for (const auto& i : player.getStoryItems()) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "\n✅ Full playthrough completed successfully" << std::endl;
}

// ====================================================================
// ТЕСТ 11: Очистка состояния (Clear State)
// ====================================================================
TEST(Day0System, ClearState) {
    std::cout << "\n=== TEST: Clear State ===" << std::endl;

    PlayerState player;

    // Добавить данные
    player.addPrinciple("understanding_context");
    player.addTrait("analytical");
    player.addStoryItem("technical_journal");

    EXPECT_EQ(player.getPrinciples().size(), 1);
    EXPECT_EQ(player.getTraits().size(), 1);
    EXPECT_EQ(player.getStoryItems().size(), 1);

    // Очистить
    player.clearPrinciples();
    player.clearTraits();
    player.clearStoryItems();

    EXPECT_EQ(player.getPrinciples().size(), 0);
    EXPECT_EQ(player.getTraits().size(), 0);
    EXPECT_EQ(player.getStoryItems().size(), 0);

    std::cout << "✅ Clear state works correctly" << std::endl;
}

// ====================================================================
// MAIN - Google Test provides main through gtest_main linkage
// ====================================================================
// Note: No main() function needed here - Google Test's gtest_main provides it
