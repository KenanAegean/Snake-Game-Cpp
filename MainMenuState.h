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
    
private:
    int selectedOption;
    std::vector<std::wstring> menuOptions;
};
