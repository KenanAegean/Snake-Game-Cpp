#include "Apple.h"
#include "SnakeGraphics.h"
#include "Snake.h" // For dynamic_cast

Apple::Apple(const Vec2& pos)
    : GameObject(pos), active(true) {}

Apple::~Apple() {}

void Apple::Update(float deltaTime) {
    // Apple doesn't require per-frame updates.
}

void Apple::Render(SnakeGraphics* graphics) {
    Color appleColor(255, 0, 0); // Red apple.
    // Draw the apple with order 1 to appear over the background.
    graphics->PlotTile(position.x, position.y, 1, appleColor, appleColor, L'@');
}

void Apple::OnCollision(GameObject* other) {
    // If the snake collides with the apple, deactivate the apple.
    Snake* snake = dynamic_cast<Snake*>(other);
    if (snake) {
        Deactivate();
    }
}
