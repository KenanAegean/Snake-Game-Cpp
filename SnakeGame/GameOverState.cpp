#include "GameOverState.h"
#include "SnakeGraphics.h"
#include <windows.h>

GameOverState::GameOverState(const std::string& message)
    : gameOverMessage(message) {}

GameOverState::~GameOverState() {}

void GameOverState::Init(SnakeGraphics* graphics) {
    // Initialization for Game Over state if needed.
}

void GameOverState::Update(float deltaTime) {
    // Optionally, animate or wait for input.
}

void GameOverState::Render(SnakeGraphics* graphics) {
    // Clear the grid.
    Color backgroundColor(0, 0, 0);
    for (int y = 0; y < graphics->GetNumRows(); y++) {
        for (int x = 0; x < graphics->GetNumColumns(); x++) {
            graphics->PlotTile(x, y, 0, backgroundColor, backgroundColor, L' ');
        }
    }
    
    // Display "Game Over" text in the center.
    std::wstring text = L"Game Over";
    int centerX = graphics->GetNumColumns() / 2;
    int centerY = graphics->GetNumRows() / 2;
    graphics->PlotText(centerX, centerY, 1, backgroundColor, text.c_str(), Color(255, 255, 255), SnakeGraphics::Center);
}

void GameOverState::KeyDown(int key) {
    // For now, exit the game on key press.
    if (key == VK_RETURN || key == VK_SPACE) {
        PostQuitMessage(0);
    }
}

void GameOverState::CleanUp() {
    // Cleanup resources if needed.
}
