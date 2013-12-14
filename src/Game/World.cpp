#include "World.hpp"
#include "../Resources.hpp"
#include <SFML/Graphics/Sprite.hpp>

World::World() : Kunlaboro::Component("Game.World"), mSheet(Resources::Texture_Ground, 4, 2)
{
}

World::~World()
{
}

void World::addedToEntity()
{
    requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
    {
        auto& target = *boost::any_cast<sf::RenderTarget*>(msg.payload);

        sf::Sprite sprite(Resources::Texture_Ground);

        target.draw(sprite);
    });
    changeRequestPriority("Event.Draw", -9001);
}