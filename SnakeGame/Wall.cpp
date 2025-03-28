#include "Wall.h"
#include "SnakeGraphics.h"
#include <iostream>

Wall::Wall(const Vec2& pos)
    : GameObject(pos)
{
}
        
Wall::~Wall() {}

void Wall::Update(float deltaTime) {}

void Wall::Render(SnakeGraphics* graphics) {
    Color wallColor(128, 128, 128);
    graphics->PlotTile(position.x, position.y, 1, wallColor, wallColor, L'#');
}

void Wall::OnCollision(GameObject* other) {}