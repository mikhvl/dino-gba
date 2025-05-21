#include "prj_enemy.h"

namespace prj
{
    Bag::Bag
    (
        bn::fixed x,
        bn::fixed y,
        bool flip
    )
        : Entity(x, y, bn::sprite_items::bag)
        , act(bn::sprite_animate_action<bag::MAX_ANIM_FRAMES>::once(
                spr, bag::ANIM_WAIT, spr_item.tiles_item(),
                bag::anim::IDLE))
    {
    // hitboxes
        body_hitbox = bn::rect(
            pos.x().round_integer(), pos.y().round_integer(),
            bag::BODY_SIZE.width(), bag::BODY_SIZE.height());
        atk_hitbox = body_hitbox;
    
    // initial logic
        spr.set_horizontal_flip(flip);
    }
    
    void Bag::update()
    {
        if(_damage_frames == 1)
        {
            act = bn::sprite_animate_action<bag::MAX_ANIM_FRAMES>::once
            (
                spr, bag::ANIM_WAIT, spr_item.tiles_item(),
                bag::anim::DAMAGE
            );
        }
        else if(_damage_frames == bag::wait::DAMAGE_STOP)
        {
            act = bn::sprite_animate_action<bag::MAX_ANIM_FRAMES>::once
            (
                spr, bag::ANIM_WAIT, spr_item.tiles_item(),
                bag::anim::IDLE
            );
        }
        
        if(!act.done()) act.update();
        
        update_states();
    }
    
    void Bag::take_damage(bool from_left)
    {
        if(from_left) {}
        if(_damage_frames == 0) _damage_frames = 1;
    }
    
    bool Bag::is_attacking() { return true; }
    
    void Bag::update_states()
    {
        if(_damage_frames > 0 && _damage_frames < bag::wait::DAMAGE_STOP) ++_damage_frames;
        else _damage_frames = 0;
    }
}