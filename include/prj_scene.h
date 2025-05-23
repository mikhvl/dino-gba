#ifndef PRJ_SCENE_H
#define PRJ_SCENE_H

#include "bn_random.h"
#include "bn_unique_ptr.h"
#include "bn_vector.h"

#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_item.h"
#include "bn_camera_ptr.h"

#include "prj_entity.h"
#include "prj_player.h"
#include "prj_enemy.h"

namespace prj
{
    class Scene
    {
    public:
        Scene();
        void update();
        
    private:
    // bg art
        bn::regular_bg_ptr bg_back;
        bn::regular_bg_ptr bg_main;
        bn::regular_bg_ptr bg_fore;
        
    // utility
        bn::camera_ptr cam;
        bn::random Random;
        
    // entities
        bn::unique_ptr<Player> dino;
        bn::vector<bn::unique_ptr<Entity>, lvl::MAX_ENTITY> all_entity;
        
    // for entity spawner
        int _spawn_frames = 0;
        
    // update functions
        void spawn_entity();
        void manage_entity();
        void set_camera_position();
    };
}

#endif