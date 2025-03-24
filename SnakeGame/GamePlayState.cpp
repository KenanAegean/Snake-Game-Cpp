#include "GamePlayState.h"
#include "World.h"
#include "Snake.h"
#include "SnakeGraphics.h"
#include <windows.h>
#include <memory>
#include <iostream>
#include <sstream>

GamePlayState::GamePlayState()
    : snake(nullptr), gridColumns(0), gridRows(0), score(0), gameOverTriggered(false)
{}

GamePlayState::~GamePlayState() {}

void GamePlayState::Init(SnakeGraphics* graphics) {
    // Create the world.
    world = std::make_unique<World>();

    // Store grid dimensions.
    gridColumns = graphics->GetNumColumns();
    gridRows    = graphics->GetNumRows();
    int centerX = gridColumns / 2;
    int centerY = gridRows / 2;

    // Create a new snake at the center.
    std::unique_ptr<Snake> newSnake = std::make_unique<Snake>(Vec2{ centerX, centerY });
    snake = newSnake.get();

    // When the snake eats an apple, increment the score.
    snake->onAppleEaten = [this]() {
        score++;
        std::cout << "Score increased to " << score << "\n";
    };

    // Note: We no longer assign snake->onGameOver here.
    // Instead, game over detection is handled in Update().

    // Add the snake to the world.
    world->AddGameObject(std::move(newSnake));

    // Load walls from a text file (ensure the file doesn't exceed gridColumns or gridRows).
    world->LoadLevel("level1.txt");

    // Spawn an initial apple.
    world->SpawnApple(gridColumns, gridRows);
}

void GamePlayState::Update(float deltaTime) {
    if (world) {
        world->Update(deltaTime);
    }
    
    if (!gameOverTriggered && snake) {
        // Check if the snake collided with a wall.
        if (snake->HasCollidedWithWall()) {
            gameOverTriggered = true;
            std::cout << "Game Over triggered: Collision with wall (flag detected).\n";
            if (onGameOver) {
                onGameOver();
            }
        }
        else {
            // Otherwise, check for out-of-bounds or self-collision.
            Vec2 head = snake->GetSegments().front();
            if (head.x < 0 || head.x >= gridColumns ||
                head.y < 0 || head.y >= gridRows ||
                snake->HasSelfCollision()) {
                gameOverTriggered = true;
                std::cout << "Game Over triggered: Collision detected.\n";
                if (onGameOver) {
                    onGameOver();
                }
                }
        }
    }
}



void GamePlayState::Render(SnakeGraphics* graphics) {
    // Clear the screen.
    Color backgroundColor(0, 0, 0);
    for (int y = 0; y < graphics->GetNumRows(); y++) {
        for (int x = 0; x < graphics->GetNumColumns(); x++) {
            graphics->PlotTile(x, y, 0, backgroundColor, backgroundColor, L' ');
        }
    }
    
    // Render all objects (snake, walls, apples, etc.).
    if (world) {
        world->Render(graphics);
    }
    
    // Display score at top-left.
    std::wstringstream ss;
    ss << L"Score: " << score;
    graphics->PlotText(1, 0, 2, backgroundColor, ss.str().c_str(), Color(255, 255, 255), SnakeGraphics::Left);
}

void GamePlayState::KeyDown(int key) {
    if (!snake) {
        return;
    }
    switch (key) {
    case VK_LEFT:  snake->ChangeDirection(Direction::Left);  break;
    case VK_RIGHT: snake->ChangeDirection(Direction::Right); break;
    case VK_UP:    snake->ChangeDirection(Direction::Up);    break;
    case VK_DOWN:  snake->ChangeDirection(Direction::Down);  break;
    default: break;
    }
}

void GamePlayState::CleanUp() {
    world.reset();
    snake = nullptr;
}
