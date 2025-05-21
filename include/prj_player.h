#ifndef PRJ_PLAYER_H
#define PRJ_PLAYER_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_size.h"

#include "bn_sprite_animate_actions.h"

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
        void take_damage(bool from_left) override;
        bool is_attacking() override;
        
        void set_camera(const bn::camera_ptr& cam) override;
        
    private:
    // movement
        bn::fixed x_speed = player::speed::RUN_X;
        bn::fixed y_speed = 0;
        
    // state logic
        bool _face_left;
        bool _queue_jump = false;
        bool _stun = false;
        
        int _turn_frames = 0;
        int _atk_frames  = 0;
        int _inv_frames  = 0;
        
        enum RUN_STATE  { start_run, full_run, end_run, not_run };
        enum JUMP_STATE { start_jump, full_jump, release_jump, end_jump, not_jump };
        enum FALL_STATE { start_fall, full_fall, end_fall, not_fall };
        enum DASH_STATE { start_dash, full_dash, not_dash };
        enum SPIN_STATE { start_spin, full_spin, end_spin, not_spin };
        
        RUN_STATE  _run  = not_run;
        JUMP_STATE _jump = not_jump;
        FALL_STATE _fall = not_fall;
        DASH_STATE _dash = not_dash;
        SPIN_STATE _spin = not_spin;

    // sprite/animation
        bn::sprite_animate_action<player::MAX_ANIM_FRAMES> act;
        
    // hitbox debug
        bn::sprite_builder hitbox_corner_builder;
        bn::vector<bn::sprite_ptr, 8> hitbox_corners;

    // state functions
        bool is_running();
        bool is_jumping();
        bool is_falling();
        bool is_dashing();
        bool is_spinning();
        bool is_on_ground();

    // update functions
        void set_face_left(bool flip);
        void set_hitbox_size(bool is_spin = false);
        void stun(bool from_left);
        
        void update_states();
        void process_input();
        void apply_movement();
        void set_hitbox_position();
        void set_sprite_position();
        void run_animation();
    };
}

#endif