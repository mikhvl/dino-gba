#ifndef PRJ_CONST_H
#define PRJ_CONST_H

#include "bn_fixed.h"

#include "bn_sprite_item.h"
#include "bn_regular_bg_item.h"

#include "bn_sprite_items_dino_main.h"
#include "bn_sprite_items_bag.h"
#include "bn_sprite_items_gatito.h"

#include "bn_regular_bg_items_bg_default.h"

namespace prj
{
    namespace player
    {
        constexpr bn::fixed GRAVITY = 0.2;
        constexpr bn::fixed START_JMP_SPEED = 4;
        constexpr bn::fixed RELEASE_JMP_SPEED = 1.3;
        
        constexpr int SPR_OFFSET_X = 4;
        constexpr int MAX_ANIM_FRAMES = 8;
        constexpr int TURN_FRAMES_STOP = 5;
        constexpr int ATK_FRAMES_STOP = 20;
        
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
    }
}

#endif