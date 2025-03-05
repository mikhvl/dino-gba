#include "bn_core.h"
#include "bn_fixed.h"
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


constexpr bn::fixed speed = 2;
constexpr int anim_speed = 5;


class Player
{
public:
    Player()
        : spr(bn::sprite_items::dino.create_sprite(x, y))
        , act(bn::create_sprite_animate_action_forever(
            spr, anim_speed, bn::sprite_items::dino.tiles_item(),
            0, 0))
    {}
    
    void update()
    {
        change_state();
        move();
        animate();
    }
    
private:
    bn::fixed x = 0;
    bn::fixed y = 32;
    bool face_left = true;
    bn::sprite_ptr spr;
    bn::sprite_animate_action<2> act;
    enum player_states { idle, start_run, run, jump };
    player_states state = idle;
    
    void change_state()
    {
        if(bn::keypad::left_pressed())
        {
            state = start_run;
            face_left = true;
        }
        else if(bn::keypad::right_pressed())
        {
            state = start_run;
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
        if(state == run || state == start_run)
        {
            spr.set_x(spr.x() + speed * (-1 + 2 * !face_left));
        }
    }
    
    void animate()
    {
        spr.set_horizontal_flip(face_left);
        
        if(state == start_run)
        {
            act = bn::create_sprite_animate_action_forever(
                spr, anim_speed, bn::sprite_items::dino.tiles_item(), 1, 2);
        }
        else if(state == idle)
        {
            act = bn::create_sprite_animate_action_forever(
                spr, anim_speed, bn::sprite_items::dino.tiles_item(), 0, 0);
        }
        
        act.update();
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
