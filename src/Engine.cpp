#include "Engine.hpp"
#include <Kunlaboro/EntitySystem.hpp>
#include <SFML/Window/Event.hpp>

Engine::Engine(Kunlaboro::EntitySystem& sys) : mSystem(sys)
{
}

Engine::~Engine()
{
}

int Engine::mainLoop()
{
    mWindow.create(sf::VideoMode(1024,768), "Ludum Dare #28");

    sf::Event ev;
    sf::Clock timer;

    while (mWindow.isOpen())
    {
        float dt = timer.restart().asSeconds();
        while (mWindow.pollEvent(ev))
        {
            switch(ev.type)
            {       
            case sf::Event::Closed:
                {
                    Kunlaboro::Message msg(Kunlaboro::Type_Message, nullptr);
                    mSystem.sendGlobalMessage(mSystem.getMessageRequestId(Kunlaboro::Reason_Message, "Event.Window.Closed"), msg);
                    if (!msg.handled)
                        mWindow.close();
                } break;
            case sf::Event::GainedFocus: mSystem.sendGlobalMessage("Event.Window.GainedFocus"); break;
            case sf::Event::LostFocus: mSystem.sendGlobalMessage("Event.Window.LostFocus"); break;
            case sf::Event::MouseEntered: mSystem.sendGlobalMessage("Event.Window.MouseEnter"); break;
            case sf::Event::MouseLeft: mSystem.sendGlobalMessage("Event.Window.MouseLeft"); break;
            case sf::Event::Resized: mSystem.sendGlobalMessage("Event.Window.Resized", sf::Vector2u(ev.size.width, ev.size.height)); break;
            }
        }

        ///\TODO Fixed timestep
        mSystem.sendGlobalMessage("Event.Update", dt);

        mWindow.clear();

        mSystem.sendGlobalMessage("Event.Window.Draw", &mWindow);

        mWindow.display();
    }
}