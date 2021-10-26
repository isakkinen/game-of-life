#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "CellWorld.h"
#include "Global.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH + WINDOW_MARGIN, WINDOW_HEIGHT + WINDOW_MARGIN), "Game of Life - Iisakki Säkkinen");
    sf::RectangleShape background = sf::RectangleShape();
    sf::Text info = sf::Text();
    info.setCharacterSize(18);
    sf::Font font = sf::Font();
    font.loadFromFile("arial.ttf");
    info.setFont(font);
    info.setString("[Up/Down/Left/Right control the World size]\t\t[A/D control speed]\n[Space to update during pause]\t\t\t\t\t\t\t[Esc to pause/play]");
    info.setPosition({ 8.f, 8.f });
    info.setFillColor(sf::Color::White);
    background.setSize({ WINDOW_WIDTH + WINDOW_MARGIN, WINDOW_HEIGHT + WINDOW_MARGIN });
    background.setFillColor(BACKGROUND_COLOR);

    size_t worldWidth = 3;
    size_t worldHeight = 3;
    float framerate = 12.f;

    CellWorld cw(worldWidth, worldHeight, WINDOW_MARGIN / 2.f, WINDOW_MARGIN / 2.f);

    bool pressed = false;
    bool paused = true;
    bool mouseLeft = false;
    bool mouseRight = false;

    auto tp = std::chrono::steady_clock::now();
    float dt = 0.0f;

    float updateTick = 0.0f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && framerate > MIN_FRAMERATE) {
                --framerate;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && framerate < MAX_FRAMERATE) {
                ++framerate;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mouseLeft = true;
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    mouseRight = true;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mouseLeft = false;
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    mouseRight = false;
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                const auto mousePos = sf::Mouse::getPosition(window);
                const auto x = std::floorf((float)(mousePos.x - (WINDOW_MARGIN + CELL_MARGIN) / 2.0f) / WINDOW_WIDTH * (cw.getSize().x));
                const auto y = std::floorf((float)(mousePos.y - (WINDOW_MARGIN + CELL_MARGIN) / 2.0f) / WINDOW_HEIGHT * (cw.getSize().y));

                cw.setCursorPosition(x, y);

                if (mouseLeft || mouseRight) {
                    const auto cellSize = cw.getCellSize();
                    cw.setCellState(x, y, mouseLeft ? CellState::Alive : mouseRight ? CellState::Dead : CellState::Dead);
                }
            }

        }

        if ( !pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            paused = !paused;
            pressed = true;
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            pressed = false;
        }

        if (!paused || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            updateTick += dt * framerate;
        }


        if (updateTick >= 1) {
            cw.processNextState();
            updateTick = 0;
        }
        window.clear();

        // Draw here
        window.draw(background);
        window.draw(cw);
        window.draw(info);

        window.display();
        const auto tp2 = std::chrono::steady_clock::now();
        dt = std::chrono::duration<float>(tp2 - tp).count();
        tp = tp2;
    }

    return 0;
}