#pragma once
#include "GameObject.h"

class Apple : public GameObject {
public:
    Apple(const Vec2& pos);
    virtual ~Apple();

    void Update(float deltaTime) override;
    void Render(SnakeGraphics* graphics) override;
    void OnCollision(GameObject* other) override;
    
    virtual bool IsActive() const override { return active; }
    void Deactivate() { active = false; }

    static void ResetFirstAppleFlag();
    static bool isFirstAppleCreated;

private:
    bool active;
};
