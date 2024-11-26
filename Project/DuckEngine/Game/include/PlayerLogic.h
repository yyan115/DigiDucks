#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include <iostream>

#include "StockLogic.h"


class PlayerLogic : public GameLogic
{
private:
    BoundingCircle* circleCollider;
    BoundingBox* boxCollider;
    Entity* interactObject;
	bool isInteracting;
    bool isHolding;

public:
    PlayerLogic()
        : GameLogic(nullptr), circleCollider(nullptr), boxCollider(nullptr), interactObject(nullptr), isInteracting(false), isHolding(false) {}

    PlayerLogic(GameLogicComponent* component)
        : GameLogic(nullptr), circleCollider(nullptr), boxCollider(nullptr), interactObject(nullptr), isInteracting(false), isHolding(false) {}

    void Start() override;
    void Update() override;
    void FixedUpdate() override;

    void InteractPressed();

	void InteractHold();
};