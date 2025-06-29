#pragma once

#include <Key.h>
#include <Selection.h>
#include <Option.h>
#include <iostream>
using namespace std;

// Simple and only used here, so no .h file
enum class Answer {
    yes,
    no,
    none
};

Selection title_input(string message);
Key world_input(string message);
Option battle_input(string message);
Answer end_input(string message);