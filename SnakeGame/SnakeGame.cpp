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
    // Initialize graphics.
    SnakeGraphics graphics(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_COLUMNS, NUM_ROWS);
    if (!graphics.Init()) {
        std::cerr << "Failed to initialize graphics." << std::endl;
        return -1;
    }
    
    // Initialize input.
    if (!SnakeInput::Init(&graphics)) {
        std::cerr << "Failed to initialize input." << std::endl;
        return -1;
    }
    
    // Create the state machine.
    StateMachine stateMachine;
    
    // Central "start game" lambda that creates a new GamePlayState,
    // sets its mode, target score, and all its callbacks.
    std::function<void(PlayMode)> startGame;
    startGame = [&](PlayMode mode) {
        auto gameplayState = std::make_unique<GamePlayState>();
        gameplayState->settings.mode = mode;
        gameplayState->settings.targetScore = 15;
        Apple::ResetFirstAppleFlag();
        
        // Set the onGameOver callback so that when a collision is detected,
        // we transition to the Game Over state.
        gameplayState->onGameOver = [&, gp = gameplayState.get()]() {
            int finalLevel = gp->GetLevel();
            int finalScore = gp->GetScore();
            std::wstring winnerMsg = L"";
            // For competitive modes, determine a winner.
            if (mode == PlayMode::TwoPlayerVersus || mode == PlayMode::PlayerVsAI) {
                const auto& players = gp->GetPlayers();
                if (players[0].score > players[1].score) {
                    winnerMsg = L"Player 1 wins!";
                } else if (players[0].score < players[1].score) {
                    winnerMsg = L"Player 2 wins!";
                } else {
                    winnerMsg = L"It's a tie!";
                }
            }
            auto gameOverState = std::make_unique<GameOverState>(L"Game Over!", winnerMsg, finalScore, finalLevel);
            // When Replay is selected, call startGame again.
            gameOverState->onReplay = [&, mode]() {
                Apple::ResetFirstAppleFlag();
                startGame(mode);
            };
            // When Main Menu is selected, return to the main menu.
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
    
    // Create the initial Main Menu state.
    auto mainMenuState = std::make_unique<MainMenuState>();
    mainMenuState->onStartGame = startGame;
    stateMachine.ChangeState(std::move(mainMenuState), &graphics);
    
    // Forward key inputs to the state machine.
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
