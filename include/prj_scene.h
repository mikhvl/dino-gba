#ifndef PRJ_SCENE_H
#define PRJ_SCENE_H

#include "bn_unique_ptr.h"

#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_item.h"

#include "prj_player.h"

namespace prj
{
    class Scene
    {
    public:
        Scene();
        void update();
        
    private:
        bn::regular_bg_item bg_art;
        bn::regular_bg_ptr bg;
        
        bn::unique_ptr<prj::Player> dino_ptr;
    };
}

#endif