#include "Player.hpp"
#include "../Resources.hpp"

#include <Kunlaboro/EntitySystem.hpp>

const float gMoveSpeed = 128;
const float gCameraFloat = 1024;

enum Move
{
    M_W = 1,
    M_A = 2,
    M_S = 4,
    M_D = 8,
    M_Shift = 16
};

Player::Player() : Kunlaboro::Component("Game.Player"), mSheet(*Resources::Texture_Player, 2, 2), mTime(0), mPressed(0)
{
}

Player::~Player()
{
}

void Player::addedToEntity()
{
    sf::View* gameView = nullptr;
    auto response = sendGlobalQuestion("Get.GameView");
    if (response.handled)
        gameView = boost::any_cast<sf::View*>(response.payload);

    requestMessage("Event.Update", [this](const Kunlaboro::Message& msg)
    {
        float dt = boost::any_cast<float>(msg.payload);

        sf::Vector2f diff((mPressed & M_D)/M_D - (mPressed & M_A)/M_A, (mPressed & M_S)/M_S - (mPressed & M_W)/M_W);
        float len = std::min(sqrt(diff.x * diff.x + diff.y * diff.y), 1.f);

        if (len > 0.5)
        {
            float ang = atan2(diff.y, diff.x);
            diff = sf::Vector2f(cos(ang), sin(ang)) * len;

            mPosition += diff * dt * gMoveSpeed * (1.f + (mPressed & M_Shift)/M_Shift * 3.f);
        }

        mTime += dt;
    });
    requestMessage("Event.Draw", [this, gameView](const Kunlaboro::Message& msg)
    {
        auto& target = *boost::any_cast<sf::RenderTarget*>(msg.payload);

        auto curPos = gameView->getCenter();
        gameView->move((mPosition - curPos) / gCameraFloat);

        ///\TODO Draw a background of some kind
        sf::RectangleShape shape(sf::Vector2f(100, 100));
        target.draw(shape);

        sf::Sprite sprite(*Resources::Texture_Player);
        sprite.setTextureRect(mSheet.getRect((int)mTime % 2, (int)mTime / 2));
        sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
        sprite.setPosition(mPosition);
        sprite.setRotation(mAngle); 
        target.draw(sprite);

        // This is an animation, dig it?
        if (mTime > 4)
            mTime -= 4;
    });

    requestMessage("Event.Key.W", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_W; else mPressed &= ~M_W; });
    requestMessage("Event.Key.A", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_A; else mPressed &= ~M_A; });
    requestMessage("Event.Key.S", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_S; else mPressed &= ~M_S; });
    requestMessage("Event.Key.D", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_D; else mPressed &= ~M_D; });
    requestMessage("Event.Key.LShift", [this](const Kunlaboro::Message& msg) { if (boost::any_cast<bool>(msg.payload)) mPressed |= M_Shift; else mPressed &= ~M_Shift; });

    requestMessage("Event.Mouse.MoveGame", [this](const Kunlaboro::Message& msg)
    {
        auto pos = boost::any_cast<sf::Vector2f>(msg.payload);
        mMouse = pos;

        float ang = atan2(pos.y - mPosition.y, pos.x - mPosition.x);

        mAngle = ang * (180.f/3.1415f);
    });

    requestMessage("Event.Key.Escape", [this](const Kunlaboro::Message& msg)
    {
        bool pressed = boost::any_cast<bool>(msg.payload);

        if (pressed)
        {
            auto& sys = *getEntitySystem();
            auto ent = sys.createEntity();
            sys.addComponent(ent, "Game.Menu");
        }
    });
}
