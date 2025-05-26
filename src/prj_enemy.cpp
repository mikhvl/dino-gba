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
    
// Coconut
    Coconut::Coconut(bn::fixed x, bn::fixed y, bn::fixed fall_speed, bool flip)
        : Entity(entity::type::COCONUT, x, y, bn::sprite_items::coconut)
        , x_speed(fall_speed)
        , _face_left(flip)
    {
        shadow.set_visible(false);
        
        body_hitbox = bn::rect(
            pos.x().round_integer(), pos.y().round_integer(),
            coco::BODY_SIZE.width(), coco::BODY_SIZE.height());
        atk_hitbox = body_hitbox;
    }
    
    void Coconut::update()
    {
        apply_movement();
        set_hitbox_position();
        set_sprite_position();
        update_states();
    }
    
    void Coconut::set_position(bn::fixed x, bn::fixed y)
    {
        if(!is_dying() && _fall == not_fall)
        {
            pos.set_x(x);
            pos.set_y(y);
        }
    }
    
    void Coconut::drop()
    {
        if(_fall == not_fall) _fall = full_fall;
    }
    
    void Coconut::apply_movement()
    {
    // death jump
        if(pos.y() > lvl::Y_LIM && !is_dying())
        {
            pos.set_y(lvl::Y_LIM);
            _fall = start_fall;
            _death = full_death;
        }
        
    // set speed
        if(_fall == start_fall)
        {
            x_speed = coco::speed::JUMP_X;
            y_speed = coco::speed::JUMP_Y;
        }
        //else if(_fall == full_fall) x_speed = coco::speed::FALL_X;
        
    
        if(_fall != not_fall)
        {
        // apply friction
            y_speed -= force::GRAVITY;
        
        // set position
            pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
            pos.set_y(pos.y() - y_speed);
        }
    }
    
    void Coconut::set_hitbox_position()
    {
        body_hitbox.set_position(pos.x().round_integer(), pos.y().round_integer());
        atk_hitbox = body_hitbox;
    }
    
    void Coconut::set_sprite_position() { spr.set_position(pos); }
    
    void Coconut::update_states()
    {
        if(_fall == start_fall) _fall = full_fall;
        
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
        , coco
            (
                pos.x() + (!from_left ? -bird::COCONUT_OFFSET_X : bird::COCONUT_OFFSET_X),
                pos.y() + bird::COCONUT_OFFSET_Y,
                coco::speed::FALL_X,
                //Random.get_fixed(coco::speed::FALL_X_MIN, coco::speed::FALL_X_MAX),
                !from_left
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
        body_hitbox = bn::rect(
            pos.x().round_integer(), pos.y().round_integer(),
            bird::BODY_SIZE.width(), bird::BODY_SIZE.height());
        atk_hitbox = coco.get_atk_hitbox();
    }
    
    void Bird::set_camera(const bn::camera_ptr& cam)
    {
        Entity::set_camera(cam);
        coco.set_camera(cam);
    }
    
    void Bird::update()
    {
        //Random.update();
        
        apply_movement();
        set_hitbox_position();
        set_sprite_position();
        run_animation();
        update_states();
        
        coco.set_position
            (
                pos.x() + (_face_left ? -bird::COCONUT_OFFSET_X : bird::COCONUT_OFFSET_X),
                pos.y() + bird::COCONUT_OFFSET_Y
            );
        coco.update();
    }
    
    bool Bird::is_attacking()
    {
        return coco.is_attacking();
        /*for(auto& coco : coconuts)
        {
            if(coco.is_attacking()) return true;
        }
        return false;*/
    }
    
    void Bird::type_specific_action(bn::fixed_point player_position) // drop coconut
    {
        /*if(coconuts.empty())
        {*/
        // checking range
            bn::fixed diff = player_position.x() - pos.x();
            diff = _face_left ? -diff : diff;
            bool in_range = 0 < diff && diff < bird::DROP_RANGE_X;
            
        // drop if in range
            if(in_range)
            {
                coco.drop();
                /*while(coconuts.size() < coconuts.max_size())
                {
                    coconuts.emplace_back(Coconut(pos.x(), pos.y(),
                        Random.get_fixed(coco::speed::FALL_X_MIN, coco::speed::FALL_X_MAX), !from_left));
                    coconuts.back().drop();
                }*/
            }
        //}
    }
    
    void Bird::set_face_left(bool flip)
    {
        _face_left = flip;
        spr.set_horizontal_flip(flip);
    }
    
    void Bird::apply_movement()
    {
    // set position
        pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
        
    // spawn enter toggle
        if(_entering && (bn::abs(pos.x()) < lvl::X_LIM)) _entering = false;
    }
    
    void Bird::set_hitbox_position()
    {
        body_hitbox.set_position(pos.x().round_integer(), pos.y().round_integer());
        atk_hitbox = coco.get_atk_hitbox();
    }
    
    void Bird::set_sprite_position() { spr.set_position(pos); }
    
    void Bird::run_animation() { if(!act.done()) act.update(); }
    
    void Bird::update_states()
    {
        if(bn::abs(pos.x()) > lvl::X_LIM + entity::OFFSCREEN_X && !_entering &&
            coco.is_dead()) _death = end_death;
    }
}