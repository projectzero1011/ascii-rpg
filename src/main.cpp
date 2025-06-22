#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
#include <iomanip>
#include <constants.h>
#include <Position.h>
using namespace std;

//-----------------------------------------------------------------------------

// Position.h

//-----------------------------------------------------------------------------

// Position.cpp

//-----------------------------------------------------------------------------

class Entity {
public:
    Entity(Position p) : pos{p} { }

    int row() const { return pos.row(); }
    int col() const { return pos.col(); }

    virtual string sprite() const = 0;
protected:
    Position pos;
};

//-----------------------------------------------------------------------------

class World_map {
public:
    void print() const;
    void refresh() const;
    string look(int row, int col) const;
    void place(const Entity&);
    void remove(Entity& e) { grid[e.row()][e.col()] = empty_cell; }
    void erase(Position pos) { grid[pos.row()][pos.col()] = empty_cell; }
private:
    string grid[ROWS][COLS]
    {
        {".", ".", ".", "."},
        {".", ".", ".", "."},
        {".", ".", ".", "."},
        {".", ".", ".", "."}
    };
};

//-----------------------------------------------------------------------------

void clear_screen() {
    system("cls");
}

//-----------------------------------------------------------------------------

void World_map::print() const {
    cout << "╔═══════════════════╗\n"
         << "║                   ║\n";

    for(int row = 0; row < ROWS; row++) {
        cout << "║   ";

        for(int col = 0; col < COLS; col++) {
            cout << grid[row][col] << "   ";
        }

        cout << "║\n";
        cout << "║                   ║\n";
    }
    
    cout << "╚═══════════════════╝\n\n";
}

//-----------------------------------------------------------------------------

void World_map::refresh() const {
    clear_screen();
    print();
}

//-----------------------------------------------------------------------------

string World_map::look(int row, int col) const {
    if (is_out_of_range(row,col)) return null_cell;
    return grid[row][col];
}

//-----------------------------------------------------------------------------

void World_map::place(const Entity& e) {
    if(grid[e.row()][e.col()] != empty_cell) {
        throw runtime_error("Entity Overlap");
    }
    grid[e.row()][e.col()] = e.sprite();
}

//-----------------------------------------------------------------------------

class Item : public Entity {
public:
    using Entity::Entity;
    string sprite() const override { return "♠"; }
};

//-----------------------------------------------------------------------------

class Block : public Entity {
public:
    using Entity::Entity;
    string sprite() const override { return "█"; }
};

//-----------------------------------------------------------------------------

enum class Status {
    burn,
    freeze,
    none
};

//-----------------------------------------------------------------------------

class Actor : public Entity {
public:
    int hp() const { return h; }
    int max_hp() const { return max_h; }
    void decr_hp(int dmg) { h -= dmg; if (h < 0) h = 0; }
    void incr_hp(int amt) { h += amt; if (h > max_h) h = max_h; }

    int mp() const { return m; }
    int max_mp() const { return max_m; }
    void decr_mp(int amt) { m -= amt; if (m < 0) m = 0; }
    void incr_mp(int amt) { m += amt; if (m > max_m) m = max_m; }

    Status status() const { return st; }
    void set_status(Status s) { st = s; }
    int counter() const { return count; }
    void set_counter(int c) { count = c; }
protected:
    Actor(Position p, int hp, int mp) 
    : Entity{p}, h{hp}, max_h{hp}, max_m{mp} { }
private:
    int h, max_h;
    int m {0}, max_m;
    Status st {Status::none};
    int count {0};
};

//-----------------------------------------------------------------------------

enum class Selection {
    start,
    help,
    invalid
};

//-----------------------------------------------------------------------------

Selection title_input(string message) {
    cout << message << "\n"
         << "> ";
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    
    char ch = (input.size() == 1) ? input[0] : ' ';

    switch(ch) {
        case 'z': return Selection::start; break;
        case 'h': return Selection::help; break;
        default: return Selection::invalid; break;
    }
}

//-----------------------------------------------------------------------------

enum class Key : char {
    up = 'w',
    left = 'a',
    down = 's',
    right = 'd',
    help = 'h',
    // quit
    invalid
};

//-----------------------------------------------------------------------------

Key world_input(string message) {
    cout << message << "\n"
         << "> ";
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    
    char ch = (input.size() == 1) ? input[0] : ' ';

    switch(ch) {
        case 'w': return Key::up;
        case 'a': return Key::left;
        case 's': return Key::down;
        case 'd': return Key::right;
        case 'h': return Key::help;
        default:  return Key::invalid;
    }
}

//-----------------------------------------------------------------------------

