#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include <iostream>

#include "StockLogic.h"
#include "TableLogic.h"
#include "HoldingLogic.h"


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
	Vec2 offSet = Vec2{ 0.f, 1.5f };

    PlayerLogic()
        : GameLogic(nullptr), circleCollider(nullptr), boxCollider(nullptr), interactObject(nullptr) {}

    PlayerLogic(GameLogicComponent* component)
        : GameLogic(nullptr), circleCollider(nullptr), boxCollider(nullptr), interactObject(nullptr) {}

    void Start() override;
    void Update() override;
    void FixedUpdate() override;

    void InteractPressed();

	void InteractHold();

    Entity* PlayerLogic::makeObject(IngredientType type);
    
};