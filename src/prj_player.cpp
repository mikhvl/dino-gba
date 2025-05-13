#include "prj_player.h"

#include "bn_math.h"
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
    // main parameters
        : Entity(x, y, bn::sprite_items::dino)
        , _face_left(flip)
        
    // animation
        , act(bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once(
                spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                player::anim_data::IDLE))
        
    // debug sprites
        , box(bn::sprite_items::gatito.create_sprite(pos)) // sprite offset test
        , hitbox_corner_builder(bn::sprite_builder(bn::sprite_items::x_corner))
    {
    // hitboxes
        body_hitbox = bn::rect(
            pos.x().round_integer() +
                (_face_left ? -player::BODY_HITBOX_OFFSET_X : player::BODY_HITBOX_OFFSET_X),
            pos.y().round_integer(),
            player::BODY_SIZE.width(), player::BODY_SIZE.height());
        atk_hitbox = bn::rect(
            pos.x().round_integer() +
                (_face_left ? -player::ATK_HITBOX_OFFSET_X : player::ATK_HITBOX_OFFSET_X),
            pos.y().round_integer(),
            player::ATK_SIZE.width(), player::ATK_SIZE.height());
        
    // initial logic
        spr.set_position(pos.x() + (flip ? -player::SPR_OFFSET_X : player::SPR_OFFSET_X), pos.y());
        set_face_left(flip);
        if(pos.y() < lvl::Y_LIM) _fall = start_fall;
    
    // debug
        box.set_scale(0.5);
        box.set_visible(false);
        
        //hitbox_corner_builder.set_scale(0.5);
        hitbox_corner_builder.set_visible(false);
        for(int i = 0; i < hitbox_corners.max_size(); ++i)
        {
            hitbox_corners.emplace_back(hitbox_corner_builder.build());
        }
    }
    
    void Player::update()
    {
        input();
        movement();
        hitbox();
        animation();
        count_frames();
    }
    
    void Player::take_damage() {}
    
    bool Player::is_attacking()
    {
        return _atk_frames >= player::wait_data::ATK_FULL && _atk_frames < player::wait_data::ATK_SLIDE;
    }
    
    void Player::set_camera(const bn::camera_ptr& cam)
    {
        Entity::set_camera(cam);
        
        box.set_camera(cam);
        for(auto& x : hitbox_corners) x.set_camera(cam);
    }
    
    bool Player::is_running() { return _run == start_run || _run == full_run; }
    bool Player::is_jumping() { return _jump != not_jump; }
    bool Player::is_falling() { return _fall == start_fall || _fall == full_fall; }
    bool Player::is_dashing() { return _dash == start_dash || _dash == full_dash; }
    bool Player::is_on_ground() { return !is_jumping() && !is_falling(); }
    
    void Player::set_face_left(bool flip)
    {
        _face_left = flip;
        spr.set_horizontal_flip(_face_left);
    }
    
    void Player::input()
    {
    // attack
        if((bn::keypad::b_pressed() || bn::keypad::b_held()) && is_on_ground() && _atk_frames == 0) 
        {
            _atk_frames = 1;
        }
        
    // vertical movement
        if(is_falling())
        {
            if(bn::keypad::a_pressed()) _queue_jump = true;
            else if(bn::keypad::a_released()) _queue_jump = false;
        }
        
        if(is_on_ground() && 
            (
                (_queue_jump && bn::keypad::a_held()) ||
                (bn::keypad::a_pressed() && _atk_frames == 0) ||
                ((bn::keypad::a_pressed() || bn::keypad::a_held()) && 
                    _atk_frames >= player::wait_data::ATK_SLIDE)
            ))
        {
            if(_atk_frames >= player::wait_data::ATK_SLIDE &&
                bn::keypad::a_pressed()) _dash = start_dash;
            if(_queue_jump) _queue_jump = false;
            _jump = start_jump;
        }
        else
        {
            if(is_falling() || is_on_ground()) _jump = not_jump;
            else
            {
                if(_jump == release_jump) _jump = end_jump;
                else if(_jump != end_jump)
                {
                    if(bn::keypad::a_held()) _jump = full_jump;
                    else if(bn::keypad::a_released()) _jump = release_jump;
                }
            }
        }
        
    // horizontal movement
        if((bn::keypad::left_pressed() &&
            (_atk_frames == 0 || (_atk_frames < player::wait_data::ATK_FULL && !_face_left))) ||
            (bn::keypad::left_held() && _atk_frames == player::wait_data::ATK_STOP))
        {
            if(bn::keypad::left_pressed() && (_atk_frames < player::wait_data::ATK_FULL && !_face_left))
            {
                _atk_frames = player::wait_data::ATK_STOP; // cancel attack
            }
            
            _run = start_run;
            if(!_face_left) _turn_frames = 1;
            set_face_left(true);
        }
        else if((bn::keypad::right_pressed() && 
            (_atk_frames == 0 || (_atk_frames < player::wait_data::ATK_FULL && _face_left))) ||
            (bn::keypad::right_held() && _atk_frames == player::wait_data::ATK_STOP))
        {
            if(bn::keypad::right_pressed() && (_atk_frames < player::wait_data::ATK_FULL && _face_left))
            {
                _atk_frames = player::wait_data::ATK_STOP; // cancel attack
            }
            
            _run = start_run;
            if(_face_left) _turn_frames = 1;
            set_face_left(false);
        }
        
        else if(bn::keypad::left_held())      _run = full_run;
        else if(bn::keypad::left_released())  _run = end_run;
        
        else if(bn::keypad::right_held())     _run = full_run;
        else if(bn::keypad::right_released()) _run = end_run;
        
        else _run = not_run;
    }
    
    void Player::movement()
    {
    // dash states
        if(_dash == not_dash && _atk_frames == player::wait_data::ATK_FULL) _dash = start_dash;
        else if(_dash == start_dash) _dash = full_dash;
        else if(_fall == end_fall ||
            (_atk_frames == player::wait_data::ATK_STOP && is_on_ground())) _dash = end_dash;
        else if(_dash == end_dash) _dash = not_dash;
        
    // horizontal movement
        if((is_on_ground() && !is_running() && !is_dashing() && _atk_frames == 0) || 
            (_atk_frames > 0 && _atk_frames < player::wait_data::ATK_FULL)) x_speed = 0;
        else if(_dash == start_dash) x_speed = player::DASH_X_SPEED;
        else if(_run == start_run) x_speed = player::X_SPEED;
        
        if(is_on_ground() && _dash == full_dash &&
            x_speed > player::FRICTION) x_speed -= player::FRICTION;
        else if(!is_on_ground() && !is_running() &&
            x_speed > player::AIR_FRICTION) x_speed -= player::AIR_FRICTION;
        
        pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
        spr.set_x(pos.x() + (_face_left ? -player::SPR_OFFSET_X : player::SPR_OFFSET_X));
        
        if(bn::abs(pos.x()) > lvl::X_LIM) // level bounds
        {
            pos.set_x(pos.x() < 0 ? -lvl::X_LIM : lvl::X_LIM);
            spr.set_x(pos.x() + (_face_left ? -player::SPR_OFFSET_X : player::SPR_OFFSET_X));
        }
        
    // vertical movement
        if(_jump == start_jump)
        {
            if(is_dashing()) y_speed = player::START_DASH_Y_SPEED;
            else y_speed = player::START_Y_SPEED;
        }
        if(_fall == end_fall || _jump == start_jump) _fall = not_fall; // resets fall state
        
        if(!is_on_ground())
        {
            pos.set_y(pos.y() - y_speed);
            y_speed -= player::GRAVITY;
            
        // fall
            if(y_speed < 0)
            {
                if(_fall == not_fall) _fall = start_fall;
                else if(_fall == start_fall) _fall = full_fall;
            };
        
        // button release
            if(_jump == release_jump && !is_dashing() && y_speed > player::RELEASE_Y_SPEED)
            {
                y_speed = player::RELEASE_Y_SPEED;
            }
            
        // landing
            if(pos.y() > lvl::Y_LIM)
            {
                pos.set_y(lvl::Y_LIM);
                y_speed = 0;
                _fall = end_fall;
            }
            
            spr.set_y(pos.y());
        }
    }
    
    void Player::hitbox()
    {
    // rect hitboxes
        body_hitbox.set_position(
            pos.x().round_integer() +
                (_face_left ? -player::BODY_HITBOX_OFFSET_X : player::BODY_HITBOX_OFFSET_X),
            pos.y().round_integer());
        atk_hitbox->set_position(
            pos.x().round_integer() +
                (_face_left ? -player::ATK_HITBOX_OFFSET_X : player::ATK_HITBOX_OFFSET_X),
            pos.y().round_integer());
        
    // debug
        if(bn::keypad::start_pressed()) box.set_visible(!box.visible());
        box.set_position(pos);
        
        if(bn::keypad::select_pressed())
        {
            for(auto& x : hitbox_corners) x.set_visible(!x.visible());
        }
        
        hitbox_corners[0].set_position(body_hitbox.bottom_right());
        hitbox_corners[1].set_position(body_hitbox.bottom_left());
        hitbox_corners[2].set_position(body_hitbox.top_right());
        hitbox_corners[3].set_position(body_hitbox.top_left());
        
        hitbox_corners[4].set_position(atk_hitbox->bottom_right());
        hitbox_corners[5].set_position(atk_hitbox->bottom_left());
        hitbox_corners[6].set_position(atk_hitbox->top_right());
        hitbox_corners[7].set_position(atk_hitbox->top_left());
    }
    
    void Player::animation()
    {
        if(is_on_ground())
        {
        // attack
            if(_atk_frames == 1)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim_data::ATK_START
                    );
            }
            else if(_atk_frames == player::wait_data::ATK_FULL)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim_data::ATK_FULL
                    );
            }
            else if(_atk_frames == player::wait_data::ATK_SLOW)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim_data::ATK_SLOW
                    );
            }
            else if(_atk_frames == player::wait_data::ATK_SLIDE)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim_data::ATK_SLIDE
                    );
            }
            
        // turn
            else if(_turn_frames == 1)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim_data::TURN_GROUND
                    );
            }
            else
            {
            // idle
                if((_run == end_run && _atk_frames == 0) || (_run == not_run &&
                    (_fall == end_fall || _atk_frames == player::wait_data::ATK_STOP)))
                {
                    act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                        (
                            spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                            player::anim_data::IDLE
                        );
                }
            // run
                else if(_run == start_run || (_run == full_run &&
                    (_fall == end_fall || _turn_frames == player::wait_data::TURN_STOP ||
                    _atk_frames == player::wait_data::ATK_STOP)))
                {
                    act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::forever
                        (
                            spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                            player::anim_data::RUN
                        );
                }
            }
        }
        else // not on ground
        {
        // turn
            if(_turn_frames == 1)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                        player::anim_data::TURN_AIR
                    );
            }
            else
            {
            // jump
                if(_jump == start_jump ||
                    (is_jumping() && _turn_frames == player::wait_data::TURN_STOP))
                {
                    if(is_dashing())
                    {
                        act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                            (
                                spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                                player::anim_data::JUMP_DASH
                            );
                    }
                    else
                    {
                        act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                            (
                                spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                                player::anim_data::JUMP
                            );
                    }
                }
            // fall
                else if(_fall == start_fall ||
                    _atk_frames == player::wait_data::ATK_SLIDE ||
                    (is_falling() && _turn_frames == player::wait_data::TURN_STOP))
                {
                    act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                        (
                            spr, player::wait_data::ANIM_WAIT, spr_item.tiles_item(),
                            player::anim_data::FALL
                        );
                }
            }
        }
        
        if(!act.done()) act.update();
    }
    
    void Player::count_frames()
    {
    // turn
        if(0 < _turn_frames && _turn_frames < player::wait_data::TURN_STOP) ++_turn_frames;
        else _turn_frames = 0;
    
    // attack
        if(0 < _atk_frames && _atk_frames < player::wait_data::ATK_STOP) ++_atk_frames;
        else _atk_frames = 0;
    }
}