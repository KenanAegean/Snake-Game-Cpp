#pragma once
#include "SnakeGraphics.h"

struct Vec2 {
    int x;
    int y;
};

class GameObject {
public:
    Vec2 position;
    
    GameObject(const Vec2& pos) : position(pos) {}
    virtual ~GameObject() {}

    virtual void Update(float deltaTime) = 0;
    virtual void Render(SnakeGraphics* graphics) = 0;
    virtual void OnCollision(GameObject* other) = 0;
    
    virtual bool IsActive() const { return true; }
};
