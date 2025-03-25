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
    int GetScore() const { return score; }
    int GetLevel() const { return currentLevel; }

    // Callback to transition to the Game Over state.
    std::function<void()> onGameOver;

private:
    std::unique_ptr<World> world;
    Snake* snake; // Raw pointer; ownership is managed by 'world'
    int gridColumns;
    int gridRows;
    
    // Overall score (accumulated across levels).
    int score;
    
    // Track the current level (starting at 1, corresponding to "level1.txt").
    int currentLevel;
    
    // Number of apples eaten in the current level.
    int applesEatenThisLevel;
    
    // Flag to ensure game over is triggered only once.
    bool gameOverTriggered;
};
