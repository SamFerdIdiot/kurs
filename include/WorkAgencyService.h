#ifndef WORK_AGENCY_SERVICE_H
#define WORK_AGENCY_SERVICE_H

#include "Service.h"
#include "PlayerState.h"
#include <SFML/Graphics.hpp>
#include <vector>

// WorkAgencyService - Work agency where player can find work to earn money
// Purpose: Allow player to earn money by completing work tasks
// Features:
//   - Display available jobs
//   - Show job pay and energy cost
//   - Handle job selection and completion
//   - Update player money and energy
class WorkAgencyService : public Service {
public:
    WorkAgencyService();
    
    ServiceType getType() const override { return ServiceType::WORK_AGENCY; }
    std::string getName() const override { return "Work Agency"; }
    
    void displayMenu(sf::RenderWindow& window) override;
    bool handleInput(const sf::Event& event) override;
    bool execute(PlayerState& playerState) override;
    bool isAvailable(const PlayerState& playerState) const override;
    void update(float deltaTime) override;
    
private:
    // Work job structure
    struct Job {
        std::string name;
        float pay;
        float energyCost;
    };
    
    std::vector<Job> m_jobs;              // Available jobs
    int m_selectedJob;                     // Currently selected job
    bool m_isFinished;                     // Service interaction complete flag
    
    // Placeholder visual elements
    sf::RectangleShape m_backgroundBox;    // Work agency background
    sf::RectangleShape m_menuBox;          // Menu container
    sf::RectangleShape m_jobBox1;          // Job 1 placeholder
    sf::RectangleShape m_jobBox2;          // Job 2 placeholder
    sf::RectangleShape m_jobBox3;          // Job 3 placeholder
    sf::RectangleShape m_selectorBox;      // Selection indicator
    sf::RectangleShape m_infoBox;          // Info/status box
    sf::RectangleShape m_boardBox;         // Job board display
};

#endif // WORK_AGENCY_SERVICE_H
