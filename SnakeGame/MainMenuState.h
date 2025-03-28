#pragma once
#include "GameState.h"
#include <vector>
#include <string>
#include <functional>
#include "GameMode.h"  // for PlayMode

class MainMenuState : public GameState {
public:
    MainMenuState();
    virtual ~MainMenuState() {}

    void Init(SnakeGraphics* graphics) override;
    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void KeyDown(int key) override;
    void CleanUp() override;
    
    std::function<void(PlayMode)> onStartGame;
    
    void SetIsMainMenuReturned(bool value) { isMainMenuReturned = value; }
    
    PlayMode GetSelectedMode() const;

private:
    int selectedOption;
    std::vector<std::wstring> menuOptions;
    bool isMainMenuReturned;
};