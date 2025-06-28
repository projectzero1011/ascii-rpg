#include <Battle.h>
#include <iomanip>
#include <prompt.h>

void Battle::print(Battle_frame bf) {
    last = bf;
    clear_screen();
    cout << "Player" << "\t\t" << setw(8) << "Enemy" << "\n"
         << "HP " << setw(2) << p.hp() << "/" << p.max_hp() << "\t" 
         << "HP " << setw(2) << e.hp() << "/" << e.max_hp() << "\n" 
         << "MP  " << p.mp() << "/ " << p.max_mp() << "\t" << "\n\n";
    fm.print(bf);
}

// Work here 6/27 8:36pm
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