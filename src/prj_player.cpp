#include "prj_player.h"

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_keypad.h"

#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_animate_actions.h"

namespace prj
{
    Player::Player
    (
        bn::fixed x,
        bn::fixed y,
        bool flip
    )
        : Entity(entity::type::PLAYER, x, y, bn::sprite_items::dino)
        , act(bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once(
                spr, player::ANIM_WAIT, spr_item.tiles_item(),
                player::anim::IDLE))
        , hitbox_corner_builder(bn::sprite_builder(bn::sprite_items::x_corner))
    {
    // initial logic
        set_face_left(flip);
        set_hitbox_size();
        set_sprite_position();
        spr.set_z_order(entity::PLAYER_Z_ORDER);
        if(pos.y() < lvl::Y_LIM) _fall = start_fall;
    
    // debug
        hitbox_corner_builder.set_visible(false);
        hitbox_corner_builder.set_bg_priority(1);
        for(int i = 0; i < hitbox_corners.max_size(); ++i)
        {
            hitbox_corners.emplace_back(hitbox_corner_builder.build());
        }
    }
    
    void Player::update()
    {
        process_input();
        apply_movement();
        set_hitbox_position();
        
        set_sprite_position();
        set_shadow_position();
        run_animation();
        
        update_states();
    }
    
    void Player::take_damage(bool from_left)
    {
        if(_inv_frames == 0 && !_stun)
        {
            _stun = true;
            _inv_frames = 1;
            _turn_frames = 0;
            _atk_frames = 0;
            set_face_left(from_left);
            if(is_spinning())
            {
                _spin = not_spin;
                set_hitbox_size(false);
            }
        }
    }
    
    bool Player::is_attacking()
    {
        return (_atk_frames >= player::wait::ATK_FULL && _atk_frames < player::wait::ATK_SLIDE) ||
            (is_spinning() && x_speed > player::speed::SPIN_STOP_X);
    }
    
    void Player::set_camera(const bn::camera_ptr& cam)
    {
        Entity::set_camera(cam);
        for(auto& x : hitbox_corners) x.set_camera(cam);
    }
    
    bool Player::is_running() { return _run == start_run || _run == full_run; }
    bool Player::is_jumping() { return _jump != not_jump; }
    bool Player::is_falling() { return _fall == start_fall || _fall == full_fall; }
    bool Player::is_dashing() { return _dash == start_dash || _dash == full_dash; }
    bool Player::is_spinning() { return _spin == start_spin || _spin == full_spin; }
    bool Player::is_on_ground() { return !is_jumping() && !is_falling(); }
    
    void Player::set_face_left(bool flip)
    {
        _face_left = flip;
        spr.set_horizontal_flip(_face_left);
    }
    
    void Player::set_hitbox_size(bool is_spin)
    {
        if(is_spin)
        {
            body_hitbox = bn::rect
            (
                pos.x().round_integer(), pos.y().round_integer(),
                player::SPIN_SIZE.width(), player::SPIN_SIZE.height()
            );
            
            atk_hitbox = body_hitbox;
        }
        else
        {
            body_hitbox = bn::rect
            (
                pos.x().round_integer() +
                    (_face_left ? -player::BODY_HITBOX_OFFSET_X : player::BODY_HITBOX_OFFSET_X),
                pos.y().round_integer(),
                player::BODY_SIZE.width(), player::BODY_SIZE.height()
            );
            
            atk_hitbox = bn::rect
            (
                pos.x().round_integer() +
                    (_face_left ? -player::ATK_HITBOX_OFFSET_X : player::ATK_HITBOX_OFFSET_X),
                pos.y().round_integer(),
                player::ATK_SIZE.width(), player::ATK_SIZE.height()
            );
        }
    }
    
