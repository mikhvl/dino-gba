#include "prj_scene.h"

#include "prj_const.h"

namespace prj
{
    Scene::Scene()
        : bg(bn::regular_bg_items::bg_default.create_bg(0, 0))
        , cam(bn::camera_ptr::create())
    {
    // player
        dino = bn::make_unique<Player>(lvl::PLAYER_START_X);
    
    // debug
        all_entity.emplace_back(bn::make_unique<Bag>(40));
        
        set_camera_for_all();
    }
    
    void Scene::set_camera_for_all()
    {
        dino->set_camera(cam);
        bg.set_camera(cam);
        for(bn::unique_ptr<Entity>& entity : all_entity) entity->set_camera(cam);
    }
    
    void Scene::update()
    {
        position_cam();
        manage_entity();
    }
    
    void Scene::position_cam()
    {
        bn::fixed x = dino->get_pos().x();
        bool is_negative = x < 0;
        x = x.multiplication(x).division(800);
        cam.set_position(is_negative ? -x : x, 0);
    }
    
    void Scene::manage_entity()
    {
        dino->update();
        
        for(bn::unique_ptr<Entity>& entity : all_entity)
        {
            if(dino->is_attacking())
            {
                if(dino->get_atk_hitbox()->intersects(entity->get_body_hitbox()))
                {
                    entity->take_damage(dino->get_pos().x() < entity->get_pos().x());
                }
            }
            entity->update();
        }
    }
}