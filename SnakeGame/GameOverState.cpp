#include "GameOverState.h"
#include "SnakeGraphics.h"
#include <windows.h>
#include <iostream>
#include <sstream>

GameOverState::GameOverState(const std::string& message, int score, int level)
    : gameOverMessage(message), finalScore(score), finalLevel(level), selectedOption(0)
{
    // Set up the menu options.
    menuOptions.push_back(L"Replay");
    menuOptions.push_back(L"Main Menu");
}

GameOverState::~GameOverState() {}

void GameOverState::Init(SnakeGraphics* graphics) {
    std::cout << "GameOverState initialized." << std::endl;
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
    
    // Calculate center positions.
    int centerX = graphics->GetNumColumns() / 2;
    int centerY = graphics->GetNumRows() / 2 - 3; // Adjust vertical position as needed.
    
    // Render "Game Over" at the center.
    graphics->PlotText(centerX, centerY, 1, backgroundColor, L"Game Over", Color(255, 255, 255), SnakeGraphics::Center);
    
    // Render score text on the next line.
    std::wstring scoreText = L"Score: " + std::to_wstring(finalScore) + L"   Level: " + std::to_wstring(finalLevel);
    graphics->PlotText(centerX, centerY + 1, 1, backgroundColor, scoreText.c_str(), Color(255, 255, 255), SnakeGraphics::Center);
    
    // Render menu options below the score.
    int optionY = centerY + 3;
    for (size_t i = 0; i < menuOptions.size(); i++) {
        // Highlight the selected option.
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
        // Enter key selects the option.
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
