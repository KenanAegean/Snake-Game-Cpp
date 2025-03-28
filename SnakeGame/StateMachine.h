#pragma once
#include "GameState.h"
#include <memory>

class StateMachine {
public:
    void ChangeState(std::unique_ptr<GameState> newState, SnakeGraphics* graphics) {
        if (currentState) {
            currentState->CleanUp();
        }
        currentState = std::move(newState);
        currentState->Init(graphics);
    }
    
    void Update(float deltaTime) {
        if (currentState) {
            currentState->Update(deltaTime);
        }
    }
    
    void Render(SnakeGraphics* graphics) {
        if (currentState) {
            currentState->Render(graphics);
        }
    }
    
    void KeyDown(int key) {
        if (currentState) {
            currentState->KeyDown(key);
        }
    }
    
private:
    std::unique_ptr<GameState> currentState;
};