    void Player::process_input()
    {
    // ATTACK
        if((bn::keypad::b_pressed() || bn::keypad::b_held()) && _inv_frames == 0 && !is_spinning())
        {
        // start jaw attack
            if(is_on_ground())
            {
                if(_atk_frames == 0 && !_queue_jump) _atk_frames = 1;
            }
        // start spin attack
            else
            {
                _spin = start_spin;
                set_hitbox_size(true);
            }
        }
        
    // QUEUE JUMP
        if(is_falling() && !is_spinning())
        {
            if(bn::keypad::a_pressed()) _queue_jump = true;
            else if(bn::keypad::a_released()) _queue_jump = false;
        }
        
    // VERTICAL MOVEMENT
    // start stun jump
        if(_stun && _inv_frames == 1) _jump = start_jump;
    // start spin jump
        else if(is_spinning() && _fall == end_fall) _jump = start_jump;
    // start normal jump
        else if(is_on_ground() && !is_spinning() &&
            (
                (_queue_jump && bn::keypad::a_held()) ||
                (bn::keypad::a_pressed() && _atk_frames == 0) ||
                ((bn::keypad::a_pressed() || bn::keypad::a_held()) && 
                    _atk_frames >= player::wait::ATK_SLIDE)
            ))
        {
            if(_atk_frames >= player::wait::ATK_SLIDE && bn::keypad::a_pressed()) _dash = start_dash;
            if(_queue_jump) _queue_jump = false;
            _jump = start_jump;
        }
    // manage jump states
        else
        {
            if(is_falling() || is_on_ground()) _jump = not_jump;
            else
            {
                if(_jump == release_jump) _jump = end_jump;
                else if(_jump != end_jump)
                {
                    if(bn::keypad::a_held() || _stun || is_spinning()) _jump = full_jump;
                    else if(bn::keypad::a_released()) _jump = release_jump;
                }
            }
        }
        
    // HORIZONTAL MOVEMENT
    // start run left
        if
        (
            // change spin direction
                (is_spinning() && _level_bound_right) ||
            // if left pressed
                (bn::keypad::left_pressed() && !_stun && !is_spinning() &&
                    (_atk_frames == 0 || (_atk_frames < player::wait::ATK_FULL && !_face_left))) ||
            // if left held
                (bn::keypad::left_held() && !is_spinning() &&
                    (_atk_frames == player::wait::ATK_STOP || _fall == end_fall))
        )
        {
        // change spin direction
            if(is_spinning() && _level_bound_right) _level_bound_right = false;
            
        // cancel attack
            if(bn::keypad::left_pressed() && (_atk_frames < player::wait::ATK_FULL && !_face_left))
            {
                _atk_frames = player::wait::ATK_STOP;
            }
            
        // start run
            _run = start_run;
            if(!_face_left) _turn_frames = 1;
            set_face_left(true);
        }
    //start run right
        else if
        (
            // change spin direction
                (is_spinning() && _level_bound_left) ||
            // if right pressed
                (bn::keypad::right_pressed() && !_stun && !is_spinning() &&
                    (_atk_frames == 0 || (_atk_frames < player::wait::ATK_FULL && _face_left))) ||
            // if right held
                (bn::keypad::right_held() && !is_spinning() &&
                    (_atk_frames == player::wait::ATK_STOP || _fall == end_fall))
        )
        {
        // change spin direction
            if(is_spinning() && _level_bound_left) _level_bound_left = false;
            
        // cancel attack
            if(bn::keypad::right_pressed() && (_atk_frames < player::wait::ATK_FULL && _face_left))
            {
                _atk_frames = player::wait::ATK_STOP;
            }
            
        // start run
            _run = start_run;
            if(_face_left) _turn_frames = 1;
            set_face_left(false);
        }
        
    // manage run states
        else if(bn::keypad::left_held() || bn::keypad::right_held()) _run = full_run;
        else if(bn::keypad::left_released() || bn::keypad::right_released()) _run = end_run;
        
        else _run = not_run;
    }
    