enum class Option {
    attack = 1,
    parry = 2,
    heal = 3,
    spell = 4,
    fire = 7,
    ice = 8,
    help,
    back,
    invalid
};

//-----------------------------------------------------------------------------

enum class Enemy_option;

//-----------------------------------------------------------------------------

bool is_battle_option(Option op) {
    switch(op) {
        case Option::invalid: return false; break;
        case Option::help:    return false; break;
        case Option::spell:   return false; break;
        case Option::back:    return false; break;
        default: return true; break;
    }
}

//-----------------------------------------------------------------------------

// WORK HERE 3/1/25 3:47 PM

Option battle_input(string message) {
    cout << message << "\n"
         << "> ";
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    
    char ch = (input.size() == 1) ? input[0] : ' ';

    switch(ch) {
        case '1': return Option::attack;
        case '2': return Option::parry;
        // case '3': return Option::heal;
        case '4': return Option::spell;
        case '7': return Option::fire;
        case '8': return Option::ice;
        case '9': return Option::heal;
        // case '0': return Option::aegis
        case 'h': return Option::help;
        case 'z': return Option::back;
        default:  return Option::invalid;
    }
}

//-----------------------------------------------------------------------------

class Battle;

//-----------------------------------------------------------------------------

class Enemy;

//-----------------------------------------------------------------------------

class Player : public Actor {
public:
    Player(Position pos, int hp, int mp) : Actor::Actor(pos,hp,mp) { }
    string sprite() const override { return "@"; }

    void move(Key input, World_map& world);
    void attack(Enemy&, Battle&);
    void parry();
    void fire(Enemy&, Battle&);
    void ice(Enemy&, Battle&);
    // cast aegis
    void heal(Battle&);

    bool has_item() const { return item; }
    void set_has_item(bool b) { item = b; }
    bool is_parry() const { return parrying; }
    bool parry_success() const { return parried; }
    void reset_parry() { parrying = false; parried = false; }
private:
    bool parrying {false}, parried {false};
    bool item {false};
};

//-----------------------------------------------------------------------------

// add get weakness, set weakness, and private member weakness
class Enemy : public Actor {
public:
    Enemy(Position pos, int hp, int mp) : Actor::Actor(pos,hp,mp) { }
    string sprite() const override { return "%"; }

    void move(const Player&, World_map& world);
    virtual Enemy_option input(Player& player, Battle& battle);
    void attack(Player&, Battle&);

    bool in_vicinity(const Player&, const World_map&);
};

// Putting enemy here will fix error of not declaring enemy before 
// refresh_battle() in line 224, do this to implement battle msgs

//-----------------------------------------------------------------------------

enum class Frame {
    title,
    help_world,
    help_battle
};

//-----------------------------------------------------------------------------

enum class Battle_frame {
    battle,
    player_attack,
    player_crit,
    player_parry,
    spell_options,
    player_fire,
    player_ice,
    enemy_attack,
    enemy_burn
};

//-----------------------------------------------------------------------------

const vector<string> frame_tbl = {"title", "help_world","help_battle"};

//-----------------------------------------------------------------------------

const vector<string> battle_frame_tbl = {"battle", "player_attack", 
"player_crit", "player_parry", "spell_options", "player_fire", "player_ice", 
"enemy_attack", "enemy_burn"};

//-----------------------------------------------------------------------------

// bool find(vector<string> v, string& name)

//-----------------------------------------------------------------------------

// declare a constant for the file extension of frames
// declare a constant for the file path
// helper function to class Frame_manager
// needs pre-condition to prevent random string inputs
string load(string& name) {
    ifstream ist {"../files/" + name + ".txt"};
    if (!ist) throw runtime_error("Can't open input file");

    string frame;
    for (char ch; ist.get(ch);) frame += ch;

    return frame;
}

//-----------------------------------------------------------------------------

class Frame_manager {
public:
    Frame_manager();
    void print(Frame f) const { cout << frames[int(f)]; }
    void print(Battle_frame bf) const { cout << battle_frames[int(bf)]; }
    // void play_cutscene(Frame f);
private:
    vector<string> frames;
    vector<string> battle_frames;
    // vector<vector<string>> script;
};

// vector<vector<string>> will contain dialogue for each cutscene
// outer vector contains entire script of the game
// inner vector contains script of particular cutscenes
// have separate files for each cutscene script
// read into vector script line by line

//-----------------------------------------------------------------------------

Frame_manager::Frame_manager() {
    for(string fname : frame_tbl) {
        string f = load(fname);
        frames.push_back(f);
    }

    for(string bfname : battle_frame_tbl) {
        string bf = load(bfname);
        battle_frames.push_back(bf);
    }
}

