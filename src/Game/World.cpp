#include "World.hpp"
#include "../Resources.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <random>

World::World() : Kunlaboro::Component("Game.World"), mSheet(Resources::Texture_Ground, 4, 2)
{
}

World::~World()
{
}

void World::addedToEntity()
{
    {
        std::random_device dev;
        std::uniform_int_distribution<int> rand(0, 3);

        sf::RenderTexture render;
        auto size = std::min(sf::Texture::getMaximumSize(), 1024U);
        render.create(size, size);

        sf::Sprite sprite(Resources::Texture_Ground);
        sprite.setTextureRect(mSheet.getRect(0, 0));
        sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);

        sprite.rotate(90);

        for (int x = 0; x < size;)
        {
            for (int y = 0; y < size;)
            {
                sprite.setTextureRect(mSheet.getRect(rand(dev), rand(dev) % 2));
                sprite.setRotation(rand(dev) * 90);

                render.draw(sprite);

                sprite.move(0, sprite.getTextureRect().height);
                y += sprite.getTextureRect().height;
            }
            
            sprite.setPosition(sprite.getPosition().x, 0);
            sprite.move(sprite.getTextureRect().width, 0);
            x += sprite.getTextureRect().width;
        }

        render.display();
        mBigOne = render.getTexture();
    }

    requestMessage("Event.Draw", [this](const Kunlaboro::Message& msg)
    {
        auto& target = *boost::any_cast<sf::RenderTarget*>(msg.payload);

        sf::Sprite sprite(mBigOne);
        target.draw(sprite);
    });
    changeRequestPriority("Event.Draw", -9001);
}