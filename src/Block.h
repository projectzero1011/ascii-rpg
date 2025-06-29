#pragma once

#include <Entity.h>

class Block : public Entity {
public:
    using Entity::Entity;
    string sprite() const override { return "█"; }
};