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
    
    // Generate random seed
    srand(time(nullptr));
    
    Player player {Position{2,2}, Stats{40,5,2}};
    Enemy enemy {Position{0,0}, Stats{99,3,2}};

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


        /* Main Game Loop */

        world.refresh();

        while(true) {
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

            world.refresh();

            vector<reference_wrapper<Enemy>> enem = world.adj_enem(player);
            
            for(Enemy& enemy : enem) {
                prompt_next("Enemy engaged!",world);
                Battle battle {player,enemy,fm};
                battle.engage(world);
                if (player.hp() == 0) break;
                world.erase(enemy);
            }

            if (player.hp() == 0) break;

            // cutscence
            // boss battle
        }

        clear_screen();
        if (player.hp() == 0) prompt_next("You lost!");
        
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
3) Maybe operator overloading with << for animations and dialogue

*/