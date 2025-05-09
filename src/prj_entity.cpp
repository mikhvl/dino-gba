#include "prj_entity.h"

namespace prj
{
    Entity::Entity
    (
        bn::fixed x,
        bn::fixed y
    )
        : pos(x < 0 ? bn::max(x, -lvl::X_LIM) : bn::min(x, lvl::X_LIM), bn::min(y, lvl::Y_LIM))
    {}
    
    bn::fixed_point& Entity::get_pos() { return pos; }
    bn::rect& Entity::get_body_hitbox() { return body_hitbox; }
    bn::optional<bn::rect>& Entity::get_atk_hitbox() { return atk_hitbox; }
    
    void Entity::update() {}
    void Entity::take_damage() {}
    bool Entity::is_attacking() { return false; }
    bool Entity::is_dead() { return false; }
}