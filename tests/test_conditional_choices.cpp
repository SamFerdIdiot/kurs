#include <gtest/gtest.h>
#include <iostream>
#include "../include/Notebook/NotebookEntry.h"
#include "../include/PlayerState.h"

// ====================================================================
// ТЕСТ: Условные выборы в зависимости от принципов
// ====================================================================
TEST(ConditionalChoices, PrincipleRequirements) {
    std::cout << "\n=== TEST: Conditional Choices - Principle Requirements ===" << std::endl;

    PlayerState player;

    // Создаём выбор с требованием принципа
    NotebookChoice choiceWithPrinciple;
    choiceWithPrinciple.text = "Использовать системный подход";
    choiceWithPrinciple.requiredPrinciples = {"understanding_context"};
    choiceWithPrinciple.isHidden = true;

    // Без принципа - выбор недоступен
    EXPECT_FALSE(choiceWithPrinciple.isAvailable(&player));
    std::cout << "  ✓ Без принципа 'understanding_context': выбор скрыт" << std::endl;

    // Добавляем принцип
    player.addPrinciple("understanding_context");

    // С принципом - выбор доступен
    EXPECT_TRUE(choiceWithPrinciple.isAvailable(&player));
    std::cout << "  ✓ С принципом 'understanding_context': выбор доступен" << std::endl;
}

// ====================================================================
// ТЕСТ: Условные выборы с артефактами
// ====================================================================
TEST(ConditionalChoices, StoryItemRequirements) {
    std::cout << "\n=== TEST: Conditional Choices - Story Item Requirements ===" << std::endl;

    PlayerState player;

    // Создаём выбор с требованием артефакта
    NotebookChoice choiceWithItem;
    choiceWithItem.text = "Использовать сожжённый палец (опыт)";
    choiceWithItem.requiredPrinciples = {"listen_material"};
    choiceWithItem.requiredStoryItems = {"burned_finger"};
    choiceWithItem.isHidden = true;

    // Без артефакта - недоступен
    EXPECT_FALSE(choiceWithItem.isAvailable(&player));
    std::cout << "  ✓ Без артефакта: выбор скрыт" << std::endl;

    // С принципом, но без артефакта - всё ещё недоступен
    player.addPrinciple("listen_material");
    EXPECT_FALSE(choiceWithItem.isAvailable(&player));
    std::cout << "  ✓ С принципом, но без артефакта: выбор скрыт" << std::endl;

    // С принципом И артефактом - доступен
    player.addStoryItem("burned_finger");
    EXPECT_TRUE(choiceWithItem.isAvailable(&player));
    std::cout << "  ✓ С принципом И артефактом: выбор доступен" << std::endl;
}

// ====================================================================
// ТЕСТ: Условные выборы с чертами характера
// ====================================================================
TEST(ConditionalChoices, TraitRequirements) {
    std::cout << "\n=== TEST: Conditional Choices - Trait Requirements ===" << std::endl;

    PlayerState player;

    // Создаём выбор с требованием черты
    NotebookChoice choiceWithTrait;
    choiceWithTrait.text = "Аналитический подход";
    choiceWithTrait.requiredTraits = {"analytical"};
    choiceWithTrait.isHidden = true;

    // Без черты - недоступен
    EXPECT_FALSE(choiceWithTrait.isAvailable(&player));
    std::cout << "  ✓ Без черты 'analytical': выбор скрыт" << std::endl;

    // Добавляем черту
    player.addTrait("analytical");

    // С чертой - доступен
    EXPECT_TRUE(choiceWithTrait.isAvailable(&player));
    std::cout << "  ✓ С чертой 'analytical': выбор доступен" << std::endl;
}