    void Player::apply_movement()
    {
    // VERTICAL MOVEMENT
    // set y speed
        if(_jump == start_jump)
        {
            if(_stun) y_speed = player::speed::STUN_Y;
            else if(is_spinning())
            {
                if(_fall == end_fall) 
                    y_speed = player::speed::JUMP_START_Y * x_speed.division(player::speed::SPIN_X);
                else y_speed = player::speed::SPIN_Y;
            }
            else if(is_dashing()) y_speed = player::speed::DASH_Y;
            else y_speed = player::speed::JUMP_START_Y;
        }
        
     // reset fall state
        if(_fall == end_fall || _jump == start_jump) _fall = not_fall;
        
        if(!is_on_ground())
        {
        // apply speed
            pos.set_y(pos.y() - y_speed);
            y_speed -= force::GRAVITY;
            
        // fall
            if(y_speed < 0)
            {
                if(_fall == not_fall) _fall = start_fall;
                else if(_fall == start_fall) _fall = full_fall;
            };
        
        // button release
            if(_jump == release_jump && !is_dashing() && y_speed > player::speed::JUMP_RELEASE_Y)
            {
                y_speed = player::speed::JUMP_RELEASE_Y;
            }
            
        // landing
            if(pos.y() > lvl::Y_LIM)
            {
                pos.set_y(lvl::Y_LIM);
                y_speed = 0;
                _fall = end_fall;
                _stun = false;
            }
        }
        
    // HORIZONTAL MOVEMENT
    // spin momentum
        if(is_spinning() && _fall == end_fall)
        {
            if(x_speed < player::speed::SPIN_STOP_X)
            {
                _spin = end_spin;
                set_hitbox_size(false);
            }
            else x_speed -= force::SPIN_FRICTION;
        }
        
    // set x speed
        if((is_on_ground() && !is_running() && !is_dashing() &&
                _atk_frames == 0 && !_stun && !is_spinning()) ||
                (_atk_frames > 0 && _atk_frames < player::wait::ATK_FULL)) x_speed = 0;
        else if(_stun && _inv_frames == 1) x_speed = -player::speed::STUN_X;
        else if(_spin == start_spin) x_speed = player::speed::SPIN_X;
        else if(_dash == start_dash && _jump != start_jump) x_speed = player::speed::DASH_X;
        else if(_run == start_run) x_speed = player::speed::RUN_X;
        
    // apply friction
        if(!is_spinning())
        {
            if(is_on_ground() && is_dashing() &&
                x_speed > force::FRICTION) x_speed -= force::FRICTION;
            else if(!is_on_ground() && (!is_running() || _stun) &&
                x_speed > force::AIR_FRICTION) x_speed -= force::AIR_FRICTION;
        }
        
    // apply speed
        pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
        
    // check level bounds
        if(pos.x() > lvl::X_LIM)
        {
            pos.set_x(lvl::X_LIM);
            _level_bound_right = true;
        }
        else if(pos.x() < -lvl::X_LIM)
        {
            pos.set_x(-lvl::X_LIM);
            _level_bound_left = true;
        }
        else 
        {
            _level_bound_right = false;
            _level_bound_left = false;
        }
    }
    
    void Player::set_hitbox_position()
    {
    // rect hitboxes
        if(is_spinning())
        {
            body_hitbox.set_position(pos.x().round_integer(), pos.y().round_integer());
            atk_hitbox.set_position(pos.x().round_integer(), pos.y().round_integer());
        }
        else
        {
            body_hitbox.set_position(
                pos.x().round_integer() +
                    (_face_left ? -player::BODY_HITBOX_OFFSET_X : player::BODY_HITBOX_OFFSET_X),
                pos.y().round_integer());
            atk_hitbox.set_position(
                pos.x().round_integer() +
                    (_face_left ? -player::ATK_HITBOX_OFFSET_X : player::ATK_HITBOX_OFFSET_X),
                pos.y().round_integer());
        }
        
    // debug
        if(bn::keypad::select_pressed())
        {
            for(auto& x : hitbox_corners) x.set_visible(!x.visible());
        }
        
        hitbox_corners[0].set_position(body_hitbox.bottom_right());
        hitbox_corners[1].set_position(body_hitbox.bottom_left());
        hitbox_corners[2].set_position(body_hitbox.top_right());
        hitbox_corners[3].set_position(body_hitbox.top_left());
        
        hitbox_corners[4].set_position(atk_hitbox.bottom_right());
        hitbox_corners[5].set_position(atk_hitbox.bottom_left());
        hitbox_corners[6].set_position(atk_hitbox.top_right());
        hitbox_corners[7].set_position(atk_hitbox.top_left());
    }
    
    void Player::set_sprite_position()
    {
        if(is_spinning()) spr.set_x(pos.x());
        else spr.set_x(pos.x() + (_face_left ? -player::SPR_OFFSET_X : player::SPR_OFFSET_X));
        
        spr.set_y(pos.y());
    }
    
