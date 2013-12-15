#pragma once

#include "../SpriteSheet.hpp"
#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

class Weapon;

class Enemy : public Kunlaboro::Component
{
public:
    Enemy();
    ~Enemy();

    void addedToEntity();



private:
    SpriteSheet mSheet;
    bool mFear;
    float mHealth, mArmor, mTime, mLastFire, mLastAng, mDrawAng, mFadeTime;
    Weapon* mWeapon;
    sf::Vector2f mPosition;
};