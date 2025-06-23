#include <World_map.h>
#include <utils.h>

void World_map::print() const {
    cout << "╔═══════════════════╗\n"
         << "║                   ║\n";

    for(int row = 0; row < ROWS; row++) {
        cout << "║   ";

        for(int col = 0; col < COLS; col++) {
            cout << grid[row][col] << "   ";
        }

        cout << "║\n";
        cout << "║                   ║\n";
    }
    
    cout << "╚═══════════════════╝\n\n";
}

void World_map::refresh() const {
    clear_screen();
    print();
}

string World_map::look(int row, int col) const {
    if (is_out_of_range(row,col)) return null_cell;
    return grid[row][col];
}

void World_map::place(const Entity& e) {
    if(grid[e.row()][e.col()] != empty_cell) {
        throw runtime_error("Entity Overlap");
    }
    grid[e.row()][e.col()] = e.sprite();
}
