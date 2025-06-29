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
};

// add get weakness, set weakness, and private member weakness
class Enemy : public Actor {
public:
    Enemy(Position pos, int hp, int mp) : Actor::Actor(pos,hp,mp) { }
    string sprite() const override { return "%"; }

    void move(const Player& player, World_map& world);
    virtual Enemy_option input(Player& player, Battle& battle);
    void attack(Player&, Battle&);

    bool in_vicinity(const Player&, const World_map&);
};