#ifndef PRJ_ENTITY_H
#define PRJ_ENTITY_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_optional.h"

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
            bn::fixed x = 0,
            bn::fixed y = lvl::Y_LIM,
            bn::sprite_item sprite_item = bn::sprite_items::bag
        );
        virtual ~Entity() = default;
        
    // interaction
        bn::fixed_point& get_pos();
        bn::rect& get_body_hitbox();
        bn::optional<bn::rect>& get_atk_hitbox();
        
    // for entity manager
        virtual void update();
        virtual void take_damage(bool from_left);
        virtual bool is_attacking();
        virtual bool is_dead();
        
    // for scene
        virtual void set_camera(const bn::camera_ptr& cam);
        
    protected:
        bn::fixed_point pos;
        
        bn::rect body_hitbox;
        bn::optional<bn::rect> atk_hitbox;
        
        bn::sprite_item spr_item;
        bn::sprite_ptr spr;
    };
}

#endif