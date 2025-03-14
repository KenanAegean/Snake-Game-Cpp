#include "GamePlayState.h"
#include "World.h"
#include "Snake.h"
#include "SnakeGraphics.h"
#include <windows.h>
#include <memory>
#include <iostream>
#include <sstream>

GamePlayState::GamePlayState() : snake(nullptr), gridColumns(0), gridRows(0), score(0) {}

GamePlayState::~GamePlayState() {}

void GamePlayState::Init(SnakeGraphics* graphics) {
    // Create the world.
    world = std::make_unique<World>();

    // Store grid dimensions.
    gridColumns = graphics->GetNumColumns();
    gridRows = graphics->GetNumRows();
    int centerX = gridColumns / 2;
    int centerY = gridRows / 2;

    // Create a new snake at the center.
    std::unique_ptr<Snake> newSnake = std::make_unique<Snake>(Vec2{ centerX, centerY });
    snake = newSnake.get();

    // Set up callback to increment score when an apple is eaten.
    snake->onAppleEaten = [this]() {
        score++;
        std::cout << "Score increased to " << score << "\n";
    };

    // Add the snake to the world.
    world->AddGameObject(std::move(newSnake));

    // Spawn an initial apple using the safe apple spawning logic.
    world->SpawnApple(gridColumns, gridRows);
}

void GamePlayState::Update(float deltaTime) {
    if (world) {
        world->Update(deltaTime);
    }
    
    // Check for wall collisions or self-collision.
    if (snake) {
        Vec2 head = snake->GetSegments().front();
        if (head.x < 0 || head.x >= gridColumns ||
            head.y < 0 || head.y >= gridRows ||
            snake->HasSelfCollision()) {
            std::cout << "Game Over triggered: Collision detected.\n";
            if (onGameOver) {
                onGameOver();
            }
        }
    }
}

void GamePlayState::Render(SnakeGraphics* graphics) {
    // Clear the grid.
    Color backgroundColor(0, 0, 0);
    for (int y = 0; y < graphics->GetNumRows(); y++) {
        for (int x = 0; x < graphics->GetNumColumns(); x++) {
            graphics->PlotTile(x, y, 0, backgroundColor, backgroundColor, L' ');
        }
    }
    
    // Render the game world (snake, apple, etc.).
    if (world) {
        world->Render(graphics);
    }
    
    // Render the score at the top-left corner.
    std::wstringstream ss;
    ss << L"Score: " << score;
    graphics->PlotText(1, 0, 2, backgroundColor, ss.str().c_str(), Color(255, 255, 255), SnakeGraphics::Left);
}

void GamePlayState::KeyDown(int key) {
    if (!snake) {
        return;
    }
    
    // Update snake direction based on key press.
    switch (key) {
    case VK_LEFT:
        snake->ChangeDirection(Direction::Left);
        break;
    case VK_RIGHT:
        snake->ChangeDirection(Direction::Right);
        break;
    case VK_UP:
        snake->ChangeDirection(Direction::Up);
        break;
    case VK_DOWN:
        snake->ChangeDirection(Direction::Down);
        break;
    default:
        break;
    }
}

void GamePlayState::CleanUp() {
    world.reset();
    snake = nullptr;
}
