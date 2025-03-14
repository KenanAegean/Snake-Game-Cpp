#include "MainMenuState.h"
#include "SnakeGraphics.h"
#include <windows.h>

MainMenuState::MainMenuState() : selectedOption(0) {
    // Set up your menu options.
    menuOptions.push_back(L"Start Game");
    menuOptions.push_back(L"Exit");
}

void MainMenuState::Init(SnakeGraphics* graphics) {
    // Initialize any state-specific variables or animations here.
}

void MainMenuState::Update(float deltaTime) {
    // Update animations or transitions if needed.
}

void MainMenuState::Render(SnakeGraphics* graphics) {
    int numOptions = static_cast<int>(menuOptions.size());
    int centerX = graphics->GetNumColumns() / 2;
    int startY = graphics->GetNumRows() / 2 - numOptions;
    
    for (int i = 0; i < numOptions; i++) {
        // Highlight the selected option with a different background color.
        Color bg = (i == selectedOption) ? Color(100, 100, 100) : Color(0, 0, 0);
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
            // Invoke the callback to change to the gameplay state.
            if (onStartGame) {
                onStartGame();
            }
        } else if (menuOptions[selectedOption] == L"Exit") {
            PostQuitMessage(0);
        }
    }
}

void MainMenuState::CleanUp() {
    // Free any resources or reset variables if needed.
}
