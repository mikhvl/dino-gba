#include "prj_entity.h"

namespace prj
{
    Entity::Entity
    (
        int entity_type,
        bn::fixed x,
        bn::fixed y,
        bn::sprite_item sprite_item
    )
        : type(entity_type)
        , pos(x, bn::min(y, lvl::Y_LIM))
        , spr_item(sprite_item)
        , spr(spr_item.create_sprite(pos))
        , shadow(bn::sprite_items::shadow.create_sprite())
    {
        set_shadow_position();
        
        spr.set_bg_priority(lvl::ENTITY_PRIORITY);
        shadow.set_bg_priority(lvl::ENTITY_PRIORITY);
        
        spr.set_z_order(entity::ENTITY_Z_ORDER);
        shadow.set_z_order(entity::SHADOW_Z_ORDER);
    }
    
    int Entity::get_type() { return type; }
    bn::fixed_point Entity::get_pos() { return pos; }
    bn::rect Entity::get_body_hitbox() { return body_hitbox; }
    bn::rect Entity::get_atk_hitbox() { return atk_hitbox; }
    
    void Entity::update() {}
    void Entity::take_damage(bool from_left) { if(from_left) {} }
    
    bool Entity::is_dying() { return _death != not_death; }
    bool Entity::is_dead() { return _death == end_death; }
    bool Entity::is_attacking() { return !is_dying(); }
    
    void Entity::set_camera(const bn::camera_ptr& cam)
    {
        spr.set_camera(cam);
        shadow.set_camera(cam);
    }
    
    void Entity::set_shadow_position()
    {
        bn::fixed height = lvl::Y_LIM - pos.y();
        
        if(height > entity::SHADOW_HEIGHT_SMALL)
        {
            shadow.set_tiles(bn::sprite_items::shadow.tiles_item(), entity::shadow_tile::SMALL);
        }
        else if(height > entity::SHADOW_HEIGHT_MEDIUM)
        {
            shadow.set_tiles(bn::sprite_items::shadow.tiles_item(), entity::shadow_tile::MEDIUM);
        }
        else
        {
            shadow.set_tiles(bn::sprite_items::shadow.tiles_item(), entity::shadow_tile::BIG);
        }
        
        shadow.set_position
        (
            pos.x() + height.division(entity::SHADOW_X_COEF),
            lvl::Y_LIM + entity::SHADOW_Y_OFFSET - height.division(entity::SHADOW_Y_COEF)
        );
    }
}