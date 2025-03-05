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


class Player
{
public:
    Player()
        : spr(bn::sprite_items::dino.create_sprite(
                pos.x() + (face_left ? -spr_offset : spr_offset), pos.y()))
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
        change_state();
        move();
        animate();
        box_update();
    }
    
private:
    bn::fixed_point pos = {0, 32};
    
    bool face_left = false;
    bn::sprite_ptr spr;
    bn::sprite_animate_action<2> act;
    
    bn::sprite_ptr box;
    
    enum player_states
    {
        idle,
        run_start,
        run,
        jump
    };
    player_states state = idle;
    
    void change_state()
    {
        if(bn::keypad::left_pressed())
        {
            state = run_start;
            face_left = true;
        }
        else if(bn::keypad::right_pressed())
        {
            state = run_start;
            face_left = false;
        }
        else if(bn::keypad::left_held() || bn::keypad::right_held())
        {
            state = run;
        }
        else state = idle;
    }
    
    void move()
    {
        if(state == run || state == run_start)
        {
            pos.set_x(pos.x() + (face_left ? -speed : speed));
            spr.set_x(pos.x() + (face_left ? -spr_offset : spr_offset));
        }
    }
    
    void animate()
    {
        spr.set_horizontal_flip(face_left);
        
        if(state == run_start)
        {
            act = bn::create_sprite_animate_action_forever(
                    spr, anim_speed, bn::sprite_items::dino.tiles_item(),
                    1, 2);
        }
        else if(state == idle)
        {
            act = bn::create_sprite_animate_action_forever(
                    spr, anim_speed, bn::sprite_items::dino.tiles_item(),
                    0, 0);
        }
        
        act.update();
    }
    
    void box_update()
    {
        box.set_position(pos.x(), pos.y());
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
