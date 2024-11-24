#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"

class MovementLogic : public GameLogic
{
private:
    TransformComponent* transform;
    RigidbodyComponent* rigidbody;
    AnimatorComponent* animator;
	BoundingCircle* circleCollider;
    float moveSpeed;

public:
    MovementLogic()
        : GameLogic(nullptr), transform(nullptr), rigidbody(nullptr), animator(nullptr), circleCollider(nullptr), moveSpeed(10.0f) {
    }

    MovementLogic(GameLogicComponent* component, float speed = 10.0f)
        : GameLogic(nullptr), transform(nullptr), rigidbody(nullptr), animator(nullptr), circleCollider(nullptr), moveSpeed(speed) {
    }

    void Start() override;
    void Update() override;
    void FixedUpdate() override;
};
