#ifndef PRJ_CONFIG_H
#define PRJ_CONFIG_H

#include "bn_fixed.h"
#include "bn_size.h"

#include "bn_sprite_items_dino.h"
#include "bn_sprite_items_bag.h"
#include "bn_sprite_items_crab.h"
#include "bn_sprite_items_starfish.h"
#include "bn_sprite_items_toucan.h"

#include "bn_sprite_items_shadow.h"
#include "bn_sprite_items_coconut.h"

#include "bn_sprite_items_gatito.h"
#include "bn_sprite_items_x_corner.h"

#include "bn_regular_bg_items_bg_default.h"
#include "bn_regular_bg_items_bg_beach_main.h"
#include "bn_regular_bg_items_bg_beach_back.h"
#include "bn_regular_bg_items_bg_beach_fore.h"

#include "bn_music_items.h"

namespace prj
{
    namespace lvl
    {
        constexpr unsigned LUCKY_SEED = 3;
        
        constexpr bn::fixed PLAYER_START_X = 0;
        
        constexpr bn::fixed Y_LIM = 16;
        constexpr bn::fixed Y_DEATH_LIM = 60;
        constexpr bn::fixed X_LIM = 114;
        
        constexpr int MAX_ENTITY = 10;
        
        namespace entity_threshold // 0 - 100
        {
            constexpr int CRAB     = 40;  //50;
            constexpr int STARFISH = 80;  //90;
            constexpr int BIRD     = 100; //100
        }
        
        constexpr int BG_FORE_PRIORITY = 0;
        constexpr int ENTITY_PRIORITY  = 1;
        constexpr int BG_MAIN_PRIORITY = 2;
        constexpr int BG_BACK_PRIORITY = 3;
        
        constexpr int CAMERA_PARALLAX_COEF = 800;
        constexpr int BG_FORE_PARALLAX_COEF = 500;
        
        constexpr int PARALLAX_EASE_COEF = 10;
        
        constexpr int SPAWN_FRAMES_MAX = 10000;
        constexpr int SPAWN_FRAMES_CYCLE_MIN = 100;
        constexpr int SPAWN_FRAMES_CYCLE_MAX = 160;
        constexpr int SPAWN_FRAMES_REDUCTION = 5;
    }
    
    namespace force
    {
        constexpr bn::fixed GRAVITY = 0.2;
        constexpr bn::fixed FRICTION = 0.16;
        constexpr bn::fixed AIR_FRICTION = 0.1;
        constexpr bn::fixed SPIN_FRICTION = 0.4;
    }
    
    namespace entity
    {
        namespace type
        {
            constexpr int DEFAULT  = -128;
            
            constexpr int PLAYER   = -1;
            
            constexpr int BAG      = 0;
            constexpr int CRAB     = 1;
            constexpr int STARFISH = 2;
            constexpr int BIRD     = 3;
        }
        
        namespace type_threshold // 0 - 100
        {
            constexpr int CRAB     = 50;
            constexpr int STARFISH = 90;
            constexpr int BIRD     = 100;
        }
        
        constexpr bn::fixed OFFSCREEN_X = 32;
        
        constexpr int PLAYER_Z_ORDER = 0;
        constexpr int ENTITY_Z_ORDER = 1;
        constexpr int SHADOW_Z_ORDER = 2;
        
        constexpr bn::fixed SHADOW_Y_OFFSET = 16;
        
        constexpr bn::fixed SHADOW_HEIGHT_SMALL  = 38;
        constexpr bn::fixed SHADOW_HEIGHT_MEDIUM = 20;
        
        constexpr bn::fixed SHADOW_X_COEF = 2.5;
        constexpr bn::fixed SHADOW_Y_COEF = 12;
        
        namespace shadow_tile
        {
            constexpr int SMALL = 2;
            constexpr int MEDIUM = 1;
            constexpr int BIG = 0;
        }
    }
    
    namespace player
    {
        constexpr bn::size BODY_SIZE = {16, 16};
        constexpr bn::size ATK_SIZE  = {24, 30};
        constexpr bn::size SPIN_SIZE = {16, 16};
        
        constexpr int SPR_OFFSET_X = 4;
        constexpr int BODY_HITBOX_OFFSET_X = 3;
        constexpr int ATK_HITBOX_OFFSET_X = 13;
        
        constexpr int MAX_ANIM_FRAMES = 3;
        constexpr int ANIM_WAIT = 4;
        constexpr int SPIN_ANIM_WAIT = 3;
        
        namespace speed
        {
            constexpr bn::fixed RUN_X = 2;
            
            constexpr bn::fixed JUMP_START_Y = 4;
            constexpr bn::fixed JUMP_RELEASE_Y = 1.3;
            
