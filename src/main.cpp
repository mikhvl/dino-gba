#include "bn_core.h"

#include "prj_player.h"
#include "prj_scene.h"

int main()
{
    bn::core::init();
    prj::Scene level;

    while(true)
    {
        level.update();
        bn::core::update();
    }
}
