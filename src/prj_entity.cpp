#include "prj_entity.h"

namespace prj
{
    Entity::Entity
    (
        bn::fixed x,
        bn::fixed y,
        bn::sprite_item sprite_item
    )
        : pos(x < 0 ? bn::max(x, -lvl::X_LIM) : bn::min(x, lvl::X_LIM), bn::min(y, lvl::Y_LIM))
        , spr_item(sprite_item)
        , spr(spr_item.create_sprite(pos))
    {}
    
    bn::fixed_point& Entity::get_pos() { return pos; }
    bn::rect& Entity::get_body_hitbox() { return body_hitbox; }
    bn::optional<bn::rect>& Entity::get_atk_hitbox() { return atk_hitbox; }
    
    void Entity::update() {}
    void Entity::take_damage() {}
    bool Entity::is_attacking() { return false; }
    bool Entity::is_dead() { return false; }
    
    void Entity::set_camera(const bn::camera_ptr& cam) { spr.set_camera(cam); }
}