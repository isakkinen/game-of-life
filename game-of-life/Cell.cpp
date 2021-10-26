#pragma once
#include "Cell.h"

Cell::Cell() {}

Cell::Cell(const CellState state)
    :state(state) {}

/*
@brief Returns cell's current state
@return Cell's current state
*/
CellState Cell::getState() const {
    return state;
}

/*
@brief Sets cell's state
@param newState New cell's state
*/
void Cell::setState(const CellState newState) {
    state = newState;
}

/*
@brief Increments neighbour count by one
*/
void Cell::addNeighbour() {
    ++neighbours;
}

/*
@brief Set's cell state based on the neighbour count
*/
void Cell::update() {
    if (neighbours == 3) {
        state = CellState::Alive;
    }
    else if (neighbours != 2) {
        state = CellState::Dead;
    }
    neighbours = 0;
}