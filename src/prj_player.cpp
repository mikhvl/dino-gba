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
        bn::fixed y,
        bool flip
    )
        : pos(x < 0 ? bn::max(x, -lvl::X_LIM) : bn::min(x, lvl::X_LIM),
                bn::min(y, lvl::Y_LIM))
        , _face_left(flip)
        , spr_item(spr::DINO_MAIN_SPR_ITEM)
        , spr(spr_item.create_sprite(
                pos.x() + (_face_left ? -player::SPR_OFFSET_X : player::SPR_OFFSET_X),
                pos.y()))                       
        , act(bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once(
                spr, anim_wait, spr_item.tiles_item(),
                player::anim_data::IDLE))
        , box(bn::sprite_items::gatito.create_sprite(pos.x(), pos.y())) // hitbox test
    {
    // air spawn
        if(is_on_ground() && pos.y() < lvl::Y_LIM) _fall = start_fall;
    
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
    
    void Player::set_face_left(bool flip)
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
            if(!_face_left) ++_turn_frames;
            set_face_left(true);
            
        }
        else if(bn::keypad::right_pressed())
        {
            _run = start_run;
            if(_face_left) ++_turn_frames;
            set_face_left(false);
        }
        
        else if(bn::keypad::left_held())      _run = full_run;
        else if(bn::keypad::left_released())  _run = end_run;
        
        else if(bn::keypad::right_held())     _run = full_run;
        else if(bn::keypad::right_released()) _run = end_run;
        
        else _run = not_run;
        
    // vertical movement
    // note: AAAAAAAAAAAAAAAAAAAAAAAAAAA 
    // note: hmm ok at the second glance it's not so bad
        if((bn::keypad::a_pressed() || bn::keypad::a_held()) && is_on_ground()) _jump = start_jump;
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
        if(bn::keypad::b_pressed()) _atk = start_atk;
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
            spr.set_x(pos.x() + (_face_left ? -player::SPR_OFFSET_X : player::SPR_OFFSET_X));
        }
        
        if(bn::abs(pos.x()) > lvl::X_LIM) // level bounds
        {
            pos.set_x(pos.x() < 0 ? -lvl::X_LIM : lvl::X_LIM);
            spr.set_x(pos.x() + (_face_left ? -player::SPR_OFFSET_X : player::SPR_OFFSET_X));
        }
        
    // vertical movement
        if(_jump == start_jump) y_speed = player::START_JMP_SPEED; // initial jump momentum
        if(_fall == end_fall || _jump == start_jump) _fall = not_fall; // resets fall state
        
        if(!is_on_ground())
        {
        // falling/jumping
            pos.set_y(pos.y() - y_speed);
            y_speed -= player::GRAVITY;
            if(y_speed < 0)
            {
                if(_fall == not_fall) _fall = start_fall;
                else if(_fall == start_fall) _fall = full_fall;
            }
        
        // button release
            if(_jump == release_jump && y_speed > player::RELEASE_JMP_SPEED)
            {
                y_speed = player::RELEASE_JMP_SPEED;
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
    
    void Player::animation()
    {
        if(is_on_ground())
        {
        // turn
            if(1 == _turn_frames)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, anim_wait, spr_item.tiles_item(),
                        player::anim_data::TURN
                    );
            }
            else
            {
            // idle
                if(_run == end_run || (_run == not_run && _fall == end_fall))
                {
                    act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                        (
                            spr, anim_wait, spr_item.tiles_item(),
                            player::anim_data::IDLE
                        );
                }
            // run
                else if(_run == start_run || (_run == full_run &&
                    (_fall == end_fall || _turn_frames == player::TURN_FRAMES_STOP)))
                {
                    act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::forever
                        (
                            spr, anim_wait, spr_item.tiles_item(),
                            player::anim_data::RUN
                        );
                }
            }
        }
        else // not on ground
        {
        // jump
            if(_jump == start_jump)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, anim_wait, spr_item.tiles_item(),
                        player::anim_data::JUMP
                    );
            }
        // fall
            else if(_fall == start_fall)
            {
                act = bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once
                    (
                        spr, anim_wait, spr_item.tiles_item(),
                        player::anim_data::FALL
                    );
            }
        }
        
        if(0 < _turn_frames && _turn_frames < player::TURN_FRAMES_STOP) ++_turn_frames;
        else _turn_frames = 0;
        
        if(!act.done()) act.update();
    }
    
    void Player::box_update()
    {
        if(bn::keypad::start_pressed()) box.set_visible(!box.visible());
        box.set_position(pos);
    }
}