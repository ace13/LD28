#pragma once

#include <Kunlaboro/Component.hpp>

namespace sf { class Texture; }

class Weapon : public Kunlaboro::Component
{
public:
    Weapon();
    ~Weapon();

    void addedToEntity();

    inline std::string weaponName() const { return Weapon::mName; }

    inline sf::Texture& weaponTexture() const { return *mTexture; }
    inline sf::Texture& bulletTexture() const { return *mBulletTexture; }
    inline sf::Texture& magazineTexture() const { return *mMagazineTexture; }

    inline int magazinesLeft() const { return mMags; }
    inline int bulletsLeft() const { return mBulletsInCurrentMag; }

private:
    std::string mName;

    sf::Texture *mTexture, *mMagazineTexture, *mBulletTexture;

    float mDamage, mSpread, mFireRate;

    int mMags, mBulletsPerMag, mBulletsInCurrentMag, mBulletsPerShot;


};