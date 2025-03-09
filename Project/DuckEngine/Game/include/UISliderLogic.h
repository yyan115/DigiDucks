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
};
