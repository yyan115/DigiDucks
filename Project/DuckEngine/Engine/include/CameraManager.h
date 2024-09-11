#pragma once

#include "Matrix3x3.h"
#include "Vector2.h"

class Camera {
public:
	Camera();

	// Set and Get camera position
	void SetPosition(const Vector2D& position);
	inline Vector2D GetPosition() const { return position; };

	// Set and Get zoom level
	void SetZoom(float zoom);
	inline float GetZoom() const { return zoom; };

	// Get view matrix (camera translation and zoom)
	Matrix3x3 GetViewMatrix() const;

	inline Camera* GetCamera() const { return cameraPtr; };

	// Update camera (if you need to do anything like interpolation)
	void Update();
private:
	Vector2D position;  // Camera's position in world space
	float zoom;          // Zoom level (default: 1.0 = no zoom)

	Camera* cameraPtr; // pointer to game object that embeds camera

	Vector2D right, up;

	Matrix3x3 viewTransform;
	Matrix3x3 CameraToNDCTransform;
	Matrix3x3 WorldToNDCTransform;

	// window parameters ...
	int cameraHeight;
	float windowAspectRatio;

	// MINIMUM ZOOM IN AND MAXIMUM ZOOM OUT HEIGHT. COMMENTED OUT FOR NOW.
	//int min_height{ 500 }, max_height{ 2000 };
};