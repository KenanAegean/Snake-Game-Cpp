#include "Snake.h"
#include "Apple.h"
#include "Wall.h"
#include "SnakeGraphics.h"
#include <iostream>

Snake::Snake(const Vec2& pos, const Color& color)
    : GameObject(pos),
      currentDirection(Direction::Right),
      speed(5.0f),
      movementAccumulator(0.0f),
      moveInterval(0.2f),
      pendingGrowth(0),
      active(true),
      bodyColor(color)
{
    segments.push_back(pos);
    std::cout << "Snake created at (" << pos.x << ", " << pos.y << ")\n";
}

Snake::~Snake() {}

void Snake::Update(float deltaTime)
{
    if (!active) {
        return;
    }
    movementAccumulator += deltaTime;
    if (movementAccumulator < moveInterval) {
        return;
    }
    movementAccumulator -= moveInterval;

    Vec2 newHead = segments.front();
    switch (currentDirection) {
    case Direction::Up:    newHead.y -= 1; break;
    case Direction::Down:  newHead.y += 1; break;
    case Direction::Left:  newHead.x -= 1; break;
    case Direction::Right: newHead.x += 1; break;
    }

    segments.push_front(newHead);
    position = newHead;
    
    if (pendingGrowth > 0) {
        pendingGrowth--;
    } else {
        segments.pop_back();
    }
}

void Snake::Render(SnakeGraphics* graphics)
{
    if (!active) {
        return;
    }
    // Use the instance's color instead of a hardcoded value
    Color snakeColor = bodyColor;
    Color textColor(0, 0, 0);
    for (const auto& segment : segments) {
        graphics->PlotTile(segment.x, segment.y, 1, snakeColor, textColor, L'O');
    }
}

void Snake::OnCollision(GameObject* other)
{
    if (!active) {
        return;
    }

    // Handle apple collision
    if (Apple* apple = dynamic_cast<Apple*>(other)) {
        std::cout << "Collision with apple detected. Growing snake.\n";
        Grow();
        if (onAppleEaten) {
            onAppleEaten();
        }
        return;
    }

    // Handle wall collision
    if (Wall* wall = dynamic_cast<Wall*>(other)) {
        std::cout << "Game Over triggered: Collision with wall.\n";
        hasCollidedWithWall = true;
        return;
    }
}


void Snake::ChangeDirection(Direction newDir)
{
    // Prevent 180-degree reversals
    if ((currentDirection == Direction::Up    && newDir == Direction::Down) ||
        (currentDirection == Direction::Down  && newDir == Direction::Up)   ||
        (currentDirection == Direction::Left  && newDir == Direction::Right)||
        (currentDirection == Direction::Right && newDir == Direction::Left))
    {
        return;
    }
    
    currentDirection = newDir;
    //std::cout << "Snake direction changed.\n";
}

void Snake::Grow()
{
    pendingGrowth++;
    std::cout << "Snake will grow. Pending segments: " << pendingGrowth << "\n";
}

bool Snake::HasSelfCollision() const
{
    // If the head's position matches any other segment, that's a self-collision.
    const Vec2& head = segments.front();
    for (size_t i = 1; i < segments.size(); ++i) {
        if (segments[i].x == head.x && segments[i].y == head.y) {
            std::cout << "Self-collision detected.\n";
            return true;
        }
    }
    return false;
}

const std::deque<Vec2>& Snake::GetSegments() const
{
    return segments;
}
