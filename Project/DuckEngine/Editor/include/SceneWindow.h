#pragma once

#include "imgui.h"

typedef unsigned int GLuint;

class SceneWindow
{
public:
	SceneWindow(DuckEngine& engine);
	void RenderSceneWindow(int width, int height);

private:
	DuckEngine& engine;
};