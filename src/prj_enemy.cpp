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
        : Entity(entity::type::BAG, x, lvl::Y_LIM, bn::sprite_items::bag)
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
        bool from_left,
        bn::fixed speed
    )
        : Entity
            (
                entity::type::CRAB,
                from_left ? -lvl::X_LIM - entity::OFFSCREEN_X : lvl::X_LIM + entity::OFFSCREEN_X,
                lvl::Y_LIM,
                bn::sprite_items::crab
            )
        , x_speed(speed)
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
        if(is_dying()) y_speed -= force::GRAVITY;
        
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
        if(_death == start_death)      _death = full_death;
        if(pos.y() > lvl::Y_DEATH_LIM) _death = end_death;
    }

// Starfish
    Starfish::Starfish
    (
        bool from_left,
        bn::fixed speed
    )
        : Entity
            (
                entity::type::STARFISH,
                from_left ? -lvl::X_LIM - entity::OFFSCREEN_X : lvl::X_LIM + entity::OFFSCREEN_X,
                lvl::Y_LIM,
                bn::sprite_items::starfish
            )
        , x_speed(speed)
        , act(bn::sprite_animate_action<starfish::MAX_ANIM_FRAMES>::forever(
                spr, starfish::ANIM_WAIT, spr_item.tiles_item(),
                starfish::anim::RUN))
    {
    // initial logic
        set_face_left(!from_left);
        
    // hitboxes
        body_hitbox = bn::rect(
            pos.x().round_integer(), pos.y().round_integer(),
            starfish::BODY_SIZE.width(), starfish::BODY_SIZE.height());
        atk_hitbox = body_hitbox;
    }
    
    void Starfish::update()
    {
        apply_movement();
        set_hitbox_position();
        set_sprite_position();
        set_shadow_position();
        run_animation();
        update_states();
    }
    
    void Starfish::take_damage(bool from_left)
    {
        if(!is_dying())
        {
            _death = start_death;
            _jump = start_jump;
            set_face_left(!from_left);
            shadow.set_visible(false);
        }
    }
    
    void Starfish::set_face_left(bool flip)
    {
        _face_left = flip;
        spr.set_horizontal_flip(flip);
    }
    
    void Starfish::apply_movement()
    {
    // set speed
        if(_jump == start_jump)
        {
            if(_death == start_death)
            {
                x_speed = starfish::speed::DAMAGE_X;
                y_speed = starfish::speed::DAMAGE_Y;
            }
            else y_speed = starfish::speed::JUMP_Y;
        }
        
    // apply friction
        if(_jump == full_jump) y_speed -= force::GRAVITY;
        
    // set position
        pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
        pos.set_y(pos.y() - y_speed);
        
    // level bounds
        if(pos.y() > lvl::Y_LIM && !is_dying())
        {
            pos.set_y(lvl::Y_LIM);
            _jump = not_jump;
        }
        if((bn::abs(pos.x()) > lvl::X_LIM) && !is_dying() && !_entering) set_face_left(!_face_left);
        
    // spawn enter toggle
        if(_entering && (bn::abs(pos.x()) < lvl::X_LIM)) _entering = false;
    }
    
    void Starfish::set_hitbox_position()
    {
        body_hitbox.set_position(pos.x().round_integer(), pos.y().round_integer());
        atk_hitbox = body_hitbox;
    }
    
    void Starfish::set_sprite_position() { spr.set_position(pos); }
    
    void Starfish::run_animation()
    {
        if(_death == start_death)
        {
            act = bn::sprite_animate_action<starfish::MAX_ANIM_FRAMES>::once
                (
                    spr, starfish::ANIM_WAIT, spr_item.tiles_item(),
                    starfish::anim::DAMAGE
                );
        }
        
        if(!act.done()) act.update();
    }
    
    void Starfish::update_states()
    {
        if(_jump == not_jump)        _jump = start_jump;
        else if(_jump == start_jump) _jump = full_jump;
        
        if(_death == start_death)      _death = full_death;
        if(pos.y() > lvl::Y_DEATH_LIM) _death = end_death;
    }
    
// Bird
    Bird::Bird
    (
        bool from_left,
        bn::fixed speed,
        bn::fixed height
    )
        : Entity
            (
                entity::type::BIRD,
                from_left ? -lvl::X_LIM - entity::OFFSCREEN_X : lvl::X_LIM + entity::OFFSCREEN_X,
                height,
                bn::sprite_items::toucan
            )
        , x_speed(speed)
        , act(bn::sprite_animate_action<bird::MAX_ANIM_FRAMES>::forever(
                spr, bird::ANIM_WAIT, spr_item.tiles_item(),
                bird::anim::FLY))
    {
    // initial logic
        set_face_left(!from_left);
        shadow.set_visible(false);
        
    // hitboxes
        /*body_hitbox = bn::rect(
            pos.x().round_integer(), pos.y().round_integer(),
            bird::BODY_SIZE.width(), bird::BODY_SIZE.height());
        atk_hitbox = body_hitbox;*/
    }
    
    void Bird::update()
    {
        apply_movement();
        set_hitbox_position();
        set_sprite_position();
        run_animation();
        update_states();
    }
    
    bool Bird::is_attacking() { return _jump == full_jump && !is_dying(); }
    
    void Bird::set_face_left(bool flip)
    {
        _face_left = flip;
        spr.set_horizontal_flip(flip);
    }
    
    void Bird::apply_movement()
    {
    // set speed
        /*if(_jump == start_jump)
        {
            if(_death == start_death)
            {
                x_speed = bird::speed::DAMAGE_X;
                y_speed = bird::speed::DAMAGE_Y;
            }
            else y_speed = bird::speed::JUMP_Y;
        }*/
        
    // apply friction
        //if(_jump == full_jump) y_speed -= force::GRAVITY;
        
    // set position
        pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
        
    // level bounds
        /*if(pos.y() > lvl::Y_LIM && !is_dying())
        {
            pos.set_y(lvl::Y_LIM);
            _jump = not_jump;
        }*/
        
    // spawn enter toggle
        if(_entering && (bn::abs(pos.x()) < lvl::X_LIM)) _entering = false;
    }
    
    void Bird::set_hitbox_position()
    {
        //body_hitbox.set_position(pos.x().round_integer(), pos.y().round_integer());
        //atk_hitbox = body_hitbox;
    }
    
    void Bird::set_sprite_position() { spr.set_position(pos); }
    
    void Bird::run_animation() { if(!act.done()) act.update(); }
    
    void Bird::update_states()
    {
        if(_death == start_death) _death = full_death;
        if(bn::abs(pos.x()) > lvl::X_LIM + entity::OFFSCREEN_X && !_entering) _death = end_death;
    }
}