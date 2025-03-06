#include "bn_core.h"
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
constexpr bn::fixed grnd_level = 32;


class Player
{
public:
    Player(bn::fixed x = 0, bn::fixed y = grnd_level)
        : pos(x, bn::min(y, grnd_level))
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
    bool _face_left = false;
    bool _standing = pos.y() == grnd_level;
    enum run_states { not_run, start_run, full_run };
    enum jmp_states { not_jmp, start_jmp, full_jmp, fall_jmp, end_jmp };
    run_states _run = not_run;
    jmp_states _jmp = not_jmp;
    
// sprites, animation
    bn::sprite_ptr spr;
    bn::sprite_animate_action<2> act;
    bn::sprite_ptr box; // hitbox test
    
    void player_input()
    {
    // horizontal movement
        if(bn::keypad::left_pressed())
        {
            _run = start_run;
            _face_left = true;
        }
        else if(bn::keypad::right_pressed())
        {
            _run = start_run;
            _face_left = false;
        }
        else if(bn::keypad::left_held() || bn::keypad::right_held())
        {
            _run = full_run;
        }
        else _run = not_run;
        
    // vertical movement
        if(bn::keypad::a_pressed() && _standing)
        {
            _standing = false;
            y_speed += max_y_speed;
            _jmp = start_jmp;
        }
        else if(bn::keypad::a_held() && !_standing && _jmp != fall_jmp) _jmp = full_jmp;
        // fall_jmp is handled by interact()
        else _jmp = not_jmp;
    }
    
    void interact()
    {
    // horizontal movement
        if(_run == start_run || _run == full_run)
        {
            pos.set_x(pos.x() + (_face_left ? -x_speed : x_speed));
            spr.set_x(pos.x() + (_face_left ? -spr_offset : spr_offset));
        }
        
    // vertical movement
        if(!_standing)
        {
            pos.set_y(pos.y() - y_speed);
            y_speed -= g;
            if(y_speed < 0 && _jmp != fall_jmp) _jmp = fall_jmp;
            if(pos.y() > grnd_level)
            {
                _standing = true;
                _jmp = end_jmp;
                y_speed = 0;
                pos.set_y(grnd_level);
            }
            spr.set_y(pos.y());
        }
    }
    
    void animate()
    {
        if(_standing)
        {
            if(_run == not_run)
            {
                act = bn::create_sprite_animate_action_forever(
                        spr, anim_frames, bn::sprite_items::dino.tiles_item(),
                        0, 0);
            }
            else if(_run == start_run || _jmp == end_jmp)
            {
                spr.set_horizontal_flip(_face_left);
                act = bn::create_sprite_animate_action_forever(
                        spr, anim_frames, bn::sprite_items::dino.tiles_item(),
                        1, 2);
            }
        }
        else // not _standing
        {
            spr.set_horizontal_flip(_face_left);
            if(_jmp == start_jmp || _jmp == full_jmp)
            {
                act = bn::create_sprite_animate_action_forever(
                        spr, anim_frames, bn::sprite_items::dino.tiles_item(),
                        3, 3);
            }
            else if(_jmp == fall_jmp)
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
