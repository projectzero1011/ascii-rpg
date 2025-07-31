#include <Actor.h>

void Actor::apply_status(Status new_st, int percent, int turn_duration) {
    if (status() != Status::none) return;

    int roll = rand() % 100;
    if(roll < percent) {
        set_status(new_st);
        set_counter(turn_duration);
    }
}

int Actor::calc_dmg(Actor& a, int base, int add, bool is_crit) {
    int dmg = base + (rand() % add+1) - a.stats.def;
    if (is_crit) dmg *= 2;
    if (status() == Status::focus) {
        set_status(Status::none);
        incr_mp(2);
        dmg *= 3;
    }
    return dmg;
}