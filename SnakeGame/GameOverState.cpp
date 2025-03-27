#include "GameOverState.h"
#include "SnakeGraphics.h"
#include <windows.h>
#include <iostream>
#include <sstream>

GameOverState::GameOverState(const std::wstring& message, const std::wstring& winnerMsg, int score, int level)
    : gameOverMessage(message), winnerMessage(winnerMsg), finalScore(score), finalLevel(level), selectedOption(0)
{
    // Set up the menu options.
    menuOptions.push_back(L"Replay");
    menuOptions.push_back(L"Main Menu");
}

GameOverState::~GameOverState() {}

void GameOverState::Init(SnakeGraphics* graphics) {
    // Additional initialization if needed.
}

void GameOverState::Update(float deltaTime) {
    // Optional: animate menu options if desired.
}

void GameOverState::Render(SnakeGraphics* graphics) {
    // Clear the screen.
    Color backgroundColor(0, 0, 0);
    for (int y = 0; y < graphics->GetNumRows(); y++) {
        for (int x = 0; x < graphics->GetNumColumns(); x++) {
            graphics->PlotTile(x, y, 0, backgroundColor, backgroundColor, L' ');
        }
    }
    
    int centerX = graphics->GetNumColumns() / 2;
    int centerY = graphics->GetNumRows() / 2 - 4; // Adjust vertical position as needed.
    
    // Render "Game Over" at the center.
    graphics->PlotText(centerX, centerY, 1, backgroundColor, L"Game Over", Color(255, 255, 255), SnakeGraphics::Center);
    
    // Render winner message if provided.
    if (!winnerMessage.empty()) {
        graphics->PlotText(centerX, centerY + 1, 1, backgroundColor, winnerMessage.c_str(), Color(255, 255, 255), SnakeGraphics::Center);
    }
    
    // Render score and level.
    std::wstring scoreText = L"Score: " + std::to_wstring(finalScore) + L"   Level: " + std::to_wstring(finalLevel);
    graphics->PlotText(centerX, centerY + 2, 1, backgroundColor, scoreText.c_str(), Color(255, 255, 255), SnakeGraphics::Center);
    
    // Render menu options below the score.
    int optionY = centerY + 4;
    for (size_t i = 0; i < menuOptions.size(); i++) {
        Color optionBg = (i == selectedOption) ? Color(100, 100, 100) : backgroundColor;
        graphics->PlotText(centerX, optionY + static_cast<int>(i), 1, optionBg, menuOptions[i].c_str(), Color(255, 255, 255), SnakeGraphics::Center);
    }
}

void GameOverState::KeyDown(int key) {
    if (key == VK_UP) {
        selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
    } else if (key == VK_DOWN) {
        selectedOption = (selectedOption + 1) % menuOptions.size();
    } else if (key == VK_RETURN) {
        if (selectedOption == 0) { // Replay
            if (onReplay) {
                onReplay();
            }
        } else if (selectedOption == 1) { // Main Menu
            if (onMainMenu) {
                onMainMenu();
            }
        }
    }
}

void GameOverState::CleanUp() {
    // Clean up resources if needed.
}
