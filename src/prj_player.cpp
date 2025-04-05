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

#include "bn_sprite_items_gatito.h"

namespace prj
{
    Player::Player
    (
        bn::fixed x,
        bn::fixed y
    )
        : pos(x < 0 ? bn::max(x, -prj::X_LIM) : bn::min(x, prj::X_LIM),
                bn::min(y, prj::Y_LIM))
        , spr_item(prj::DINO_SPR_ITEM)
        , spr(spr_item.create_sprite(
                pos.x() + (_face_left ? -spr_offset_x : spr_offset_x), pos.y()))                       
        , act(bn::create_sprite_animate_action_once(
                spr, anim_frames, spr_item.tiles_item(),
                0, 0))
        , box(bn::sprite_items::gatito.create_sprite(pos.x(), pos.y())) // hitbox test
    {
    // air spawn
        if(is_on_ground() && pos.y() < prj::Y_LIM) _fall = start_fall;
    
    // test sprite
        box.set_scale(0.5);
        box.set_visible(false);
    }
    
    void Player::update()
    {
        input();
        attack();
        movement();
        animation();
        box_update();
    }
    
    bool Player::is_running() { return _run == start_run || _run == full_run; }
    bool Player::is_jumping() { return _jump != not_jump; }
    bool Player::is_falling() { return _fall == start_fall || _fall == full_fall; }
    bool Player::is_on_ground() { return !is_jumping() && !is_falling(); }
    
    void Player::set_flip(bool flip)
    {
        _face_left = flip;
        spr.set_horizontal_flip(_face_left);
    }
    
    void Player::input()
    {
    // horizontal movement
        if(bn::keypad::left_pressed())
        {
            _run = start_run;
            set_flip(true);
        }
        else if(bn::keypad::right_pressed())
        {
            _run = start_run;
            set_flip(false);
        }
        
        else if(bn::keypad::left_held())      _run = full_run;
        else if(bn::keypad::left_released())  _run = end_run;
        
        else if(bn::keypad::right_held())     _run = full_run;
        else if(bn::keypad::right_released()) _run = end_run;
        
        else _run = not_run;
        
    // vertical movement
    // note: AAAAAAAAAAAAAAAAAAAAAAAAAAA 
    // note: hmm ok at the second glance it's not so bad
        if(bn::keypad::a_pressed() && is_on_ground()) _jump = start_jump;
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
        
    // attack
        if(bn::keypad::a_pressed()) _atk = start_atk;
        else if(_atk == end_atk) _atk = not_atk;
    }
    
    void Player::attack()
    {
        if(_atk == start_atk)
        {
            _atk = end_atk;
        }
    }
    
    void Player::movement()
    {
    // horizontal movement
        if(is_running())
        {
            pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
            spr.set_x(pos.x() + (_face_left ? -spr_offset_x : spr_offset_x));
        }
        
        if(bn::abs(pos.x()) > prj::X_LIM) // level bounds
        {
            pos.set_x(pos.x() < 0 ? -prj::X_LIM : prj::X_LIM);
            spr.set_x(pos.x() + (_face_left ? -spr_offset_x : spr_offset_x));
        }
        
    // vertical movement
        if(_jump == start_jump) y_speed = start_y_speed; // initial jump momentum
        if(_fall == end_fall || _jump == start_jump) _fall = not_fall; // resets fall state
        
        if(!is_on_ground())
        {
        // falling/jumping
            pos.set_y(pos.y() - y_speed);
            y_speed -= g;
            if(y_speed < 0)
            {
                if(_fall == not_fall) _fall = start_fall;
                else if(_fall == start_fall) _fall = full_fall;
            }
        
        // button release
            if(y_speed > release_y_speed && _jump == release_jump)
            {
                y_speed = release_y_speed;
            }
            
        // landing
            if(pos.y() > prj::Y_LIM)
            {
                pos.set_y(prj::Y_LIM);
                y_speed = 0;
                _fall = end_fall;
            }
            
            spr.set_y(pos.y());
        }
    }
    
    void Player::animation()
    {
        if(is_on_ground())
        {
        // idle
            if(_run == end_run || (_run == not_run && _fall == end_fall))
            {
                act = bn::create_sprite_animate_action_once(
                        spr, anim_frames, spr_item.tiles_item(),
                        0, 0);
            }
        // run
            else if(_run == start_run || (_run == full_run && _fall == end_fall))
            {
                act = bn::create_sprite_animate_action_forever(
                        spr, anim_frames, spr_item.tiles_item(),
                        1, 2);
            }
        }
        else // if(is_on_ground())
        {
        // jump
            if(_jump == start_jump)
            {
                act = bn::create_sprite_animate_action_once(
                        spr, anim_frames, spr_item.tiles_item(),
                        3, 3);
            }
        // fall
            else if(_fall == start_fall)
            {
                act = bn::create_sprite_animate_action_once(
                        spr, anim_frames, spr_item.tiles_item(),
                        4, 4);
            }
        }
        
        if(!act.done()) act.update();
    }
    
    void Player::box_update()
    {
        if(bn::keypad::start_pressed()) box.set_visible(!box.visible());
        box.set_position(pos);
    }
}