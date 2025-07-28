#pragma once

#include <iostream>
#include <vector>
using namespace std;

enum class Frame {
    title,
    help_world,
    help_battle
};

const vector<string> frame_tbl = { 
    "title", 
    "help_world", 
    "help_battle" 
};

enum class Battle_frame {
    battle,
    player_attack,
    player_crit,
    player_parry,
    spell_options,
    player_fire,
    player_ice,
    enemy_attack,
    enemy_burn,
    enemy_freeze,
    enemy_fire_shield,
    enemy_ice_shield
};

const vector<string> battle_frame_tbl = { 
    "battle", 
    "player_attack", 
    "player_crit", 
    "player_parry", 
    "spell_options", 
    "player_fire", 
    "player_ice", 
    "enemy_attack", 
    "enemy_burn",
    "enemy_freeze",
    "enemy_fire_shield",
    "enemy_ice_shield"
};