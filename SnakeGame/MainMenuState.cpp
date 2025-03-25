#include "MainMenuState.h"
#include "SnakeGraphics.h"
#include <windows.h>

MainMenuState::MainMenuState() : selectedOption(0), isMainMenuReturned(false) {
    // Set up your menu options.
    menuOptions.push_back(L"Start Game");
    menuOptions.push_back(L"Exit");
}

void MainMenuState::Init(SnakeGraphics* graphics) {
    // Any initialization you need.
}

void MainMenuState::Update(float deltaTime) {
    // Update animations or transitions if needed.
}

void MainMenuState::Render(SnakeGraphics* graphics) {
    Color backgroundColor(0, 0, 0);
    
    // If coming back to main menu, force clear the entire screen.
    if (isMainMenuReturned) {
        for (int y = 0; y < graphics->GetNumRows(); y++) {
            for (int x = 0; x < graphics->GetNumColumns(); x++) {
                graphics->PlotTile(x, y, 0, backgroundColor, backgroundColor, L' ');
            }
        }
        // Reset the flag after clearing.
        isMainMenuReturned = false;
    }
    
    // Draw the main menu options.
    int numOptions = static_cast<int>(menuOptions.size());
    int centerX = graphics->GetNumColumns() / 2;
    int startY = graphics->GetNumRows() / 2 - numOptions;
    
    for (int i = 0; i < numOptions; i++) {
        Color bg = (i == selectedOption) ? Color(100, 100, 100) : backgroundColor;
        Color fg = Color(255, 255, 255);
        graphics->PlotText(centerX, startY + i, 0, bg, menuOptions[i].c_str(), fg, SnakeGraphics::Center);
    }
}

void MainMenuState::KeyDown(int key) {
    if (key == VK_UP) {
        selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
    } else if (key == VK_DOWN) {
        selectedOption = (selectedOption + 1) % menuOptions.size();
    } else if (key == VK_RETURN) {
        if (menuOptions[selectedOption] == L"Start Game") {
            if (onStartGame) {
                onStartGame();
            }
        } else if (menuOptions[selectedOption] == L"Exit") {
            PostQuitMessage(0);
        }
    }
}

void MainMenuState::CleanUp() {
    // Free any resources if needed.
}
