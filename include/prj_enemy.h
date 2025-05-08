#ifndef PRJ_ENEMY_H
#define PRJ_ENEMY_H

#include "bn_sprite_item.h"
#include "bn_sprite_ptr.h"

#include "prj_const.h"
#include "prj_entity.h"

namespace prj
{
    class Bag : public Entity
    {
    public:
        Bag(bn::fixed x = 0, bn::fixed y = lvl::Y_LIM, bool flip = true);
        
    private:
        bn::sprite_item spr_item;
        bn::sprite_ptr spr;
    };
}

#endif