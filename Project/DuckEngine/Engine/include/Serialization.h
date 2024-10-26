/******************************************************************************/
/*
\file:		Serialization.h
\authors:	Tan Yan Kai, yankai.tan, 2301312 (100%)
\par:		yankai.tan@digipen.edu


\brief:     Contains the declarations that are used to read json files for initialization

 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/
#pragma once

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "json.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "Vector2.h"

typedef int GLint;
using json = nlohmann::json;
struct WindowInit
{
	std::string title = "";
	GLint width = 0;
	GLint height = 0;

};

struct GameObjectData
{
	Vec2 position;
	Vec2 velocity;
	Vec2 scale;
	bool isStatic;
};


extern WindowInit W_init;

class Serialization {

public:

	static void InitJson(const std::string& filePath);
	static json LoadJsonFile(const std::string& filePath);
	static Vec2 GetVec2(const json& j, const std::string& key, const Vec2& defaultValue = Vec2(0.0f, 0.0f));
	static WindowInit GetWindowInit();
	static void SaveJsonFile(const std::string& filePath, const nlohmann::json& data);

private:
	static json jsonData;
	static WindowInit windowInit;

};

#endif // SERIALIZATION_H