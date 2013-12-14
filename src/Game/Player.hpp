#pragma once

#include "../SpriteSheet.hpp"
#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

class Player : public Kunlaboro::Component
{
public:
    Player();
    ~Player();

    void addedToEntity();

private:
    float mTime;
    sf::Vector2f mPosition;
    sf::Vector2f mDiff;
    SpriteSheet mSheet;
};
