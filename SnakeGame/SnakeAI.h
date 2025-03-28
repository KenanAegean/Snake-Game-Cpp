#pragma once
#include <vector>
#include "GameObject.h"
#include "Snake.h"
#include "World.h"

class SnakeAI {
public:
    static std::vector<Vec2> FindPath(const Vec2& start, const Vec2& goal, int gridColumns, int gridRows, World* world, Snake* aiSnake);
    static Direction GetDirection(const Vec2& start, const Vec2& next);
    static void UpdateAI(Snake* aiSnake, World* world, int gridColumns, int gridRows);
};
