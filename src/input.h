#pragma once

#include <Selection.h>
#include <Key.h>
#include <Option.h>
#include <Spell.h>
#include <iostream>
using namespace std;

// Only input.cpp uses it, so no .h file
enum class Answer {
    yes,
    no,
    none
};

Selection title_input(string msg);
Key world_input(string msg);
Option battle_input(string msg);
Spell spell_input(string msg);
Answer end_input(string msg);