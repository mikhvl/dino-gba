#include "bn_core.h"

#include "bn_regular_bg_item.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"

#include "player.h"

int main()
{
    bn::core::init();
    
    prj::Player dino;

    while(true)
    {
        dino.update();
        bn::core::update();
    }
}
