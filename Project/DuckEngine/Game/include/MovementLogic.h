#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"

class MovementLogic : public GameLogic
{
private:
    TransformComponent* transform;
    RigidbodyComponent* rigidbody;
    AnimatorComponent* animator;
    float moveSpeed;

public:
    MovementLogic(GameLogicComponent* component, float speed = 10.0f)
        : GameLogic(component), transform(nullptr), rigidbody(nullptr), animator(nullptr), moveSpeed(speed) {
    }

    void Start() override;
    void Update() override;
};
