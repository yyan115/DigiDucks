#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include "Matrix3x3.h"
#include "Vector2.h"

class DUCKENGINE_API CameraManager {
public:
	static void Initialize(const float posX, const float posY, const float zoom, const int height);

	// Set and Get camera position
	static void SetPosition(const float posX, const float posY);
	static void SetZoom(const float zoom);
	static void SetHeight(const int height);

	static inline Vector2D GetPosition() { return position; };
	static inline float GetAR() { return windowAspectRatio; };
	static inline float GetHeight() { return cameraHeight; };
	static inline float GetZoom() { return zoom; };

	void Update();

	//static inline Matrix3x3 GetCameraToNDCMatrix() { return CameraToNDCTransform; };

	//inline CameraManager* GetCamera() const { return cameraPtr; };
private:
	static Vector2D position;  // Camera's position in world space
	static float zoom;          // Zoom level (default: 1.0 = no zoom)

	// window parameters ...
	static int cameraHeight;
	static float windowAspectRatio;

	//CameraManager* cameraPtr; // pointer to game object that embeds camera

	//Vector2D right, up;

	//static Matrix3x3 viewTransform;
	//static Matrix3x3 CameraToNDCTransform;
	//Matrix3x3 WorldToNDCTransform;

	// MINIMUM ZOOM IN AND MAXIMUM ZOOM OUT HEIGHT. COMMENTED OUT FOR NOW.
	//int min_height{ 500 }, max_height{ 2000 };
};