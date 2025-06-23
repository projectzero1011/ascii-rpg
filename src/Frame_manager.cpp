#include <Frame_manager.h>
#include <utils.h>
#include <prompt.h>
#include <fstream>

string load(string& name) {
    ifstream ist {"../files/" + name + ".txt"};
    if (!ist) throw runtime_error("Can't open input file");

    string frame;
    for (char ch; ist.get(ch);) frame += ch;

    return frame;
}

void show_help_world(const Frame_manager& fm) {
    clear_screen();
    fm.print(Frame::help_world);
    prompt_next("Back",Frame::help_world,fm);
}

void show_help_battle(const Frame_manager& fm) {
    clear_screen();
    fm.print(Frame::help_battle);
    prompt_next("Back",Frame::help_battle,fm);
}

Frame_manager::Frame_manager() {
    for(string fname : frame_tbl) {
        string f = load(fname);
        frames.push_back(f);
    }

    for(string bfname : battle_frame_tbl) {
        string bf = load(bfname);
        battle_frames.push_back(bf);
    }
}