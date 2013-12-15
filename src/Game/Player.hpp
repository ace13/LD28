#pragma once

#include "../SpriteSheet.hpp"
#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

class Weapon;

class Player : public Kunlaboro::Component
{
public:
    Player();
    ~Player();

    void addedToEntity();

    inline sf::Vector2f getPosition() const { return mPosition; }
    inline float getAngle() const { return mAngle; }

private:
    Weapon* mWeapon;
    char mPressed;
    float mTime, mAngle, mLastWalkAng, mHealth;
    sf::Vector2f mPosition, mMouse;
    SpriteSheet mSheet, mHeart;
};
