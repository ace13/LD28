#include "Countdown.hpp"
#include "../Resources.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>

#ifndef M_PI
#define M_PI (4.f*atan(1.f))
#endif

const time_t Countdown::Target = 1386986400;

Countdown::Countdown() : Kunlaboro::Component("Game.Countdown"), mRandomValue(0)
{
    mCurrent = time(NULL);
}

Countdown::~Countdown()
{
}

void Countdown::addedToEntity()
{
    requestMessage("Event.Update", &Countdown::update);
    requestMessage("Event.DrawUi", &Countdown::drawUi);

}

void Countdown::drawUi(const Kunlaboro::Message& msg)
{
    sf::Text string("I am a string! :D", Resources::Font_Dosis);

    int diff = ::difftime(Target, mCurrent);

    int minutes = diff / 60;
    int seconds = diff - (minutes * 60);

    std::ostringstream oss;

    oss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;

    string.setString(oss.str());
    string.setColor(sf::Color((int)(mRandomValue * 100) % 256, (int)((mRandomValue * 100) * M_PI) % 256, (int)((mRandomValue * 100) / (M_PI*2)) % 256));

    auto& target = *boost::any_cast<sf::RenderTarget*>(msg.payload);

    auto rect = string.getGlobalBounds();
    string.setOrigin(rect.width / 2.f, rect.height / 2.f);
    auto size = target.getView().getSize();
    string.setPosition(size.x / 2.f, size.y / 2.f);

    string.move(sin(mRandomValue) * 100, cos(mRandomValue * 2.f) * 100);
    string.rotate(sin(mRandomValue * 4.f) * 3.1415);

    target.draw(string);
}

void Countdown::update(const Kunlaboro::Message& msg)
{
    mCurrent = time(NULL);

    mRandomValue += boost::any_cast<float>(msg.payload);
}
