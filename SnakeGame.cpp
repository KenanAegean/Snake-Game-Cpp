#include <iostream>
#include <chrono>
#include <thread>
#include "SnakeGraphics.h"
#include "SnakeInput.h"
#include "StateMachine.h"
#include "MainMenuState.h"
#include "GamePlayState.h"
#include "GameOverState.h" // Include the new game over state

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
    
    // Create MainMenuState and set the callback to transition to gameplay.
    auto mainMenuState = std::make_unique<MainMenuState>();
    mainMenuState->onStartGame = [&]() {
        auto gamePlayState = std::make_unique<GamePlayState>();
        // Set the game over callback to transition to GameOverState.
        gamePlayState->onGameOver = [&]() {
            stateMachine.ChangeState(std::make_unique<GameOverState>("Game Over!"), &graphics);
        };
        stateMachine.ChangeState(std::move(gamePlayState), &graphics);
    };
    stateMachine.ChangeState(std::move(mainMenuState), &graphics);
    
    // Forward key inputs to the state machine.
    SnakeInput::AddKeyDownCallback([&](int key) {
        stateMachine.KeyDown(key);
    });
    
    // Main game loop.
    bool running = true;
    auto lastFrame = std::chrono::high_resolution_clock::now();
    while (running) {
        // Process window messages.
        if (!graphics.UpdateWindowMessages()) {
            running = false;
            break;
        }
        
        // Calculate delta time.
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = now - lastFrame;
        lastFrame = now;
        
        // Update and render the active state.
        stateMachine.Update(deltaTime.count());
        stateMachine.Render(&graphics);
        graphics.Render();
        
        // Simple frame rate control.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    // Cleanup input.
    SnakeInput::CleanUp();
    return 0;
}
