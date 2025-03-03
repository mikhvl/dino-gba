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


constexpr bn::fixed_t<8> speed = 2;
constexpr int run_speed = 5;


inline void control_player(bn::sprite_ptr& player, bn::sprite_animate_action<2>& action)
{
    //
    if(bn::keypad::start_pressed())
    {
        player.set_visible(!player.visible());
    }
    
    // movement test
    if(bn::keypad::left_held())
    {
        player.set_x(player.x() - speed);
    }
    else if(bn::keypad::right_held())
    {
        player.set_x(player.x() + speed);
    }
    
    // animation test
    if(bn::keypad::left_pressed())
    {
        player.set_horizontal_flip(true);
        action = bn::create_sprite_animate_action_forever(
            player, run_speed, bn::sprite_items::dino.tiles_item(), 1, 2);
    } 
    else if(bn::keypad::right_pressed())
    {
        player.set_horizontal_flip(false);
        action = bn::create_sprite_animate_action_forever(
            player, run_speed, bn::sprite_items::dino.tiles_item(), 1, 2);
    }
    else if(!bn::keypad::left_held() && !bn::keypad::right_held())
    {
        action = bn::create_sprite_animate_action_once(
            player, run_speed, bn::sprite_items::dino.tiles_item(), 0, 0);
    }
    
    action.update();
}

int main()
{
    bn::core::init();
    
    bn::sprite_ptr player = bn::sprite_items::dino.create_sprite(0, 32);
    bn::regular_bg_ptr bg = bn::regular_bg_items::bg_temp.create_bg(0, 0);
    
    bn::sprite_animate_action<2> action = bn::create_sprite_animate_action_once(
        player, run_speed, bn::sprite_items::dino.tiles_item(), 0, 0);

    while(true)
    {
        control_player(player, action);
        bn::core::update();
    }
}
