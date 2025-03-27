#include "GamePlayState.h"
#include "World.h"
#include "Snake.h"
#include "SnakeGraphics.h"
#include <windows.h>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>

GamePlayState::GamePlayState()
    : world(nullptr), gridColumns(0), gridRows(0),
      currentLevel(1), applesEatenThisLevel(0),
      gameOverTriggered(false)
{
    // Set default game mode (can be modified by the caller).
    settings.mode = PlayMode::OnePlayer;
    settings.targetScore = 10;
}

GamePlayState::~GamePlayState() {}

void GamePlayState::Init(SnakeGraphics* graphics) {
    // Create the game world.
    world = std::make_unique<World>();

    // Get grid dimensions.
    gridColumns = graphics->GetNumColumns();
    gridRows    = graphics->GetNumRows();

    // Clear any previous players.
    players.clear();

    int centerX = gridColumns / 2;
    int centerY = gridRows / 2;

    // Create snake objects based on the selected game mode.
    switch(settings.mode) {
        case PlayMode::OnePlayer: {
            std::unique_ptr<Snake> snake1 = std::make_unique<Snake>(Vec2{ centerX, centerY });
            Snake* s1 = snake1.get();
            s1->onAppleEaten = [this]() {
                players[0].score++;
                applesEatenThisLevel++;
                if (applesEatenThisLevel >= 10) {
                    std::string nextLevelFile = "level" + std::to_string(currentLevel + 1) + ".txt";
                    std::ifstream file(nextLevelFile);
                    if (file.good()) {
                        currentLevel++;
                        applesEatenThisLevel = 0;
                        world->ClearWalls();
                        world->LoadLevel(nextLevelFile);
                    }
                }
            };
            players.push_back({ s1, 0, false });
            world->AddGameObject(std::move(snake1));
            break;
        }
        case PlayMode::TwoPlayerVersus: {
            std::unique_ptr<Snake> snake1 = std::make_unique<Snake>(Vec2{ centerX - 3, centerY });
            std::unique_ptr<Snake> snake2 = std::make_unique<Snake>(Vec2{ centerX + 3, centerY });
            Snake* s1 = snake1.get();
            Snake* s2 = snake2.get();
            s1->onAppleEaten = [this]() { players[0].score++; };
            s2->onAppleEaten = [this]() { players[1].score++; };
            players.push_back({ s1, 0, false });
            players.push_back({ s2, 0, false });
            world->AddGameObject(std::move(snake1));
            world->AddGameObject(std::move(snake2));
            break;
        }
        case PlayMode::TwoPlayerCooperative: {
            std::unique_ptr<Snake> snake1 = std::make_unique<Snake>(Vec2{ centerX - 3, centerY });
            std::unique_ptr<Snake> snake2 = std::make_unique<Snake>(Vec2{ centerX + 3, centerY });
            Snake* s1 = snake1.get();
            Snake* s2 = snake2.get();
            s1->onAppleEaten = [this]() { players[0].score++; };
            s2->onAppleEaten = [this]() { players[1].score++; };
            players.push_back({ s1, 0, false });
            players.push_back({ s2, 0, false });
            world->AddGameObject(std::move(snake1));
            world->AddGameObject(std::move(snake2));
            break;
        }
        case PlayMode::PlayerVsAI: {
            std::unique_ptr<Snake> playerSnake = std::make_unique<Snake>(Vec2{ centerX - 3, centerY });
            std::unique_ptr<Snake> aiSnake     = std::make_unique<Snake>(Vec2{ centerX + 3, centerY });
            Snake* s1 = playerSnake.get();
            Snake* s2 = aiSnake.get();
            s1->onAppleEaten = [this]() { players[0].score++; };
            s2->onAppleEaten = [this]() { players[1].score++; };
            players.push_back({ s1, 0, false });
            players.push_back({ s2, 0, true });
            world->AddGameObject(std::move(playerSnake));
            world->AddGameObject(std::move(aiSnake));
            break;
        }
        case PlayMode::PlayerAndAI: {
            std::unique_ptr<Snake> playerSnake = std::make_unique<Snake>(Vec2{ centerX - 3, centerY });
            std::unique_ptr<Snake> aiSnake     = std::make_unique<Snake>(Vec2{ centerX + 3, centerY });
            Snake* s1 = playerSnake.get();
            Snake* s2 = aiSnake.get();
            s1->onAppleEaten = [this]() { players[0].score++; };
            s2->onAppleEaten = [this]() { players[1].score++; };
            players.push_back({ s1, 0, false });
            players.push_back({ s2, 0, true });
            world->AddGameObject(std::move(playerSnake));
            world->AddGameObject(std::move(aiSnake));
            break;
        }
    }

    // Load the initial level and spawn an apple.
    world->LoadLevel("level1.txt");
    world->SpawnApple(gridColumns, gridRows);
}

