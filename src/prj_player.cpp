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
        : pos(x < 0 ? bn::max(x, -lvl::X_LIM) : bn::min(x, lvl::X_LIM),
                bn::min(y, lvl::Y_LIM))
        , _face_left(flip)
        
    // hitboxes
        , body_hitbox(bn::rect(
                pos.x().round_integer() + (_face_left ? -player::BODY_HITBOX_OFFSET_X : player::BODY_HITBOX_OFFSET_X),
                pos.y().round_integer(),
                body_size.width(), body_size.height()))
        , atk_hitbox(bn::rect(
                pos.x().round_integer() + (_face_left ? -player::ATK_HITBOX_OFFSET_X : player::ATK_HITBOX_OFFSET_X),
                pos.y().round_integer(),
                atk_size.width(), atk_size.height()))
                
    // debug sprites
        , box(bn::sprite_items::gatito.create_sprite(pos.x(), pos.y())) // sprite offset test
        , hitbox_corner_builder(bn::sprite_builder(bn::sprite_items::x_corner))
        
    // main sprite
        , spr_item(bn::sprite_items::dino)
        , spr(spr_item.create_sprite(
                pos.x() + (_face_left ? -player::SPR_OFFSET_X : player::SPR_OFFSET_X),
                pos.y()))
        , act(bn::sprite_animate_action<player::MAX_ANIM_FRAMES>::once(
                spr, anim_wait, spr_item.tiles_item(),
                player::anim_data::IDLE))
    {
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
    }
    
    void Player::get_hurt(bool from_left) {}
    
    bool Player::is_attacking() { return _atk != not_atk || _atk != end_atk; }
    bn::rect& Player::get_body_hitbox() { return body_hitbox; }
    bn::rect& Player::get_atk_hitbox() { return atk_hitbox; }
    
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
            if(!_face_left) _turn_frames = 1;
            set_face_left(true);
            
        }
        else if(bn::keypad::right_pressed())
        {
            _run = start_run;
            if(_face_left) _turn_frames = 1;
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
        else
        {
            if(_atk == start_atk)
            {
                _atk = end_atk;
            }
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
    
    void Player::hitbox()
    {
    // rect hitboxes
        body_hitbox.set_position(
                pos.x().round_integer() + (_face_left ? -player::BODY_HITBOX_OFFSET_X : player::BODY_HITBOX_OFFSET_X),
                pos.y().round_integer());
        atk_hitbox.set_position(
                pos.x().round_integer() + (_face_left ? -player::ATK_HITBOX_OFFSET_X : player::ATK_HITBOX_OFFSET_X),
                pos.y().round_integer());
        
    // box sprite
        if(bn::keypad::start_pressed()) box.set_visible(!box.visible());
        box.set_position(pos);
        
    // hitbox corners sprites
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
}