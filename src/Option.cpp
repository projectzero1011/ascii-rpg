#include <Option.h>

bool is_battle_option(Option op) {
    switch(op) {
        case Option::invalid: return false; break;
        case Option::help:    return false; break;
        case Option::spell:   return false; break;
        case Option::back:    return false; break;
        default: return true; break;
    }
}