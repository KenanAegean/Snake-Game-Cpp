#pragma once
#include "GameState.h"
#include <memory>
#include <functional>

class World;
class Snake;

class GamePlayState : public GameState {
public:
    GamePlayState();
    virtual ~GamePlayState();

    void Init(SnakeGraphics* graphics) override;
    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void KeyDown(int key) override;
    void CleanUp() override;

    // Callback to transition to the Game Over state.
    std::function<void()> onGameOver;

private:
    std::unique_ptr<World> world;
    Snake* snake; // Raw pointer; ownership is managed by 'world'
    int gridColumns;
    int gridRows;
    
    // Score tracking variable.
    int score;
    
    // Flag to ensure game over is triggered only once.
    bool gameOverTriggered;
};
