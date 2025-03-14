#pragma once
#include "GameState.h"
#include <string>

class GameOverState : public GameState {
public:
    GameOverState(const std::string& message);
    virtual ~GameOverState();

    void Init(SnakeGraphics* graphics) override;
    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void KeyDown(int key) override;
    void CleanUp() override;

private:
    std::string gameOverMessage;
};
