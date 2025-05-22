#ifndef PRJ_ENEMY_H
#define PRJ_ENEMY_H

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "prj_const.h"
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
        explicit Crab(bool from_left);
        
        void update() override;
        void take_damage(bool from_left = false) override;
        
    private:
        bn::fixed x_speed = crab::speed::RUN_X;
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
}

#endif