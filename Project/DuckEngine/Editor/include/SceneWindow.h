#pragma once

#include "imgui.h"

typedef unsigned int GLuint;

class SceneWindow
{
public:
	static void Initialize();
	static void RenderSceneWindow(int width, int height);
	static Vector2D ConvertScreenToWorld();

private:
	static int width;
	static int height;
};