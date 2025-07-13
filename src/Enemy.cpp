#include <constants.h>
#include <Enemy.h>
#include <Player.h>
#include <World_map.h>
#include <Battle.h>
#include <prompt.h>

// Could use if (player_to_left) or (player_to_right), etc
void Enemy::move(const Player& player, World_map& world) {
    int player_row = player.row();
    int player_col = player.col();
    int new_row = row();
    int new_col = col();

    if (player_row == new_row && player_col < new_col) --new_col;
    else if (player_row == new_row && player_col > new_col) ++new_col;

    if (player_row < new_row) --new_row;
    else if (player_row > new_row) ++new_row;

    string new_cell = world.look(new_row,new_col);

    if(new_cell == empty_cell) {
        world.remove(*this);
        pos = Position{new_row,new_col};
        world.place(*this);
    }
}

// WORK HERE 3/1/25 5:47 PM
// if status is Power_slash, then choice is attack
// keep rolling as long as it's invalid choice (not enough mp, etc.)
// is_valid(choice)

vector<Enemy_option> enemy_option_tbl = { 
    Enemy_option::attack 
};

Enemy_option Enemy::input() {
    int roll = rand() % enemy_option_tbl.size();
    Enemy_option choice = enemy_option_tbl[roll];
    // Limit freeze to 2 turns
    if(status() == Status::freeze && counter() > 0) {
        choice = Enemy_option::none;
    }
    return choice;
}

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

bool Enemy::in_vicinity(const Player& p, const World_map& world) {
    string enemy_left = world.look(row(), col() - 1);
    string enemy_right = world.look(row(), col() + 1);
    string enemy_up = world.look(row() - 1, col());
    string enemy_down = world.look(row() + 1, col());

    if (enemy_left == p.sprite()) return true;
    if (enemy_right == p.sprite()) return true;
    if (enemy_up == p.sprite()) return true;
    if (enemy_down == p.sprite()) return true;

    return false;
}