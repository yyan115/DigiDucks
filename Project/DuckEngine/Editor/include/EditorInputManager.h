#pragma once
#include "DuckEngine.h"

class EditorInputManager
{
public:
	static void Initialize();
	static void Update();
	static void HandleScrollInput(double offsetY);
	static void HandleMouseDrag(double deltaX, double deltaY);
	static void SetIsDragging(bool enable) { isDragging = enable; }

private:
	static bool isDragging;

};