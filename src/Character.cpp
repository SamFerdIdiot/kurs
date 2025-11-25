#include "Character.h"
#include <cstdlib>
#include <ctime>

// Constructor
Character::Character(Backstory backstory, const std::string& name)
    : m_name(name), m_backstory(backstory) {
    initializeBackstory();
}

// Set backstory
void Character::setBackstory(Backstory backstory) {
    m_backstory = backstory;
    initializeBackstory();
}

// Get backstory name in Russian
std::string Character::getBackstoryName() const {
    switch (m_backstory) {
        case Backstory::EX_RACER:
            return "Ex-Racer";
        case Backstory::STUDENT:
            return "Student";
        case Backstory::UNEMPLOYED:
            return "Unemployed";
        default:
            return "Unknown";
    }
}

// Get backstory description
std::string Character::getBackstoryDescription() const {
    switch (m_backstory) {
        case Backstory::EX_RACER:
            return "Used to race the streets, now going wherever your eyes lead.\n"
                   "Driving experience helps you avoid trouble.";
        case Backstory::STUDENT:
            return "A dropout from the provinces. People are drawn to you,\n"
                   "your companions feel better on the road.";
        case Backstory::UNEMPLOYED:
            return "No job, no purpose. Life hits harder than usual,\n"
                   "but you have a chance to find your way to the end.";
        default:
            return "";
    }
}

// Get backstory bonus description
std::string Character::getBackstoryBonus() const {
    switch (m_backstory) {
        case Backstory::EX_RACER:
            return "Bonus: 16% chance to skip random events";
        case Backstory::STUDENT:
            return "Bonus: Companions get joy effect (+10% morale)";
        case Backstory::UNEMPLOYED:
            return "Feature: Hard mode, depression, 2 unique endings";
        default:
            return "";
    }
}

// Ex-Racer ability: 16% chance to skip events
bool Character::canSkipEvent() const {
    if (m_backstory != Backstory::EX_RACER) {
        return false;
    }
    
    // 16% chance
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }
    
    return (rand() % 100) < 16;
}

// Student ability: Companions get joy effect
float Character::getCompanionMoraleBonus() const {
    if (m_backstory == Backstory::STUDENT) {
        return 0.10f; // +10% morale bonus
    }
    return 0.0f;
}

// Unemployed: Hard mode
bool Character::isHardMode() const {
    return m_backstory == Backstory::UNEMPLOYED;
}

// Unemployed: Depression multiplier (makes things harder)
float Character::getDepressionMultiplier() const {
    if (m_backstory == Backstory::UNEMPLOYED) {
        return 1.25f; // 25% harder (more fuel consumption, higher costs, etc.)
    }
    return 1.0f;
}

// Unemployed: Has unique endings
bool Character::hasUniqueEndings() const {
    return m_backstory == Backstory::UNEMPLOYED;
}

// Initialize backstory properties
void Character::initializeBackstory() {
    // Any additional initialization based on backstory can go here
    // For now, all logic is in the getter methods
}
