#include <iostream>
#include <utils.h>  // Misc.
#include <input.h>  // 
#include <prompt.h> // 
#include <Player.h>
#include <Enemy.h>
#include <Block.h>
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
    
    Player player {Position{2,2},40,5};
    Enemy enemy {Position{0,0},99,3};
    Block block {Position{1,1}};

    World_map world {player,{enemy}};

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

        // Make this into main game loop
        // If merged into loop -> don't need this vicinity while loop
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

            enemy.move(player,world);
            world.refresh();

            vector<reference_wrapper<Enemy>> enemies;
            enemies = world.adj_enemies(player);
            
            for(Enemy& enemy : enemies) {
                // Battle battle {player,enemy,fm};
                // battle.engage();
                // if (player.hp() == 0) break;
                // world.erase(enemy);
            }

            // if (player.hp() == 0) break;

            // cutscence
            // boss battle
        }

        prompt_next("Enemy engaged!",world);
        

        /* Battle */

        Battle battle {player,enemy,fm};
        battle.refresh();

        while(true) {
            player.reset_parry();

            battle.player_turn();
            if (enemy.hp() == 0) break;

            battle.enemy_turn();
            if (player.hp() == 0) break;

            battle.apply_status();

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