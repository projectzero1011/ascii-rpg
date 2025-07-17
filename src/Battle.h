#pragma once

#include <frames.h>
#include <utils.h>
#include <Player.h>
#include <Enemy.h>

class Frame_manager;

enum class State {
    option,
    spell,
    action
};

class Battle {
public:
    Battle(Player& pp, Enemy& ee, Frame_manager& fmm) 
    : p{pp}, e{ee}, fm{fmm}, last{Battle_frame::battle} { }

    Player& player() { return p; }
    Enemy& enemy() { return e; }

    void print(Battle_frame bf);
    void refresh() { clear_screen(); print(Battle_frame::battle); }
    void print_last() { print(last); }
    void refresh_last() { clear_screen(); print_last(); }
    void apply_status();

    State handle_spell();
    void player_turn();
    void enemy_turn();
    // void engage();
private:
    Player& p;
    Enemy& e;
    Frame_manager& fm;
    Battle_frame last;
    // int turn {1};
};