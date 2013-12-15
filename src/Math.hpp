#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

#ifndef M_PI
const float M_PI = (4.f*atan(1.f));
#endif

namespace Math
{
    inline float distance(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        sf::Vector2f diff = b - a;
        return sqrt(diff.x * diff.x + diff.y * diff.y);
    }

}
