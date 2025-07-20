#include <constants.h>
#include <Enemy.h>
#include <Player.h>
#include <World_map.h>
#include <Battle.h>
#include <prompt.h>

// WORK HERE 7/19/25 3:34 PM
// if status is Power_slash, then choice is attack
// keep rolling as long as it's invalid choice (not enough mp, etc.)
// is_valid(choice)
// Enemy_option Enemy::input(Player& player, Battle& battle); // Enemy.cpp

// WORK HERE 7/19/25 3:34 PM
// vector<Enemy_option> enemy_option_tbl = { Enemy_option::attack };
// Enemy.h

vector<Enemy_option> enemy_option_tbl = { 
    Enemy_option::attack 
};

Enemy_option Enemy::input() {
    // Decide on choice based on available options in enemy_option_tbl
    // For enemies derived from class Enemy, this will be private member
    // Make different Enemy types based on their enemy_option_tbl
    int roll = rand() % enemy_option_tbl.size();
    Enemy_option choice = enemy_option_tbl[roll];
    // Limit freeze to 2 turns
    // Enemy choice during turn based on Enemy::status()
    if(status() == Status::freeze && counter() > 0) {
        choice = Enemy_option::none;
    }
    return choice;
}

// WORK HERE 7/20/25 7:17 PM
// Modify to use calc_dmg()
void Enemy::attack(Battle& battle) {
    Player& player = battle.player();
    int roll = rand() % 10;
    bool hit = (roll == 0) ? false : true;
    int dmg = 3 + (rand() % 3);

    if(hit) {
        if(player.parry_success()) {
            player.incr_mp(1);
            battle.print(Battle_frame::player_parry);
            prompt_next("Parry successful!",battle);
        }
        else if(!player.parry_success() && player.is_parry()) {
            player.decr_hp(dmg);
            battle.print(Battle_frame::enemy_attack);
            prompt_next("Parry fail!",battle);
        }
        else {
            player.decr_hp(dmg);
            battle.print(Battle_frame::enemy_attack);
            prompt_next("Enemy dealt " + to_string(dmg) + " DMG!",battle);
        }
    }
    else {
        battle.print(Battle_frame::enemy_attack);
        prompt_next("Enemy miss!",battle);
    }
}

// void Enemy::fire_shield()