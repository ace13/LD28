#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

namespace Kunlaboro { class EntitySystem; }

class Engine
{
public:
    Engine(Kunlaboro::EntitySystem& sys);
    ~Engine();

    int mainLoop();

private:
    Kunlaboro::EntitySystem& mSystem;
    sf::RenderWindow mWindow;
};
