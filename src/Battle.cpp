#include <Battle.h>
#include <iomanip>
#include <prompt.h>
#include <input.h>
#include <World_map.h>

const vector<string> enemy_status_tbl = {
    "Burn",
    "Freeze",
    "Fire Shield",
    // "Ice Shield"
    "Stun",
    "None"
};

void Battle::print(Battle_frame bf) {
    last = bf;
    clear_screen();
    const string e_st = enemy_status_tbl[int(e.status())];
    cout << "Player" << "\t\t" << setw(8) << "Enemy" << "\n"
         << "HP " << setw(2) << p.hp() << "/" << p.max_hp() << "\t" 
         << "HP " << setw(2) << e.hp() << "/" << e.max_hp() << "\n" 
         << "MP  " << p.mp() << "/ " << p.max_mp() << "\t" << "ST " << e_st 
         << "\n\n";
    fm.print(bf);
}

constexpr int burn = 3;

void Battle::status_tick(Actor& a) {
    if(a.counter() > 0) {
        a.set_counter(a.counter()-1);
        switch(a.status()) {
            case Status::burn:
                a.decr_hp(burn);
                print(Battle_frame::enemy_burn);
                prompt_next("Burn dealt " + to_string(burn) + " DMG", *this);
                break;
            case Status::freeze:
                print(Battle_frame::enemy_freeze);
                prompt_next("The Enemy is Frozen!");
                break;
            case Status::fire_shield:
                break;
            case Status::stun:
                // 
                break;
            default: 
                throw runtime_error("Invalid Status!"); 
                break;
        }
    }
    else {
        a.set_status(Status::none);
        a.reset_stats();
    }
}

State Battle::handle_spell() {
    State state = State::spell;
    while(state == State::spell) {
        print(Battle_frame::spell_options);
        Spell choice = spell_input("What Spell? [x: back]");

        switch(choice) {
            case Spell::fire:
                state = p.fire(*this);
                break;
            case Spell::ice:
                state = p.ice(*this);
                break;
            case Spell::heal:
                state = p.heal(*this);
                break;
            /*
            case Spell::aegis:
                state = State::action;
                player.aegis();
                break;
            */
            case Spell::back:
                state = State::option;
                refresh();
                break;
            case Spell::invalid:
                refresh();
                break;
            default:
                throw runtime_error("Invalid Spell!");
                break;
        }
    }
    return state;
}

void Battle::player_turn() {
    p.reset_parry();
    State state = State::option;
    while(state == State::option) {
        Option choice = battle_input("What will you do? [h: help]");

        switch(choice) {
            case Option::attack:
                state = State::action;
                p.attack(*this);
                break;
            case Option::parry:
                state = State::action;
                p.parry();
                break;
            case Option::spell:
                state = handle_spell();
                break;
            /*
            case Option::item:
                // 
                break;
            */
            case Option::help:
                show_help_battle(fm);
                refresh();
                break;
            case Option::back:
                state = State::option;
                refresh();
                break;
            case Option::invalid:
                refresh_last();
                break;
            default:
                throw runtime_error("Invalid Option!");
                break;
        }
    }
}

void Battle::enemy_turn() {
    Enemy_option enemy_choice = e.input();

    switch(enemy_choice) {
        case Enemy_option::attack:
            e.attack(*this);
            break;
        /*
        case Enemy_option::fire:
            e.fire(player,battle);
            break;
        case Enemy_option::ice:
            e.ice(player,battle);
            break;
        case Enemy_option::attack_up:
            e.attack_up(battle);
            break;
        */
        case Enemy_option::fire_shield:
            e.fire_shield(*this);
            break;
        case Enemy_option::none:
            break;
        default:
            throw runtime_error("Enemy_option doesn't exist!");
            break;
    }
}

void Battle::engage(const World_map& world) {
    refresh();

    while(true) {
        player_turn();
        if(e.hp() == 0) {
            world.refresh();
            prompt_next("You won!");
            return;
        }

        enemy_turn();
        if(p.hp() == 0) {
            return;
        }

        status_tick(p);
        status_tick(e);
        refresh();
    }
}