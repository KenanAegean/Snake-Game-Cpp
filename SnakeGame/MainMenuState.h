#pragma once
#include "GameState.h"
#include <vector>
#include <string>
#include <functional>

class MainMenuState : public GameState {
public:
    MainMenuState();
    virtual ~MainMenuState() {}
    
    void Init(SnakeGraphics* graphics) override;
    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void KeyDown(int key) override;
    void CleanUp() override;

    // Callback that will be set by the main function to change state.
    std::function<void()> onStartGame;
    
    // Setter for the clear screen flag.
    void SetIsMainMenuReturned(bool value) { isMainMenuReturned = value; }
    
private:
    int selectedOption;
    std::vector<std::wstring> menuOptions;
    
    // Flag to indicate that we are returning to main menu and need to clear the screen.
    bool isMainMenuReturned;
};
