#include "Wall.h"
#include "SnakeGraphics.h"
#include <iostream>

Wall::Wall(const Vec2& pos)
    : GameObject(pos)
{
}
        
Wall::~Wall() {}

void Wall::Update(float deltaTime) {
    // Walls are static and do not update.
}

void Wall::Render(SnakeGraphics* graphics) {
    // Render the wall as a grey block.
    Color wallColor(128, 128, 128); // grey
    graphics->PlotTile(position.x, position.y, 1, wallColor, wallColor, L'#');
}

void Wall::OnCollision(GameObject* other) {
    // Walls don't react to collisions.
    // You might handle collision effects (e.g., game over) in your snake's collision logic.
}
