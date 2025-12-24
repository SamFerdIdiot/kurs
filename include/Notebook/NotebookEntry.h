#ifndef NOTEBOOK_ENTRY_H
#define NOTEBOOK_ENTRY_H

#include <string>
#include <vector>
#include <functional>


class PlayerState;




enum class EntryType {
    PRESENT,
    PAST,
    THOUGHT
};




struct NotebookChoice {
    std::string text;
    std::function<void(PlayerState*)> action;
    std::vector<std::string> nextEntryIds;
    float probability = 1.0f;


    float energyRequired = 0.0f;
    float fuelRequired = 0.0f;
    float moneyRequired = 0.0f;
    bool isDisabled = false;
    std::string disabledReason;


    std::vector<std::string> requiredPrinciples;
    std::vector<std::string> requiredTraits;
    std::vector<std::string> requiredStoryItems;
    bool isHidden = false;




    bool isAvailable(const PlayerState* player) const;
};




struct NotebookEntry {
    std::string id;
    EntryType type;
    std::string text;
    std::vector<NotebookChoice> choices;


    float printSpeed = 50.0f;
    bool canSkip = true;


    std::string branchId;


    std::string date;
    std::string location;
    std::string mood;




    NotebookEntry();




    NotebookEntry(const std::string& id,
                  EntryType type,
                  const std::string& text);




    void addChoice(const NotebookChoice& choice);




    std::vector<NotebookChoice> getAvailableChoices(const PlayerState* player) const;




    bool hasChoices() const;
};

#endif
