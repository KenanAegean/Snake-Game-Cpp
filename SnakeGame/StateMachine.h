#pragma once
#include "GameState.h"
#include <memory>

class StateMachine {
public:
    // Change the current state.
    void ChangeState(std::unique_ptr<GameState> newState, SnakeGraphics* graphics) {
        if (currentState) {
            currentState->CleanUp();
        }
        currentState = std::move(newState);
        currentState->Init(graphics);
    }
    
    // Update the current state.
    void Update(float deltaTime) {
        if (currentState) {
            currentState->Update(deltaTime);
        }
    }
    
    // Render the current state.
    void Render(SnakeGraphics* graphics) {
        if (currentState) {
            currentState->Render(graphics);
        }
    }
    
    // Pass key down events to the current state.
    void KeyDown(int key) {
        if (currentState) {
            currentState->KeyDown(key);
        }
    }
    
private:
    std::unique_ptr<GameState> currentState;
};
