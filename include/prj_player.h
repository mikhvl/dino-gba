#ifndef PRJ_PLAYER_H
#define PRJ_PLAYER_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_size.h"

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_camera_ptr.h"

#include "bn_vector.h"
#include "bn_sprite_builder.h"

#include "prj_const.h"
#include "prj_entity.h"

namespace prj
{
    class Player : public Entity
    {
    public:
        explicit Player(bn::fixed x = 0, bn::fixed y = lvl::Y_LIM, bool flip = false);
        
        void update() override;
        void take_damage() override;
        bool is_attacking() override;
        
        void attach_cam(const bn::camera_ptr& cam);
        
    private:
    // interaction
        bn::fixed x_speed = player::X_SPEED;
        bn::fixed y_speed = 0;
        bool _face_left;
        int _turn_frames = 0;
        int _atk_frames = 0;

    // state logic
        enum RUN_STATE  { start_run, full_run, end_run, not_run };
        enum JUMP_STATE { start_jump, full_jump, release_jump, end_jump, not_jump };
        enum FALL_STATE { start_fall, full_fall, end_fall, not_fall };
        
        RUN_STATE  _run  = not_run;
        JUMP_STATE _jump = not_jump;
        FALL_STATE _fall = not_fall;

    // sprite/animation item
        bn::sprite_item spr_item;
        bn::sprite_ptr spr;
        bn::sprite_animate_action<player::MAX_ANIM_FRAMES> act;
        
        bn::sprite_ptr box; // sprite offset test
        bn::sprite_builder hitbox_corner_builder;
        bn::vector<bn::sprite_ptr, 8> hitbox_corners;

    // state functions
        bool is_running();
        bool is_jumping();
        bool is_falling();
        bool is_on_ground();

    // update functions
        void set_face_left(bool flip);
        void input();
        void movement();
        void hitbox();
        void animation();
        void count_frames();
    };
}

#endif