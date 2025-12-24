#include "Notebook/NotebookEntry.h"
#include "PlayerState.h"
#include <iostream>



bool NotebookChoice::isAvailable(const PlayerState* player) const {
    if (isDisabled) {
        return false;
    }

    if (!player) {
        bool hasConditionalRequirements = !requiredPrinciples.empty() ||
                                          !requiredTraits.empty() ||
                                          !requiredStoryItems.empty();
        return !(isHidden || hasConditionalRequirements);
    }

    bool hasConditionalRequirements = !requiredPrinciples.empty() ||
                                      !requiredTraits.empty() ||
                                      !requiredStoryItems.empty();

    if (isHidden || hasConditionalRequirements) {
        for (const auto& principle : requiredPrinciples) {
            if (!player->hasPrinciple(principle)) {
                return false;
            }
        }

        for (const auto& trait : requiredTraits) {
            if (!player->hasTrait(trait)) {
                return false;
            }
        }

        for (const auto& item : requiredStoryItems) {
            if (!player->hasStoryItem(item)) {
                return false;
            }
        }
    }

    if (energyRequired > 0.0f && player->getEnergy() < energyRequired) {
        return false;
    }

    if (fuelRequired > 0.0f && player->getFuel() < fuelRequired) {
        return false;
    }

    if (moneyRequired > 0.0f && player->getMoney() < moneyRequired) {
        return false;
    }

    return true;
}



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
