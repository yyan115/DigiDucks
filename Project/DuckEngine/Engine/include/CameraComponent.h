#pragma once
#include "Component.h"
#include "Matrix3x3.h"
#include "Vector2.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API CameraComponent : public Component
{
public:
	// Camera's position in world space
	Vector2D position;

	// Camera's zoom in/out distance
	float zoom;

	// Height is used to change how much the camera will show - THIS IS NEEDED
	int cameraHeight;

	// Used to not skew camera (according to window width and height) - THIS IS NEEDED
	float windowAspectRatio;

	// MINIMUM ZOOM IN AND MAXIMUM ZOOM OUT HEIGHT. Removed for now, reimplement feature if necessary
	//int min_height{ 500 }, max_height{ 2000 };

	// VIEW TRANSFORM USED TO TRANSFORM EVERY WORLD OBJ TO CAMERA, removed for the renderer to calculate
	//Matrix3x3 viewTransform;

	// CAMERA TO NDC FOR RENDERING, removed for the renderer to calculate
	//Matrix3x3 CameraToNDCTransform;

	// THESE TWO ARE USED FOR IF YOU WANT TO TAKE CAMERA'S ROTATION INTO CONSIDERATION WHEN RENDERING, removed as feature isn't necessary for now
	//Vector2D right, up;
	//Matrix3x3 WorldToNDCTransform;
};