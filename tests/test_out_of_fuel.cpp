#include <gtest/gtest.h>
#include <iostream>
#include "../include/EventFactory.h"
#include "../include/EventManager.h"

// ====================================================================
// ТЕСТ: Событие "Закончилось топливо" с случайными исходами
// ====================================================================
TEST(OutOfFuelEvent, RandomOutcomes) {
    std::cout << "\n=== TEST: Out of Fuel Event with Random Outcomes ===" << std::endl;

    EventManager eventManager;
    EventFactory::initializeAllEvents(eventManager);

    GameEvent* outOfFuelEvent = eventManager.getEvent("out_of_fuel");
    ASSERT_NE(outOfFuelEvent, nullptr);

    std::cout << "  ✓ Событие найдено: " << outOfFuelEvent->title << std::endl;
    std::cout << "    Описание: " << outOfFuelEvent->description.substr(0, 80) << "..." << std::endl;

    // Проверяем, что есть 2 выбора
    EXPECT_EQ(outOfFuelEvent->choices.size(), 2);
    std::cout << "  ✓ Количество выборов: " << outOfFuelEvent->choices.size() << std::endl;

    // Проверяем первый выбор (ждать попутку)
    const EventChoice& hitchhike = outOfFuelEvent->choices[0];
    std::cout << "\n  Выбор 1: " << hitchhike.text << std::endl;
    EXPECT_EQ(hitchhike.randomOutcomes.size(), 3);
    std::cout << "    - Количество случайных исходов: " << hitchhike.randomOutcomes.size() << std::endl;

    float totalProb = 0.0f;
    for (size_t i = 0; i < hitchhike.randomOutcomes.size(); i++) {
        const auto& outcome = hitchhike.randomOutcomes[i];
        totalProb += outcome.probability;
        std::cout << "    Исход " << (i+1) << " (вероятность " << (outcome.probability * 100) << "%):" << std::endl;
        std::cout << "      Текст: " << outcome.outcomeText.substr(0, 60) << "..." << std::endl;
        std::cout << "      Топливо: " << (outcome.fuelChange > 0 ? "+" : "") << outcome.fuelChange << std::endl;
        std::cout << "      Энергия: " << (outcome.energyChange > 0 ? "+" : "") << outcome.energyChange << std::endl;
        std::cout << "      Деньги: " << (outcome.moneyChange > 0 ? "+" : "") << outcome.moneyChange << std::endl;
    }
    EXPECT_FLOAT_EQ(totalProb, 1.0f);
    std::cout << "  ✓ Сумма вероятностей: " << totalProb << " (ожидается 1.0)" << std::endl;

    // Проверяем второй выбор (идти пешком)
    const EventChoice& walk = outOfFuelEvent->choices[1];
    std::cout << "\n  Выбор 2: " << walk.text << std::endl;
    EXPECT_EQ(walk.randomOutcomes.size(), 3);
    std::cout << "    - Количество случайных исходов: " << walk.randomOutcomes.size() << std::endl;

    totalProb = 0.0f;
    for (size_t i = 0; i < walk.randomOutcomes.size(); i++) {
        const auto& outcome = walk.randomOutcomes[i];
        totalProb += outcome.probability;
        std::cout << "    Исход " << (i+1) << " (вероятность " << (outcome.probability * 100) << "%):" << std::endl;
        std::cout << "      Текст: " << outcome.outcomeText.substr(0, 60) << "..." << std::endl;
        std::cout << "      Топливо: " << (outcome.fuelChange > 0 ? "+" : "") << outcome.fuelChange << std::endl;
        std::cout << "      Энергия: " << (outcome.energyChange > 0 ? "+" : "") << outcome.energyChange << std::endl;
        std::cout << "      Деньги: " << (outcome.moneyChange > 0 ? "+" : "") << outcome.moneyChange << std::endl;
    }
    EXPECT_FLOAT_EQ(totalProb, 1.0f);
    std::cout << "  ✓ Сумма вероятностей: " << totalProb << " (ожидается 1.0)" << std::endl;

    std::cout << "\n✅ Событие 'Закончилось топливо' работает корректно" << std::endl;
}
