#include "Snake.h"
#include "Apple.h"
#include "Wall.h"  // For wall collision detection
#include "SnakeGraphics.h"
#include <iostream>

Snake::Snake(const Vec2& pos)
    : GameObject(pos),
      currentDirection(Direction::Right),
      speed(5.0f),
      movementAccumulator(0.0f),
      moveInterval(0.2f),
      pendingGrowth(0),
      active(true) // The snake starts active
{
    segments.push_back(pos);
    std::cout << "Snake created at (" << pos.x << ", " << pos.y << ")\n";
}

Snake::~Snake() {}

void Snake::Update(float deltaTime)
{
    // Skip updates if inactive (e.g., after wall collision).
    if (!active) {
        return;
    }

    // Accumulate time until we reach moveInterval.
    movementAccumulator += deltaTime;
    if (movementAccumulator < moveInterval) {
        return;
    }
    movementAccumulator -= moveInterval;

    // Calculate the new head position.
    Vec2 newHead = segments.front();
    switch (currentDirection) {
        case Direction::Up:    newHead.y -= 1; break;
        case Direction::Down:  newHead.y += 1; break;
        case Direction::Left:  newHead.x -= 1; break;
        case Direction::Right: newHead.x += 1; break;
    }

    // Insert new head at the front of the deque.
    segments.push_front(newHead);
    // Update our GameObject::position so collision checks see the correct tile.
    position = newHead;
    
    // If growth is pending, keep the tail to add length.
    if (pendingGrowth > 0) {
        pendingGrowth--;
    } else {
        // Otherwise, remove the tail segment (snake moves forward).
        segments.pop_back();
    }

    std::cout << "Snake moved. New head: (" << newHead.x << ", " << newHead.y << ")\n";
}

void Snake::Render(SnakeGraphics* graphics)
{
    // Skip rendering if inactive.
    if (!active) {
        return;
    }

    Color snakeColor(0, 255, 0); // Green
    Color textColor(0, 0, 0);    // Black
    for (const auto& segment : segments) {
        graphics->PlotTile(segment.x, segment.y, 1, snakeColor, textColor, L'O');
    }
}

void Snake::OnCollision(GameObject* other)
{
    if (!active) {
        return;
    }

    // Handle apple collision.
    if (Apple* apple = dynamic_cast<Apple*>(other)) {
        std::cout << "Collision with apple detected. Growing snake.\n";
        Grow();
        if (onAppleEaten) {
            onAppleEaten();
        }
        return;
    }

    // Handle wall collision.
    if (Wall* wall = dynamic_cast<Wall*>(other)) {
        std::cout << "Game Over triggered: Collision with wall.\n";
        hasCollidedWithWall = true;  // Set our dedicated flag.
        // Do not call onGameOver here; let GamePlayState::Update handle the transition.
        return;
    }
}


void Snake::ChangeDirection(Direction newDir)
{
    // Prevent 180-degree reversals.
    if ((currentDirection == Direction::Up    && newDir == Direction::Down) ||
        (currentDirection == Direction::Down  && newDir == Direction::Up)   ||
        (currentDirection == Direction::Left  && newDir == Direction::Right)||
        (currentDirection == Direction::Right && newDir == Direction::Left))
    {
        return;
    }
    
    currentDirection = newDir;
    std::cout << "Snake direction changed.\n";
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