//-----------------------------------------------------------------------------

class Battle {
public:
    Battle(Player& pp, Enemy& ee, Frame_manager& fmm) 
    : p{pp}, e{ee}, fm{fmm}, last{Battle_frame::battle} { }

    void print(Battle_frame bf);
    void refresh() { clear_screen(); print(Battle_frame::battle); }
    void print_last() { print(last); }
    void refresh_last() { clear_screen(); print_last(); }
    void apply_status(Actor& a);
private:
    Player& p;
    Enemy& e;
    Frame_manager& fm;
    Battle_frame last;
    // int turn {1};
};

//-----------------------------------------------------------------------------

// WORK HERE 3/10 8:37 AM

// apply_status()

//-----------------------------------------------------------------------------

void Battle::print(Battle_frame bf) {
    last = bf;
    clear_screen();
    cout << "Player" << "\t\t" << setw(8) << "Enemy" << "\n"
         << "HP " << setw(2) << p.hp() << "/" << p.max_hp() << "\t" 
         << "HP " << setw(2) << e.hp() << "/" << e.max_hp() << "\n" 
         << "MP  " << p.mp() << "/ " << p.max_mp() << "\t" << "\n\n";
    fm.print(bf);
}

//-----------------------------------------------------------------------------

void prompt_next(string message) {
    cout << message + " [z: next]" << "\n"
         << "> ";
    string s;
    cin >> s;
}

//-----------------------------------------------------------------------------

void prompt_next(string message, const World_map& world) {
    char ch = ' ';
    while(ch != 'z') {
        cout << message + " [z: next]" << "\n"
             << "> ";
        string input;
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    
        ch = (input.size() == 1) ? input[0] : ' ';
        world.refresh();
    }
}

//-----------------------------------------------------------------------------

void prompt_next(string message, Battle& battle) {
    battle.refresh_last();
    char ch = ' ';
    while(ch != 'z') {
        cout << message + " [z: next]" << "\n"
             << "> ";
        string input;
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    
        ch = (input.size() == 1) ? input[0] : ' ';
        battle.refresh_last();
    }
}

//-----------------------------------------------------------------------------

void prompt_next(string message, Frame f, const Frame_manager& fm) {
    char ch = ' ';
    while(ch != 'z') {
        cout << message + " [z: next]" << "\n"
             << "> ";
        string input;
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    
        ch = (input.size() == 1) ? input[0] : ' ';
        clear_screen();
        fm.print(f);
    }
}

//-----------------------------------------------------------------------------

void Battle::apply_status(Actor& a) {
    if(a.counter() > 0) {
        switch(a.status()) {
            case Status::burn:
                // a.burn();
                a.decr_hp(3);
                a.set_counter(a.counter()-1);
                print(Battle_frame::enemy_burn);
                prompt_next("Burn DMG Taken!");
                break;
            default: 
                throw runtime_error("Invalid Status!"); 
                break;
        }
    }
    else {
        a.set_status(Status::none);
    }
}

//-----------------------------------------------------------------------------

void show_help_world(const Frame_manager& fm) {
    clear_screen();
    fm.print(Frame::help_world);
    prompt_next("Back",Frame::help_world,fm);
}

//-----------------------------------------------------------------------------

void show_help_battle(const Frame_manager& fm) {
    clear_screen();
    fm.print(Frame::help_battle);
    prompt_next("Back",Frame::help_battle,fm);
}

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

// implement message that says "Player dealt [x] DMG!"
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

//-----------------------------------------------------------------------------

void Player::parry() {
    parrying = true;
    int roll = rand() % 10;
    (roll <= 6) ? parried = true : parried = false;
}

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

// Implement messages that say "Player dealt [x] DMG!"
void Player::ice(Enemy& enemy, Battle& battle) {
    decr_mp(2);
    enemy.decr_hp(8);
    // chance of freeze

    battle.print(Battle_frame::player_ice);
    prompt_next("Cast ice!",battle);
}

//-----------------------------------------------------------------------------

void Player::heal(Battle& battle) {
    int amt = max_hp()*(2.0/3.0);
    incr_hp(amt);
    decr_mp(2);
    battle.refresh();
    prompt_next("Player healed " + to_string(amt) + " HP!",battle);
}

//-----------------------------------------------------------------------------

// void Player::aegis(Battle& battle)

//-----------------------------------------------------------------------------

// class Enemy was here

//-----------------------------------------------------------------------------

// Needs work (particularly in simplifying the conditionals)
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

//-----------------------------------------------------------------------------

