#include "prj_enemy.h"

#include "bn_math.h"

namespace prj
{
// Bag
    Bag::Bag
    (
        bn::fixed x,
        bool flip
    )
        : Entity(x, lvl::Y_LIM, bn::sprite_items::bag)
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
        run_animation();
        update_states();
    }
    
    void Bag::take_damage(bool from_left)
    {
        spr.set_horizontal_flip(from_left);
        if(_damage_frames == 0) _damage_frames = 1;
    }
    
    void Bag::run_animation()
    {
        if(_damage_frames == 1)
        {
            spr.set_tiles(bn::sprite_items::bag.tiles_item(), bag::tile::DAMAGE);
        }
        else if(_damage_frames == bag::wait::DAMAGE_STOP)
        {
            spr.set_tiles(bn::sprite_items::bag.tiles_item(), bag::tile::IDLE);
        }
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
        bn::fixed y
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
    }
    
    void Crab::update()
    {
        apply_movement();
        set_sprite_position();
        set_shadow_position();
        run_animation();
    }
    
    //void Crab::take_damage(bool from_left) {}
    bool Crab::is_attacking() { return false; }
    
    void Crab::apply_movement()
    {
        pos.set_x(pos.x() + x_speed);
        if(bn::abs(pos.x()) > lvl::X_LIM) x_speed = -x_speed;
    }
    
    void Crab::set_sprite_position() { spr.set_position(pos); }
    
    void Crab::run_animation()
    {
        if(!act.done()) act.update();
    }
}