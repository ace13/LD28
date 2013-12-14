#pragma once

#include "../SpriteSheet.hpp"
#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

class Enemy : public Kunlaboro::Component
{
public:
    Enemy();
    ~Enemy();

    void addedToEntity();



private:
    SpriteSheet mSheet;
    float mHealth, mArmor, mTime, mLastFire, mLastAng;
    sf::Vector2f mPosition;
};