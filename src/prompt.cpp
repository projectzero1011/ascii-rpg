#include <prompt.h>
#include <limits>
#include <World_map.h>
#include <Battle.h>
#include <utils.h>

void prompt_next(string message) {
    cout << message + " [z: next]" << "\n"
         << "> ";
    string s;
    cin >> s;
}

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