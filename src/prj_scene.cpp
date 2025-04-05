#include "prj_scene.h"

namespace prj
{
    Scene::Scene()
        : bg_art(prj::DEFAULT_BG_ITEM)
        , bg(bn::regular_bg_items::bg_temp.create_bg(0, 0))
    {
        dino_ptr = bn::make_unique<prj::Player>();
    }
    
    void Scene::update()
    {
        dino_ptr->update();
    }
}