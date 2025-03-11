/******************************************************************************/
/*!
\file     UISliderLogic.cpp
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the implementation of the UISliderLogic class
		  which is responsible for handling the logic of the UI slider component.
		  The class is responsible for updating the slider handle position and scale
		  based on the slider's current value.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "UISliderLogic.h"
#include "DuckEngine.h"
#include <iostream>

/****************************************************************
* @brief Start function for the UI Slider Logic
****************************************************************/
void UISliderLogic::Start() {
	sliderEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(component->GetEntityID()).get();
	if (sliderEntity) {
		slider = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SliderComponent>(component->GetEntityID());
		MainsliderTrfm = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(sliderEntity->entityID);
		// Ensure it's a UI slider before proceeding
		if (slider) {
			sliderBgSpt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(component->GetEntityID());

			// Get slider handle (child entity)
			if (!sliderEntity->childEntities.empty()) {
				sliderTrfm = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(sliderEntity->childEntities[0]->entityID);
				if (sliderTrfm) {
					originalPos = sliderTrfm->GetPosition();
					originalScale = sliderTrfm->scale;
					slider->sliderStartPos = originalPos;
					std::cout << "Slider Start Pos: " << slider->sliderStartPos.x << ", " << slider->sliderStartPos.y << std::endl;
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
		// Calculate the leftmost position of the slider (base of the grey bar)
		float leftEdge = slider->sliderStartPos.x - (MainsliderTrfm->scale.x * 0.5f);

		// Set the new scale of the green bar based on progress
		sliderTrfm->scale.x = originalScale.x * progress;

		// Update position so the green bar grows from the left
		sliderTrfm->SetPosition(Vec2(leftEdge + (sliderTrfm->scale.x * 0.5f), sliderTrfm->GetPosition().y));

	}
}

/****************************************************************
* @brief Convert Screen Mouse Position to World/UI Space
****************************************************************/
Vec2 UISliderLogic::GetNormalizedMousePosition() {
	float mouseX = 0.0f;
	float mouseY = 0.0f;

	if (DuckEngine::GetEditorBool()) {
		// Get the mouse position in screen coordinates
		mouseX = DuckEngine::editorMouseScreenPos.x;
		mouseY = DuckEngine::editorMouseScreenPos.y;
	}
	else {
		mouseX = static_cast<float>(DuckEngine_Input::GetMouseX());
		mouseY = static_cast<float>(DuckEngine_Input::GetMouseY());
	}

	float screenWidth = static_cast<float>(DuckEngine::GetViewportWidth());
	float screenHeight = static_cast<float>(DuckEngine::GetViewportHeight());

	// Normalize X: (0 to 1)
	float normalizedX = mouseX / screenWidth;

	// Normalize Y: (0 to -1)
	float normalizedY = -(mouseY / screenHeight);

	return { normalizedX, normalizedY };
}

/****************************************************************
* @brief Check if the mouse is inside the slider bounds
****************************************************************/
bool UISliderLogic::IsMouseInsideSlider(const Vec2& mousePos) {
	Vec2 position = MainsliderTrfm->GetPosition();
	Vec2 scale = MainsliderTrfm->scale;
	Vec2 min = position - (scale * 0.5f);
	Vec2 max = position + (scale * 0.5f);

	return (mousePos.x >= min.x && mousePos.x <= max.x) &&
		(mousePos.y >= min.y && mousePos.y <= max.y);
}


/****************************************************************
* @brief Handle Mouse Dragging for the UI Slider
****************************************************************/
void UISliderLogic::HandleMouseDrag() {
	if (!slider) return;

	// Get normalized mouse position
	Vec2 mousePos = GetNormalizedMousePosition();
	

	// Check if the mouse is within the slider bounds
	if (DuckEngine_Input::IsMouseButtonPressed(DuckEngine_Input::MOUSE_BUTTON_LEFT)) {
		if (IsMouseInsideSlider(mousePos)) {
			std::cout << "Mouse is inside the slider bounds" << std::endl;
			slider->isDragging = true;
		}
	}

	// Update slider position while dragging
	if (slider->isDragging) {
		float leftEdge = slider->sliderStartPos.x - (MainsliderTrfm->scale.x * 0.5f);
		float newValue = (mousePos.x - leftEdge) / slider->sliderWidth;

		// Clamp within 0.0f - 1.0f
		if (newValue < 0.0f) {
			newValue = 0.0f;
		}
		else if (newValue > 1.0f) {
			newValue = 1.0f;
		}

		// Map newValue to the slider's actual value range
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
