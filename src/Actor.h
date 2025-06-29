#pragma once

#include <Entity.h>

enum class Status {
    burn,
    freeze,
    none
};

class Actor : public Entity {
public:
    int hp() const { return h; }
    int max_hp() const { return max_h; }
    void decr_hp(int dmg) { h -= dmg; if (h < 0) h = 0; }
    void incr_hp(int amt) { h += amt; if (h > max_h) h = max_h; }

    int mp() const { return m; }
    int max_mp() const { return max_m; }
    void decr_mp(int amt) { m -= amt; if (m < 0) m = 0; }
    void incr_mp(int amt) { m += amt; if (m > max_m) m = max_m; }

    Status status() const { return st; }
    void set_status(Status s) { st = s; }
    int counter() const { return count; }
    void set_counter(int c) { count = c; }
protected:
    Actor(Position p, int hp, int mp) 
    : Entity{p}, h{hp}, max_h{hp}, max_m{mp} { }
private:
    int h, max_h;
    int m {0}, max_m;
    Status st {Status::none};
    int count {0};
};