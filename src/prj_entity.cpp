#include "prj_entity.h"

namespace prj
{
    Bag::Bag
    (
        bn::fixed x,
        bn::fixed y,
        bool flip
    )
        : pos(x < 0 ? bn::max(x, -lvl::X_LIM) : bn::min(x, lvl::X_LIM),
                bn::min(y, lvl::Y_LIM))
        , spr_item(bn::sprite_items::bag)
        , spr(spr_item.create_sprite(pos)) 
    {
        spr.set_horizontal_flip(flip);
    }
}