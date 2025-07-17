#include <Position.h>
#include <constants.h>

bool is_out_of_range(int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return true;
    return false;
}

Position::Position(int row, int col) 
    : r{row}, c{col}
{
    if (is_out_of_range(row,col)) throw runtime_error("Invalid Position");
}

bool Position::operator==(const Position& pos) const {
    return (r == pos.row()) && (c == pos.col());
}