#pragma once

#include <constants.h>
#include <Entity.h>

class World_map {
public:
    void print() const;
    void refresh() const;
    string look(int row, int col) const;
    void place(const Entity&);
    void remove(Entity& e) { grid[e.row()][e.col()] = empty_cell; }
    void erase(Position pos) { grid[pos.row()][pos.col()] = empty_cell; }
private:
    string grid[ROWS][COLS]
    {
        {".", ".", ".", "."},
        {".", ".", ".", "."},
        {".", ".", ".", "."},
        {".", ".", ".", "."}
    };
};