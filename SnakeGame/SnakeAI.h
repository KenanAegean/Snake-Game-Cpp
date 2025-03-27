#pragma once
#include <vector>
#include "GameObject.h"
#include "Snake.h"
#include "World.h"

class SnakeAI {
public:
    // Computes a path from start to goal using A*.
    // Returns a vector of positions from start to goal.
    // Now takes 6 parameters: extra parameter for the AI snake.
    static std::vector<Vec2> FindPath(const Vec2& start, const Vec2& goal, int gridColumns, int gridRows, World* world, Snake* aiSnake);

    // Computes the direction from the start cell to the next cell.
    static Direction GetDirection(const Vec2& start, const Vec2& next);

    // Update the AI-controlled snake's direction by planning a path toward the apple.
    static void UpdateAI(Snake* aiSnake, World* world, int gridColumns, int gridRows);
};
