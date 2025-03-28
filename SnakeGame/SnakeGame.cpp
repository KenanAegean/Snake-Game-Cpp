#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include "Apple.h"
#include "SnakeGraphics.h"
#include "SnakeInput.h"
#include "StateMachine.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "GameOverState.h"
#include "GameMode.h"

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;
const int NUM_COLUMNS   = 40;
const int NUM_ROWS      = 30;

int main() {
    SnakeGraphics graphics(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_COLUMNS, NUM_ROWS);
    if (!graphics.Init()) {
        std::cerr << "Failed to initialize graphics." << std::endl;
        return -1;
    }
    
    if (!SnakeInput::Init(&graphics)) {
        std::cerr << "Failed to initialize input." << std::endl;
        return -1;
    }
    
    StateMachine stateMachine;
    

    std::function<void(PlayMode)> startGame;
    startGame = [&](PlayMode mode) {
        auto gameplayState = std::make_unique<GamePlayState>();
        gameplayState->settings.mode = mode;
        gameplayState->settings.targetScore = 15;
        Apple::ResetFirstAppleFlag();
        
        gameplayState->onGameOver = [&, gp = gameplayState.get()]() {
            int finalLevel = gp->GetLevel();
            int finalScore = gp->GetScore(); // default for non-competitive modes
            std::wstring winnerMsg = L"";
            // For competitive modes, determine a winner and show the winner's score.
            if (mode == PlayMode::TwoPlayerVersus || mode == PlayMode::PlayerVsAI) {
                const auto& players = gp->GetPlayers();
                // Get the winning score instead of summing both players' scores.
                int winnerScore = (players[0].score > players[1].score) ? players[0].score :
                                  (players[0].score < players[1].score) ? players[1].score : players[0].score;
                finalScore = winnerScore;
                if (players[0].score > players[1].score) {
                    winnerMsg = L"Player 1 wins!";
                } else if (players[0].score < players[1].score) {
                    winnerMsg = L"Player 2 wins!";
                } else {
                    winnerMsg = L"It's a tie!";
                }
            }
            auto gameOverState = std::make_unique<GameOverState>(L"Game Over!", winnerMsg, finalScore, finalLevel);
            gameOverState->onReplay = [&, mode]() {
                // int finalLevel = gp->GetLevel();
                int finalScore = gp->GetScore(); // default for non-competitive modes
                std::wstring winnerMsg = L"";
                // For competitive modes, determine a winner and show the winner's score.
                if (mode == PlayMode::TwoPlayerVersus || mode == PlayMode::PlayerVsAI) {
                    const auto& players = gp->GetPlayers();
                    // Get the winning score instead of summing both players' scores.
                    int winnerScore = (players[0].score > players[1].score) ? players[0].score :
                                      (players[0].score < players[1].score) ? players[1].score : players[0].score;
                    finalScore = winnerScore;
                    if (players[0].score > players[1].score) {
                        winnerMsg = L"Player 1 wins!";
                    } else if (players[0].score < players[1].score) {
                        winnerMsg = L"Player 2 wins!";
                    } else {
                        winnerMsg = L"It's a tie!";
                    }
                }
                startGame(mode);
            };
            gameOverState->onMainMenu = [&]() {
                auto newMainMenuState = std::make_unique<MainMenuState>();
                newMainMenuState->SetIsMainMenuReturned(true);
                newMainMenuState->onStartGame = startGame;
                stateMachine.ChangeState(std::move(newMainMenuState), &graphics);
            };
            stateMachine.ChangeState(std::move(gameOverState), &graphics);
        };
        
        stateMachine.ChangeState(std::move(gameplayState), &graphics);
    };
    
    auto mainMenuState = std::make_unique<MainMenuState>();
    mainMenuState->onStartGame = startGame;
    stateMachine.ChangeState(std::move(mainMenuState), &graphics);
    
    SnakeInput::AddKeyDownCallback([&](int key) {
        stateMachine.KeyDown(key);
    });
    
    // Main game loop.
    bool running = true;
    auto lastFrame = std::chrono::high_resolution_clock::now();
    while (running) {
        if (!graphics.UpdateWindowMessages()) {
            running = false;
            break;
        }
        
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = now - lastFrame;
        lastFrame = now;
        
        stateMachine.Update(deltaTime.count());
        stateMachine.Render(&graphics);
        graphics.Render();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    SnakeInput::CleanUp();
    return 0;
}