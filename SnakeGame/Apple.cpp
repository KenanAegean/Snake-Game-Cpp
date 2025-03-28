#include "Apple.h"
#include "SnakeGraphics.h"
#include "Snake.h"

bool Apple::isFirstAppleCreated = false;

Apple::Apple(const Vec2& pos)
    : GameObject(pos), active(true) {
    if (!isFirstAppleCreated) {
        Deactivate();
        isFirstAppleCreated = true;
    }
}

Apple::~Apple() {}

void Apple::Update(float deltaTime) {}

void Apple::Render(SnakeGraphics* graphics) {
    if (!active) return;
    Color appleColor(255, 0, 0);
    graphics->PlotTile(position.x, position.y, 1, appleColor, appleColor, L'@');
}

void Apple::OnCollision(GameObject* other) {
    Snake* snake = dynamic_cast<Snake*>(other);
    if (snake) {
        Deactivate();
    }
}

void Apple::ResetFirstAppleFlag() {
    isFirstAppleCreated = false;
}
