#include <World_map.h>
#include <utils.h>
#include <Player.h>
#include <Enemy.h>

World_map::World_map(const Player& p, const vector<Enemy>& enemy_v)
: enemies{enemy_v}
{
    place(p);
    for(const Enemy& enemy : enemies) {
        place(enemy);
    }
}

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

void World_map::erase(Entity& e) {
    for(int i = 0; i < enemies.size(); ++i) {
        if(&enemies[i] == &e) {
            enemies.erase(enemies.begin() + i);
            return;
        }
    }
}

vector<Position> adjacent(const Player& player) {
    vector<Position> adj;

    bool up_valid = !is_out_of_range(player.row()-1, player.col());
    bool left_valid = !is_out_of_range(player.row(), player.col()-1);
    bool down_valid = !is_out_of_range(player.row()+1, player.col());
    bool right_valid = !is_out_of_range(player.row(), player.col()+1);

    if (up_valid) adj.push_back(Position{player.row()-1, player.col()});
    if (left_valid) adj.push_back(Position{player.row(), player.col()-1});
    if (down_valid) adj.push_back(Position{player.row()+1, player.col()});
    if (right_valid) adj.push_back(Position{player.row(), player.col()+1});

    return adj;
}

// Check if Entity is an Enemy by using Entity::sprite()
vector<reference_wrapper<Enemy>> World_map::adj_enemies(const Player& player) {
    vector<reference_wrapper<Enemy>> adj_enemies;
    vector<Position> adj = adjacent(player);

    for(const Position& pos : adj) {
        for(Enemy& enemy : enemies) {
            if(pos == enemy.position()) {
                adj_enemies.push_back(enemy);
            }
        }
    }

    return adj_enemies;
}