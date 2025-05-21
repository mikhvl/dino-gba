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
        bg_back.set_priority(lvl::BG_BACK_PRIORITY);
        bg_main.set_priority(lvl::BG_MAIN_PRIORITY);
        bg_fore.set_priority(lvl::BG_FORE_PRIORITY);
        
    // player
        dino = bn::make_unique<Player>(lvl::PLAYER_START_X);
    
    // debug
        all_entity.emplace_back(bn::make_unique<Crab>(40));
        
        set_camera_entity();
    }
    
    void Scene::set_camera_entity()
    {
        bg_main.set_camera(cam);
        bg_fore.set_camera(cam);
        
        dino->set_camera(cam);
        for(bn::unique_ptr<Entity>& entity : all_entity) entity->set_camera(cam);
    }
    
    void Scene::update()
    {
        set_camera_position();
        manage_entity();
    }
    
    void Scene::set_camera_position()
    {
    // player x
        bn::fixed x = dino->get_pos().x();
        bool player_x_is_negative  = x < 0;
        
    // position target
        bn::fixed camera_x_target  = x.multiplication(x).division(lvl::CAMERA_PARALLAX_COEF);
        bn::fixed bg_fore_x_target = x.multiplication(x).division(lvl::BG_FORE_PARALLAX_COEF);
        camera_x_target  = player_x_is_negative ? -camera_x_target : camera_x_target;
        bg_fore_x_target = player_x_is_negative ? bg_fore_x_target : -bg_fore_x_target;
        
    // target easing
        bn::fixed camera_ease  = (camera_x_target - cam.x()).division(lvl::PARALLAX_EASE_COEF);
        bn::fixed bg_fore_ease = (bg_fore_x_target - bg_fore.x()).division(lvl::PARALLAX_EASE_COEF);
        
    // set position
        cam.set_position(cam.x() + camera_ease, 0);
        bg_fore.set_position(bg_fore.x() + bg_fore_ease, 0);
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