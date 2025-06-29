#include <utils.h>
#include <iostream>
#include <limits>
using namespace std;

void clear_screen() {
    system("cls");
}

// Get char with input validation
char get_char(string message) {
    cout << message << "\n"
         << "> ";
    string input;
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    
    char ch = (input.size() == 1) ? input[0] : ' ';
    return ch;
}