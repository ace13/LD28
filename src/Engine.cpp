#include "Engine.hpp"
#include "Resources.hpp"
#include <Kunlaboro/EntitySystem.hpp>
#include <Kunlaboro/Component.hpp>
#include <SFML/Window/Event.hpp>

class EngineHandler : public Kunlaboro::Component
{
public:
    EngineHandler(sf::RenderWindow& win, sf::View& ui, sf::View& game) : Kunlaboro::Component("I am an internal component, don't poke me :("), mWin(win), mUi(ui), mGame(game) { }

    void addedToEntity()
    {
        requestMessage("Event.Engine.Init", &EngineHandler::resized);
        requestMessage("Event.Window.Resized", &EngineHandler::resized);
        changeRequestPriority("Event.Window.Resized", -42);
        requestMessage("ExitGame", [this](const Kunlaboro::Message& msg) { mWin.close(); });

        requestMessage("Get.GameView", [this](Kunlaboro::Message& msg) { msg.payload = &mGame; msg.handled = true; });
        requestMessage("Get.UiView", [this](Kunlaboro::Message& msg) { msg.payload = &mUi; msg.handled = true; });
    }

    void resized(const Kunlaboro::Message& msg)
    {
        sf::Vector2u size = mWin.getSize();

        mUi.setSize((sf::Vector2f)size);
        mUi.setCenter((sf::Vector2f)size / 2.f);

        auto gSize = mGame.getSize();
        float aspect = (float)size.x / size.y;

        mGame.setSize(gSize.y * aspect, gSize.y);
    }

private:
    sf::View &mUi, &mGame;
    sf::RenderWindow& mWin;
};

Engine::Engine(Kunlaboro::EntitySystem& sys) : mSystem(sys)
{
    sys.registerComponent("I am an internal component, don't poke me :(", [&]() { return new EngineHandler(mWindow, mUiView, mGameView); });

    auto ent = sys.createEntity();
    sys.addComponent(ent, "I am an internal component, don't poke me :(");
}

Engine::~Engine()
{
}


int Engine::mainLoop()
{
#define KEY(key) case sf::Keyboard::##key##: return #key; break;
    auto keyToString = [](sf::Keyboard::Key k) -> std::string {
        switch(k)
        {
        KEY(W) KEY(A) KEY(S) KEY(D)
        KEY(Up) KEY(Down) KEY(Left) KEY(Right)
        KEY(Space) KEY(Return) KEY(Escape)
        KEY(LShift)

        default:
        return "Invalid";
        }
    };
#undef KEY

    mWindow.create(sf::VideoMode(1024,768), Resources::String_Name);

    sf::Event ev;
    sf::Clock timer;

    Kunlaboro::Message msg(Kunlaboro::Type_Message, nullptr);
    mSystem.sendGlobalMessage(mSystem.getMessageRequestId(Kunlaboro::Reason_Message, "Event.Engine.Init"), msg);

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
                    msg.payload = (ev.type == sf::Event::KeyPressed);
                    mSystem.sendGlobalMessage(mSystem.getMessageRequestId(Kunlaboro::Reason_Message, "Event.Key." + keyToString(ev.key.code)), msg);
                } break;

            case sf::Event::MouseButtonPressed:
            case sf::Event::MouseButtonReleased:
                {
                    bool pressed = ev.type == sf::Event::MouseButtonPressed;

                    msg.payload = std::tuple<sf::Mouse::Button, sf::Vector2f, bool>(ev.mouseButton.button, sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y), pressed);
                    mSystem.sendGlobalMessage(mSystem.getMessageRequestId(Kunlaboro::Reason_Message, "Event.Mouse.Click"), msg);
                } break;

            case sf::Event::MouseMoved:
                {
                    auto pos = sf::Vector2f(ev.mouseMove.x, ev.mouseMove.y);
                    msg.payload = pos;
                    mSystem.sendGlobalMessage(mSystem.getMessageRequestId(Kunlaboro::Reason_Message, "Event.Mouse.Move"), msg);
                    {
                        sf::Vector2f vS = mGameView.getSize(), vC = mGameView.getCenter(), wS = (sf::Vector2f)mWindow.getSize();
                        sf::FloatRect vR;
                        vR.left = vC.x - vS.x / 2;
                        vR.top = vC.y - vS.y / 2;
                        vR.width = vS.x;
                        vR.height = vS.y;

                        msg.payload = sf::Vector2f(vR.left + vR.width * (pos.x / wS.x), vR.top + vR.height * (pos.y / wS.y));
                    }
                    mSystem.sendGlobalMessage(mSystem.getMessageRequestId(Kunlaboro::Reason_Message, "Event.Mouse.MoveGame"), msg);
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