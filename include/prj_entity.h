#ifndef PRJ_ENTITY_H
#define PRJ_ENTITY_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"

#include "bn_camera_ptr.h"

#include "prj_const.h"

namespace prj
{
    class Entity
    {
    public:
        explicit Entity
        (
            int entity_type,
            bn::fixed x = 0,
            bn::fixed y = lvl::Y_LIM,
            bn::sprite_item sprite_item = bn::sprite_items::gatito
        );
        virtual ~Entity() = default;
        
    // interaction
        int get_type();
        bn::fixed_point get_pos();
        bn::rect get_body_hitbox();
        bn::rect get_atk_hitbox();
        
    // for entity manager
        virtual void update();
        virtual void take_damage(bool from_left);
        
        bool is_dying();
        bool is_dead();
        virtual bool is_attacking();
        
    // for scene
        virtual void set_camera(const bn::camera_ptr& cam);
        
    protected:
        int type = entity::type::DEFAULT;
        
        bn::fixed_point pos;
        
        bn::rect body_hitbox;
        bn::rect atk_hitbox;
        
        enum DEATH_STATE { start_death, full_death, end_death, not_death };
        DEATH_STATE _death = not_death;
        
        bn::sprite_item spr_item;
        bn::sprite_ptr spr;
        bn::sprite_ptr shadow;
        
        void set_shadow_position();
    };
}

#endif