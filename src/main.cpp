#include "Engine.hpp"
#include "Resources.hpp"
#include "Game/Menu.hpp"
#include "Game/EndScreen.hpp"
#include "Game/InfoScreen.hpp"
#include "Game/Intro.hpp"
#include "Game/Player.hpp"
#include "Game/World.hpp"
#include "Game/Weapon.hpp"
#include "Game/Bullet.hpp"
#include "Game/Enemy.hpp"
#include "Game/Dialog.hpp"
#include "Game/Pickup.hpp"

#include <Kunlaboro/EntitySystem.hpp>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <iostream>
#endif

void registerComponents(Kunlaboro::EntitySystem& sys)
{
    sys.registerComponent<Menu>("Game.Menu");
    sys.registerComponent<Intro>("Game.Intro");
    sys.registerComponent<InfoScreen>("Game.InfoScreen");
    sys.registerComponent<EndScreen>("Game.EndScreen");
    sys.registerComponent<Player>("Game.Player");
    sys.registerComponent<World>("Game.World");
    sys.registerComponent<Weapon>("Game.Weapon");
    sys.registerComponent<Bullet>("Game.Bullet");
    sys.registerComponent<Enemy>("Game.Enemy");
    sys.registerComponent<Dialog>("Game.Dialog");
    sys.registerComponent<Pickup>("Game.Pickup");
}

int main(int argc, char** argv)
{
    try
    {
        Resources::initialize();

        Kunlaboro::EntitySystem sys;
        registerComponents(sys);

        Engine eng(sys);

        auto ent = sys.createEntity();
#ifdef _DEBUG
        sys.addComponent(ent, "Game.Menu");
#else
        sys.addComponent(ent, "Game.Intro");
#endif

        return eng.mainLoop();
    }
    catch (std::runtime_error err)
    {
#if WIN32
        MessageBox(NULL, err.what(), "Critical failure", MB_OK | MB_ICONERROR);
#else
        std::cerr << "Critical failure: " << std::endl << err.what() << std::endl;
#endif

        return 1;
    }
}
