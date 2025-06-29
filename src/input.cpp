#include <input.h>
#include <limits>

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
        case '3': return Option::spell;
        case '7': return Option::fire;
        case '8': return Option::ice;
        case '9': return Option::heal;
        // case '0': return Option::aegis
        case 'h': return Option::help;
        case 'z': return Option::back;
        default:  return Option::invalid;
    }
}

Answer end_input(string message) {
    cout << message << "\n"
         << "> ";
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    char ch = (input.size() == 1) ? input[0] : ' ';

    switch(ch) {
        case 'y': case 'Y': return Answer::yes;
        case 'n': case 'N': return Answer::no;
        default: return Answer::none; 
    }
}