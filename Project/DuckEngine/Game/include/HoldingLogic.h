#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "PlayerLogic.h"

class HoldingLogic : public GameLogic
{
private:
    TransformComponent* transform;
	SpriteRendererComponent* spriteRenderer;
    Entity* holder;
    TransformComponent* holderTransform;
    Vec2 offSet;

public:
    HoldingLogic() :
        GameLogic(nullptr), transform(nullptr), spriteRenderer(nullptr), holder(nullptr), holderTransform(nullptr), offSet(0.f, 1.5f) {}

    HoldingLogic(GameLogicComponent* component, Entity* holder_, Vec2 offSet_ = Vec2{ 0.f, 1.5f }) :
        GameLogic(nullptr), transform(nullptr), spriteRenderer(nullptr), holder(holder_), holderTransform(nullptr), offSet(offSet_) {}

    void Start() override;
    void Update() override;
    void FixedUpdate() override;

	void changeIngredient(IngredientType type);
};