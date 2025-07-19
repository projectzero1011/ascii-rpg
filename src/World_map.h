#pragma once

#include <constants.h>
#include <Entity.h>
#include <vector>
#include <Player.h>
#include <Enemy.h>

class World_map {
public:
    World_map(const Player& p, const vector<Enemy>& enemy_v);
    void print() const;
    void refresh() const;
    string look(int row, int col) const;
    void place(const Entity&);
    void remove(Entity& e) { grid[e.row()][e.col()] = empty_cell; }
    void erase(Entity&);
    vector<reference_wrapper<Enemy>> adj_enem(const Player&);
private:
    string grid[ROWS][COLS]
    {
        {".", ".", ".", "."},
        {".", ".", ".", "."},
        {".", ".", ".", "."},
        {".", ".", ".", "."}
    };
    vector<Enemy> enemies;
};

vector<Position> adjacent(const Player& player);