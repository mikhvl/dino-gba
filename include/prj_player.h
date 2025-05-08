#ifndef PRJ_PLAYER_H
#define PRJ_PLAYER_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "prj_const.h"

namespace prj
{
    class Player
    {
    public:
        Player(bn::fixed x = 0, bn::fixed y = lvl::Y_LIM, bool flip = false);
        void update();
        
        void get_hurt(bool from_left);
        bool is_attacking();
        bn::rect& get_body_hitbox();
        bn::rect& get_atk_hitbox();
        
    private:
    // interaction
        bn::fixed_point pos;
        bn::fixed x_speed = 2;
        bn::fixed y_speed = 0;
        
        bn::rect body_hitbox;
        bn::rect atk_hitbox;
    
    // animation
        int anim_wait = 4;
        int _turn_frames = 0;
        int _atk_frames = 0;
        bool _face_left = false;

    // state logic
        enum RUN_STATE  { start_run, full_run, end_run, not_run };
        enum JUMP_STATE { start_jump, full_jump, release_jump, end_jump, not_jump };
        enum FALL_STATE { start_fall, full_fall, end_fall, not_fall };
        enum ATK_STATE  { start_atk, full_atk, end_atk, not_atk };
        
        RUN_STATE  _run  = not_run;
        JUMP_STATE _jump = not_jump;
        FALL_STATE _fall = not_fall;
        ATK_STATE  _atk  = not_atk;

    // sprite/animation item
        bn::sprite_item spr_item;
        bn::sprite_ptr spr;
        bn::sprite_animate_action<player::MAX_ANIM_FRAMES> act;
        bn::sprite_ptr box; // hitbox test

    // state functions
        bool is_running();
        bool is_jumping();
        bool is_falling();
        bool is_on_ground();

    // update functions
        void set_face_left(bool flip);
        
        void input();
        void hitbox();
        void movement();
        void animation();
        
        void box_update();
    };
}

#endif