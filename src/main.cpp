#include "bn_core.h"
#include "bn_math.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_keypad.h"

#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_animate_actions.h"

#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"

#include "bn_sprite_items_dino.h"
#include "bn_regular_bg_items_bg_temp.h"

#include "bn_sprite_items_gatito.h"

// game constants
constexpr bn::fixed ground_level = 32;
constexpr bn::fixed bound = 114;


class Player
{
public:
    Player(bn::fixed x = 0, bn::fixed y = ground_level)
        : pos(x, bn::min(y, ground_level))
        , spr(bn::sprite_items::dino.create_sprite(
                pos.x() + (_face_left ? -spr_offset : spr_offset), pos.y()))
        , act(bn::create_sprite_animate_action_forever(
                spr, anim_frames, bn::sprite_items::dino.tiles_item(),
                0, 0))
        , box(bn::sprite_items::gatito.create_sprite(pos.x(), pos.y()))
    {
        box.set_scale(0.5);
        box.set_visible(false);
    }
    
    void update()
    {
        player_input();
        interact();
        animate();
        box_update();
    }
    
private:
// some player constants
    const int anim_frames = 4;
    const int spr_offset = 4;
    const bn::fixed g = 0.2;
    const bn::fixed max_y_speed = 4;

// player variables
    bn::fixed_point pos;
    bn::fixed x_speed = 2;
    bn::fixed y_speed = 0;

// state logic
// note: this state mindf/ckery is KILLING ME
    enum run_states  { start_run, full_run, end_run, not_run };
    enum jump_states { start_jump, full_jump, end_jump, not_jump };
    enum fall_states { start_fall, full_fall, end_fall, not_fall };
    
    run_states  _run  = not_run;
    jump_states _jump = not_jump;
    fall_states _fall = not_fall;
    
    bool _face_left = false;
    
// sprites, animation
    bn::sprite_ptr spr;
    bn::sprite_animate_action<2> act;
    bn::sprite_ptr box; // hitbox test

// state functions
    bool is_on_ground() { return _jump == not_jump && (_fall == not_fall || _fall == end_fall); }
    bool is_running() { return _run == start_run || _run == full_run; }
    bool is_jumping() { return _jump == start_jump || _jump == full_jump; } // maybe _jump == end_jump
    bool is_falling() { return _fall == start_fall || _fall == full_fall; }

// update functions
    void set_flip() { spr.set_horizontal_flip(_face_left); }
    
    void player_input()
    {
    // horizontal movement
        if(bn::keypad::left_pressed())
        {
            _run = start_run;
            _face_left = true;
            set_flip();
        }
        else if(bn::keypad::right_pressed())
        {
            _run = start_run;
            _face_left = false;
            set_flip();
        }
        
        else if(bn::keypad::left_held())      _run = full_run;
        else if(bn::keypad::left_released())  _run = end_run;
        
        else if(bn::keypad::right_held())     _run = full_run;
        else if(bn::keypad::right_released()) _run = end_run;
        
        else _run = not_run;
        
    // vertical movement
        if(bn::keypad::a_pressed() && is_on_ground())
        {
            _jump = start_jump;
        }
        else
        {
            if(is_falling() || is_on_ground()) _jump = not_jump;
            else
            {
                if(bn::keypad::a_held() && _jump != end_jump) _jump = full_jump;
                else if(bn::keypad::a_released()) _jump = end_jump;
            }
        }
    }
    
    void interact()
    {
    // horizontal movement
        if(is_running())
        {
            pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
            spr.set_x(pos.x() + (_face_left ? -spr_offset : spr_offset));
        }
        
        if(bn::abs(pos.x()) > bound) // level bounds
        {
            pos.set_x(pos.x() < 0 ? -bound : bound);
            spr.set_x(pos.x() + (_face_left ? -spr_offset : spr_offset));
        }
        
    // vertical movement
        if(is_on_ground() && pos.y() < ground_level) _fall = start_fall; // air spawn
        if(_jump == start_jump) y_speed += max_y_speed; // jump momentum
        if(_fall == end_fall) _fall = not_fall;
        
        if(!is_on_ground())
        {
            pos.set_y(pos.y() - y_speed);
            y_speed -= g;
            
            if(y_speed < 0)
            {
                if(_fall == not_fall) _fall = start_fall;
                else if(_fall == start_fall) _fall = full_fall;
            }
            
            if(pos.y() > ground_level) // landing
            {
                pos.set_y(ground_level);
                y_speed = 0;
                _fall = end_fall;
            }
            
            spr.set_y(pos.y());
        }
    }
    
    void animate()
    {
        if(is_on_ground())
        {
        // idle
            if(_run == end_run || (_run == not_run && _fall == end_fall))
            {
                act = bn::create_sprite_animate_action_forever(
                        spr, anim_frames, bn::sprite_items::dino.tiles_item(),
                        0, 0);
            }
        // run
            else if(_run == start_run || (_run == full_run && _fall == end_fall))
            {
                act = bn::create_sprite_animate_action_forever(
                        spr, anim_frames, bn::sprite_items::dino.tiles_item(),
                        1, 2);
            }
        }
        else // if(is_on_ground())
        {
        // jump
            if(_jump == start_jump)
            {
                act = bn::create_sprite_animate_action_forever(
                        spr, anim_frames, bn::sprite_items::dino.tiles_item(),
                        3, 3);
            }
        // fall
            else if(_fall == start_fall)
            {
                act = bn::create_sprite_animate_action_forever(
                        spr, anim_frames, bn::sprite_items::dino.tiles_item(),
                        4, 4);
            }
        }
        
        act.update();
    }
    
    void box_update()
    {
        box.set_position(pos);
    }
};


int main()
{
    bn::core::init();
    
    Player dino;
    bn::regular_bg_ptr bg = bn::regular_bg_items::bg_temp.create_bg(0, 0);

    while(true)
    {
        dino.update();
        bn::core::update();
    }
}
