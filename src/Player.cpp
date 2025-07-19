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

void Player::attack(Battle& battle) {
    Enemy& enemy = battle.enemy();

    int roll = rand() % 10;
    bool hit = (roll == 0) ? false : true;
    if (enemy.status() == Status::freeze) hit = true;
    bool crit = (roll == 1) ? true : false;

    // Damage calc
    int dmg = 3 + rand() % 3;
    if (crit) dmg *= 3;

    if(hit) {
        if(crit) {
            battle.print(Battle_frame::player_crit);
            prompt_next("Take this!",battle);
            incr_mp(1);
        }
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
    int roll = rand() % 10;
    (roll <= 6) ? parried = true : parried = false;
}

constexpr int fire_dmg = 8;

State Player::fire(Battle& battle) {
    if(mp() < 1) { 
        prompt_next("Fire requires 1 MP!", battle); 
        return State::spell; 
    }
    Enemy& enemy = battle.enemy();
    decr_mp(1);

    // Damage calc
    int dmg = fire_dmg + (rand() % 5);
    enemy.decr_hp(dmg);
    
    // Handle burn chance
    int roll = rand() % 10;
    bool is_burn = (roll < 4) ? true : false;
    if(is_burn && enemy.status() == Status::none) { 
        enemy.set_status(Status::burn); 
        enemy.set_counter(3); 
    }
    
    battle.print(Battle_frame::player_fire);
    prompt_next("Player dealt " + to_string(dmg) + " DMG!", battle);
    return State::action;
}

constexpr int ice_dmg = 10;

State Player::ice(Battle& battle) {
    if(mp() < 2) { 
        prompt_next("Ice requires 2 MP!", battle); 
        return State::spell; 
    }
    Enemy& enemy = battle.enemy();
    decr_mp(2);

    // Damage calc
    int dmg = ice_dmg + (rand() % 5);
    enemy.decr_hp(dmg);
    
    // Handle freeze chance
    int roll = rand() % 10;
    bool is_freeze = (roll < 4) ? true : false;
    if(is_freeze && enemy.status() == Status::none) {
        enemy.set_status(Status::freeze);
        enemy.set_counter(2);
    }

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
