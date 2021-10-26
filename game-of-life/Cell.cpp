#pragma once
#include "Cell.h"

Cell::Cell() {}

Cell::Cell(const CellState state)
    :state(state) {}

CellState Cell::getState() const {
    return state;
}

void Cell::setState(const CellState newState) {
    state = newState;
}

void Cell::addNeighbour() {
    ++neighbours;
}

void Cell::update() {
    if (neighbours == 3) {
        state = CellState::Alive;
    }
    else if (neighbours != 2) {
        state = CellState::Dead;
    }
    neighbours = 0;
}