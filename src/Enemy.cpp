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

vector<Enemy_option> enemy_option_tbl = { 
    Enemy_option::attack,
    Enemy_option::fire_shield,
    Enemy_option::ice_shield
};

bool is_buff(Enemy_option op) {
    switch(op) {
        case Enemy_option::fire_shield: return true;
        case Enemy_option::ice_shield: return true;
        case Enemy_option::attack: return false;
        default: throw runtime_error("Invalid Enemy_option!");
    }
    return false;
}

// Decide on choice based on available options in enemy_option_tbl
// For enemies derived from class Enemy, this will be private member
// Make different Enemy types based on their enemy_option_tbl
Enemy_option Enemy::input() {
    int roll = rand() % enemy_option_tbl.size();
    Enemy_option choice = enemy_option_tbl[roll];

    // Don't buff while another buff is active
    while(is_buff(choice) && status() != Status::none) { 
        roll = rand() % enemy_option_tbl.size();
        choice = enemy_option_tbl[roll];
    }

    const bool is_frozen = status() == Status::freeze && counter() > 0;
    if (is_frozen || status() == Status::stun) choice = Enemy_option::none;
    
    return choice;
}

constexpr int enemy_base_dmg = 3;
constexpr int enemy_hit_percent = 90;
constexpr int enemy_crit_percent = 10;

void Enemy::attack(Battle& battle) {
    Player& player = battle.player();
    bool hit = (rand() % 100) < enemy_hit_percent;
    bool is_crit = (rand() % 100) < enemy_crit_percent;
    int dmg = calc_dmg(player, enemy_base_dmg, stats.atk, is_crit);

    battle.print(Battle_frame::enemy_attack);
    if (!hit) { prompt_next("Enemy miss!", battle); return; }

    if(player.is_parry()) {
        if(player.parry_success()) {
            player.incr_mp(1);
            battle.print(Battle_frame::player_parry);
            prompt_next("Parry successful!", battle);
        }
        else {
            player.decr_hp(dmg);
            prompt_next("Parry fail!", battle);
        }
    }
    else {
        player.decr_hp(dmg);
        prompt_next("Enemy dealt " + to_string(dmg) + " DMG!", battle);
    }
}

void Enemy::fire_shield(Battle& battle) {
    stats.def += 5;
    apply_status(Status::fire_shield, 100, 3);
    battle.print(Battle_frame::enemy_fire_shield);
    prompt_next("Enemy Fire Shield!");
}

void Enemy::ice_shield(Battle& battle) {
    stats.def += 5;
    apply_status(Status::ice_shield, 100, 3);
    battle.print(Battle_frame::enemy_ice_shield);
    prompt_next("Enemy Ice Shield!");
}