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

public:
    bool isInteracting = false;
    bool isHolding = false;
	IngredientType currHolding = IngredientType::EMPTY;

    PlayerLogic()
        : GameLogic(nullptr), circleCollider(nullptr), boxCollider(nullptr), interactObject(nullptr) {}

    PlayerLogic(GameLogicComponent* component)
        : GameLogic(nullptr), circleCollider(nullptr), boxCollider(nullptr), interactObject(nullptr) {}

    void Start() override;
    void Update() override;
    void FixedUpdate() override;

    void InteractPressed();

	void InteractHold();
};