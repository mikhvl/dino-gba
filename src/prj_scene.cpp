#include "prj_scene.h"

#include "prj_const.h"

namespace prj
{
    Scene::Scene()
        : bg_back(bn::regular_bg_items::bg_beach_back.create_bg(0, 0))
        , bg_main(bn::regular_bg_items::bg_beach_main.create_bg(0, 0))
        , bg_fore(bn::regular_bg_items::bg_beach_fore.create_bg(0, 0))
        , cam(bn::camera_ptr::create())
    {
    // bg
        bg_back.set_priority(3);
        bg_main.set_priority(2);
        bg_fore.set_priority(0);
        
    // player
        dino = bn::make_unique<Player>(lvl::PLAYER_START_X);
    
    // debug
        all_entity.emplace_back(bn::make_unique<Bag>(40));
        
        set_camera_for_all();
    }
    
    void Scene::set_camera_for_all()
    {
        dino->set_camera(cam);
        bg_main.set_camera(cam);
        bg_fore.set_camera(cam);
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
        
        bn::fixed camera_x  = x.multiplication(x).division(800);
        bn::fixed bg_fore_x = x.multiplication(x).division(500);
        
        cam.set_position    (is_negative ? -camera_x : camera_x, 0);
        bg_fore.set_position(is_negative ? bg_fore_x : -bg_fore_x, 0);
    }
    
    void Scene::manage_entity()
    {
        for(bn::unique_ptr<Entity>& entity : all_entity)
        {
            if(dino->is_attacking())
            {
                if(dino->get_atk_hitbox().intersects(entity->get_body_hitbox()))
                {
                    entity->take_damage(dino->get_pos().x() < entity->get_pos().x());
                }
            }
            
            if(entity->is_attacking())
            {
                if(entity->get_atk_hitbox().intersects(dino->get_body_hitbox()))
                {
                    dino->take_damage(entity->get_pos().x() < dino->get_pos().x());
                }
            }
            
            entity->update();
        }
        
        dino->update();
    }
}