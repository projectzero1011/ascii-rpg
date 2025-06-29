#pragma once

#include <frames.h>

class Frame_manager {
public:
    Frame_manager();
    void print(Frame f) const { cout << frames[int(f)]; }
    void print(Battle_frame bf) const { cout << battle_frames[int(bf)]; }
    // void play_cutscene(Frame f, Cutscene c);
private:
    vector<string> frames;
    vector<string> battle_frames;
    // vector<vector<string>> script;
};

string load(string& name);
void show_help_world(const Frame_manager& fm);
void show_help_battle(const Frame_manager& fm);
void play_cutscene(const Frame_manager& fm, Frame f, string in_file);

// vector<vector<string>> will contain dialogue for each cutscene
// outer vector contains entire script of the game
// inner vector contains script of particular cutscenes
// have separate files for each cutscene script
// read into vector script line by line
// enum class Cutscene, with vector<string> cutscene_tbl