#include "prj_scene.h"

#include "prj_const.h"

namespace prj
{
    Scene::Scene()
        : bg_item(bn::regular_bg_items::bg_default)
        , bg(bg_item.create_bg(0, 0))
    {
    // create player
        dino = bn::make_unique<Player>();
        
    // debug
        all_entity.emplace_back(bn::make_unique<Bag>(44));
    }
    
    void Scene::update()
    {
        manage_entity();
    }
    
    void Scene::manage_entity()
    {
        dino->update();
        
        for(int i = 0; i < all_entity.size(); ++i)
        {
            if(dino->is_attacking())
            {
                if(dino->get_atk_hitbox().value().intersects(all_entity[i]->get_body_hitbox()))
                {
                    all_entity[i]->take_damage();
                }
            }
            all_entity[i]->update();
        }
    }
}