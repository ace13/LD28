#pragma once

#include "../SpriteSheet.hpp"
#include <Kunlaboro/Component.hpp>

class World : public Kunlaboro::Component
{
public:
    World();
    ~World();

    void addedToEntity();

private:
    SpriteSheet mSheet;

};
