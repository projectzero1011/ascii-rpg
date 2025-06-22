#pragma once

#include <Entity.h>

class Item : public Entity {
public:
    using Entity::Entity;
    string sprite() const override { return "♠"; }
};