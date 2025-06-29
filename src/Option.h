#pragma once

enum class Option {
    attack = 1,
    parry = 2,
    spell = 3,
    fire = 7,
    ice = 8,
    heal = 9,
    help,
    back,
    invalid
};

bool is_battle_option(Option op);