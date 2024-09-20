#include "CameraManager.h"
#include "WindowManager.h"

Vector2D CameraManager::position;  // Camera's position in world space

// window parameters ...
int CameraManager::cameraHeight;
float CameraManager::windowAspectRatio;
//float CameraManager::zoom;

void CameraManager::Initialize(const float posX, const float posY, const int height) {
	CameraManager::position = { posX, posY };
	//CameraManager::zoom = zoom;
	CameraManager::cameraHeight = height;
	CameraManager::windowAspectRatio = WindowManager::GetWindowWidth() / WindowManager::GetWindowHeight();
}

// Set and Get camera position
void CameraManager::SetPosition(const float x, const float y) {
	CameraManager::position = { x, y };
}

//void CameraManager::SetZoom(const float zoom) {
//	CameraManager::zoom = zoom;
//}

void CameraManager::SetHeight(const int height) {
	CameraManager::cameraHeight = height;
}

void CameraManager::Update() {

}