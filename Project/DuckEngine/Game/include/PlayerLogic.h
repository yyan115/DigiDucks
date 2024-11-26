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

    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<PlayerLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

    void Start() override;
    void Update() override;
    void FixedUpdate() override;

    void InteractPressed();

	void InteractHold();

    Entity* PlayerLogic::makeObject(IngredientType type);
    
};