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


constexpr bn::fixed speed = 2;
constexpr int anim_speed = 5;
constexpr int spr_offset = 4;
constexpr bn::fixed grnd_level = 32;


class Player
{
public:
    Player(bn::fixed x = 0, bn::fixed y = grnd_level)
        : pos(x, bn::min(y, grnd_level))
        , spr(bn::sprite_items::dino.create_sprite(
                pos.x() + (_face_left ? -spr_offset : spr_offset), pos.y()))
        , act(bn::create_sprite_animate_action_forever(
                spr, anim_speed, bn::sprite_items::dino.tiles_item(),
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
    bn::fixed_point pos;
    bn::fixed g = 3;
    bn::fixed y_speed = 0;
    
    bool _face_left = false;
    bool _standing = pos.y() <= grnd_level;
    enum run_states { not_run, start_run, full_run };
    enum jmp_states { not_jmp, start_jmp, full_jmp };
    run_states _run = not_run;
    jmp_states _jmp = not_jmp;
    
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
            _jmp = start_jmp;
        }
        else if(bn::keypad::a_held() && !_standing) _jmp = full_jmp;
        else _jmp = not_jmp;
    }
    
    void interact()
    {
        if(_run == start_run || _run == full_run)
        {
            pos.set_x(pos.x() + (_face_left ? -speed : speed));
            spr.set_x(pos.x() + (_face_left ? -spr_offset : spr_offset));
        }
    }
    
    void animate()
    {
        if(_run == start_run)
        {
            spr.set_horizontal_flip(_face_left);
            act = bn::create_sprite_animate_action_forever(
                    spr, anim_speed, bn::sprite_items::dino.tiles_item(),
                    1, 2);
        }
        else if(_run == not_run)
        {
            act = bn::create_sprite_animate_action_forever(
                    spr, anim_speed, bn::sprite_items::dino.tiles_item(),
                    0, 0);
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
