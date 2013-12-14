#include "Engine.hpp"
#include <Kunlaboro/EntitySystem.hpp>
#include <Kunlaboro/Component.hpp>
#include <SFML/Window/Event.hpp>

class ResizeHandler : public Kunlaboro::Component
{
public:
    ResizeHandler(sf::View& ui, sf::View& game) : Kunlaboro::Component("I am an internal component, don't poke me :("), mUi(ui), mGame(game) { }

    void addedToEntity()
    {
        requestMessage("Event.Window.Resized", &ResizeHandler::resized);
        changeRequestPriority("Event.Window.Resized", -42);
    }

    void resized(const Kunlaboro::Message& msg)
    {
        auto size = boost::any_cast<sf::Vector2u>(msg.payload);

        mUi.setSize((sf::Vector2f)size);
        mUi.setCenter((sf::Vector2f)size / 2.f);

    }

private:
    sf::View& mUi, mGame;
};

Engine::Engine(Kunlaboro::EntitySystem& sys) : mSystem(sys)
{
    sys.registerComponent("I am an internal component, don't poke me :(", [&]() { return new ResizeHandler(mUiView, mGameView); });

    auto ent = sys.createEntity();
    sys.addComponent(ent, "I am an internal component, don't poke me :(");
}

Engine::~Engine()
{
}

#define KEY(key) case sf::Keyboard::##key##: return #key; break;

int Engine::mainLoop()
{
    auto keyToString = [](sf::Keyboard::Key k) -> std::string {
        switch(k)
        {
        KEY(W) KEY(A) KEY(S) KEY(D)
        KEY(Up) KEY(Down) KEY(Left) KEY(Right)
        KEY(Space) KEY(Return) KEY(Escape)

        default:
        return "Invalid";
        }
    };

    mWindow.create(sf::VideoMode(1024,768), "Ludum Dare #28");

    sf::Event ev;
    sf::Clock timer;

    Kunlaboro::Message msg(Kunlaboro::Type_Message, nullptr);

    while (mWindow.isOpen())
    {
        float dt = timer.restart().asSeconds();
        while (mWindow.pollEvent(ev))
        {
            switch(ev.type)
            {       
            case sf::Event::Closed:
                {
                    msg.payload = nullptr;
                    mSystem.sendGlobalMessage(mSystem.getMessageRequestId(Kunlaboro::Reason_Message, "Event.Window.Closed"), msg);
                    if (!msg.handled)
                        mWindow.close();
                } break;
            case sf::Event::GainedFocus: mSystem.sendGlobalMessage("Event.Window.GainedFocus"); break;
            case sf::Event::LostFocus: mSystem.sendGlobalMessage("Event.Window.LostFocus"); break;
            case sf::Event::MouseEntered: mSystem.sendGlobalMessage("Event.Window.MouseEnter"); break;
            case sf::Event::MouseLeft: mSystem.sendGlobalMessage("Event.Window.MouseLeft"); break;
            case sf::Event::Resized: mSystem.sendGlobalMessage("Event.Window.Resized", sf::Vector2u(ev.size.width, ev.size.height)); break;

            case sf::Event::KeyPressed:
            case sf::Event::KeyReleased:
                {
                    bool pressed = ev.type == sf::Event::KeyPressed;

                    msg.payload = pressed;
                    mSystem.sendGlobalMessage(mSystem.getMessageRequestId(Kunlaboro::Reason_Message, "Event.Key." + keyToString(ev.key.code)), msg);
                } break;
            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
                {

                } break;
            }
        }

        ///\TODO Fixed timestep
        mSystem.sendGlobalMessage("Event.Update", dt);

        mWindow.clear();

        mWindow.setView(mGameView);

        mSystem.sendGlobalMessage("Event.Draw", (sf::RenderTarget*)&mWindow);

        mWindow.setView(mUiView);

        mSystem.sendGlobalMessage("Event.DrawUi", (sf::RenderTarget*)&mWindow);
        
        mWindow.display();
    }

    return 0;
}