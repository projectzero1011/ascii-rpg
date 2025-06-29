#pragma once

#include <Actor.h>
#include <Key.h>

class Enemy;
class Battle;
class World_map;

class Player : public Actor {
public:
    Player(Position pos, int hp, int mp) : Actor::Actor(pos,hp,mp) { }
    string sprite() const override { return "@"; }

    void move(Key input, World_map& world);
    void attack(Enemy&, Battle&);
    void parry();
    void fire(Enemy&, Battle&);
    void ice(Enemy&, Battle&);
    // cast aegis
    void heal(Battle&);

    bool has_item() const { return item; }
    void set_has_item(bool b) { item = b; }
    bool is_parry() const { return parrying; }
    bool parry_success() const { return parried; }
    void reset_parry() { parrying = false; parried = false; }
private:
    bool parrying {false}, parried {false};
    bool item {false};
};