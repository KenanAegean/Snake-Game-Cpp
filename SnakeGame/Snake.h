#pragma once
#include "GameObject.h"
#include <deque>
#include <iostream>
#include <functional> // for std::function

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

class Snake : public GameObject {
public:
    // Constructor with an extra color parameter; default is green.
    Snake(const Vec2& pos, const Color& color = Color(0, 255, 0));
    virtual ~Snake();

    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void OnCollision(GameObject* other) override;
    
    void ChangeDirection(Direction newDir);
    void Grow();
    bool HasSelfCollision() const;
    const std::deque<Vec2>& GetSegments() const;
    
    std::function<void()> onAppleEaten;
    std::function<void()> onGameOver;
    
    virtual bool IsActive() const override { return active; }
    bool HasCollidedWithWall() const { return hasCollidedWithWall; }
    Direction GetCurrentDirection() const { return currentDirection; }
    
    // Setter to change the snake's color if needed.
    void SetColor(const Color& newColor) { bodyColor = newColor; }
    
private:
    std::deque<Vec2> segments;
    Direction currentDirection;
    
    float speed;
    float movementAccumulator;
    const float moveInterval;
    
    int pendingGrowth;
    
    bool active;
    bool hasCollidedWithWall = false;
    
    Color bodyColor; // New member to store the snake's color.
};
