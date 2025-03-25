#pragma once
#include "GameState.h"
#include <string>
#include <vector>
#include <functional>

class GameOverState : public GameState {
public:
    // Modified constructor to accept final score and level.
    GameOverState(const std::string& message, int score, int level);
    virtual ~GameOverState();

    void Init(SnakeGraphics* graphics) override;
    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void KeyDown(int key) override;
    void CleanUp() override;

    // Callbacks to transition to Replay or Main Menu.
    std::function<void()> onReplay;
    std::function<void()> onMainMenu;

private:
    std::string gameOverMessage;
    int finalScore;
    int finalLevel;
    std::vector<std::wstring> menuOptions;
    int selectedOption;
};
