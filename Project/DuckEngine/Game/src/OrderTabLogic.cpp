#include "OrderTabLogic.h"
#include "DuckEngine.h"
#include "SpriteRendererComponent.h"
#include <cmath>
#include <algorithm>

void OrderTabLogic::Start()
{
	for (int i = 1; ; ++i)
	{
		std::string entityName = "Order_Tab_" + std::to_string(i);
		auto entityPtr = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName(entityName);
		if (!entityPtr)
		{
			break;
		}

		OrderTabData tabData;
		tabData.entity = entityPtr.get();
		tabData.transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(tabData.entity->entityID);
		tabData.spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(tabData.entity->entityID);
		tabData.animationTime = 1.0f;
		tabData.elapsedTime = 0.0f;
		tabData.isAnimating = false;
		tabData.startPosition = Vec2(0.0f, 0.0f);
		tabData.targetPosition = Vec2(0.0f, 0.0f);
		tabData.bounceFactor = 0.2f;

		if (tabData.spriteRenderer)
		{
			tabData.spriteRenderer->isVisible = false;
		}

		orderTabs.push_back(tabData);
	}

	std::cout << "Loaded " << orderTabs.size() << " order tabs." << std::endl;
}

void OrderTabLogic::Update()
{
	float dt = DuckEngine::DeltaTime();
	for (auto& tab : orderTabs)
	{
		if (!tab.isAnimating)
		{
			continue;
		}

		tab.elapsedTime += dt;
		float t = tab.elapsedTime / tab.animationTime;
		if (t > 1.0f)
		{
			t = 1.0f;
		}

		float bounce = std::sin(t * 3.14159f) * tab.bounceFactor * (1.0f - t);

		float interpolatedY = (1 - t) * 0.25f + t * tab.targetPosition.y + bounce;
		tab.transform->SetPosition(Vec2(tab.startPosition.x, interpolatedY));

		if (t >= 1.0f)
		{
			tab.isAnimating = false;
			std::cout << "AddOrder animation completed for entity: "
				<< tab.entity->name << std::endl;
		}
	}
}

void OrderTabLogic::FixedUpdate()
{
}

void OrderTabLogic::AddOrder(ItemType order, CustomerLogic* customer)
{
	currentOrder = order;
	currentCustomer = customer;

	OrderTabData* freeTab = nullptr;
	for (auto& tab : orderTabs)
	{
		if (!tab.isAnimating && tab.tabCustomer == nullptr)
		{
			freeTab = &tab;
			break;
		}
	}

	if (!freeTab)
	{
		std::cerr << "No free order tab available!" << std::endl;
		return;
	}

	
	//if (order == ItemType::CHEESE_BURGER_PLATE)
	//{
	//	freeTab->spriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("Dish_1");
	//}
	//else if (order == ItemType::SALAD_PLATE)
	//{
	//	freeTab->spriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("Dish_2");
	//}
	//else if (order == ItemType::LETTUCE_PLATE)
	//{
	//	freeTab->spriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("Dish_Lettuce_Plate");
	//}
	//else
	//{
	//	std::cerr << "Unknown order ItemType, no texture set." << std::endl;
	//}

	freeTab->spriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("order_" + whatType(order));

	freeTab->spriteRenderer->isVisible = true;
	freeTab->tabOrder = order;
	freeTab->tabCustomer = customer;

	freeTab->startPosition = freeTab->transform->GetPosition();
	freeTab->targetPosition = Vec2(freeTab->startPosition.x, -0.175f);
	freeTab->transform->SetPosition(Vec2(freeTab->startPosition.x, 0.15f));

	freeTab->animationTime = 1.0f;
	freeTab->elapsedTime = 0.0f;
	freeTab->isAnimating = true;

	std::cout << "AddOrder animation started for entity: " << freeTab->entity->name << std::endl;
}

void OrderTabLogic::RemoveOrder(CustomerLogic* customer)
{
	if (!customer) return;

	for (auto& tab : orderTabs)
	{
		if (tab.tabCustomer == customer)
		{
			tab.spriteRenderer->isVisible = false;
			tab.tabOrder = ItemType::EMPTY;
			tab.tabCustomer = nullptr;
			tab.isAnimating = false;
			tab.elapsedTime = 0.0f;
			break;
		}
	}
}



bool OrderTabLogic::HasFreeTab() const
{
	for (auto& tab : orderTabs)
	{
		if (tab.tabCustomer == nullptr)
		{
			return true;
		}
	}
	return false;
}

int OrderTabLogic::GetUsedTabCount() const
{
	int used = 0;
	for (auto& tab : orderTabs)
	{
		if (tab.tabCustomer != nullptr)
		{
			used++;
		}
	}
	return used;
}

int OrderTabLogic::GetMaxTabCount() const
{
	return static_cast<int>(orderTabs.size());
}
