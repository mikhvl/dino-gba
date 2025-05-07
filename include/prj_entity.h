#ifndef PRJ_ENTITY_H
#define PRJ_ENTITY_H

#include "bn_fixed.h"
#include "bn_fixed_point.h"

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"

#include "prj_const.h"

namespace prj
{
    class Bag
    {
    public:
        Bag(bn::fixed x = 0, bn::fixed y = lvl::Y_LIM, bool flip = true);
        
    private:
        bn::fixed_point pos;
        
        bn::sprite_item spr_item;
        bn::sprite_ptr spr;
    };
}

#endif