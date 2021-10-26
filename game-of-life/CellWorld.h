#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Cell.h"
#include "Global.h"

/*
@brief Governs a group of cell's and their states based on game-of-life rules
*/
class CellWorld : public sf::Drawable {
private:
    std::vector<Cell> cells;
    size_t width;
    size_t height;
    size_t offsetX;
    size_t offsetY;
    //static constexpr float CELL_MARGIN = 0.5f;
    float gridWidth = (WINDOW_WIDTH - WINDOW_MARGIN) / (width + (width * CELL_MARGIN));
    float gridHeight = (WINDOW_HEIGHT - WINDOW_MARGIN) / (height + (height * CELL_MARGIN));
    sf::VertexArray vertices;
    sf::RectangleShape cursor;
public:
    CellWorld(const size_t width, const size_t height);
    CellWorld(const size_t width, const size_t height, const size_t x, const size_t y);

    CellState getCellState(const int x, const int y);

    sf::Vector2u getSize() const;

    sf::Vector2f getCellSize() const;

    Cell* getCell(const int x, const int y);

    void updateCell(const int x, const int y);

    void countNeighbours(const size_t x, const size_t y);

    void setCellState(const size_t x, const size_t y, const CellState state);

    void processNextState();

    void setCellColor(const int x, const int y, const sf::Color color);

    void load();

    void setWorldSize(const size_t width, const size_t height);

    void setCursorPosition(const size_t x, const size_t y);

    //void handleEvent(const sf::Event event);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


};