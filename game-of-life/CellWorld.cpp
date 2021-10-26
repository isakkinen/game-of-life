#pragma once
#include "CellWorld.h"

CellWorld::CellWorld(const size_t width, const size_t height)
    : width(width), height(height) {
    cells = std::vector<Cell>(width * height);
    std::fill(cells.begin(), cells.end(), Cell(CellState::Dead));
    cursor = sf::RectangleShape({ gridWidth, gridHeight });
    cursor.setFillColor(sf::Color::Red);
    std::cout << "Cells width: " << cells.size() / height << ", Cells height: " << cells.size() / width << std::endl;
    load();
}

CellWorld::CellWorld(const size_t width, const size_t height, const size_t x, const size_t y)
    : width(width), height(height), offsetX(x), offsetY(y) {
    cells = std::vector<Cell>(width * height);
    cursor = sf::RectangleShape({ gridWidth, gridHeight });
    cursor.setFillColor(sf::Color::Red);
    std::cout << "Cells width: " << cells.size() / height << ", Cells height: " << cells.size() / width << std::endl;
    load();
}

CellState CellWorld::getCellState(const int x, const int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return CellState::Dead;
    }
    return cells[x + y * width].getState();
}

sf::Vector2u CellWorld::getSize() const {
    return { (unsigned)width, (unsigned)height };
}

sf::Vector2f CellWorld::getCellSize() const {
    return { gridWidth, gridHeight };
}

Cell* CellWorld::getCell(const int x, const int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return nullptr;
    }
    return &cells[x + y * width];
}

void CellWorld::updateCell(const int x, const int y) {
    Cell* cell = getCell(x, y);
    if (!cell) return;
    cell->update();

    const sf::Color cellColor = cell->getState() == CellState::Alive ? sf::Color::White : sf::Color::Black;
    setCellColor(x, y, cellColor);
}

void CellWorld::countNeighbours(const size_t x, const size_t y) {
    Cell* cell = getCell(x, y);
    if (!cell || (cell->getState() != CellState::Alive)) return;
    for (int x_offset = -1; x_offset <= 1; ++x_offset) {
        for (int y_offset = -1; y_offset <= 1; ++y_offset) {
            if (x_offset == 0 && y_offset == 0) continue; // Ignore the center cell

            Cell* target = getCell(x + x_offset, y + y_offset);
            if (!target) continue;
            target->addNeighbour(); // Add this cell as a neighbour;
        }
    }
}

void CellWorld::setCellState(const size_t x, const size_t y, const CellState state) {
    Cell *cell = getCell(x, y);
    if (!cell) return;
    cell->setState(state);
    const sf::Color cellColor = state == CellState::Alive ? sf::Color::White : sf::Color::Black;
    setCellColor(x, y, cellColor);
}

void CellWorld::processNextState() {
    for (size_t x = 0; x <= width + 2; ++x) {
        for (size_t y = 0; y <= height + 2; ++y) {
            countNeighbours(x, y);

            // Update old cells to their new states
            const int oldX = int(x) - 2;
            const int oldY = int(y) - 2;
            updateCell(oldX, oldY);

            if (x == width - 1 && y == height - 1) {
                updateCell(int(x) - 1, int(y) - 1);
                updateCell(int(x), int(y));
            }
        }
    }
}

void CellWorld::load() {
    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(width * height * 4);

    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            const sf::Color cellColor = getCell(x, y)->getState() == CellState::Alive ? sf::Color::White : sf::Color::Black;
            setCellColor(x, y, cellColor);
        }
    }
}

void CellWorld::setWorldSize(const size_t width, const size_t height) {
    if (width <= 0 || height <= 0) return;
    if (width > this->width) {
        for (size_t y = 0; y < this->height; ++y) {
            cells.insert(cells.begin() + (width - 1) + y * (width), Cell(CellState::Dead));
        }
    }
    else if (width < this->width) {
        for (size_t y = 0; y < this->height; ++y) {
            cells.erase(cells.begin() + width + y * width);
        }
    }
    cells.resize(width * height, Cell(CellState::Dead));
    this->width = width;
    this->height = height;
    //cells.resize(width * height, Cell());
    gridWidth = (WINDOW_WIDTH - WINDOW_MARGIN) / (width + (width * CELL_MARGIN));
    gridHeight = (WINDOW_HEIGHT - WINDOW_MARGIN) / (height + (height * CELL_MARGIN));
    cursor.setSize({ gridWidth, gridHeight });
    setCursorPosition(0,0);
    load();
}

void CellWorld::setCursorPosition(const size_t x, const size_t y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    const auto xPos = (x + CELL_MARGIN * x) * gridWidth + offsetX;
    const auto yPos = (y + CELL_MARGIN * y) * gridHeight + offsetY;
    cursor.setPosition({ xPos, yPos });
}

void CellWorld::setCellColor(const int x, const int y, const sf::Color color) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    sf::Vertex* quad = &vertices[(x + y * width) * 4];

    const auto xPos = x + CELL_MARGIN * x;
    const auto yPos = y + CELL_MARGIN * y;

    quad[0].position = { xPos * gridWidth + offsetX , yPos * gridHeight + offsetY };
    quad[1].position = { (xPos + 1) * gridWidth + offsetX, yPos * gridHeight + offsetY };
    quad[2].position = { (xPos + 1) * gridWidth + offsetX, (yPos + 1) * gridHeight + offsetY };
    quad[3].position = { xPos * gridWidth + offsetX, (yPos + 1) * gridHeight + offsetY };
    quad[0].color = color;
    quad[1].color = color;
    quad[2].color = color;
    quad[3].color = color;
}

void CellWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform.translate({ (float)offsetX, (float)offsetY });
    target.draw(vertices, states);
    target.draw(cursor, states);
}