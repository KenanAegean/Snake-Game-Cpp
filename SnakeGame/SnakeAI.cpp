#include "SnakeAI.h"
#include "World.h"
#include "Wall.h"
#include "Snake.h"
#include <queue>
#include <cmath>
#include <algorithm>
#include <deque>

#include "Apple.h"

// Helper structure for A* nodes.
struct Node {
    int x, y;
    int g; // cost from start
    int h; // heuristic cost to goal
    int f; // total cost (g + h)
    int parentX, parentY;
    Node(int _x, int _y, int _g, int _h, int _parentX, int _parentY)
        : x(_x), y(_y), g(_g), h(_h), f(_g + _h), parentX(_parentX), parentY(_parentY) {}
};

// Comparison functor for the priority queue (lowest f cost first).
struct NodeComparator {
    bool operator()(const Node& a, const Node& b) const {
        return a.f > b.f;
    }
};

static int Heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// Compute a penalty cost for cell (x,y) if it's adjacent to a wall.
static int WallPenalty(int x, int y, int gridColumns, int gridRows, World* world) {
    int penalty = 0;
    int directions[4][2] = { {0,-1}, {0,1}, {-1,0}, {1,0} };
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        if (nx >= 0 && ny >= 0 && nx < gridColumns && ny < gridRows) {
            if (world->IsWall(nx, ny))
                penalty += 50; // increased penalty value
        }
    }
    return penalty;
}


// Modified IsWalkable that accepts the goal cell and the AI snake.
// It returns true if the cell is inside the grid, is either the goal,
// is not occupied by any non-apple object, and is not part of aiSnake's body.
static bool IsWalkable(int x, int y, int gridColumns, int gridRows, World* world, const Vec2& goal, Snake* aiSnake) {
    if (x < 0 || y < 0 || x >= gridColumns || y >= gridRows)
        return false;
    // Always allow the goal cell (even if it might normally be occupied)
    if (x == goal.x && y == goal.y)
        return true;
    // Explicitly reject wall cells.
    if (world->IsWall(x, y))
        return false;
    // Also reject cells that are occupied by other objects.
    if (world->IsOccupied(x, y))
        return false;
    // Prevent the AI from planning a route through its own body (except the head)
    const std::deque<Vec2>& segments = aiSnake->GetSegments();
    for (size_t i = 1; i < segments.size(); i++) {
        if (segments[i].x == x && segments[i].y == y)
            return false;
    }
    return true;
}

// Updated FindPath that now accepts the AI snake pointer.
std::vector<Vec2> SnakeAI::FindPath(const Vec2& start, const Vec2& goal, int gridColumns, int gridRows, World* world, Snake* aiSnake) {
    std::vector<Vec2> path;
    std::priority_queue<Node, std::vector<Node>, NodeComparator> openList;
    std::vector<std::vector<bool>> closed(gridColumns, std::vector<bool>(gridRows, false));
    std::vector<std::vector<std::pair<int,int>>> parent(gridColumns, std::vector<std::pair<int,int>>(gridRows, {-1,-1}));

    int startH = Heuristic(start.x, start.y, goal.x, goal.y);
    openList.push(Node(start.x, start.y, 0, startH, -1, -1));

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();
        if (closed[current.x][current.y])
            continue;
        closed[current.x][current.y] = true;
        parent[current.x][current.y] = { current.parentX, current.parentY };

        if (current.x == goal.x && current.y == goal.y) {
            int cx = current.x;
            int cy = current.y;
            while (!(cx == start.x && cy == start.y)) {
                path.push_back(Vec2{ cx, cy });
                auto pr = parent[cx][cy];
                cx = pr.first;
                cy = pr.second;
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            break;
        }

        int directions[4][2] = { {0,-1}, {0,1}, {-1,0}, {1,0} };
        for (int i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];
            if (!IsWalkable(nx, ny, gridColumns, gridRows, world, goal, aiSnake) || closed[nx][ny])
                continue;
            int penalty = WallPenalty(nx, ny, gridColumns, gridRows, world);
            int newG = current.g + 1 + penalty;
            int newH = Heuristic(nx, ny, goal.x, goal.y);
            openList.push(Node(nx, ny, newG, newH, current.x, current.y));
        }
    }
    return path;
}

