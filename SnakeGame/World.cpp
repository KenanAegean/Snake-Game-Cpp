#include "World.h"
#include "Apple.h"
#include "Snake.h"
#include "Wall.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>

World::World() {}

World::~World() {}

void World::Update(float deltaTime) {
    for (auto& obj : gameObjects) {
        obj->Update(deltaTime);
    }
    CheckCollisions();
    
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj->IsActive();
            }),
        gameObjects.end());
    
    bool appleExists = false;
    for (const auto& obj : gameObjects) {
        if (dynamic_cast<Apple*>(obj.get()) != nullptr) {
            appleExists = true;
            break;
        }
    }
    if (!appleExists) {
        SpawnApple(40, 30);
    }
}

void World::Render(SnakeGraphics* graphics) {
    for (auto& obj : gameObjects) {
        obj->Render(graphics);
    }
}

void World::AddGameObject(std::unique_ptr<GameObject> obj) {
    gameObjects.push_back(std::move(obj));
}

void World::RemoveGameObject(GameObject* obj) {}

void World::CheckCollisions() {
    for (size_t i = 0; i < gameObjects.size(); i++) {
        for (size_t j = i + 1; j < gameObjects.size(); j++) {
            if (gameObjects[i]->position.x == gameObjects[j]->position.x &&
                gameObjects[i]->position.y == gameObjects[j]->position.y) {
                gameObjects[i]->OnCollision(gameObjects[j].get());
                gameObjects[j]->OnCollision(gameObjects[i].get());
            }
        }
    }
}

void World::SpawnApple(int maxColumns, int maxRows) {
    bool validPosition = false;
    int attempts = 0;
    const int maxAttempts = 1000;
    int x = 0, y = 0;
    
    do {
        double rand1 = static_cast<double>(rand()) / RAND_MAX;
        double rand2 = static_cast<double>(rand()) / RAND_MAX;
        x = static_cast<int>(((rand1 + rand2) / 2.0) * maxColumns);

        rand1 = static_cast<double>(rand()) / RAND_MAX;
        rand2 = static_cast<double>(rand()) / RAND_MAX;
        y = static_cast<int>(((rand1 + rand2) / 2.0) * maxRows);

        validPosition = true;

        for (const auto& obj : gameObjects) {
            if (Snake* snake = dynamic_cast<Snake*>(obj.get())) {
                for (const auto& segment : snake->GetSegments()) {
                    if (segment.x == x && segment.y == y) {
                        validPosition = false;
                        break;
                    }
                }
            }
            if (Wall* wall = dynamic_cast<Wall*>(obj.get())) {
                if (wall->position.x == x && wall->position.y == y) {
                    validPosition = false;
                }
            }
            if (!validPosition)
                break;
        }
        attempts++;
    } while (!validPosition && attempts < maxAttempts);

    auto apple = std::make_unique<Apple>(Vec2{x, y});
    
    if (!apple->IsActive()) {
        SpawnApple(maxColumns, maxRows);
        return;
    }

    AddGameObject(std::move(apple));
}




void World::LoadLevel(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << filePath << std::endl;
        return;
    }
    std::string line;
    int y = 0;
    while (std::getline(file, line)) {
        for (size_t x = 0; x < line.size(); x++) {
            char c = line[x];
            if (c == '#') {
                // Create a wall at (x, y).
                AddGameObject(std::make_unique<Wall>(Vec2{ static_cast<int>(x), y }));
            }
        }
        y++;
    }
    file.close();
}

void World::ClearWalls() {
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return dynamic_cast<Wall*>(obj.get()) != nullptr;
            }),
        gameObjects.end());
    
    Apple::ResetFirstAppleFlag();
}

Vec2 World::GetApplePosition() const {
    for (const auto& obj : gameObjects) {
        Apple* apple = dynamic_cast<Apple*>(obj.get());
        if (apple && apple->IsActive()) {
            return apple->position;
        }
    }
    return Vec2{-1, -1};
}

bool World::IsOccupied(int x, int y) const {
    for (const auto& obj : gameObjects) {
        if (obj->position.x == x && obj->position.y == y) {
            if (dynamic_cast<Apple*>(obj.get()) != nullptr) {
                continue;
            }
            return true;
        }
    }
    return false;
}


bool World::IsWall(int x, int y) const {
    for (const auto& obj : gameObjects) {
        if (obj->position.x == x && obj->position.y == y) {
            if (dynamic_cast<Wall*>(obj.get()) != nullptr)
                return true;
        }
    }
    return false;
}
