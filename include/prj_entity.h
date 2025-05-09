#ifndef PRJ_ENTITY_H
#define PRJ_ENTITY_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_optional.h"

#include "prj_const.h"

namespace prj
{
    class Entity
    {
    public:
        explicit Entity(bn::fixed x = 0, bn::fixed y = lvl::Y_LIM);
        virtual ~Entity() = default;
        
        bn::fixed_point& get_pos();
        bn::rect& get_body_hitbox();
        bn::optional<bn::rect>& get_atk_hitbox();
        
        virtual void update();
        virtual void take_damage();
        virtual bool is_attacking();
        virtual bool is_dead();
        
    protected:
        bn::fixed_point pos;
        bn::rect body_hitbox;
        bn::optional<bn::rect> atk_hitbox;
    };
}

#endif