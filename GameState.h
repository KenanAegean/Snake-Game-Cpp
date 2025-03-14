#pragma once
#include "SnakeGraphics.h"

class GameState {
public:
    virtual ~GameState() {}
    
    // Called when the state becomes active.
    virtual void Init(SnakeGraphics* graphics) = 0;
    
    // Called every frame to update game logic.
    virtual void Update(float deltaTime) = 0;
    
    // Called every frame to render graphics.
    virtual void Render(SnakeGraphics* graphics) = 0;
    
    // Handle key down events.
    virtual void KeyDown(int key) = 0;
    
    // Called when the state is deactivated.
    virtual void CleanUp() = 0;
};
