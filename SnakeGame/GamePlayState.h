#pragma once
#include "GameState.h"
#include <memory>
#include <functional>
#include <vector>
#include "GameMode.h"

class World;
class Snake;

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
    
    int GetScore() const { 
        int total = 0;
        for (auto& p : players) {
            total += p.score;
        }
        return total;
    }
    int GetLevel() const { return currentLevel; }
    
    std::function<void()> onGameOver;
    
    GameModeSettings settings;

    const std::vector<PlayerInfo>& GetPlayers() const { return players; }

private:
    std::unique_ptr<World> world;
    int gridColumns;
    int gridRows;
    int currentLevel;
    int applesEatenThisLevel;
    bool gameOverTriggered;
    
    std::vector<PlayerInfo> players;

    void HandleAppleEaten(int playerIndex);
};
