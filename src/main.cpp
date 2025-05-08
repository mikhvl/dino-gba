#include "bn_core.h"

#include "prj_scene.h"

int main()
{
    bn::core::init();
    prj::Scene scene;

    while(true)
    {
        scene.update();
        bn::core::update();
    }
}
