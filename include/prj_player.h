#ifndef PRJ_PLAYER_H
#define PRJ_PLAYER_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "prj_const.h"

namespace prj
{
    class Player
    {
    public:
        Player(bn::fixed x = 0, bn::fixed y = prj::Y_LIM, bool flip = false);
        void update();
        
    private:
    // some player constants
        const bn::fixed g = 0.2;
        const bn::fixed start_y_speed = 4;
        const bn::fixed release_y_speed = 1.3;
        const bn::fixed x_speed = 2;
        const int anim_frames = 4;
        const int turn_frames_max = 10;
        int spr_offset_x = 4; // sprite flip adjustment

    // player variables
        bn::fixed_point pos;
        bn::fixed y_speed = 0;
        bool _face_left = false;
        int _turn_frames = 0;

    // state logic
        enum run_states  { start_run, full_run, end_run, not_run };
        enum jump_states { start_jump, full_jump, release_jump, end_jump, not_jump };
        enum fall_states { start_fall, full_fall, end_fall, not_fall };
        enum atk_states  { start_atk, full_atk, end_atk, not_atk };
        
        run_states  _run  = not_run;
        jump_states _jump = not_jump;
        fall_states _fall = not_fall;
        atk_states  _atk  = not_atk;

    // sprite/animation item
        bn::sprite_item spr_item;
        bn::sprite_ptr spr;
        bn::sprite_animate_action<2> act;
        bn::sprite_ptr box; // hitbox test

    // state functions
        bool is_running();
        bool is_jumping();
        bool is_falling();
        bool is_on_ground();

    // update functions
        void set_face_left(bool flip);
        
        void input();
        void attack();
        void movement();
        void animation();
        void box_update();
    };
}

#endif