/******************************************************************************/
/*!
\file       OrderTabLogic.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 6 2025
\brief      Implements the OrderTabLogic class, which handles dynamic order tab
			management for customer UI elements. This includes assigning orders
			to tabs, playing bounce animations, and clearing or restoring orders
			when customers leave.

			Tabs are animated from left to right across the screen, and orders
			are visually linked to their customers through textures and logic
			references.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


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

		// Modified to interpolate x-coordinate instead of y-coordinate
		float interpolatedX = (1 - t) * (-0.1f) + t * 0.1f + bounce;
		tab.transform->SetPosition(Vec2(interpolatedX, tab.startPosition.y));

		if (t >= 1.0f)
		{
			// Set final position to exactly 0.1 when animation is complete
			tab.transform->SetPosition(Vec2(0.1f, tab.startPosition.y));
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

	// add back later
	freeTab->spriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("order_" + whatType(order));

	freeTab->spriteRenderer->isVisible = true;
	freeTab->tabOrder = order;
	freeTab->tabCustomer = customer;

	freeTab->startPosition = freeTab->transform->GetPosition();
	// Save the y-position but target x-position is 0.1
	freeTab->targetPosition = Vec2(0.1f, freeTab->startPosition.y);
	// Start from x-position -0.1
	freeTab->transform->SetPosition(Vec2(-0.1f, freeTab->startPosition.y));

	freeTab->animationTime = 1.0f;
	freeTab->elapsedTime = 0.0f;
	freeTab->isAnimating = true;

	std::cout << "AddOrder animation started for entity: " << freeTab->entity->name << std::endl;
}

void OrderTabLogic::RemoveOrder(CustomerLogic* customer)
{
	if (!customer) return;

	std::vector<std::pair<ItemType, CustomerLogic*>> activeOrders;
	for (const auto& tab : orderTabs)
	{
		if (tab.tabCustomer && tab.tabCustomer != customer)
		{
			activeOrders.push_back(std::make_pair(tab.tabOrder, tab.tabCustomer));
		}
	}

	for (auto& tab : orderTabs)
	{
		tab.spriteRenderer->isVisible = false;
		tab.tabOrder = ItemType::EMPTY;
		tab.tabCustomer = nullptr;
		tab.isAnimating = false;
		tab.elapsedTime = 0.0f;
	}

	for (size_t i = 0; i < activeOrders.size(); i++)
	{
		if (i < orderTabs.size())
		{
			orderTabs[i].spriteRenderer->texture = DuckEngine::DUCKENGINE_AssetManager.GetTextureByName("order_" + whatType(activeOrders[i].first));
			orderTabs[i].spriteRenderer->isVisible = true;
			orderTabs[i].tabOrder = activeOrders[i].first;
			orderTabs[i].tabCustomer = activeOrders[i].second;
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