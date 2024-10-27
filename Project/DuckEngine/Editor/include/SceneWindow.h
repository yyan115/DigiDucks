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
	static bool GetIsPlaying() { return isPlaying;  }

private:
	static bool inSceneFBO;
	static bool isPlaying;
	static int width;
	static int height;
};