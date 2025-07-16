#pragma once

#include <constants.h>
#include <Entity.h>
#include <vector>
#include <Player.h>

class World_map {
public:
    World_map(initializer_list<reference_wrapper<Entity>> list);
    void print() const;
    void refresh() const;
    string look(int row, int col) const;
    void place(const Entity&);
    void remove(Entity& e) { grid[e.row()][e.col()] = empty_cell; }
    void reset(Position pos) { grid[pos.row()][pos.col()] = empty_cell; }
    void erase(Entity&);
    vector<reference_wrapper<Entity>> adj_enemies(const Player&);
private:
    string grid[ROWS][COLS]
    {
        {".", ".", ".", "."},
        {".", ".", ".", "."},
        {".", ".", ".", "."},
        {".", ".", ".", "."}
    };
    vector<reference_wrapper<Entity>> entities;
};

// reset() is only used for checking the item in World_map