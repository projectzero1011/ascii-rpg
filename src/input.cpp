#include <input.h>
#include <utils.h>
#include <limits>

Selection title_input(string msg) {
    char ch = get_char(msg);
    switch(ch) {
        case 'z': return Selection::start; break;
        case 'h': return Selection::help; break;
        default: return Selection::invalid; break;
    }
}

Key world_input(string msg) {
    char ch = get_char(msg);
    switch(ch) {
        case 'w': return Key::up;
        case 'a': return Key::left;
        case 's': return Key::down;
        case 'd': return Key::right;
        case 'h': return Key::help;
        default:  return Key::invalid;
    }
}

Option battle_input(string msg) {
    char ch = get_char(msg);
    switch(ch) {
        case '1': return Option::attack;
        case '2': return Option::parry;
        case '3': return Option::spell;
        // case '4': 
        case 'h': return Option::help;
        case 'x': return Option::back;
        default:  return Option::invalid;
    }
}

Spell spell_input(string message) {
    char ch = get_char(message);
    switch(ch) {
        case '7': return Spell::fire;
        case '8': return Spell::ice;
        case '9': return Spell::heal;
        // case '0': return Spell::aegis;
        case 'x': return Spell::back;
        default:  return Spell::invalid;
    }
}

Answer end_input(string msg) {
    char ch = get_char(msg);
    switch(ch) {
        case 'y': case 'Y': return Answer::yes;
        case 'n': case 'N': return Answer::no;
        default: return Answer::none; 
    }
}