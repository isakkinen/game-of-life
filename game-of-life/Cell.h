#pragma once
#include "CellState.h"

/*
@brief A singular cell that changes state based on number of neighbouring cells
*/
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