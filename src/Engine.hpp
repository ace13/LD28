#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

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

    sf::View mGameView;
    sf::View mUiView;
};