enum class Enemy_option {
    attack,
    // fire
};

//-----------------------------------------------------------------------------

vector<Enemy_option> enemy_option_tbl = {Enemy_option::attack};

//-----------------------------------------------------------------------------

// WORK HERE 3/1/25 5:47 PM
// if status is Power_slash, then choice is attack
// keep rolling as long as it's invalid choice (not enough mp, etc.)
// is_valid(choice)

Enemy_option Enemy::input(Player& player, Battle& battle) {
    int roll = rand() % enemy_option_tbl.size();
    Enemy_option choice = enemy_option_tbl[roll];
    return choice;
}

//-----------------------------------------------------------------------------

// Could use more work
void Enemy::attack(Player& player, Battle& battle) {
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

//-----------------------------------------------------------------------------

// getting enemy vicinity cells can be helper functions
// needs to be reworked
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

//-----------------------------------------------------------------------------

void play_cutscene(const Frame_manager& fm, Frame f, string in_file) {
    ifstream ist {in_file};
    if (!ist) throw runtime_error("Can't open file!");

    clear_screen();
    fm.print(f);
    cout << "════════════════════════\n ";

    int char_count = 0;
    int line_char_count = 0;

    for(string word; ist >> word;) {
        char_count += word.size()+1;
        line_char_count += word.size()+1;

        if(char_count > 46) {
            // Bottom border
            cout << "\n════════════════════════\n\n";
            prompt_next("Next");

            // Top border
            clear_screen();
            fm.print(f);
            cout << "════════════════════════\n ";

            // Reset char counts
            char_count = word.size()+1;
            line_char_count = word.size()+1;
        }

        if(line_char_count < 23) {
            cout << word << " ";
        }
        else {
            cout << "\n " << word << " ";
            line_char_count = word.size()+1;
            char_count += word.size()+1;
        }
    }

    if (char_count < 25) cout << "\n";
    cout << "\n════════════════════════\n\n";
}

//-----------------------------------------------------------------------------

// Inherited Enemy behavior (member functions) for fire enemy, etc.

//-----------------------------------------------------------------------------

int main() try {
    system("chcp 65001");
    clear_screen();
    
    // generate random seed
    srand(time(nullptr));

    World_map world;
    
    Player player {Position{2,2},30,5};
    Enemy enemy {Position{0,0},99,3};
    Item fire_crystal {Position{1,0}};
    Block block {Position{1,1}};

    world.place(player);
    world.place(enemy);
    world.place(fire_crystal);
    world.place(block);

    Frame_manager fm;

    while(true) {
        /* Title */
        
        fm.print(Frame::title);
        Selection input = Selection::invalid;

        while(input != Selection::start) {
            input = title_input("Welcome to Aegis");

            switch(input) {
                case Selection::start:   
                    break;
                case Selection::help:    
                    show_help_world(fm); 
                    break;
                case Selection::invalid: 
                    break;
                default: throw runtime_error("Selection doesn't exist!");
            }

            clear_screen();
            fm.print(Frame::title);
        }


        /* World */

        world.refresh();

        while(!enemy.in_vicinity(player,world)) {
            Key input = Key::invalid;

            while(input == Key::invalid) {
                world.refresh();
                input = world_input("Use [w,a,s,d] to move [h: help]");

                switch(input) {
                    case Key::up: case Key::left: case Key::down: 
                    case Key::right:
                        player.move(input,world);
                        world.refresh();
                        break;
                    case Key::help:
                        show_help_world(fm);
                        break;
                    //case Key::quit:
                    case Key::invalid:
                        break;
                    default:
                        throw runtime_error("Key doesn't exist!");
                }
            }

            // item is still true here if picked up item

            enemy.move(player,world);
            world.refresh();
        }

        prompt_next("Enemy engaged!",world);
        

        /* Battle */

        /* Player Turn */

        Battle battle {player,enemy,fm};
        battle.refresh();

        // Used for Spell Menu
        bool spell_display = false;
        const string spell_prompt = "What Spell? [z: back]";
        const string battle_prompt = "What will you do? [h: help]";

        while(enemy.hp() > 0 && player.hp() > 0) {
            player.reset_parry();
            Option choice = Option::invalid;

            while(!is_battle_option(choice)) {
                string msg = (spell_display) ? spell_prompt : battle_prompt;
                choice = battle_input(msg);

                switch(choice) {
                    case Option::attack:
                        player.attack(enemy,battle);
                        break;
                    case Option::parry:
                        player.parry();
                        break;
                    case Option::heal:
                        if(player.mp() < 2) { 
                            choice = Option::invalid;
                            prompt_next("Heal requires 2 MP!",battle);
                        }
                        if(choice != Option::invalid) {
                            player.heal(battle);
                        }
                        break;
                    case Option::fire:
                        if(player.mp() < 1) { 
                            choice = Option::invalid;
                            prompt_next("Fire requires 1 MP!",battle);
                        }
                        if(choice != Option::invalid) {
                            player.fire(enemy,battle);
                        }
                        break;
                    case Option::ice:
                        if(player.mp() < 2) { 
                            choice = Option::invalid;
                            prompt_next("Ice requires 2 MP!",battle);
                        }
                        if(choice != Option::invalid) {
                            player.ice(enemy,battle);
                        }
                        break;
                    /*
                    case Option::aegis:
                        // cast aegis
                        break;
                    */
                    case Option::help:
                        show_help_battle(fm);
                        battle.refresh();
                        break;
                    case Option::spell:
                        battle.print(Battle_frame::spell_options);
                        spell_display = true;
                        continue;
                    case Option::invalid:
                        battle.refresh_last();
                        break;
                    case Option::back:
                        spell_display = false;
                        battle.refresh();
                        break;
                    default:
                        throw runtime_error("Option doesn't exist!");
                        break;
                }

                if (is_battle_option(choice)) spell_display = false;
            }

            if (enemy.hp() == 0) break;

            /* Enemy Turn */

            Enemy_option enemy_choice = enemy.input(player,battle);

            switch(enemy_choice) {
                case Enemy_option::attack:
                    enemy.attack(player,battle);
                    break;
                /*
                case Enemy_option::fire:
                    enemy.fire(player,battle);
                    break;
                case Enemy_option::ice:
                    enemy.ice(player,battle);
                    break;
                case Enemy_option::attack_up:
                    enemy.attack_up(battle);
                    break;
                case Enemy_option::frozen:
                    break;
                */
                default:
                    throw runtime_error("Enemy_option doesn't exist!");
                    break;
            }

            if (player.hp() == 0) break;

            // Not essential to class Battle, can be regular function
            battle.apply_status(player);
            battle.apply_status(enemy);

            battle.refresh();
        }

        clear_screen();
        
        if (player.hp() == 0) prompt_next("You lost!");
        if (enemy.hp() == 0) prompt_next("You won!");

        // Answer ans = end_input();

        cout << "\nReturn to title screen? (y/n)\n"
             << "> ";
        char answer = ' ';
        cin >> answer;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        if (answer != 'y') break;

        clear_screen();
    }
    return 0;
}
catch(exception& e) {
    cerr << "Error: " << e.what() << "\n\n";
    prompt_next("Next");
    return 1;
}
catch(...) {
    cerr << "Unknown Exception!\n\n";
    prompt_next("Next");
    return 2;
}

//-----------------------------------------------------------------------------

/*

I'll do the following: 

1) refine core of battle system (trial & error)
2) consult design document for more battle system additions



//
1) Fix bool Enemy::in_vicinity(); use helper functions left, right, up, down

2) candidates for enum classes 

enemy lvl
cells (empty, null)
sprites (player, enemy, block, and item sprites)

3) Player data members hp and mp, health and mana respectively, and member 
function getters are hp() and mp() respectively

4) Rework Enemy::attack()




//
Implement Entity class hierarchy 

Good idea for the following reasons:
- Get to have only one place() and one remove() for class World_map, 
simplifying the code to make it more readable
- Fixes the weird cyclical nature of both class Player & Enemy attack() 
passing an instance of the other
- Reduces the size of class Player & Enemy with a common base class
- Practice & Demonstrate OOP Design
//


//
Organize and roll back to have constructors from classes : Entity to be at the 
top, after their class definitions. 

- Adjust Position to disallow the creation of an invalid Position to get rid 
of the outside_range() function in the constructor
- Get rid of the place() in the constructors
- Have place() disallow overlapping placing of entities

This will inevitably make it so that place() is no longer inline with it's 
extra lines for validation before placing entities, but the efficiency of 
inlining is negligible for this kind of program in hindsight. Even if you do 
go significantly "slower" by not inlining place(), you're supposed to go slow 
when moving anyway. You're not supposed to be moving fast paced. On top of 
that, efficiency takes the back seat to simplicity, readability, and 
maintainability. Which is what making these changes would do for the program.
//


//
1) Maybe have last args of Enemy be int level
2) Maybe reduce size of void Enemy::move(); 
3) Maybe for Enemy AI, move around block horizontally if can't chase vertically
4) Maybe operator overloading with << for animations and dialogue
//

*/