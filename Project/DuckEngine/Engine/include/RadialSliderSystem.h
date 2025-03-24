#pragma once

#include "System.h"
#include "DuckEngine.h"
#include "RadialSliderComponent.h"
#include "Vector2.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API RadialSliderSystem : public System
{
public:
	// Core system methods
	void Start() override;
	void Update() override;
	void Render() override;

private:
	// Helper methods
	bool IsPointInRadialSlider(const Vector2D& point, const Vector2D& center, float radius) const;
	float CalculateAngleFromPoint(const Vector2D& center, const Vector2D& point) const;
	float NormalizeAngle(float angle) const;
	float ConvertAngleToValue(float angle, float minAngle, float maxAngle, float minValue, float maxValue) const;
	float ConvertValueToAngle(float value, float minValue, float maxValue, float minAngle, float maxAngle) const;
	float ClampValue(float value, float min, float max) const;

	// Handle slider interaction logic
	void HandleSliderInteraction(int entityId, RadialSliderComponent* slider, TransformComponent* transform);

	// Rendering helpers
	void DrawRadialSlider(const Vector2D& center,
		float radius,
		float thickness,
		float startAngle,
		float endAngle,
		float currentAngle,
		const Color& backgroundColor,
		const Color& fillColor,
		RadialSliderComponent* slider);
};