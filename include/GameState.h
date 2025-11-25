#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "GameTypes.h"

// GameState - Singleton to manage global game state
// Purpose: Share player choices and state across different scenes
// Features:
//   - Character origin and car type from creation
//   - Persistent across scene transitions
//   - Singleton pattern for easy access
class GameState {
public:
    // Get singleton instance
    static GameState& getInstance() {
        static GameState instance;
        return instance;
    }
    
    // Prevent copying
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    
    // Character creation data
    void setOrigin(OriginType origin) { m_origin = origin; }
    OriginType getOrigin() const { return m_origin; }
    
    void setCarType(CarType carType) { m_carType = carType; }
    CarType getCarType() const { return m_carType; }
    
    // Check if character has been created
    bool isCharacterCreated() const {
        return m_origin != OriginType::UNKNOWN && m_carType != CarType::UNKNOWN;
    }
    
    // Reset state (for new game)
    void reset() {
        m_origin = OriginType::UNKNOWN;
        m_carType = CarType::UNKNOWN;
    }
    
private:
    GameState() 
        : m_origin(OriginType::UNKNOWN), m_carType(CarType::UNKNOWN) {}
    
    OriginType m_origin;
    CarType m_carType;
};

#endif // GAME_STATE_H
