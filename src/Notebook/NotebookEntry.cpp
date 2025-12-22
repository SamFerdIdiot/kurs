#include "Notebook/NotebookEntry.h"
#include "PlayerState.h"
#include <iostream>

// === NotebookChoice методы ===

bool NotebookChoice::isAvailable(const PlayerState* player) const {
    if (isDisabled) {
        return false;
    }

    if (!player) {
        return true;  // Если нет игрока, считаем доступным (для тестов)
    }

    // Проверка энергии
    if (energyRequired > 0.0f && player->getEnergy() < energyRequired) {
        return false;
    }

    // Проверка топлива
    if (fuelRequired > 0.0f && player->getFuel() < fuelRequired) {
        return false;
    }

    // Проверка денег
    if (moneyRequired > 0.0f && player->getMoney() < moneyRequired) {
        return false;
    }

    return true;
}

// === NotebookEntry методы ===

NotebookEntry::NotebookEntry()
    : id(""),
      type(EntryType::PRESENT),
      text(""),
      printSpeed(50.0f),
      canSkip(true),
      branchId(""),
      date(""),
      location(""),
      mood("") {
}

NotebookEntry::NotebookEntry(const std::string& id,
                             EntryType type,
                             const std::string& text)
    : id(id),
      type(type),
      text(text),
      printSpeed(50.0f),
      canSkip(true),
      branchId(""),
      date(""),
      location(""),
      mood("") {
}

void NotebookEntry::addChoice(const NotebookChoice& choice) {
    choices.push_back(choice);
}

std::vector<NotebookChoice> NotebookEntry::getAvailableChoices(const PlayerState* player) const {
    std::vector<NotebookChoice> available;

    for (const auto& choice : choices) {
        if (choice.isAvailable(player)) {
            available.push_back(choice);
        }
    }

    return available;
}

bool NotebookEntry::hasChoices() const {
    return !choices.empty();
}
