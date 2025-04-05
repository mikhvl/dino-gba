#include "bn_core.h"

#include "bn_unique_ptr.h"

#include "bn_regular_bg_item.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"

#include "prj_player.h"
#include "prj_const.h"

class Scene
{
public:
    Scene()
        : bg_art(prj::DEFAULT_BG_ITEM)
        , bg(bn::regular_bg_items::bg_temp.create_bg(0, 0))
    {
        dino_ptr = bn::make_unique<prj::Player>();
    }
    
    void update()
    {
        dino_ptr->update();
    }
    
private:
    bn::regular_bg_item bg_art;
    bn::regular_bg_ptr bg;
    
    bn::unique_ptr<prj::Player> dino_ptr;
};

int main()
{
    bn::core::init();
    
    Scene level;

    while(true)
    {
        level.update();
        bn::core::update();
    }
}
