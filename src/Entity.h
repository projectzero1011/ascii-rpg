#pragma once

#include <iostream>
#include <Position.h>
using namespace std;

class Entity {
public:
    Entity(Position p) : pos{p} { }

    Position position() const { return pos; }
    int row() const { return pos.row(); }
    int col() const { return pos.col(); }

    virtual string sprite() const = 0;
protected:
    Position pos;
};