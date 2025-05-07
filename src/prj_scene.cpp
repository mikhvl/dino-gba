#include "prj_scene.h"

#include "prj_const.h"

namespace prj
{
    Scene::Scene()
        : bg_item(bn::regular_bg_items::bg_default)
        , bg(bg_item.create_bg(0, 0))
        , dino_ptr(bn::make_unique<prj::Player>())
        , bag_ptr(bn::make_unique<prj::Bag>(40))
    {}
    
    void Scene::update()
    {
        dino_ptr->update();
    }
}