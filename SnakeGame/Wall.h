#pragma once
#include "GameObject.h"

class Wall : public GameObject {
public:
    Wall(const Vec2& pos);
    virtual ~Wall();

    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void OnCollision(GameObject* other) override;
};