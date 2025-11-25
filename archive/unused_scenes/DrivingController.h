#ifndef DRIVING_CONTROLLER_H
#define DRIVING_CONTROLLER_H

#include <SFML/Graphics.hpp>

// DrivingController class
// Purpose: Handles player car movement logic during driving gameplay
// Responsibilities:
//   - Process keyboard input for left/right movement (A/D keys ONLY)
//   - Apply horizontal movement to player car
//   - Enforce road boundaries to prevent car from going off-screen
//   - Provide clean interface for movement control
//   - Stub extensions for future gameplay features (acceleration, lanes, etc.)
// ⚠️ CONTROL RESTRICTIONS: Only A (left) and D (right) keys are permitted for movement
class DrivingController {
public:
    // Конструктор
    // Parameters:
    //   - playerCar: Reference to the player's car rectangle shape
    //   - roadLeftBound: Left edge of the driveable road area
    //   - roadRightBound: Right edge of the driveable road area
    DrivingController(sf::RectangleShape& playerCar, float roadLeftBound, float roadRightBound);
    
    // Update player car position based on elapsed time
    // Parameters:
    //   - deltaTime: Time elapsed since last frame (in seconds)
    // This method applies movement based on current input state
    void update(float deltaTime);
    
    // Process keyboard input events
    // Parameters:
    //   - event: SFML event to process
    // Handles key press/release for movement controls
    void handleInput(const sf::Event& event);
    
    // Check if movement is currently paused
    // Returns: true if movement is paused, false otherwise
    bool isMovementPaused() const { return m_movementPaused; }
    
    // Set movement pause state
    // Parameters:
    //   - paused: true to pause movement, false to resume
    void setMovementPaused(bool paused) { m_movementPaused = paused; }
    
    // TODO: Future extensions
    // - Add lane-based movement system (3-4 distinct lanes)
    // - Implement smooth acceleration/deceleration
    // - Add speed management (tie to DrivingScene speed variable)
    // - Implement collision detection with other vehicles
    // - Add drift/skid mechanics for weather effects
    // - Support gamepad/controller input
    
private:
    sf::RectangleShape& m_playerCar;  // Reference to player's car shape
    float m_roadLeftBound;             // Left boundary of road
    float m_roadRightBound;            // Right boundary of road
    float m_movementSpeed;             // Horizontal movement speed (pixels per second)
    bool m_movingLeft;                 // Is left key pressed
    bool m_movingRight;                // Is right key pressed
    bool m_movementPaused;             // Is movement currently paused
    
    // Apply road boundaries to keep car on screen
    void enforceRoadBoundaries();
};

#endif // DRIVING_CONTROLLER_H
