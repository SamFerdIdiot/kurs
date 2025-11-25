#ifndef SERVICE_H
#define SERVICE_H

#include <SFML/Graphics.hpp>
#include <string>

// Forward declaration
class PlayerState;

// ServiceType - Enum for different service types available in town
enum class ServiceType {
    STORE,         // Buy items and supplies
    GARAGE,        // Repair car and refuel
    WORK_AGENCY    // Find work to earn money
};

// Service - Interface for town service interactions
// Purpose: Provide a common interface for all town services
// Features:
//   - Display service menu
//   - Handle user input for service interaction
//   - Execute service actions on player state
//   - Check if service is available/affordable
class Service {
public:
    virtual ~Service() = default;
    
    // Get the type of this service
    virtual ServiceType getType() const = 0;
    
    // Get the name of this service
    virtual std::string getName() const = 0;
    
    // Display the service menu
    virtual void displayMenu(sf::RenderWindow& window) = 0;
    
    // Handle input for the service menu
    // Returns true if service interaction is complete
    virtual bool handleInput(const sf::Event& event) = 0;
    
    // Execute service action (purchase, repair, work, etc.)
    // Returns true if action was successful
    virtual bool execute(PlayerState& playerState) = 0;
    
    // Проверить доступна ли услуга игроку
    virtual bool isAvailable(const PlayerState& playerState) const = 0;
    
    // Update service state
    virtual void update(float deltaTime) = 0;
};

#endif // SERVICE_H
