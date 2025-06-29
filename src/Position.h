#pragma once

class Position {
public:
    Position(int row, int col);
    int row() const { return r; }
    int col() const { return c; }
private:
    int r;
    int c;
};

bool is_out_of_range(int row, int col);