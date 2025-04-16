#ifndef PRJ_CONST_H
#define PRJ_CONST_H

#include "bn_fixed.h"

#include "bn_sprite_item.h"
#include "bn_regular_bg_item.h"

#include "bn_sprite_items_dino_main.h"
#include "bn_regular_bg_items_bg_default.h"

namespace prj
{
    namespace lvl
    {
        constexpr bn::fixed Y_LIM = 32;
        constexpr bn::fixed X_LIM = 114;
        
        constexpr bn::regular_bg_item DEFAULT_BG_ITEM = bn::regular_bg_items::bg_default;
    }
    
    namespace spr
    {
        constexpr bn::sprite_item DINO_MAIN_SPR_ITEM = bn::sprite_items::dino_main;
    }
}

#endif