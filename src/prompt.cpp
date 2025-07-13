#include <prompt.h>
#include <limits>
#include <World_map.h>
#include <Battle.h>
#include <utils.h>

void prompt_next(string msg) {
    char ch = get_char(msg + " [z: next]");
}

void prompt_next(string msg, const World_map& world) {
    char ch = ' ';
    while(ch != 'z') {
        ch = get_char(msg + " [z: next]");
        world.refresh();
    }
}

void prompt_next(string msg, Battle& battle) {
    battle.refresh_last();
    char ch = ' ';
    while(ch != 'z') {
        ch = get_char(msg + " [z: next]");
        battle.refresh_last();
    }
}

// Only used for show_help_world() and show_help_battle()
void prompt_next(string msg, Frame f, const Frame_manager& fm) {
    char ch = ' ';
    while(ch != 'x') {
        ch = get_char(msg + " [x: back]");
        clear_screen();
        fm.print(f);
    }
}