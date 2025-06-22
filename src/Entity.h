#pragma once

#include <iostream>
#include <Position.h>
using namespace std;

class Entity {
public:
    Entity(Position p) : pos{p} { }

    int row() const { return pos.row(); }
    int col() const { return pos.col(); }

    virtual string sprite() const = 0;
protected:
    Position pos;
};