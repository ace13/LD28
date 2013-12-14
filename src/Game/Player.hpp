#pragma once

#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

class Player : public Kunlaboro::Component
{
public:
    Player();
    ~Player();

    void addedToEntity();

private:
    sf::Vector2f mPosition;
};
