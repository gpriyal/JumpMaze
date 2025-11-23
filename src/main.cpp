#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <map>
#include "MazeGenerator.h"
#include "PathValidator.h"
#include "GameEngine.h"
using namespace std;
using namespace sf;

int main() {
    int n;
    cout << "Enter maze size (e.g. 6): ";
    cin >> n;
    if (n <= 1) {
        cout << "Maze size must be >= 2\n";
        return 0;
    }

    MazeGenerator generator(n);
    generator.generateMaze();
    auto maze = generator.getMaze();
    auto start = generator.getStart();
    auto goal = generator.getGoal();

    if (!PathValidator::isSolvable(maze, start, goal)) {
        cout << "Maze generation failed (unsolvable)." << endl;
        return 0;
    }

    GameEngine engine(maze, start, goal);

    int cellSize = 80;
    int windowSize = n * cellSize;
    RenderWindow window(VideoMode(windowSize, windowSize + 60), "Jump Maze — SFML 2.5.1 Edition");

    Font font;
    if (!font.loadFromFile("assets/font.ttf")) {
        cout << "Font not found.\n";
        return 0;
    }

    bool showInvalidMove = false;
    Clock invalidMoveClock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                int jump = engine.getCurrentJump();
                bool success = false;

                if (event.key.code == Keyboard::Up) success = engine.attemptMove("up", jump);
                else if (event.key.code == Keyboard::Down) success = engine.attemptMove("down", jump);
                else if (event.key.code == Keyboard::Left) success = engine.attemptMove("left", jump);
                else if (event.key.code == Keyboard::Right) success = engine.attemptMove("right", jump);

                if (!success && !engine.isLost() && engine.hasAnyValidMove()) {
                    showInvalidMove = true;
                    invalidMoveClock.restart();
                }

                if (engine.isLost()) {
                    Text lostText;
                    lostText.setFont(font);
                    lostText.setString("YOU LOST!");
                    lostText.setCharacterSize(60);
                    lostText.setFillColor(Color::White);
                    lostText.setPosition(Vector2f(windowSize / 4.0f, windowSize / 3.0f));
                    window.clear(Color::Red);
                    window.draw(lostText);
                    window.display();
                    sf::sleep(sf::seconds(3));
                    window.close();
                }
            }
        }

        window.clear(Color(30, 30, 30));

        // Win condition
        if (engine.isGoalReached()) {
            Text winText;
            winText.setFont(font);
            winText.setString("YOU WIN!");
            winText.setCharacterSize(60);
            winText.setFillColor(Color::White);
            winText.setPosition(Vector2f(windowSize / 4.0f, windowSize / 3.0f));
            window.clear(Color::Green);
            window.draw(winText);
            window.display();
            sf::sleep(sf::seconds(3));
            break;
        }

        auto pos = engine.getPosition();

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                RectangleShape cell(Vector2f(cellSize - 2, cellSize - 2));
                cell.setPosition(Vector2f(j * cellSize, i * cellSize));

                if (make_pair(i, j) == goal)
                    cell.setFillColor(Color::Red);
                else if (make_pair(i, j) == pos)
                    cell.setFillColor(Color::Cyan);
                else
                    cell.setFillColor(Color(70, 70, 70));

                window.draw(cell);

                Text num;
                num.setFont(font);
                num.setCharacterSize(24);
                num.setFillColor(Color::White);
                if (maze[i][j] == -1)
                    num.setString("X");
                else
                    num.setString(to_string(maze[i][j]));
                num.setPosition(Vector2f(j * cellSize + 30.f, i * cellSize + 20.f));
                window.draw(num);
            }
        }

        if (showInvalidMove && invalidMoveClock.getElapsedTime().asSeconds() < 1.0f) {
            Text msg;
            msg.setFont(font);
            msg.setString("Invalid Move!");
            msg.setCharacterSize(40);
            msg.setFillColor(Color::Yellow);
            msg.setPosition(Vector2f(windowSize / 3.0f, windowSize - 50));
            window.draw(msg);
        } else if (showInvalidMove && invalidMoveClock.getElapsedTime().asSeconds() >= 1.0f) {
            showInvalidMove = false;
        }

        window.display();
        sf::sleep(sf::milliseconds(60)); 
    }

    return 0;
}