void GamePlayState::Update(float deltaTime) {
    if (world) {
        world->Update(deltaTime);
    }
    
    // Check collisions for each player's snake.
    // (Now, in any mode, if a snake collides, we trigger game over.)
    for (auto& player : players) {
        if (player.snake) {
            Vec2 head = player.snake->GetSegments().front();
            if (head.x < 0 || head.x >= gridColumns ||
                head.y < 0 || head.y >= gridRows ||
                player.snake->HasSelfCollision() ||
                player.snake->HasCollidedWithWall())
            {
                if (!gameOverTriggered && onGameOver) {
                    gameOverTriggered = true;
                    onGameOver();
                }
                return;
            }
        }
    }
    
    // --- Scoring and Game End Conditions ---
    bool isCompetitive = (settings.mode == PlayMode::OnePlayer ||
                          settings.mode == PlayMode::TwoPlayerVersus ||
                          settings.mode == PlayMode::PlayerVsAI);
    bool isCooperative   = (settings.mode == PlayMode::TwoPlayerCooperative ||
                          settings.mode == PlayMode::PlayerAndAI);
    
    if (isCompetitive) {
        // For competitive modes, if any player's score reaches the target, trigger game over.
        for (auto& player : players) {
            if (player.score >= settings.targetScore) {
                if (!gameOverTriggered && onGameOver) {
                    gameOverTriggered = true;
                    onGameOver();
                }
                return;
            }
        }
    } else if (isCooperative) {
        // For cooperative modes, trigger game over when the combined score reaches the target.
        int totalScore = 0;
        for (auto& player : players) {
            totalScore += player.score;
        }
        if (totalScore >= settings.targetScore) {
            if (!gameOverTriggered && onGameOver) {
                gameOverTriggered = true;
                onGameOver();
            }
            return;
        }
    }
}

void GamePlayState::Render(SnakeGraphics* graphics) {
    Color backgroundColor(0, 0, 0);
    // Clear the screen.
    for (int y = 0; y < graphics->GetNumRows(); y++) {
        for (int x = 0; x < graphics->GetNumColumns(); x++) {
            graphics->PlotTile(x, y, 0, backgroundColor, backgroundColor, L' ');
        }
    }
    
    // Render game objects (snakes, walls, apples).
    if (world) {
        world->Render(graphics);
    }
    
    // Display score and level information.
    std::wstringstream ss;
    // For competitive modes, display separate scores for Player 1 and Player 2.
    if (settings.mode == PlayMode::TwoPlayerVersus || settings.mode == PlayMode::PlayerVsAI) {
        ss << L"P1: " << players[0].score << L"   P2: " << players[1].score 
           << L"   Level: " << currentLevel;
    } else {
        ss << L"Score: " << GetScore() << L"   Level: " << currentLevel;
    }
    graphics->PlotText(1, 0, 2, backgroundColor, ss.str().c_str(), Color(255, 255, 255), SnakeGraphics::Left);
}

void GamePlayState::KeyDown(int key) {
    // Player 1: Arrow keys.
    if (!players.empty() && players[0].snake) {
        switch (key) {
            case VK_UP:    players[0].snake->ChangeDirection(Direction::Up); break;
            case VK_DOWN:  players[0].snake->ChangeDirection(Direction::Down); break;
            case VK_LEFT:  players[0].snake->ChangeDirection(Direction::Left); break;
            case VK_RIGHT: players[0].snake->ChangeDirection(Direction::Right); break;
        }
    }
    
    // Player 2: WASD keys (if present and not AI).
    if (players.size() >= 2 && !players[1].isAI && players[1].snake) {
        switch (key) {
            case 'W':  players[1].snake->ChangeDirection(Direction::Up); break;
            case 'S':  players[1].snake->ChangeDirection(Direction::Down); break;
            case 'A':  players[1].snake->ChangeDirection(Direction::Left); break;
            case 'D':  players[1].snake->ChangeDirection(Direction::Right); break;
        }
    }
}

void GamePlayState::CleanUp() {
    world.reset();
    players.clear();
}