            constexpr bn::fixed DASH_X = 4.6;
            constexpr bn::fixed DASH_Y = 3;
            
            constexpr bn::fixed STUN_X = 1.6;
            constexpr bn::fixed STUN_Y = 3;
            
            constexpr bn::fixed SPIN_X = 2.7;
            constexpr bn::fixed SPIN_Y = 1;
            constexpr bn::fixed SPIN_STOP_X = 2;
        }
        
        namespace wait
        {
            constexpr int TURN_STOP = 5;
        
            constexpr int ATK_FULL     = 10;
            constexpr int ATK_SLOW     = 14;
            constexpr int ATK_SLIDE    = 16;
            constexpr int ATK_INV_STOP = 22;
            constexpr int ATK_STOP     = 35;
            
            constexpr int INV_WAIT = 5;
            constexpr int INV_STOP = 80;
        }
        
        namespace anim
        {
            constexpr uint16_t IDLE[]        = {0, 0};
            constexpr uint16_t RUN[]         = {1, 2};
            
            constexpr uint16_t JUMP[]        = {4, 4};
            constexpr uint16_t FALL[]        = {5, 5};
            constexpr uint16_t JUMP_DASH[]   = {6, 6};
            
            constexpr uint16_t TURN_GROUND[] = {3, 3};
            constexpr uint16_t TURN_AIR[]    = {7, 7};
            
            constexpr uint16_t ATK_START[]   = {8, 8};
            constexpr uint16_t ATK_FULL[]    = {9, 9};
            constexpr uint16_t ATK_SLOW[]    = {10, 10};
            constexpr uint16_t ATK_SLIDE[]   = {11, 11};
            
            constexpr uint16_t ATK_SPIN[]    = {12, 13, 14};
            
            constexpr uint16_t STUN[]        = {15, 15};
        }
    }
    
    namespace bag
    {
        constexpr bn::size BODY_SIZE = {16, 16};
        
        namespace wait
        {
            constexpr int DAMAGE_STOP = 10;
        }
        
        namespace tile
        {
            constexpr int IDLE   = 0;
            constexpr int DAMAGE = 1;
        }
    }
    
    namespace crab
    {
        constexpr bn::size BODY_SIZE = {26, 16};
        constexpr int HITBOX_OFFSET_Y = -2;
        
        constexpr int MAX_ANIM_FRAMES = 2;
        constexpr int ANIM_WAIT = 6;
        
        namespace speed
        {
            constexpr bn::fixed RUN_X_DEFAULT = 1;
            constexpr bn::fixed RUN_X_MIN = 0.9;
            constexpr bn::fixed RUN_X_MAX = 1.2;
            
            constexpr bn::fixed DAMAGE_X = 2.2;
            constexpr bn::fixed DAMAGE_Y = 3;
        }
        
        namespace anim
        {
            constexpr uint16_t RUN[]    = {0, 1};
            constexpr uint16_t DAMAGE[] = {2, 2};
        }
    }
    
    namespace starfish
    {
        constexpr bn::size BODY_SIZE = {20, 20};
        
        constexpr int MAX_ANIM_FRAMES = 8;
        constexpr int ANIM_WAIT = 3;
        
        namespace speed
        {
            constexpr bn::fixed RUN_X_DEFAULT = 1;
            constexpr bn::fixed RUN_X_MIN = 1;
            constexpr bn::fixed RUN_X_MAX = 1.8;
            
            constexpr bn::fixed JUMP_Y = 4;
            
            constexpr bn::fixed DAMAGE_X = 2.2;
            constexpr bn::fixed DAMAGE_Y = 3;
        }
        
        namespace anim
        {
            constexpr uint16_t RUN[]    = {0, 1, 2, 3, 4, 5, 6, 7};
            constexpr uint16_t DAMAGE[] = {8, 8};
        }
    }
    
    namespace bird
    {
        constexpr bn::size COCO_SIZE = {10, 10};
        
        constexpr bn::fixed START_Y_MIN = -50;
        constexpr bn::fixed START_Y_MAX = -60;
        
        constexpr int MAX_ANIM_FRAMES = 2;
        constexpr int ANIM_WAIT = 8;
        
        namespace speed
        {
            constexpr bn::fixed RUN_X_DEFAULT = 1;
            constexpr bn::fixed RUN_X_MIN = 1;
            constexpr bn::fixed RUN_X_MAX = 1.8;
            
            constexpr bn::fixed COCO_X = 2.2;
            constexpr bn::fixed COCO_Y = 3;
        }
        
        namespace anim
        {
            constexpr uint16_t FLY[] = {0, 1};
        }
    }
}

#endif