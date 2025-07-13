#include <iostream>
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
    
    Player player {Position{2,2},40,5};
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
                default: 
                    throw runtime_error("Selection doesn't exist!");
                    break;
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
                    case Key::up: case Key::left: 
                    case Key::down: case Key::right:
                        player.move(input,world);
                        world.refresh();
                        break;
                    case Key::help:
                        show_help_world(fm);
                        break;
                    // case Key::quit:
                    case Key::invalid:
                        break;
                    default:
                        throw runtime_error("Key doesn't exist!");
                }
            }

            // item is still true here if picked up item

            // *** Make struct Adjacent /w 4 Position objects adj to Player
            // Have to do this to prevent making null Enemy or null Position
            // Also to prevent throw runtime_error for nothing adj
            // Adjacent adj = player.adjacent();
            // bool is_enemy = world.is_enemy(adj);
            /*
            if(is_enemy) {
                Enemy encounter = world.get_enemy(adj);
                battle(player,encounter);
            }
            */

            enemy.move(player,world);
            world.refresh();
        }

        prompt_next("Enemy engaged!",world);
        

        /* Battle */

        Battle battle {player,enemy,fm};
        battle.refresh();

        while(enemy.hp() > 0 && player.hp() > 0) {
            player.reset_parry();

            battle.player_turn();      
            if (enemy.hp() == 0) break;

            battle.enemy_turn();
            if (player.hp() == 0) break;

            // Not essential to class Battle, can be regular function
            battle.apply_status(player);
            battle.apply_status(enemy);

            battle.refresh();
        }

        clear_screen();
        
        if (player.hp() == 0) prompt_next("You lost!");
        if (enemy.hp() == 0) prompt_next("You won!");
        
        cout << "\n";
        Answer ans = end_input("Return to title screen? (y/n)");
        if (ans != Answer::yes) break;

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
1) Maybe have last args of Enemy be int level
2) Maybe reduce size of void Enemy::move(); 
3) Maybe for Enemy AI, move around block horizontally if can't chase vertically
4) Maybe operator overloading with << for animations and dialogue
//


// Old comments from before main.cpp split

// Selection.h - header to stay consistent with Key.h
// Key.h header so that Player.h and input.h can access enum class Key


// add get weakness, set weakness, and private member weakness
// class Enemy : public Actor; // Enemy.h


// WORK HERE 6/27/25 8:41 PM
// vector<Enemy_option> enemy_option_tbl = { Enemy_option::attack };
// Enemy.h


// WORK HERE 6/27/25 8:40 PM
// if status is Power_slash, then choice is attack
// keep rolling as long as it's invalid choice (not enough mp, etc.)
// is_valid(choice)
// Enemy_option Enemy::input(Player& player, Battle& battle); // Enemy.cpp

*/