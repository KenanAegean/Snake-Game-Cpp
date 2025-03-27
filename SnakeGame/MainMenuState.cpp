#include "MainMenuState.h"
#include "SnakeGraphics.h"
#include <windows.h>

MainMenuState::MainMenuState() : selectedOption(0), isMainMenuReturned(false) {
    // Set up menu options: five game modes and an Exit option.
    menuOptions.push_back(L"One Player");
    menuOptions.push_back(L"Player vs Player");
    menuOptions.push_back(L"Player and Player");
    menuOptions.push_back(L"Player vs AI");
    menuOptions.push_back(L"Player and AI");
    //menuOptions.push_back(L"--------------------");
    menuOptions.push_back(L"Exit");
}

void MainMenuState::Init(SnakeGraphics* graphics) {
    // Additional initialization if needed.
}

void MainMenuState::Update(float deltaTime) {
    // Update animations or transitions if needed.
}

void MainMenuState::Render(SnakeGraphics* graphics) {
    Color backgroundColor(0, 0, 0);
    
    // If coming back to main menu, clear the screen.
    if (isMainMenuReturned) {
        for (int y = 0; y < graphics->GetNumRows(); y++) {
            for (int x = 0; x < graphics->GetNumColumns(); x++) {
                graphics->PlotTile(x, y, 0, backgroundColor, backgroundColor, L' ');
            }
        }
        isMainMenuReturned = false;
    }
    
    int numOptions = static_cast<int>(menuOptions.size());
    int centerX = graphics->GetNumColumns() / 2;
    int startY = graphics->GetNumRows() / 2 - numOptions;
    
    for (int i = 0; i < numOptions; i++) {
        Color bg = (i == selectedOption) ? Color(100, 100, 100) : backgroundColor;
        graphics->PlotText(centerX, startY + i, 1, bg, menuOptions[i].c_str(), Color(255, 255, 255), SnakeGraphics::Center);
    }
}

void MainMenuState::KeyDown(int key) {
    if (key == VK_UP) {
        selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
    } else if (key == VK_DOWN) {
        selectedOption = (selectedOption + 1) % menuOptions.size();
    } else if (key == VK_RETURN) {
        // If "Exit" is selected, quit.
        if (selectedOption == static_cast<int>(menuOptions.size()) - 1) {
            PostQuitMessage(0);
        } else {
            if (onStartGame) {
                onStartGame(GetSelectedMode());
            }
        }
    }
}

void MainMenuState::CleanUp() {
    // Clean up resources if needed.
}

PlayMode MainMenuState::GetSelectedMode() const {
    // Map menu index to the corresponding game mode.
    switch(selectedOption) {
        case 0: return PlayMode::OnePlayer;
        case 1: return PlayMode::TwoPlayerVersus;
        case 2: return PlayMode::TwoPlayerCooperative;
        case 3: return PlayMode::PlayerVsAI;
        case 4: return PlayMode::PlayerAndAI;
        default: return PlayMode::OnePlayer; // Fallback.
    }
}
