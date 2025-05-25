#ifndef PRJ_ENEMY_H
#define PRJ_ENEMY_H

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "prj_config.h"
#include "prj_entity.h"

namespace prj
{
    class Bag : public Entity
    {
    public:
        explicit Bag(bn::fixed x = 0, bool flip = true);
        
        void update() override;
        void take_damage(bool from_left = false) override;
        
    private:
        int _damage_frames = 0;
        
        void run_animation();
        void update_states();
    };
    
    class Crab : public Entity
    {
    public:
        explicit Crab(bool from_left = false, bn::fixed speed = crab::speed::RUN_X_DEFAULT);
        
        void update() override;
        void take_damage(bool from_left = false) override;
        
    private:
        bn::fixed x_speed = 0;
        bn::fixed y_speed = 0;
        
        bool _face_left = false;
        bool _entering = true;
        
        bn::sprite_animate_action<crab::MAX_ANIM_FRAMES> act;
        
        void apply_movement();
        void set_hitbox_position();
        void set_sprite_position();
        void run_animation();
        void update_states();
    };
    
    class Starfish : public Entity
    {
    public:
        explicit Starfish(bool from_left = false, bn::fixed speed = starfish::speed::RUN_X_DEFAULT);
        
        void update() override;
        void take_damage(bool from_left = false) override;
        
    private:
        bn::fixed x_speed = 0;
        bn::fixed y_speed = 0;
        
        bool _face_left = false;
        bool _entering = true;
        
        enum JUMP_STATE { start_jump, full_jump, not_jump };
        JUMP_STATE _jump = start_jump;
        
        bn::sprite_animate_action<starfish::MAX_ANIM_FRAMES> act;
        
        void set_face_left(bool flip);
        
        void apply_movement();
        void set_hitbox_position();
        void set_sprite_position();
        void run_animation();
        void update_states();
    };
    
    class Bird : public Entity
    {
    public:
        explicit Bird
            (
                bool from_left = false,
                bn::fixed speed = bird::speed::RUN_X_DEFAULT,
                bn::fixed height = bird::START_Y_MIN
            );
        
        void update() override;
        bool is_attacking() override;
        
    private:
        bn::fixed x_speed = 0;
        
        bn::fixed coco_x_speed = 0;
        bn::fixed coco_y_speed = 0;
        
        bool _face_left = false;
        bool _entering = true;
        
        enum JUMP_STATE { start_jump, full_jump, not_jump };
        JUMP_STATE _jump = start_jump;
        
        bn::sprite_animate_action<bird::MAX_ANIM_FRAMES> act;
        
        void set_face_left(bool flip);
        
        void apply_movement();
        void set_hitbox_position();
        void set_sprite_position();
        void run_animation();
        void update_states();
    };
}

#endif