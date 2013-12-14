#include "World.hpp"

World::World() : Kunlaboro::Component("Game.World")
{
}

World::~World()
{
}

void World::addedToEntity()
{
    requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
    {

    });
}