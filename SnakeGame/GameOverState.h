#pragma once
#include "GameState.h"
#include <string>
#include <vector>
#include <functional>

class GameOverState : public GameState {
public:
    // Modified constructor to accept a winner message.
    GameOverState(const std::wstring& message, const std::wstring& winnerMessage, int score, int level);
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
    std::wstring gameOverMessage;
    std::wstring winnerMessage;
    int finalScore;
    int finalLevel;
    std::vector<std::wstring> menuOptions;
    int selectedOption;
    
};
