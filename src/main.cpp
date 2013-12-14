#include "Engine.hpp"
#include "Resources.hpp"
#include "Game/Countdown.hpp"
#include "Game/Menu.hpp"
#include "Game/Player.hpp"

#include <Kunlaboro/EntitySystem.hpp>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <iostream>
#endif

void registerComponents(Kunlaboro::EntitySystem& sys)
{
    sys.registerComponent<Countdown>("Game.Countdown");
    sys.registerComponent<Menu>("Game.Menu");
    sys.registerComponent<Player>("Game.Player");
}

int main(int argc, char** argv)
{
    try
    {
        Resources::initialize();
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

    Kunlaboro::EntitySystem sys;
    registerComponents(sys);

    Engine eng(sys);

    auto ent = sys.createEntity();
    sys.addComponent(ent, "Game.Menu");
    sys.addComponent(ent, "Game.Player");

    return eng.mainLoop();
}
