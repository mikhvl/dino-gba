#ifndef PRJ_CONST_H
#define PRJ_CONST_H

#include "bn_fixed.h"
#include "bn_size.h"

#include "bn_sprite_items_dino.h"
#include "bn_sprite_items_bag.h"

#include "bn_sprite_items_gatito.h"
#include "bn_sprite_items_x_corner.h"

#include "bn_regular_bg_items_bg_default.h"

namespace prj
{
    namespace player
    {
        constexpr bn::size BODY_SIZE = {16, 16};
        constexpr bn::size ATK_SIZE = {32, 30};
        
        constexpr bn::fixed GRAVITY = 0.2;
        constexpr bn::fixed FRICTION = 0.2;
        
        constexpr bn::fixed X_SPEED = 2;
        constexpr bn::fixed START_JMP_SPEED = 4;
        constexpr bn::fixed RELEASE_JMP_SPEED = 1.3;
        constexpr bn::fixed ATK_SPEED = 4.6;
        
        constexpr int SPR_OFFSET_X = 4;
        constexpr int BODY_HITBOX_OFFSET_X = 3;
        constexpr int ATK_HITBOX_OFFSET_X = 16;
        
        constexpr int MAX_ANIM_FRAMES = 2;
        
        namespace wait_data
        {
            constexpr int ANIM_WAIT = 4;
            
            constexpr int TURN_STOP = 5;
        
            constexpr int ATK_FULL  = 10;
            constexpr int ATK_SLOW  = 14;
            constexpr int ATK_SLIDE = 16;
            constexpr int ATK_STOP  = 35;
        }
        
        namespace anim_data
        {
            constexpr uint16_t IDLE[] = {0, 0};
            constexpr uint16_t RUN[]  = {1, 2};
            constexpr uint16_t JUMP[] = {3, 3};
            constexpr uint16_t FALL[] = {4, 4};
            
            constexpr uint16_t TURN_GROUND[] = {5, 5};
            
            constexpr uint16_t ATK_START[] = {6, 6};
            constexpr uint16_t ATK_FULL[]  = {7, 7};
            constexpr uint16_t ATK_SLOW[]  = {8, 8};
            constexpr uint16_t ATK_SLIDE[] = {9, 9};
        }
    }
    
    namespace bag
    {
        constexpr bn::size BODY_SIZE = {18, 18};
        
        constexpr int MAX_ANIM_FRAMES = 2;
        
        namespace wait_data
        {
            constexpr int ANIM_WAIT = 4;
            constexpr int DAMAGE_STOP = 10;
        }
        
        namespace anim_data
        {
            constexpr uint16_t IDLE[]   = {0, 0};
            constexpr uint16_t DAMAGE[] = {1, 1};
        }
    }
    
    namespace lvl
    {
        constexpr bn::fixed PLAYER_START_X = 0;
        
        constexpr bn::fixed Y_LIM = 32;
        constexpr bn::fixed X_LIM = 114;
        
        constexpr int MAX_ENTITY = 10;
    }
}

#endif