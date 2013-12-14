#pragma once

#include <Kunlaboro/Component.hpp>

class Menu : public Kunlaboro::Component
{
public:
    Menu();
    ~Menu();

    void addedToEntity();

private:
    void update();
    void mouseMoved();
    void mouseClick();

};
