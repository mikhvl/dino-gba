#include "bn_core.h"

#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"

#include "bn_regular_bg_items_bg_temp.h"

#include "player.h"

int main()
{
    bn::core::init();
    
    prj::Player dino;
    bn::regular_bg_ptr bg = bn::regular_bg_items::bg_temp.create_bg(0, 0);

    while(true)
    {
        dino.update();
        bn::core::update();
    }
}
