#include "prj_enemy.h"

namespace prj
{
    Bag::Bag
    (
        bn::fixed x,
        bn::fixed y,
        bool flip
    )
        : Entity(x, y)
        , spr_item(bn::sprite_items::bag)
        , spr(spr_item.create_sprite(pos))
    {
        spr.set_horizontal_flip(flip);
    }
}