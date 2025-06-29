#pragma once

#include <iostream>
#include <Frame_manager.h>
using namespace std;

class World_map;
class Battle;

void prompt_next(string message);
void prompt_next(string message, const World_map& world);
void prompt_next(string message, Battle& battle);
void prompt_next(string message, Frame f, const Frame_manager& fm);