#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "GameTypes.h"







class GameState {
public:

    static GameState& getInstance() {
        static GameState instance;
        return instance;
    }


    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;


    void setOrigin(OriginType origin) { m_origin = origin; }
    OriginType getOrigin() const { return m_origin; }

    void setCarType(CarType carType) { m_carType = carType; }
    CarType getCarType() const { return m_carType; }


    bool isCharacterCreated() const {
        return m_origin != OriginType::UNKNOWN && m_carType != CarType::UNKNOWN;
    }


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

#endif
