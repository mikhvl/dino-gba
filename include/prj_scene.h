#ifndef PRJ_SCENE_H
#define PRJ_SCENE_H

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
        bn::regular_bg_item bg_item;
        bn::regular_bg_ptr bg;
        
        bn::camera_ptr cam;
        
        bn::unique_ptr<Player> dino;
        bn::vector<bn::unique_ptr<Entity>, lvl::MAX_ENTITY> all_entity;
        
        void position_cam();
        void manage_entity();
    };
}

#endif