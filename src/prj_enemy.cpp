#include "prj_enemy.h"

#include "bn_math.h"

namespace prj
{
// Bag
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
    
    void Bag::update_states()
    {
        if(_damage_frames > 0 && _damage_frames < bag::wait::DAMAGE_STOP) ++_damage_frames;
        else _damage_frames = 0;
    }
    
// Crab
    Crab::Crab
    (
        bn::fixed x,
        bn::fixed y,
        bool flip
    )
        : Entity(x, y, bn::sprite_items::crab)
        , act(bn::sprite_animate_action<crab::MAX_ANIM_FRAMES>::forever(
                spr, crab::ANIM_WAIT, spr_item.tiles_item(),
                crab::anim::RUN))
    {
    // hitboxes
        body_hitbox = bn::rect(
            pos.x().round_integer(), pos.y().round_integer(),
            crab::BODY_SIZE.width(), crab::BODY_SIZE.height());
        atk_hitbox = body_hitbox;
    
    // initial logic
        spr.set_horizontal_flip(flip);
    }
    
    void Crab::update()
    {
        pos.set_x(pos.x() + x_speed);
        spr.set_x(pos.x());
        
        if(bn::abs(pos.x()) > lvl::X_LIM) x_speed = -x_speed;
        
        set_shadow_position();
        
        if(!act.done()) act.update();
    }
    
    void Crab::take_damage(bool from_left) {}
    bool Crab::is_attacking() { return false; }
}