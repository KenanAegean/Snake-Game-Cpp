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
    Snake(const Vec2& pos);
    virtual ~Snake();

    // Update the snake's position using a movement timer.
    void Update(float deltaTime) override;
    
    // Render each snake segment.
    void Render(SnakeGraphics* graphics) override;
    
    // Handle collisions with other game objects.
    void OnCollision(GameObject* other) override;
    
    // Change the snake's moving direction.
    void ChangeDirection(Direction newDir);
    
    // Grow the snake by incrementing the pending growth counter.
    void Grow();
    
    // Check if the snake's head collides with any other segment.
    bool HasSelfCollision() const;
    
    // Retrieve snake segments (for debugging or further collision checking).
    const std::deque<Vec2>& GetSegments() const;
    
    // Callback triggered when the snake eats an apple.
    std::function<void()> onAppleEaten;

private:
    std::deque<Vec2> segments;  // The front element is the head.
    Direction currentDirection;
    float speed;                // Reserved for future use.
    
    // Timer variables to control movement updates.
    float movementAccumulator;
    const float moveInterval;   // Time interval (in seconds) between moves.
    
    int pendingGrowth;          // Number of segments to grow.
};
