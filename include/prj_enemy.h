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
        explicit Bag(bn::fixed x = 0, bn::fixed y = lvl::Y_LIM, bool flip = true);
        
        void update() override;
        void take_damage(bool from_left = false) override;
        
    private:
        bn::sprite_animate_action<bag::MAX_ANIM_FRAMES> act;
        
        int _damage_frames = 0;
        void update_states();
    };
    
    class Crab : public Entity
    {
    public:
        explicit Crab(bn::fixed x = 0, bn::fixed y = lvl::Y_LIM, bool flip = true);
        
        void update() override;
        void take_damage(bool from_left = false) override;
        bool is_attacking() override;
        
    private:
        bn::fixed x_speed = crab::RUN_SPEED;
        
        bn::sprite_animate_action<crab::MAX_ANIM_FRAMES> act;
        
        //int _damage_frames = 0;
        //void update_states();
    };
}

#endif