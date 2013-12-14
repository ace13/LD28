#include "Player.hpp"
#include "../Resources.hpp"

const float gMoveSpeed = 128;
const float gCameraFloat = 1024;

Player::Player() : Kunlaboro::Component("Game.Player"), mSheet(*Resources::Texture_Player, 2, 2), mTime(0)
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
        mPosition += mDiff * dt * gMoveSpeed;
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
        target.draw(sprite);

        if (mTime > 4)
        {
            mTime -= 4;
        }
    });

    requestMessage("Event.Key.W", [this](const Kunlaboro::Message& msg) { mDiff.y = -boost::any_cast<bool>(msg.payload); });
    requestMessage("Event.Key.A", [this](const Kunlaboro::Message& msg) { mDiff.x = -boost::any_cast<bool>(msg.payload); });
    requestMessage("Event.Key.S", [this](const Kunlaboro::Message& msg) { mDiff.y = boost::any_cast<bool>(msg.payload); });
    requestMessage("Event.Key.D", [this](const Kunlaboro::Message& msg) { mDiff.x = boost::any_cast<bool>(msg.payload); });
}
