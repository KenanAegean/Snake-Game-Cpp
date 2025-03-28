#include "MainMenuState.h"
#include "SnakeGraphics.h"
#include <windows.h>

#include "Apple.h"

MainMenuState::MainMenuState() : selectedOption(0), isMainMenuReturned(false) {
    menuOptions.push_back(L"One Player");
    menuOptions.push_back(L"Player vs Player");
    menuOptions.push_back(L"Player and Player");
    menuOptions.push_back(L"Player vs AI");
    menuOptions.push_back(L"Player and AI");
    //menuOptions.push_back(L"--------------------");
    menuOptions.push_back(L"Exit");
}

void MainMenuState::Init(SnakeGraphics* graphics) {}

void MainMenuState::Update(float deltaTime) {}

void MainMenuState::Render(SnakeGraphics* graphics) {
    Color backgroundColor(0, 0, 0);
    for (int y = 0; y < graphics->GetNumRows(); y++) {
        for (int x = 0; x < graphics->GetNumColumns(); x++) {
            graphics->PlotTile(x, y, 0, backgroundColor, backgroundColor, L' ');
        }
    }
    
    int numRows = graphics->GetNumRows();
    int numColumns = graphics->GetNumColumns();
    int centerX = numColumns / 2;
    
    graphics->PlotText(centerX, 2, 1, backgroundColor, L"Snake  Game Cpp", Color(255, 255, 255), SnakeGraphics::Center);
    graphics->PlotText(centerX, 5, 1, backgroundColor, L"Game Modes:", Color(255, 255, 255), SnakeGraphics::Center);
    graphics->PlotText(centerX, 6, 1, backgroundColor, L"-----------------------", Color(255, 255, 255), SnakeGraphics::Center);
    
    int startY = 8;
    for (size_t i = 0; i < menuOptions.size() - 1; i++) {
        Color optionBg = (selectedOption == i) ? Color(100, 100, 100) : backgroundColor;
        graphics->PlotText(centerX, startY + static_cast<int>(i), 1, optionBg, menuOptions[i].c_str(), Color(255, 255, 255), SnakeGraphics::Center);
    }
    
    // Blank line after game modes
    int exitY = startY + static_cast<int>(menuOptions.size() - 1) + 1;
    Color exitBg = (selectedOption == menuOptions.size() - 1) ? Color(100, 100, 100) : backgroundColor;
    graphics->PlotText(centerX, exitY, 1, exitBg, menuOptions.back().c_str(), Color(255, 255, 255), SnakeGraphics::Center);
    graphics->PlotText(centerX, numRows - 2, 1, backgroundColor, L"The Kenan Ege Game", Color(255, 255, 255), SnakeGraphics::Center);
}

void MainMenuState::KeyDown(int key) {
    if (key == VK_UP) {
        selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
    } else if (key == VK_DOWN) {
        selectedOption = (selectedOption + 1) % menuOptions.size();
    } else if (key == VK_RETURN) {
        if (selectedOption == static_cast<int>(menuOptions.size()) - 1) {
            PostQuitMessage(0);
        } else {
            if (onStartGame) {
                Apple::ResetFirstAppleFlag();
                onStartGame(GetSelectedMode());
            }
        }
    }
}

void MainMenuState::CleanUp() {}

PlayMode MainMenuState::GetSelectedMode() const {
    switch(selectedOption) {
        case 0: return PlayMode::OnePlayer;
        case 1: return PlayMode::TwoPlayerVersus;
        case 2: return PlayMode::TwoPlayerCooperative;
        case 3: return PlayMode::PlayerVsAI;
        case 4: return PlayMode::PlayerAndAI;
        default: return PlayMode::OnePlayer;
    }
}