Direction SnakeAI::GetDirection(const Vec2& start, const Vec2& next) {
    if (next.x > start.x) return Direction::Right;
    if (next.x < start.x) return Direction::Left;
    if (next.y > start.y) return Direction::Down;
    if (next.y < start.y) return Direction::Up;
    return Direction::Right; // Default case.
}

// Helper function to check if two directions are opposites.
static bool IsOpposite(Direction a, Direction b) {
    return (a == Direction::Up && b == Direction::Down) ||
           (a == Direction::Down && b == Direction::Up) ||
           (a == Direction::Left && b == Direction::Right) ||
           (a == Direction::Right && b == Direction::Left);
}

void SnakeAI::UpdateAI(Snake* aiSnake, World* world, int gridColumns, int gridRows) {
    // Skip AI target until the second apple is created
    if (!Apple::isFirstAppleCreated) {
        return; // AI should wait before targeting
    }

    // Retrieve the apple's position.
    Vec2 applePos = world->GetApplePosition();
    if (applePos.x == -1 && applePos.y == -1)
        return;
    
    Vec2 head = aiSnake->GetSegments().front();
    // Compute the path using A* (which avoids walls, snake's own body, etc.)
    std::vector<Vec2> path = FindPath(head, applePos, gridColumns, gridRows, world, aiSnake);
    
    Direction currentDir = aiSnake->GetCurrentDirection();
    Direction chosenDir = currentDir; // default to current direction
    
    if (path.size() >= 2) {
        chosenDir = GetDirection(head, path[1]);
        // If the candidate move is a U-turn relative to the current direction, look further into the path.
        if (IsOpposite(chosenDir, currentDir)) {
            bool foundAlternative = false;
            for (size_t i = 2; i < path.size(); i++) {
                Direction altDir = GetDirection(head, path[i]);
                if (!IsOpposite(altDir, currentDir)) {
                    chosenDir = altDir;
                    foundAlternative = true;
                    break;
                }
            }
            if (!foundAlternative)
                chosenDir = currentDir;
        }
    }
    
    // Check if the next cell in the chosen direction is safe.
    Vec2 next = head;
    switch (chosenDir) {
        case Direction::Up:    next.y -= 1; break;
        case Direction::Down:  next.y += 1; break;
        case Direction::Left:  next.x -= 1; break;
        case Direction::Right: next.x += 1; break;
    }
    
    if (world->IsOccupied(next.x, next.y) || world->IsWall(next.x, next.y)) {
        // Fallback: Evaluate all candidate directions (except the one that's a U-turn)
        std::vector<Direction> safeDirections;
        Direction directions[4] = { Direction::Up, Direction::Down, Direction::Left, Direction::Right };
        for (Direction d : directions) {
            if (IsOpposite(d, currentDir))
                continue;
            Vec2 cand = head;
            switch (d) {
                case Direction::Up:    cand.y -= 1; break;
                case Direction::Down:  cand.y += 1; break;
                case Direction::Left:  cand.x -= 1; break;
                case Direction::Right: cand.x += 1; break;
            }
            if (!(world->IsOccupied(cand.x, cand.y) || world->IsWall(cand.x, cand.y)))
                safeDirections.push_back(d);
        }
        if (!safeDirections.empty()) {
            // Choose the safe move that minimizes the Manhattan distance to the apple.
            Direction bestDir = safeDirections[0];
            int bestDist = INT_MAX;
            for (Direction d : safeDirections) {
                Vec2 cand = head;
                switch (d) {
                    case Direction::Up:    cand.y -= 1; break;
                    case Direction::Down:  cand.y += 1; break;
                    case Direction::Left:  cand.x -= 1; break;
                    case Direction::Right: cand.x += 1; break;
                }
                int dist = std::abs(cand.x - applePos.x) + std::abs(cand.y - applePos.y);
                if (dist < bestDist) {
                    bestDist = dist;
                    bestDir = d;
                }
            }
            chosenDir = bestDir;
        } else {
            chosenDir = currentDir;
        }
    }
    
    aiSnake->ChangeDirection(chosenDir);
}
