#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

// Character backstory types for Russian 90s theme
enum class Backstory {
    EX_RACER,      // Бывший гонщик - 16% chance to skip event
    STUDENT,       // Студент - Companions have joy effect
    UNEMPLOYED     // Безработный - Hard mode, depression, 2 unique endings
};

// Character class
// Purpose: Manages player character with backstory-specific bonuses
// Russian 90s theme with Keep Driving mechanics
class Character {
public:
    // Constructor
    Character(Backstory backstory = Backstory::STUDENT, const std::string& name = "Путник");
    
    // Get character name
    std::string getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    
    // Get character backstory
    Backstory getBackstory() const { return m_backstory; }
    void setBackstory(Backstory backstory);
    
    // Get backstory name in Russian
    std::string getBackstoryName() const;
    
    // Get backstory description in Russian
    std::string getBackstoryDescription() const;
    
    // Get backstory bonus description
    std::string getBackstoryBonus() const;
    
    // Backstory-specific abilities
    
    // Ex-Racer: 16% chance to skip random events
    bool canSkipEvent() const;
    
    // Student: Companions have joy effect (morale boost)
    float getCompanionMoraleBonus() const;
    
    // Unemployed: Hard mode active, depression effect
    bool isHardMode() const;
    float getDepressionMultiplier() const;
    bool hasUniqueEndings() const;
    
private:
    std::string m_name;
    Backstory m_backstory;
    
    // Helper to initialize backstory properties
    void initializeBackstory();
};

#endif // CHARACTER_H
