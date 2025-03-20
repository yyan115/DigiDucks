#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "IngredientType.h"

class PotLogic : public GameLogic
{
private:
	Entity* potEntity = nullptr;
	SpriteRendererComponent* potSprite = nullptr;
	ItemType type = ItemType::POT;
	const int maxPortion = 3;
	int currPortion = 0;

public:
	// Boolean to check if there is soup in the pot
	bool isPotFilled = false;

	PotLogic() : GameLogic(nullptr) {}

	PotLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<PotLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	/****************************************************************
	* @brief Start function for the Pot Logic
	* ****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the Pot Logic
	* ****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief FixedUpdate function for the Pot Logic
	* ****************************************************************/
	void FixedUpdate() override;

	/****************************************************************
	* @brief Minus the portion of soup in the pot
	* 
	* @return - the type of soup
	* ****************************************************************/
	ItemType TakeSoup();

	ItemType getType() const { return type; }

	/****************************************************************
	* @brief Set the soup in the pot
	* 
	* @param soupType - the type of soup to be set
	* ****************************************************************/
	void SetSoup(ItemType soupType)
	{
		type = soupType;
		currPortion = maxPortion;
		isPotFilled = true;
	}

	/****************************************************************
	* @brief Empty the pot
	* ****************************************************************/
	void EmptyPot()
	{
		type = ItemType::POT;
		currPortion = 0;
		isPotFilled = false;
		if (potSprite)
		{
			potSprite->texture = AssetManager::GetTextureByName(whatType(type));
		}
	}

};