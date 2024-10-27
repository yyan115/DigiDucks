#pragma once

#include "imgui.h"

typedef unsigned int GLuint;

class SceneWindow
{
public:
	static void Initialize();
	static void RenderSceneWindow(int width, int height);
	static Vector2D ConvertScreenToWorld();
	static bool IsMouseInFBO();

	static bool GetInSceneFBO() { return inSceneFBO; }

private:
	static bool inSceneFBO;
	static int width;
	static int height;
};