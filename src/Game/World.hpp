#pragma once

#include "../SpriteSheet.hpp"
#include <Kunlaboro/Component.hpp>
#include <SFML/Graphics/Texture.hpp>

class World : public Kunlaboro::Component
{
public:
    World();
    ~World();

    void addedToEntity();

private:
    SpriteSheet mSheet;
    sf::Texture mBigOne;
};
