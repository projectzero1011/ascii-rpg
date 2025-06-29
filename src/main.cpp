#include <iostream>
#include <limits>
#include <utils.h>  // Misc.
#include <input.h>  // 
#include <prompt.h> // 
#include <Item.h>
#include <Block.h>
#include <Player.h>
#include <Enemy.h>
#include <World_map.h>
#include <Frame_manager.h>
#include <Battle.h>
using namespace std;

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


// Old comments from main.cpp -> .h and .cpp split

// Selection.h - header to stay consistent with Key.h
// Key.h header so that Player.h and input.h can access enum class Key


// add get weakness, set weakness, and private member weakness
// class Enemy : public Actor; // Enemy.h


// Implement messages that say "Player dealt [x] DMG!"
// Make this an Actor method so that enemies can cast fire too
// void Player::fire(Enemy& enemy, Battle& battle) // Player.cpp


// Implement messages that say "Player dealt [x] DMG!"
// void Player::ice(Enemy& enemy, Battle& battle) // Player.cpp


// Needs work (particularly in simplifying the conditionals)
// Could use if (player_to_left) or (player_to_right), etc
// void Enemy::move(const Player& player, World_map& world) // Enemy.cpp


// WORK HERE 6/27/25 8:41 PM
// vector<Enemy_option> enemy_option_tbl = { Enemy_option::attack };
// Enemy.h


// WORK HERE 6/27/25 8:40 PM
// if status is Power_slash, then choice is attack
// keep rolling as long as it's invalid choice (not enough mp, etc.)
// is_valid(choice)
// Enemy_option Enemy::input(Player& player, Battle& battle); // Enemy.cpp


// getting enemy vicinity cells can be helper functions
// needs to be reworked

// bool Enemy::in_vicinity(const Player& p, const World_map& world)
// Enemy.cpp


// Could use more work
// void Enemy::attack(Player& player, Battle& battle) // Enemy.cpp


// Inherited Enemy behavior (member functions) for fire enemy, etc.

*/