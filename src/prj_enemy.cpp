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
        body_hitbox = bn::rect(
            pos.x().round_integer(), pos.y().round_integer(),
            bag::BODY_SIZE.width(), bag::BODY_SIZE.height());
        spr.set_horizontal_flip(flip);
    }
    
    void Bag::update()
    {
        if(_damage_frames == 1) spr.set_tiles(spr_item.tiles_item(), 1);
        else if(_damage_frames == bag::wait_data::DAMAGE_STOP) spr.set_tiles(spr_item.tiles_item(), 0);
    }
    
    void Bag::take_damage()
    {
        if(_damage_frames == 0) _damage_frames = 1;
    }
    
    void Bag::count_frames()
    {
        if(_damage_frames > 0 && _damage_frames < bag::wait_data::DAMAGE_STOP) ++_damage_frames;
        else _damage_frames = 0;
    }
}