﻿#pragma once
#include "GameObject.h"
#include <vector>
#include <memory>

class World {
public:
    World();
    ~World();

    void Update(float deltaTime);
    void Render(SnakeGraphics* graphics);

    // Add a new game object.
    void AddGameObject(std::unique_ptr<GameObject> obj);
    
    // Remove a game object (you can implement by flagging objects for removal).
    void RemoveGameObject(GameObject* obj);

    // Check for collisions among objects.
    void CheckCollisions();

    // Spawn an apple at a random position.
    void SpawnApple(int maxColumns, int maxRows);

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
};
