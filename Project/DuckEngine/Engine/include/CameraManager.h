#pragma once

#include "Matrix3x3.h"
#include "Vector2.h"

class CameraManager {
public:
	CameraManager();

	// Set and Get camera position
	void SetPosition(const Vector2D& position);
	static inline Vector2D GetPosition() { return position; };
	static inline float GetAR() { return windowAspectRatio; };
	static inline float GetHeight() { return cameraHeight; };

	// Set and Get zoom level
	void SetZoom(float zoom);
	inline float GetZoom() const { return zoom; };

	// Get view matrix (camera translation and zoom)
	inline static Matrix3x3 GetCameraToNDCMatrix() { return CameraToNDCTransform; };

	inline CameraManager* GetCamera() const { return cameraPtr; };

	// Update camera (if you need to do anything like interpolation)
	void Update();
private:
	static Vector2D position;  // Camera's position in world space
	float zoom;          // Zoom level (default: 1.0 = no zoom)

	CameraManager* cameraPtr; // pointer to game object that embeds camera

	Vector2D right, up;

	static Matrix3x3 viewTransform;
	static Matrix3x3 CameraToNDCTransform;
	Matrix3x3 WorldToNDCTransform;

	// window parameters ...
	static int cameraHeight;
	static float windowAspectRatio;

	// MINIMUM ZOOM IN AND MAXIMUM ZOOM OUT HEIGHT. COMMENTED OUT FOR NOW.
	//int min_height{ 500 }, max_height{ 2000 };
};