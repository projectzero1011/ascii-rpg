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

    if(new_cell == item_sprite || new_cell == empty_cell) {
        world.erase(Position{new_row,new_col});
        world.remove(*this);
        pos = Position{new_row,new_col};
        world.place(*this);
    }

    if(new_cell == item_sprite) {
        set_has_item(true);
        world.refresh();
        prompt_next("Item Get!",world);
    }
}

void Player::attack(Enemy& enemy, Battle& battle) {
    int roll = rand() % 10;
    bool hit = (roll == 0) ? false : true;
    if (enemy.status() == Status::freeze) hit = true;
    bool crit = (roll == 1) ? true : false;

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

void Player::fire(Enemy& enemy, Battle& battle) {
    decr_mp(1);
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
}

constexpr int ice_dmg = 11;

void Player::ice(Enemy& enemy, Battle& battle) {
    decr_mp(2);
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
}

void Player::heal(Battle& battle) {
    int amt = max_hp()*(2.0/3.0);
    incr_hp(amt);
    decr_mp(2);
    battle.refresh();
    prompt_next("Player healed " + to_string(amt) + " HP!",battle);
}
