#pragma once

#include <Position.h>
#include <vector>
#include <Actor.h>

class Player;
class Battle;
class World_map;

enum class Enemy_option {
    attack,
    // fire
    // ice
    fire_shield,
    ice_shield,
    none
};

// add get weakness, set weakness, and private member weakness
class Enemy : public Actor {
public:
    Enemy(Position pos, Stats stats) : Actor::Actor(pos,stats) { }
    string sprite() const override { return "%"; }

    virtual Enemy_option input();
    void attack(Battle&);
    void fire_shield(Battle&);
    void ice_shield(Battle&);
};

bool is_buff(Enemy_option op);