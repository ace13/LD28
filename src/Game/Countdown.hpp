#pragma once

#include <Kunlaboro/Component.hpp>
#include <ctime>

class Countdown : public Kunlaboro::Component
{
public:
    Countdown();
    ~Countdown();

    void addedToEntity();

private:
    void drawUi(const Kunlaboro::Message& msg);
    void update(const Kunlaboro::Message& msg);

    float mRandomValue;

    static const ::time_t Target;
    ::time_t mCurrent;
};
