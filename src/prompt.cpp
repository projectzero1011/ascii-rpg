#include <prompt.h>
#include <limits>
#include <World_map.h>
#include <Battle.h>
#include <utils.h>

void prompt_next(string message) {
    char ch = get_char(message + " [z: next]");
}

void prompt_next(string message, const World_map& world) {
    char ch = ' ';
    while(ch != 'z') {
        ch = get_char(message + " [z: next]");
        world.refresh();
    }
}

void prompt_next(string message, Battle& battle) {
    battle.refresh_last();
    char ch = ' ';
    while(ch != 'z') {
        ch = get_char(message + " [z: next]");
        battle.refresh_last();
    }
}

void prompt_next(string message, Frame f, const Frame_manager& fm) {
    char ch = ' ';
    while(ch != 'z') {
        ch = get_char(message + " [z: next]");
        clear_screen();
        fm.print(f);
    }
}