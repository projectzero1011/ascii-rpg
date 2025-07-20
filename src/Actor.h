#pragma once

#include <Entity.h>

enum class Status {
    burn,
    freeze,
    none
};

struct Stats {
    Stats(int health, int mana, int attack, int defense = 0)
    : hp{health}, mp{mana}, atk{attack}, def{defense} { }
    int hp;
    int mp;
    int atk;
    int def;
};

class Actor : public Entity {
public:
    int hp() const { return h; }
    int max_hp() const { return stats.hp; }
    void decr_hp(int dmg) { h -= dmg; if (h < 0) h = 0; }
    void incr_hp(int amt) { h += amt; if (h > stats.hp) h = stats.hp; }

    int mp() const { return m; }
    int max_mp() const { return stats.mp; }
    void decr_mp(int amt) { m -= amt; if (m < 0) m = 0; }
    void incr_mp(int amt) { m += amt; if (m > stats.mp) m = stats.mp; }

    Stats stat() const { return stats; }
    Status status() const { return st; }
    void set_status(Status s) { st = s; }
    int counter() const { return count; }
    void set_counter(int c) { count = c; }
    void apply_status(Status st, int percent, int turn_duration);
protected:
    Actor(Position p, Stats s) 
    : Entity{p}, stats{s}, h{s.hp}, m{s.mp} { }
    Stats stats;
private:
    int h, m;
    Status st {Status::none};
    int count {0};
};