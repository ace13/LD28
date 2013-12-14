#pragma once

#include <Kunlaboro/Component.hpp>

class Enemy : public Kunlaboro::Component
{
public:
    Enemy();
    ~Enemy();

    void addedToEntity();



private:
    float mHealth;
    float mArmor;
};