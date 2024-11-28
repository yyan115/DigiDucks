#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"

enum FacingDirection
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

class MovementLogic : public GameLogic
{
private:
    TransformComponent* transform;
    RigidbodyComponent* rigidbody;
    AnimatorComponent* animator;
	BoundingCircle* circleCollider;
    float moveSpeed;

public:
    FacingDirection dir = FRONT;

    MovementLogic()
        : GameLogic(nullptr), transform(nullptr), rigidbody(nullptr), animator(nullptr), circleCollider(nullptr), moveSpeed(10.0f) {
    }

    MovementLogic(GameLogicComponent* component, float speed = 10.0f)
        : GameLogic(nullptr), transform(nullptr), rigidbody(nullptr), animator(nullptr), circleCollider(nullptr), moveSpeed(speed) {
    }

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<MovementLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void TestTest()
    {
        std::cout << "test successful" << std::endl;
    }
};
