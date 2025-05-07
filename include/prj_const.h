#ifndef PRJ_CONST_H
#define PRJ_CONST_H

#include "bn_fixed.h"

#include "bn_sprite_item.h"
#include "bn_regular_bg_item.h"

#include "bn_sprite_items_dino_main.h"
#include "bn_regular_bg_items_bg_default.h"

namespace prj
{
    namespace player
    {
        constexpr int MAX_ANIM_FRAMES = 8;
        
        namespace anim_data
        {
            constexpr uint16_t IDLE[] = {0, 0};
            constexpr uint16_t RUN[]  = {1, 2};
            constexpr uint16_t JUMP[] = {3, 3};
            constexpr uint16_t FALL[] = {4, 4};
            constexpr uint16_t TURN[] = {5, 5};
        }
    }
    
    namespace lvl
    {
        constexpr bn::fixed Y_LIM = 32;
        constexpr bn::fixed X_LIM = 114;
        
        constexpr bn::regular_bg_item DEFAULT_BG_ITEM = bn::regular_bg_items::bg_default;
    }
    
    namespace spr
    {
        constexpr bn::sprite_item DINO_MAIN_SPR_ITEM = bn::sprite_items::dino_main;
    }
}

#endif