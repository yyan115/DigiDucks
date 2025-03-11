/******************************************************************************/
/*!
\file     UISliderLogic.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the UISliderLogic class
		  which is responsible for handling the logic of the UI slider component.
		  The class is responsible for updating the slider handle position and scale
		  based on the slider's current value.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "SliderComponent.h"

class UISliderLogic : public GameLogic {
private:
	Entity* sliderEntity = nullptr;
	SliderComponent* slider = nullptr;
	SpriteRendererComponent* sliderBgSpt = nullptr;
	TransformComponent* sliderTrfm = nullptr;
	TransformComponent* MainsliderTrfm = nullptr;
	Vec2 originalPos{};
	Vec2 originalScale{};
	bool isDragging = false; // Track if the slider is being dragged

public:
	UISliderLogic() : GameLogic(nullptr) {}

	UISliderLogic(GameLogicComponent* component) : GameLogic(component) {}

	std::shared_ptr<GameLogic> Clone() const override {
		auto clone = std::make_shared<UISliderLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	/****************************************************************
	* @brief Start function for the UI Slider Logic
	****************************************************************/
	void Start() override;

	/****************************************************************
	* @brief Update function for the UI Slider Logic
	****************************************************************/
	void Update() override;

	/****************************************************************
	* @brief Handle Mouse Dragging for the UI Slider
	****************************************************************/
	void HandleMouseDrag();

	/****************************************************************
	* @brief Reset Slider function for UI
	****************************************************************/
	void ResetSlider();

	/****************************************************************
	* @brief Enable/Disable Slider Interaction
	****************************************************************/
	void EnableSlider(bool state) {
		slider->isEnable = state;
	}

	/****************************************************************
	* @brief Convert Screen Mouse Position to World/UI Space
	****************************************************************/
	Vec2 GetNormalizedMousePosition();

	/****************************************************************
	* @brief Check if the mouse is inside the slider bounds
	* @param mousePos The current mouse position

	****************************************************************/
	bool IsMouseInsideSlider(const Vec2& mousePos);
};
