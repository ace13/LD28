#include "Engine.hpp"
#include "Resources.hpp"
#include "Game/Countdown.hpp"
#include "Game/Menu.hpp"

#include <Kunlaboro/EntitySystem.hpp>

void registerComponents(Kunlaboro::EntitySystem& sys)
{
    sys.registerComponent<Countdown>("Game.Countdown");
    sys.registerComponent<Menu>("Game.Menu");
}

int main(int argc, char** argv)
{
    Resources::initialize();

    Kunlaboro::EntitySystem sys;
    registerComponents(sys);

    Engine eng(sys);

    auto ent = sys.createEntity();
    sys.addComponent(ent, "Game.Menu");

    return eng.mainLoop();
}
