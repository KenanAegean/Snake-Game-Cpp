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

    // Called each frame to update the snake's position.
    void Update(float deltaTime) override;
    
    // Called each frame to render the snake segments.
    void Render(SnakeGraphics* graphics) override;
    
    // Handles collisions with other game objects (Apple, Wall, etc.).
    void OnCollision(GameObject* other) override;
    
    // Change the snake's movement direction.
    void ChangeDirection(Direction newDir);
    
    // Grow the snake by incrementing the pending growth counter.
    void Grow();
    
    // Check if the snake's head collides with any other segment of its body.
    bool HasSelfCollision() const;
    
    // Access snake segments (for collision checks or debugging).
    const std::deque<Vec2>& GetSegments() const;
    
    // Called when the snake eats an apple.
    std::function<void()> onAppleEaten;
    
    // Called when the snake collides with a wall (or triggers a game over).
    std::function<void()> onGameOver;

    // Override to let the world skip updating or rendering this snake if inactive.
    bool IsActive() const override { return active; }

    bool HasCollidedWithWall() const { return hasCollidedWithWall; }

    Direction GetCurrentDirection() const { return currentDirection; }

private:
    std::deque<Vec2> segments;  // Front element is the head
    Direction currentDirection;
    
    float speed;                // (Reserved for future use)
    float movementAccumulator;  // Accumulates time between moves
    const float moveInterval;   // How often (seconds) the snake moves
    
    int pendingGrowth;          // How many extra segments to add
    
    // If false, the snake won't update or render anymore.
    bool active;

    bool hasCollidedWithWall = false;
};
