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
    State heal(Battle&);
    State focus(Battle&);

    bool is_parry() const { return parrying; }
    bool parry_success() const { return parried; }
    void reset_parry() { parrying = false; parried = false; }
private:
    bool parrying {false}, parried {false};
};
