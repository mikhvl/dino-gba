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
        
    // initialize all_entity with nullptr
        while(all_entity.size() < all_entity.max_size())
        {
            all_entity.emplace_back(bn::unique_ptr<Entity>(nullptr));
        }
        
    // camera link
        bg_main.set_camera(cam);
        bg_fore.set_camera(cam);
        dino->set_camera(cam);
        
    // set seed (not random at first)
        Random.update();
        
    // music
        bn::music_items::mermaid_falls_loop.play();
    }
    
    void Scene::update()
    {
        set_camera_position();
        spawn_entity();
        manage_entity();
    }
    
    void Scene::set_camera_position()
    {
    // player x
        bn::fixed x = dino->get_pos().x();
        bool player_x_is_negative = x < 0;
        
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
    
    void Scene::spawn_entity()
    {
        if(_spawn_frames % lvl::SPAWN_FRAMES_CYCLE == 0)
        {
            for(bn::unique_ptr<Entity>& entity : all_entity)
            {
                if(!entity)
                {
                    if(Random.get_bool())
                    {
                        entity = bn::make_unique<Crab>
                            (
                                Random.get_bool(),
                                Random.get_fixed(crab::speed::RUN_X_MIN, crab::speed::RUN_X_MAX)
                            );
                    }
                    else
                    {
                        entity = bn::make_unique<Starfish>
                            (
                                Random.get_bool(),
                                Random.get_fixed(starfish::speed::RUN_X_MIN, starfish::speed::RUN_X_MAX)
                            );
                    }
                    
                    entity->set_camera(cam);
                    break;
                }
            }
        }
        
        if(_spawn_frames > lvl::SPAWN_FRAMES_MAX) _spawn_frames = 1;
        else ++_spawn_frames;
    }
    
    void Scene::manage_entity()
    {
        for(bn::unique_ptr<Entity>& entity : all_entity)
        {
            if(!entity) continue;
            
            if(dino->is_attacking())
            {
                if(dino->get_atk_hitbox().intersects(entity->get_body_hitbox()))
                {
                    entity->take_damage(dino->get_pos().x() < entity->get_pos().x());
                }
                Random.update();
            }
            
            if(entity->is_attacking())
            {
                if(entity->get_atk_hitbox().intersects(dino->get_body_hitbox()))
                {
                    dino->take_damage(entity->get_pos().x() < dino->get_pos().x());
                }
            }
            
            entity->update();
            
            if(entity->is_dead()) entity.reset();
        }
        
        dino->update();
    }
}