#pragma once

#include <Kunlaboro/Component.hpp>

namespace sf { class Texture; }

class Weapon : public Kunlaboro::Component
{
public:
    Weapon();
    ~Weapon();

    void addedToEntity();

    inline std::string weaponName() const { return mName; }
    inline std::string weaponType() const { return mType; }
    inline std::string bulletName() const { return mBulletName; }

    inline sf::Texture& weaponTexture() const { return *mTexture; }
    inline sf::Texture& bulletTexture() const { return *mBulletTexture; }
    inline sf::Texture& magazineTexture() const { return *mMagazineTexture; }

    inline int magazinesLeft() const { return mMags; }
    inline int bulletsLeft() const { return mBulletsInCurrentMag; }

    inline float getDamage() const { return mDamage; }
    inline float getSpread() const { return mSpread; }
    inline float getRate() const { return mFireRate; }

    inline void throwMagazine() { --mMags; }

private:
    std::string mName, mBulletName, mType;

    sf::Texture *mTexture, *mMagazineTexture, *mBulletTexture;

    float mDamage, mSpread, mFireRate, mReloadTime;

    int mMags, mBulletsPerMag, mBulletsInCurrentMag, mBulletsPerShot;


};