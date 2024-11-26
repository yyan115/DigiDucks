#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "StockLogic.h"

class HoldingLogic : public GameLogic
{
private:
    Entity* holder;
    TransformComponent* holderTransform;
    Entity* object;
	TransformComponent* objectTransform;
	IngredientType type;

public:
    Vec2 offSet{ 0.f, 1.5f };

    HoldingLogic() :
        GameLogic(nullptr), holder(nullptr), holderTransform(nullptr), object(nullptr), objectTransform(nullptr), type(IngredientType::EMPTY) {}

    HoldingLogic(GameLogicComponent* component, Entity* holder_) :
        GameLogic(nullptr), holder(nullptr), holderTransform(nullptr), object(nullptr), objectTransform(nullptr), type(IngredientType::EMPTY) {}
    
    std::shared_ptr<GameLogic> Clone() const override
    {
        auto clone = std::make_shared<HoldingLogic>(*this);
        clone->component = nullptr;
        return clone;
    }

    void Start() override;
    void Update() override;
    void FixedUpdate() override;

    void setObject(std::pair<int, IngredientType> objData);

	void deleteObject();

    std::pair<int, IngredientType> moveObject();
};