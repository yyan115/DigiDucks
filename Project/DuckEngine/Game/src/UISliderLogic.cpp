#include "UISliderLogic.h"
#include <iostream>

/****************************************************************
* @brief Start function for the UI Slider Logic
****************************************************************/
void UISliderLogic::Start() {
	sliderEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	if (sliderEntity) {
		slider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(component->GetEntityID());

		// Ensure it's a UI slider before proceeding
		if (slider) {
			sliderBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(component->GetEntityID());

			// Get slider handle (child entity)
			if (!sliderEntity->childEntities.empty()) {
				sliderTrfm = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(sliderEntity->childEntities[1]->entityID);
				if (sliderTrfm) {
					originalPos = sliderTrfm->GetPosition();
					originalScale = sliderTrfm->scale;
					slider->sliderStartPos = originalPos;
				}
			}
		}
	}
}

/****************************************************************
* @brief Update function for the UI Slider Logic
****************************************************************/
void UISliderLogic::Update() {
	if (!slider || !slider->isEnable) return;  // Ensure the slider is enabled before updating

	HandleMouseDrag();

	// Compute progress (0.0 to 1.0)
	float progress = (slider->currentValue - slider->minValue) / (slider->maxValue - slider->minValue);

	// Update the slider handle position and scale based on progress
	if (sliderTrfm) {
		sliderTrfm->scale.x = originalScale.x * progress;  // Scale based on value
		sliderTrfm->SetPosition(Vec2(slider->sliderStartPos.x + (sliderTrfm->scale.x / 2.0f), sliderTrfm->GetPosition().y));
	}
}

/****************************************************************
* @brief Handle Mouse Dragging for the UI Slider
****************************************************************/
void UISliderLogic::HandleMouseDrag() {
	if (!slider) return;

	Vec2 mousePos(DuckEngine_Input::GetMouseX(), DuckEngine_Input::GetMouseY());
	//std::cout << "Mouse Pos: " << mousePos.x << ", " << mousePos.y << std::endl;
	// Detect if mouse clicks inside the slider bar
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {
		float leftBound = slider->sliderStartPos.x;
		float rightBound = slider->sliderStartPos.x + slider->sliderWidth;
		std::cout << "Left Bound: " << leftBound << " Right Bound: " << rightBound << std::endl;
		// Check if mouse is inside the slider bounds
		if (mousePos.x >= leftBound && mousePos.x <= rightBound) {
			std::cout << "Mouse is inside the slider bounds" << std::endl;
			slider->isDragging = true;
		}
	}

	// Update slider while dragging
	if (slider->isDragging) {
		float newValue = (mousePos.x - slider->sliderStartPos.x) / slider->sliderWidth;

		// Clamp within 0.0f - 1.0f
		if (newValue < 0.0f) newValue = 0.0f;
		if (newValue > 1.0f) newValue = 1.0f;

		slider->currentValue = slider->minValue + newValue * (slider->maxValue - slider->minValue);
	}

	// Stop dragging when mouse is released
	if (DuckEngine_Input::IsMouseButtonReleased(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {
		slider->isDragging = false;
	}
}

/****************************************************************
* @brief Reset Slider function for UI
****************************************************************/
void UISliderLogic::ResetSlider() {
	if (!slider || !sliderTrfm) return;

	slider->currentValue = slider->minValue;
	sliderTrfm->scale = originalScale;
	sliderTrfm->SetPosition(originalPos);
}
