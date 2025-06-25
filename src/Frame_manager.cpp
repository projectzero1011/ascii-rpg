#include <Frame_manager.h>
#include <utils.h>
#include <prompt.h>
#include <fstream>

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

void play_cutscene(const Frame_manager& fm, Frame f, string in_file) {
    ifstream ist {in_file};
    if (!ist) throw runtime_error("Can't open file!");

    // Top border
    clear_screen();
    fm.print(f);
    cout << "════════════════════════\n ";

    int total_char_count = 0;
    int line_char_count = 0;

    for(string word; ist >> word;) {
        total_char_count += word.size()+1;
        line_char_count += word.size()+1;

        if(total_char_count > 46) {
            // Bottom border
            cout << "\n════════════════════════\n\n";
            prompt_next("Next");

            // Top border
            clear_screen();
            fm.print(f);
            cout << "════════════════════════\n ";

            // Reset char counts
            total_char_count = word.size()+1;
            line_char_count = word.size()+1;
        }

        if(line_char_count < 23) {
            cout << word << " ";
        }
        else {
            cout << "\n " << word << " ";
            line_char_count = word.size()+1;
            total_char_count += word.size()+1;
        }
    }

    if (total_char_count < 25) cout << "\n";
    cout << "\n════════════════════════\n\n";
}