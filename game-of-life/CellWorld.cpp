#pragma once
#include "CellWorld.h"

/*
@brief Initiates CellWorld to given size according to given parameters
@param width Width of the active play area
@param height Height of the active play area
*/
CellWorld::CellWorld(const size_t width, const size_t height)
    : width(width), height(height) {
    cells = std::vector<Cell>(width * height);
    std::fill(cells.begin(), cells.end(), Cell(CellState::Dead));
    cursor = sf::RectangleShape({ gridWidth, gridHeight });
    cursor.setFillColor(sf::Color::Red);
    load();
}

/*
@brief Initiates CellWorld to given size and position according to given parameters
@param width Width of the active play area
@param height Height of the active play area
@param x World's x-coordinate
@param y World's y-coordinate
*/
CellWorld::CellWorld(const size_t width, const size_t height, const size_t x, const size_t y)
    : width(width), height(height), offsetX(x), offsetY(y) {
    cells = std::vector<Cell>(width * height);
    cursor = sf::RectangleShape({ gridWidth, gridHeight });
    cursor.setFillColor(sf::Color::Red);
    load();
}

/*
@brief Gives the state of cell inhabiting given coordinates
@return Cell's state, or CellState::Dead if not found
*/
CellState CellWorld::getCellState(const int x, const int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return CellState::Dead;
    }
    return cells[x + y * width].getState();
}

/*
@brief Gives size of the CellWorld in width and height
@return CellWorld's size in width and heigth
*/
sf::Vector2u CellWorld::getSize() const {
    return { (unsigned)width, (unsigned)height };
}

/*
@brief Gives size of the cells in width and height
@return Size of the cells in width and height
*/
sf::Vector2f CellWorld::getCellSize() const {
    return { gridWidth, gridHeight };
}

/*
@brief Gives a pointer to cell inhabiting given coordinates
@param x Cell's x-coordinate
@param y Cell's y-coordinate
@return Cell if found in given position, null if not found
*/
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

/*
@brief Calculates cell in to it's neighbouring cells' counting
*/
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

/*
@brief Sets given cell's state, if cell is found
*/
void CellWorld::setCellState(const size_t x, const size_t y, const CellState state) {
    Cell *cell = getCell(x, y);
    if (!cell) return;
    cell->setState(state);
    const sf::Color cellColor = state == CellState::Alive ? sf::Color::White : sf::Color::Black;
    setCellColor(x, y, cellColor);
}

/*
@brief Calculates CellWorld's next state and updates singular cells
*/
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

/*
@brief Loads whole World and updates cells' coloring accordingly
*/
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

/*
@brief Set's World's size
@param width New width
@param height New height
*/
void CellWorld::setWorldSize(const size_t width, const size_t height) {
    if (width <= 0 || height <= 0) return;
    while (width > this->width) {
        for (size_t y = 0; y < this->height; ++y) {
            cells.insert(cells.begin() + (width - 1) + y * (width), Cell(CellState::Dead));
        }
        ++this->width;
    }
    while (width < this->width) {
        for (size_t y = 0; y < this->height; ++y) {
            cells.erase(cells.begin() + width + y * width);
        }
        --this->width;
    }
    cells.resize(width * height, Cell(CellState::Dead));
    this->width = width;
    this->height = height;
    gridWidth = (WINDOW_WIDTH - WINDOW_MARGIN) / (width + (width * CELL_MARGIN));
    gridHeight = (WINDOW_HEIGHT - WINDOW_MARGIN) / (height + (height * CELL_MARGIN));
    cursor.setSize({ gridWidth, gridHeight });
    setCursorPosition(0,0);
    load();
}

/*
@brief Sets cursor position
@param x Cursor's x-coordinate
@param y Cursor's y-coordinate
*/
void CellWorld::setCursorPosition(const size_t x, const size_t y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    const auto xPos = (x + CELL_MARGIN * x) * gridWidth + offsetX;
    const auto yPos = (y + CELL_MARGIN * y) * gridHeight + offsetY;
    cursor.setPosition({ xPos, yPos });
}

/*void CellWorld::handleEvent(const sf::Event event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        worldWidth = --worldWidth >= MIN_WORLD_SIZE ? worldWidth : MIN_WORLD_SIZE;
        cw.setWorldSize(worldWidth, worldHeight);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        worldWidth = ++worldWidth <= MAX_WORLD_SIZE ? worldWidth : MAX_WORLD_SIZE;
        cw.setWorldSize(worldWidth, worldHeight);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        worldHeight = ++worldHeight <= MAX_WORLD_SIZE ? worldHeight : MAX_WORLD_SIZE;
        cw.setWorldSize(worldWidth, worldHeight);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        worldHeight = --worldHeight >= MIN_WORLD_SIZE ? worldHeight : MIN_WORLD_SIZE;
        cw.setWorldSize(worldWidth, worldHeight);
    }
}*/

/*
@brief Set's cell's color if found in given coordinates
@param x Targeting cell's x-coordinate
@param y Targeting cell's y-coordinate
@param color Color to be set for the cell
*/
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

/*
@brief Draws the CellWorld and the cursor
@param target Rendering target
@param states Rendering states
*/
void CellWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform.translate({ (float)offsetX, (float)offsetY });
    target.draw(vertices, states);
    target.draw(cursor, states);
}
