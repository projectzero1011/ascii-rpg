#pragma once

#include <Actor.h>
#include <Key.h>

class Enemy;
class Battle;
class World_map;
enum class State;

class Player : public Actor {
public:
    Player(Position pos, int hp, int mp) : Actor::Actor(pos,hp,mp) { }
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
private:
    bool parrying {false}, parried {false};
};

int calc_dmg(int base_dmg, int add_dmg, bool is_crit = false);