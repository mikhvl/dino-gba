#ifndef PRJ_GAME_CONST_H
#define PRJ_GAME_CONST_H

#include "bn_fixed.h"

#include "bn_sprite_item.h"

#include "bn_sprite_items_dino.h"

namespace prj
{
    constexpr bn::fixed Y_LIM = 32;
    constexpr bn::fixed X_LIM = 114;
    
    constexpr bn::sprite_item DINO_SPR_ITEM = bn::sprite_items::dino;
}

#endif