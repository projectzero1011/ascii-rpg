#include <Actor.h>

void Actor::apply_status(Status new_st, int percent, int turn_duration) {
    if (status() != Status::none) return;

    int roll = rand() % 100;
    if(roll < percent) {
        set_status(new_st);
        set_counter(turn_duration);
    }
}