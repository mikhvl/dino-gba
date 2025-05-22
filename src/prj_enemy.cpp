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
        bool from_left
    )
        : Entity
            (
                from_left ? -lvl::X_LIM - entity::OFFSCREEN_X : lvl::X_LIM + entity::OFFSCREEN_X,
                lvl::Y_LIM,
                bn::sprite_items::crab
            )
        , _face_left(!from_left)
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
        set_hitbox_position();
        set_sprite_position();
        set_shadow_position();
        run_animation();
        update_states();
    }
    
    void Crab::take_damage(bool from_left)
    {
        if(!is_dying())
        {
            _death = start_death;
            _face_left = !from_left;
            shadow.set_visible(false);
        }
    }
    
    void Crab::apply_movement()
    {
    // death jump
        if(_death == start_death)
        {
            x_speed = crab::speed::DAMAGE_X;
            y_speed = crab::speed::DAMAGE_Y;
        }
        
    // death jump trajectory
        if(is_dying() && (pos.y() < lvl::Y_DEATH_LIM)) y_speed -= force::GRAVITY;
        
    // set position
        pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
        pos.set_y(pos.y() - y_speed);
        
    // level bounds
        if((bn::abs(pos.x()) > lvl::X_LIM) && !is_dying() && !_entering) _face_left = !_face_left;
        
    // spawn enter toggle
        if(_entering && (bn::abs(pos.x()) < lvl::X_LIM)) _entering = false;
    }
    
    void Crab::set_hitbox_position()
    {
        body_hitbox.set_position(pos.x().round_integer(), pos.y().round_integer() + crab::HITBOX_OFFSET_Y);
        atk_hitbox = body_hitbox;
    }
    
    void Crab::set_sprite_position() { spr.set_position(pos); }
    
    void Crab::run_animation()
    {
        if(_death == start_death)
        {
            act = bn::sprite_animate_action<crab::MAX_ANIM_FRAMES>::once
                (
                    spr, crab::ANIM_WAIT, spr_item.tiles_item(),
                    crab::anim::DAMAGE
                );
        }
        
        if(!act.done()) act.update();
    }
    
    void Crab::update_states()
    {
        if(_death == start_death) _death = full_death;
        if(pos.y() > lvl::Y_DEATH_LIM) _death = end_death;
    }
}