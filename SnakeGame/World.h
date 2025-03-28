#pragma once
#include "GameObject.h"
#include <vector>
#include <memory>
#include <string>

class World {
public:
    World();
    ~World();

    void Update(float deltaTime);
    void Render(SnakeGraphics* graphics);
    void AddGameObject(std::unique_ptr<GameObject> obj);
    void RemoveGameObject(GameObject* obj);
    void CheckCollisions();
    void SpawnApple(int maxColumns, int maxRows);
    void LoadLevel(const std::string& filePath);
    void ClearWalls();

    Vec2 GetApplePosition() const;
    
    bool IsOccupied(int x, int y) const;
    bool IsWall(int x, int y) const;


private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
};