    void Player::run_animation()
    {
    // STUNNED
        if(_stun && _inv_frames == 1)
        {
            act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                (
                    spr, player::ANIM_WAIT, spr_item.tiles_item(),
                    player::anim::STUN
                );
        }
        
    // SPIN ATTACK
        else if(_spin == start_spin)
        {
            act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::forever
                (
                    spr, player::SPIN_ANIM_WAIT, spr_item.tiles_item(),
                    player::anim::ATK_SPIN
                );
        }
        
    // ON GROUND
        else if(is_on_ground() && !_stun && !is_spinning())
        {
        // attack
            if(_atk_frames == 1)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::ATK_START
                    );
            }
            else if(_atk_frames == player::wait::ATK_FULL)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::ATK_FULL
                    );
            }
            else if(_atk_frames == player::wait::ATK_SLOW)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::ATK_SLOW
                    );
            }
            else if(_atk_frames == player::wait::ATK_SLIDE)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::ATK_SLIDE
                    );
            }
            
        // turn
            else if(_turn_frames == 1)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::TURN_GROUND
                    );
            }
        // idle
            else if((_run == end_run && _atk_frames == 0) || (_run == not_run &&
                (_fall == end_fall || _turn_frames == player::wait::TURN_STOP ||
                _atk_frames == player::wait::ATK_STOP)))
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::IDLE
                    );
            }
        // run
            else if(_run == start_run || (_run == full_run &&
                (_fall == end_fall || _turn_frames == player::wait::TURN_STOP ||
                _atk_frames == player::wait::ATK_STOP)))
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::forever
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::RUN
                    );
            }
        }
    // IN AIR
        else if(!_stun && !is_spinning())
        {
        // turn
            if(_turn_frames == 1)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::TURN_AIR
                    );
            }
        // jump
            else if(_jump == start_jump ||
                (is_jumping() && _turn_frames == player::wait::TURN_STOP))
            {
                if(is_dashing())
                {
                    act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                        (
                            spr, player::ANIM_WAIT, spr_item.tiles_item(),
                            player::anim::JUMP_DASH
                        );
                }
                else
                {
                    act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                        (
                            spr, player::ANIM_WAIT, spr_item.tiles_item(),
                            player::anim::JUMP
                        );
                }
            }
        // fall
            else if(_fall == start_fall ||
                (is_falling() && _turn_frames == player::wait::TURN_STOP))
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim::FALL
                    );
            }
        }
        
    // INVINCIBILITY FLASH
        if(_inv_frames > 0 && _inv_frames % player::wait::INV_WAIT == 0 &&
            _atk_frames == 0 && !is_spinning())
        {
            spr.set_visible(!spr.visible());
            shadow.set_visible(!shadow.visible());
        }
        if(_inv_frames == 0 && !spr.visible())
        {
            spr.set_visible(true);
            shadow.set_visible(true);
        }
        
    // ANIMATION UPDATE
        if(!act.done()) act.update();
    }
    
    void Player::update_states()
    {
    // SPIN STATES
        if(_spin == start_spin) _spin = full_spin;
        else if(_spin == end_spin) _spin = not_spin;
        
    // DASH STATES
        if(_dash == not_dash && _atk_frames == player::wait::ATK_FULL) _dash = start_dash;
        else if(_dash == start_dash) _dash = full_dash;
        else if(_fall == end_fall ||
            (_atk_frames == player::wait::ATK_STOP && is_on_ground())) _dash = not_dash;
        
    // INVINCIBILITY
        if(0 < _inv_frames && _inv_frames < player::wait::INV_STOP) ++_inv_frames;
        else _inv_frames = 0;
        
        if(_atk_frames == player::wait::ATK_FULL) _inv_frames = 1;
        if(_atk_frames == player::wait::ATK_INV_STOP) _inv_frames = 0;
        
    // TURN
        if(0 < _turn_frames && _turn_frames < player::wait::TURN_STOP) ++_turn_frames;
        else _turn_frames = 0;
    
    // ATTACK
        if(0 < _atk_frames && _atk_frames < player::wait::ATK_STOP) ++_atk_frames;
        else _atk_frames = 0;
    }
}