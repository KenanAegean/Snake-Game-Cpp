#include "World.h"
#include "Apple.h"
#include "Snake.h"
#include "Wall.h"
#include <algorithm>
#include <cstdlib> // For rand()
#include <fstream>
#include <iostream>

World::World() {}

World::~World() {}

void World::Update(float deltaTime) {
    for (auto& obj : gameObjects) {
        obj->Update(deltaTime);
    }
    CheckCollisions();
    
    // Remove inactive objects (e.g., apples that have been eaten).
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj->IsActive();
            }),
        gameObjects.end());
    
    // Check if an apple exists. If not, spawn a new one.
    bool appleExists = false;
    for (const auto& obj : gameObjects) {
        if (dynamic_cast<Apple*>(obj.get()) != nullptr) {
            appleExists = true;
            break;
        }
    }
    if (!appleExists) {
        // Here, grid dimensions are assumed (e.g., 40 columns and 30 rows).
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

void World::RemoveGameObject(GameObject* obj) {
    // Removal logic can be implemented if needed.
}

void World::CheckCollisions() {
    // Simple collision detection: check if any two objects occupy the same tile.
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
    int x, y;
    bool validPosition = false;
    int attempts = 0;
    const int maxAttempts = 1000;
    
    // Attempt to find a position that is not occupied by any snake segment.
    do {
        x = rand() % maxColumns;
        y = rand() % maxRows;
        validPosition = true;
        
        // Check all game objects to see if a snake occupies the position.
        for (const auto& obj : gameObjects) {
            Snake* snake = dynamic_cast<Snake*>(obj.get());
            if (snake) {
                for (const auto& segment : snake->GetSegments()) {
                    if (segment.x == x && segment.y == y) {
                        validPosition = false;
                        break;
                    }
                }
            }
            if (!validPosition)
                break;
        }
        attempts++;
    } while (!validPosition && attempts < maxAttempts);
    
    // Add the apple even if a valid position wasn't found after many attempts.
    AddGameObject(std::make_unique<Apple>(Vec2{ x, y }));
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
