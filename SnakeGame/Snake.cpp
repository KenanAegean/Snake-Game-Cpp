#include "Snake.h"
#include "Apple.h"
#include "SnakeGraphics.h"
#include <iostream>

Snake::Snake(const Vec2& pos)
    : GameObject(pos), currentDirection(Direction::Right), speed(5.0f),
      movementAccumulator(0.0f), moveInterval(0.2f)
{
    segments.push_back(pos);
    std::cout << "Snake created at (" << pos.x << ", " << pos.y << ")\n";
}

Snake::~Snake() {}

void Snake::Update(float deltaTime)
{
    movementAccumulator += deltaTime;
    if (movementAccumulator < moveInterval)
        return;

    movementAccumulator -= moveInterval;

    // Calculate new head position.
    Vec2 newHead = segments.front();
    switch (currentDirection) {
        case Direction::Up:    newHead.y -= 1; break;
        case Direction::Down:  newHead.y += 1; break;
        case Direction::Left:  newHead.x -= 1; break;
        case Direction::Right: newHead.x += 1; break;
    }

    // Insert new head and remove the tail.
    segments.push_front(newHead);
    segments.pop_back();

    std::cout << "Snake moved. New head: (" << newHead.x << ", " << newHead.y << ")\n";
}

void Snake::Render(SnakeGraphics* graphics)
{
    Color snakeColor(0, 255, 0); // Green snake.
    Color textColor(0, 0, 0);     // Black text.
    for (const auto& segment : segments) {
        // Render with order 1 so snake appears over the background.
        graphics->PlotTile(segment.x, segment.y, 1, snakeColor, textColor, L'O');
    }
}

void Snake::OnCollision(GameObject* other)
{
    // Check if the collision is with an apple.
    Apple* apple = dynamic_cast<Apple*>(other);
    if (apple) {
        std::cout << "Collision with apple detected. Growing snake.\n";
        Grow();
        // Trigger score update via the callback.
        if (onAppleEaten) {
            onAppleEaten();
        }
    }
}

void Snake::ChangeDirection(Direction newDir)
{
    // Prevent the snake from directly reversing.
    if ((currentDirection == Direction::Up && newDir == Direction::Down) ||
        (currentDirection == Direction::Down && newDir == Direction::Up) ||
        (currentDirection == Direction::Left && newDir == Direction::Right) ||
        (currentDirection == Direction::Right && newDir == Direction::Left)) {
        return;
    }
    
    currentDirection = newDir;
    std::cout << "Snake direction changed.\n";
}

void Snake::Grow()
{
    // Duplicate the last segment.
    segments.push_back(segments.back());
    std::cout << "Snake grew. Length now: " << segments.size() << "\n";
}

bool Snake::HasSelfCollision() const
{
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
