#include <Player.h>
#include <World_map.h>
#include <Battle.h>
#include <prompt.h>

void Player::move(Key input, World_map& world) {
    int new_row = row();
    int new_col = col();

    switch(input) {
        case Key::up:    --new_row; break;
        case Key::left:  --new_col; break;
        case Key::down:  ++new_row; break;
        case Key::right: ++new_col; break;
        default: throw runtime_error("Invalid key");
    }

    string new_cell = world.look(new_row,new_col);

    if(new_cell == empty_cell) {
        world.remove(*this);
        pos = Position{new_row,new_col};
        world.place(*this);
    }
}

int Player::calc_dmg(Battle& b, int base, int add, bool is_crit) {
    Enemy& enemy = b.enemy();
    int dmg = base + (rand() % add+1) - enemy.stat().def;
    if (is_crit) dmg *= 3;
    return dmg;
}

constexpr int base_dmg = 3;

void Player::attack(Battle& battle) {
    Enemy& enemy = battle.enemy();

    int hit_roll = rand() % 100;
    bool hit = (hit_roll < 10) ? false : true;
    if (enemy.status() == Status::freeze) hit = true;

    if(hit) {
        int crit_roll = rand() % 100;
        bool is_crit = (crit_roll < 10) ? true : false; 
        if(is_crit) { 
            battle.print(Battle_frame::player_crit);
            prompt_next("Take this!", battle); 
            incr_mp(1); 
        }
        int dmg = calc_dmg(battle, base_dmg, stats.atk, is_crit);
        enemy.decr_hp(dmg);
        incr_mp(1);
        battle.print(Battle_frame::player_attack);
        prompt_next("Player dealt " + to_string(dmg) + " DMG!",battle);
    }
    else {
        battle.print(Battle_frame::player_attack);
        prompt_next("Miss!",battle);
    }
}

void Player::parry() {
    parrying = true;
    int roll = rand() % 100;
    (roll < 70) ? parried = true : parried = false;
}

constexpr int fire_dmg = 8;
constexpr int fire_percent = 40;
constexpr int fire_turn_duration = 3;

State Player::fire(Battle& battle) {
    if(mp() < 1) { 
        prompt_next("Fire requires 1 MP!", battle); 
        return State::spell; 
    }
    Enemy& enemy = battle.enemy();
    decr_mp(1);

    int dmg = calc_dmg(battle, fire_dmg, 5, false);
    enemy.decr_hp(dmg);
    enemy.apply_status(Status::burn, fire_percent, fire_turn_duration);
    
    battle.print(Battle_frame::player_fire);
    prompt_next("Player dealt " + to_string(dmg) + " DMG!", battle);
    return State::action;
}

constexpr int ice_dmg = 10;
constexpr int ice_percent = 40;
constexpr int ice_turn_duration = 2;

State Player::ice(Battle& battle) {
    if(mp() < 2) { 
        prompt_next("Ice requires 2 MP!", battle); 
        return State::spell; 
    }
    Enemy& enemy = battle.enemy();
    decr_mp(2);

    int dmg = calc_dmg(battle, ice_dmg, 5, false);
    enemy.decr_hp(dmg);
    enemy.apply_status(Status::freeze, ice_percent, ice_turn_duration);

    battle.print(Battle_frame::player_ice);
    prompt_next("Player dealt " + to_string(dmg) + " DMG!",battle);
    return State::action;
}

State Player::heal(Battle& battle) {
    if(mp() < 2) { 
        prompt_next("Heal requires 2 MP!", battle); 
        return State::spell; 
    }
    decr_mp(2);

    // Heal calc
    int amt = max_hp()*(2.0/3.0);
    incr_hp(amt);

    // Animation
    battle.refresh();
    prompt_next("Player healed " + to_string(amt) + " HP!",battle);
    return State::action;
}
