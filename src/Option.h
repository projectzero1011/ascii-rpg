#pragma once

enum class Option {
    attack = 1,
    parry = 2,
    heal = 3,
    spell = 4,
    fire = 7,
    ice = 8,
    help,
    back,
    invalid
};

bool is_battle_option(Option op);