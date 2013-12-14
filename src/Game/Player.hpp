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

private:
    Weapon* mWeapon;
    char mPressed;
    float mTime, mAngle;
    sf::Vector2f mPosition, mMouse;
    SpriteSheet mSheet;
};
