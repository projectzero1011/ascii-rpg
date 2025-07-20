#pragma once

#include <Actor.h>
#include <Key.h>

class Enemy;
class Battle;
class World_map;
enum class State;

class Player : public Actor {
public:
    Player(Position pos, Stats stats) : Actor::Actor(pos,stats) { }
    string sprite() const override { return "@"; }

    void move(Key input, World_map& world);
    void attack(Battle&);
    void parry();
    State fire(Battle&);
    State ice(Battle&);
    // cast aegis
    State heal(Battle&);

    bool is_parry() const { return parrying; }
    bool parry_success() const { return parried; }
    void reset_parry() { parrying = false; parried = false; }
    int calc_dmg(Battle& b, int base, int add, bool is_crit);
private:
    bool parrying {false}, parried {false};
};
