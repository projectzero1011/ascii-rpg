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

constexpr int burn = 3;

// Work here 6/27 8:36pm
void Battle::apply_status(Actor& a) {
    if(a.counter() > 0) {
        switch(a.status()) {
            case Status::burn:
                // a.burn();
                a.decr_hp(burn);
                a.set_counter(a.counter()-1);
                print(Battle_frame::enemy_burn);
                prompt_next("Burn dealt " + to_string(burn) + " DMG", *this);
                break;
            case Status::freeze:
                // a.freeze();
                a.set_counter(a.counter()-1);
                print(Battle_frame::enemy_freeze);
                prompt_next("The Enemy is Frozen!");
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