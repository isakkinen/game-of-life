#pragma once
#include "CellState.h"

class Cell {
private:
    CellState state{ CellState::Alive };
    size_t neighbours{ 0 };
public:
    Cell();
    Cell(const CellState state);
    CellState getState() const;
    void setState(const CellState newState);
    void addNeighbour();
    void update();
};