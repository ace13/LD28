#pragma once

#include <Kunlaboro/Component.hpp>

class Weapon : public Kunlaboro::Component
{
public:
    Weapon();
    ~Weapon();

    void addedToEntity();

private:
    std::string mName;

    float mDamage;
    float mSpread;
    float mFireRate;

    int mMags;
    int mBulletsPerMag;
    int mBulletsInCurrentMag;


};