#include "prj_scene.h"

#include "prj_const.h"

namespace prj
{
    Scene::Scene()
        : bg_item(bn::regular_bg_items::bg_default)
        , bg(bg_item.create_bg(0, 0))
        , cam(bn::camera_ptr::create())
    {
    // player
        dino = bn::make_unique<Player>(lvl::PLAYER_START_X);
        dino->attach_cam(cam);
        bg.set_camera(cam);
    }
    
    void Scene::update()
    {
        position_cam();
        manage_entity();
    }
    
    void Scene::position_cam()
    {
        cam.set_position(dino->get_pos());
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