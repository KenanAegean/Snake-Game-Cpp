// GamePlayState.h
#pragma once
#include "GameState.h"
#include <memory>
#include <functional>
#include <vector>
#include "GameMode.h" // Include our game mode definitions

class World;
class Snake;

// Structure to store player-related info.
struct PlayerInfo {
    Snake* snake;
    int score;
    bool isAI;
};

class GamePlayState : public GameState {
public:
    GamePlayState();
    virtual ~GamePlayState();

    void Init(SnakeGraphics* graphics) override;
    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void KeyDown(int key) override;
    void CleanUp() override;

    // Returns the combined score (or you can customize for competitive modes).
    int GetScore() const { 
        int total = 0;
        for (auto& p : players) {
            total += p.score;
        }
        return total;
    }
    int GetLevel() const { return currentLevel; }

    // Callback to transition to the Game Over state.
    std::function<void()> onGameOver;

    // New game mode settings.
    GameModeSettings settings;

    const std::vector<PlayerInfo>& GetPlayers() const { return players; }

private:
    std::unique_ptr<World> world;
    int gridColumns;
    int gridRows;
    
    // Level progression and apple counter.
    int currentLevel;
    int applesEatenThisLevel;
    
    // Flag to ensure game over is triggered only once.
    bool gameOverTriggered;

    // Vector holding each player's info.
    std::vector<PlayerInfo> players;
};
