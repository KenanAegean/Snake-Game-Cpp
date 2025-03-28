#pragma once
#include "SnakeGraphics.h"

class GameState {
public:
    virtual ~GameState() {}
    
    virtual void Init(SnakeGraphics* graphics) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(SnakeGraphics* graphics) = 0;
    virtual void KeyDown(int key) = 0;
    virtual void CleanUp() = 0;
};