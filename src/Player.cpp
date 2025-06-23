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
    bool crit = (roll == 1) ? true : false;

    int dmg = 3 + rand() % 2;
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

// Implement messages that say "Player dealt [x] DMG!"
// Make this an Actor method so that enemies can cast fire too
void Player::fire(Enemy& enemy, Battle& battle) {
    decr_mp(1);
    enemy.decr_hp(6);
    
    // Handle burn status
    int roll = rand() % 10;
    int burn = (roll <= 3) ? true : false;
    if(burn && enemy.status() == Status::none) { 
        enemy.set_status(Status::burn); 
        enemy.set_counter(3); 
    }
    
    battle.print(Battle_frame::player_fire);
    prompt_next("Cast fire!",battle);
}

// Implement messages that say "Player dealt [x] DMG!"
void Player::ice(Enemy& enemy, Battle& battle) {
    decr_mp(2);
    enemy.decr_hp(8);
    // chance of freeze

    battle.print(Battle_frame::player_ice);
    prompt_next("Cast ice!",battle);
}

void Player::heal(Battle& battle) {
    int amt = max_hp()*(2.0/3.0);
    incr_hp(amt);
    decr_mp(2);
    battle.refresh();
    prompt_next("Player healed " + to_string(amt) + " HP!",battle);
}
