#pragma once

#include "imgui.h"

typedef unsigned int GLuint;

class SceneWindow
{
public:
	SceneWindow(DuckEngine& engine, int width, int height);
	void Initialize();
	void RenderSceneWindow(int width, int height);

private:
	DuckEngine& engine;
	int width;
	int height;
};