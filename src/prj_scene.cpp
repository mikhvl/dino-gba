#include "prj_scene.h"

#include "prj_const.h"

namespace prj
{
    Scene::Scene()
        : bg_item(lvl::DEFAULT_BG_ITEM)
        : bg_item(bn::regular_bg_items::bg_default)
        , bg(bg_item.create_bg(0, 0))
        , dino_ptr(bn::make_unique<prj::Player>())
    {}
    
    void Scene::update()
    {
        dino_ptr->update();
    }
}