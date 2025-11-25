/**
 * @file System.hpp
 * @brief Mock SFML System types for testing without SFML
 * @date 2025-11-18
 * 
 * This file provides mock implementations of SFML System types
 * to allow testing of code that depends on SFML without requiring
 * SFML to be installed.
 */

#ifndef MOCK_SFML_SYSTEM_HPP
#define MOCK_SFML_SYSTEM_HPP

#include <cstdint>

namespace sf {

/**
 * Mock Vector2f - 2D vector with float components
 */
template<typename T>
class Vector2 {
public:
    T x;
    T y;
    
    Vector2() : x(0), y(0) {}
    Vector2(T x, T y) : x(x), y(y) {}
    
    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;

/**
 * Mock Rect - Rectangle with position and size
 */
template<typename T>
class Rect {
public:
    T left;
    T top;
    T width;
    T height;
    
    Rect() : left(0), top(0), width(0), height(0) {}
    Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight)
        : left(rectLeft), top(rectTop), width(rectWidth), height(rectHeight) {}
    Rect(const Vector2<T>& position, const Vector2<T>& size)
        : left(position.x), top(position.y), width(size.x), height(size.y) {}
    
    bool contains(const Vector2<T>& point) const {
        return (point.x >= left && point.x < left + width &&
                point.y >= top && point.y < top + height);
    }
};

using FloatRect = Rect<float>;
using IntRect = Rect<int>;

// Type aliases
using Uint32 = uint32_t;

/**
 * Mock Time - Time value
 */
class Time {
public:
    Time() : m_microseconds(0) {}
    explicit Time(int64_t microseconds) : m_microseconds(microseconds) {}
    
    float asSeconds() const { return m_microseconds / 1000000.0f; }
    int32_t asMilliseconds() const { return static_cast<int32_t>(m_microseconds / 1000); }
    int64_t asMicroseconds() const { return m_microseconds; }
    
    static Time Zero;
    
private:
    int64_t m_microseconds;
};

inline Time Time::Zero = Time(0);

/**
 * Mock Clock - Measures elapsed time
 */
class Clock {
public:
    Clock() : m_startTime(0) {}
    
    Time restart() {
        Time elapsed(m_startTime);
        m_startTime = 16666; // ~60 FPS for testing
        return elapsed;
    }
    
    Time getElapsedTime() const {
        return Time(m_startTime);
    }
    
private:
    int64_t m_startTime;
};

} // namespace sf

#endif // MOCK_SFML_SYSTEM_HPP
