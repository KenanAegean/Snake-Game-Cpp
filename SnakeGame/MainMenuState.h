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

    // Callback to start game, passing the selected mode.
    std::function<void(PlayMode)> onStartGame;

    // Setter for the clear screen flag.
    void SetIsMainMenuReturned(bool value) { isMainMenuReturned = value; }

    // Returns the selected game mode based on the menu index.
    PlayMode GetSelectedMode() const;

private:
    int selectedOption;
    std::vector<std::wstring> menuOptions;
    bool isMainMenuReturned;
};