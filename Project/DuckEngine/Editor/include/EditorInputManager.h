#pragma once

class EditorInputManager
{
public:
	static void Initialize();
	static void Update();
	static void HandleScrollInput(double offsetY);
	static void HandleMouseDrag(double deltaX, double deltaY);

private:
	static bool isDragging;

};