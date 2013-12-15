#include "InfoScreen.hpp"
#include "../SpriteSheet.hpp"
#include "../Resources.hpp"
#include <Kunlaboro/EntitySystem.hpp>

sf::ConvexShape createRoundedRect(const sf::Vector2f& size, float radius = 15.f)
{
    ///\TODO 16 points, rounded corners
    sf::ConvexShape toReturn(4);

    toReturn.setPoint(0, sf::Vector2f());
    toReturn.setPoint(1, sf::Vector2f(size.x, 0));
    toReturn.setPoint(2, sf::Vector2f(size.x, size.y));
    toReturn.setPoint(3, sf::Vector2f(0, size.y));

    return toReturn;
}

InfoScreen::InfoScreen() : Kunlaboro::Component("Game.InfoScreen"), mFirstUpdate(true), mPage(0), mTime(0)
{
}

InfoScreen::~InfoScreen()
{
}

void InfoScreen::addedToEntity()
{
    mPages.push_back([](sf::RenderTarget& target)
    {
        sf::Text string("This is a rather simple game, all things considered.\nAll you have to do is stay alive.\n\nOf course, the amount of enemies makes this slightly hard...\nSo in these pages are a few tips to staying alive out there.\n\n\n\n\n\n\n(By reading these tips you absolve me of any damages that might\nhappen to your personal being)", Resources::Font_Dosis);
        string.setPosition(target.getView().getCenter() - sf::Vector2f(384, 256) + sf::Vector2f(5, 5));

        target.draw(string);
    });
    mPages.push_back([this](sf::RenderTarget& target)
    {
        static SpriteSheet sheet(Resources::Texture_Player, 4, 2);
        
        float time = mTime * 4;

        sf::Text string("To start off with, this here is you: ", Resources::Font_Dosis);
        string.setPosition(target.getView().getCenter() - sf::Vector2f(384 - string.getLocalBounds().width / 2.f, 256) + sf::Vector2f(5, 5));

        target.draw(string);

        sf::Sprite player(Resources::Texture_Player);
        player.setTextureRect(sheet.getRect(0,0));
        player.setOrigin(player.getTextureRect().width / 2.f, player.getTextureRect().height / 2.f);
        player.setPosition(target.getView().getCenter());
        player.setScale(2,2);

        player.setRotation(mTime * 45);

        int val = (int)(time) % 4;
        player.setTextureRect(sheet.getRect(val/2, 1));
        if (val % 2)
            target.draw(player);

        player.setTextureRect(sheet.getRect(val, 0));

        target.draw(player);

        string.setCharacterSize(18);
        string.setString("(Yes, you're an ugly one)");
        string.setPosition(target.getView().getCenter() + sf::Vector2f(-string.getLocalBounds().width / 2.f, 256 - 32));

        target.draw(string);
    });
    mPages.push_back([this](sf::RenderTarget& target)
    {
        static SpriteSheet sheet(Resources::Texture_Enemy, 4, 2);

        sf::Text string("And these are some of the evil guys you'll run into:", Resources::Font_Dosis);
        string.setPosition(target.getView().getCenter() - sf::Vector2f(string.getLocalBounds().width / 2.f, 256) + sf::Vector2f(5, 5));

        target.draw(string);

        sf::Sprite enemy(Resources::Texture_Enemy);
        enemy.setTextureRect(sheet.getRect(0,0));
        enemy.setOrigin(enemy.getTextureRect().width / 2.f, enemy.getTextureRect().height / 2.f);
        enemy.setPosition(target.getView().getCenter());
        enemy.setScale(2,2);

        enemy.setRotation(mTime * 45);

        int val = (int)(mTime) % 8;
        enemy.setTextureRect(sheet.getRect(val % 4, val / 4));
        
        target.draw(enemy);

        string.setCharacterSize(18);
        string.setString("(Yes, they too are ugly ones)");
        string.setPosition(target.getView().getCenter() + sf::Vector2f(-string.getLocalBounds().width / 2.f, 256 - 32));

        target.draw(string);
    });
    mPages.push_back([this](sf::RenderTarget& target)
    {
        sf::Text string("And a few of the things you'll run into (literally in most cases):", Resources::Font_Dosis);
        string.setPosition(target.getView().getCenter() - sf::Vector2f(string.getLocalBounds().width / 2.f, 256) + sf::Vector2f(5, 5));

        target.draw(string);

        int val = (int)(mTime) % Resources::Texture_Weapons.size();
        auto it = Resources::Texture_Weapons.begin();
        for (int i = 0; i < val; ++i, ++it);

        sf::Sprite weapon(it->second);
        weapon.setOrigin(weapon.getTextureRect().width / 2.f, weapon.getTextureRect().height / 2.f);
        weapon.setPosition(target.getView().getCenter());
        weapon.setScale(2,2);

        weapon.setRotation(mTime * 45);

        target.draw(weapon);
    });
    mPages.push_back([](sf::RenderTarget& target)
    {
        sf::Text string("Your controls are as follow:\n\n  WASD - Movement of yourself\n  Q - Throw your current weapon\n  E - Pick up a nearby weapon\n  R - Reload your weapon\n  Shift - Run\n  Left-click - Fire your weapon", Resources::Font_Dosis);
        string.setPosition(target.getView().getCenter() - sf::Vector2f(384, 256) + sf::Vector2f(5, 5));

        target.draw(string);
    });
    mPages.push_back([](sf::RenderTarget& target)
    {
        sf::Text string("Now go and have fun!", Resources::Font_Dosis);
        string.setPosition(target.getView().getCenter() - sf::Vector2f(string.getLocalBounds().width / 2.f, 0));

        target.draw(string);
    });

    requestMessage("Event.Update", [this](const Kunlaboro::Message& msg) 
    {
        if (mFirstUpdate)
        {
            mFirstUpdate = false;
            changeRequestPriority("Event.Mouse.Click", -2);
        }

        mTime += boost::any_cast<float>(msg.payload);
    });
    changeRequestPriority("Event.Update", -10);

    requestMessage("Event.DrawUi", [this](const Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::RenderTarget*,float>>(msg.payload);
        auto& target = *std::get<0>(data);

        mView = &target.getView();

        auto toneOut = sf::RectangleShape(target.getView().getSize());
        toneOut.setFillColor(sf::Color(0,0,0,196));
        target.draw(toneOut);

        auto rect = createRoundedRect(sf::Vector2f(768, 512));
        rect.setOrigin(sf::Vector2f(384, 256));
        rect.setOutlineThickness(5.f);
        rect.setOutlineColor(sf::Color(255,255,255));
        rect.setFillColor(sf::Color(64,64,64,196));

        rect.setPosition(target.getView().getCenter());

        target.draw(rect);

        auto it = mPages.begin();
        for (int i = 0; i < mPage; ++i, ++it);
        (*it)(target);


        sf::Text button("How-To", Resources::Font_Dosis, 67);
        button.setOrigin(button.getLocalBounds().width / 2.f, button.getLocalBounds().height);
        button.setPosition(target.getView().getCenter() + sf::Vector2f(0, -256 - button.getLocalBounds().height));

        target.draw(button);

        button.setCharacterSize(30);

        button.setString("Previous");
        button.setOrigin(0, 0);
        button.setPosition(target.getView().getCenter() + sf::Vector2f(-384, 256 + button.getLocalBounds().height));

        if (button.getGlobalBounds().contains(mMousePos))
        {
            if (mPage == 0)
                button.setColor(sf::Color(128, 128, 128, 255));
            else
                button.setColor(sf::Color(128, 196, 0));
        }

        target.draw(button);

        button.setColor(sf::Color::White);
        button.setString("Close");
        button.setOrigin(button.getLocalBounds().width / 2.f, 0);
        button.setPosition(target.getView().getCenter() + sf::Vector2f(0, 256 + button.getLocalBounds().height));
        if (button.getGlobalBounds().contains(mMousePos))
            button.setColor(sf::Color(128, 196, 0));

        target.draw(button);

        button.setColor(sf::Color::White);
        button.setString("Next");
        button.setOrigin(button.getLocalBounds().width, 0);
        button.setPosition(target.getView().getCenter() + sf::Vector2f(384, 256 + button.getLocalBounds().height));
        if (button.getGlobalBounds().contains(mMousePos))
        {
            if (mPage >= (int)mPages.size() - 1)
                button.setColor(sf::Color(128, 128, 128, 255));
            else
                button.setColor(sf::Color(128, 196, 0));
        }

        target.draw(button);
    });
    changeRequestPriority("Event.DrawUi", 2);

    requestMessage("Event.Mouse.Click", [this](Kunlaboro::Message& msg)
    {
        auto data = boost::any_cast<std::tuple<sf::Mouse::Button, sf::Vector2f, bool> >(msg.payload); 

        if (std::get<0>(data) != sf::Mouse::Left || !std::get<2>(data))
            return;

        sf::Text button("Previous", Resources::Font_Dosis, 30);
        button.setOrigin(0, 0);
        button.setPosition(mView->getCenter() + sf::Vector2f(-384, 256 + button.getLocalBounds().height));

        if (button.getGlobalBounds().contains(mMousePos))
        {
            if (mPage != 0)
                mPage--;
        }

        button.setString("Close");
        button.setOrigin(button.getLocalBounds().width / 2.f, 0);
        button.setPosition(mView->getCenter() + sf::Vector2f(0, 256 + button.getLocalBounds().height));
        if (button.getGlobalBounds().contains(mMousePos))
            getEntitySystem()->destroyComponent(this);

        button.setString("Next");
        button.setOrigin(button.getLocalBounds().width, 0);
        button.setPosition(mView->getCenter() + sf::Vector2f(384, 256 + button.getLocalBounds().height));
        if (button.getGlobalBounds().contains(mMousePos))
        {
            if (mPage < mPages.size() - 1)
                mPage++;
        }
        
        msg.handled = true;
    });
    
    requestMessage("Event.Mouse.Move", [this](Kunlaboro::Message& msg)
    {
        msg.handled = true;

        auto pos = boost::any_cast<sf::Vector2f>(msg.payload);
        mMousePos = pos;
    });
    changeRequestPriority("Event.Mouse.Move", -3);

    requestMessage("Event.Key.Escape", [this](Kunlaboro::Message& msg)
    {
        getEntitySystem()->destroyComponent(this);
        msg.handled = true;
    });
    changeRequestPriority("Event.Key.Escape", -1);
}