// ====================================================================
// ТЕСТ: Множественные требования
// ====================================================================
TEST(ConditionalChoices, MultipleRequirements) {
    std::cout << "\n=== TEST: Conditional Choices - Multiple Requirements ===" << std::endl;

    PlayerState player;

    // Создаём выбор с несколькими требованиями
    NotebookChoice complexChoice;
    complexChoice.text = "Комбинированный подход";
    complexChoice.requiredPrinciples = {"understanding_context", "listen_material"};
    complexChoice.requiredTraits = {"analytical"};
    complexChoice.requiredStoryItems = {"technical_journal"};
    complexChoice.isHidden = true;

    // Без требований - недоступен
    EXPECT_FALSE(complexChoice.isAvailable(&player));
    std::cout << "  ✓ Без требований: выбор скрыт" << std::endl;

    // Добавляем первый принцип - всё ещё недоступен
    player.addPrinciple("understanding_context");
    EXPECT_FALSE(complexChoice.isAvailable(&player));
    std::cout << "  ✓ С 1 принципом из 2: выбор скрыт" << std::endl;

    // Добавляем второй принцип - всё ещё недоступен (нужны черта и артефакт)
    player.addPrinciple("listen_material");
    EXPECT_FALSE(complexChoice.isAvailable(&player));
    std::cout << "  ✓ С 2 принципами: выбор скрыт (нужны черта и артефакт)" << std::endl;

    // Добавляем черту - всё ещё недоступен (нужен артефакт)
    player.addTrait("analytical");
    EXPECT_FALSE(complexChoice.isAvailable(&player));
    std::cout << "  ✓ С принципами и чертой: выбор скрыт (нужен артефакт)" << std::endl;

    // Добавляем артефакт - ТЕПЕРЬ доступен!
    player.addStoryItem("technical_journal");
    EXPECT_TRUE(complexChoice.isAvailable(&player));
    std::cout << "  ✓ Со ВСЕМИ требованиями: выбор доступен ✅" << std::endl;
}

// ====================================================================
// ТЕСТ: Проверка getAvailableChoices
// ====================================================================
TEST(ConditionalChoices, GetAvailableChoices) {
    std::cout << "\n=== TEST: NotebookEntry::getAvailableChoices ===" << std::endl;

    PlayerState player;
    NotebookEntry entry("test_entry", EntryType::PRESENT, "Тестовая запись");

    // Базовый выбор (всегда доступен)
    NotebookChoice baseChoice;
    baseChoice.text = "Базовый выбор";
    entry.addChoice(baseChoice);

    // Условный выбор 1 (требует принцип)
    NotebookChoice conditionalChoice1;
    conditionalChoice1.text = "Условный выбор 1";
    conditionalChoice1.requiredPrinciples = {"understanding_context"};
    conditionalChoice1.isHidden = true;
    entry.addChoice(conditionalChoice1);

    // Условный выбор 2 (требует другой принцип)
    NotebookChoice conditionalChoice2;
    conditionalChoice2.text = "Условный выбор 2";
    conditionalChoice2.requiredPrinciples = {"listen_material"};
    conditionalChoice2.isHidden = true;
    entry.addChoice(conditionalChoice2);

    // Без принципов - только базовый выбор
    auto available = entry.getAvailableChoices(&player);
    EXPECT_EQ(available.size(), 1);
    std::cout << "  ✓ Без принципов: доступен 1 выбор (базовый)" << std::endl;

    // С первым принципом - 2 выбора
    player.addPrinciple("understanding_context");
    available = entry.getAvailableChoices(&player);
    EXPECT_EQ(available.size(), 2);
    std::cout << "  ✓ С принципом 'understanding_context': доступно 2 выбора" << std::endl;

    // С обоими принципами - все 3 выбора
    player.addPrinciple("listen_material");
    available = entry.getAvailableChoices(&player);
    EXPECT_EQ(available.size(), 3);
    std::cout << "  ✓ С обоими принципами: доступно 3 выбора" << std::endl;

    std::cout << "\n✅ Все тесты условных выборов пройдены!" << std::endl;
}
