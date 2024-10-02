/******************************************************************************/
/*
\file:		Serialization.h
\authors:	Tan Yan Kai
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

/****************************************************************
* @brief Open and read json file data for windows initialization
*
* @param filepath - path to json file
*
***************************************************************/
static void InitJson(const std::string& filePath);

/****************************************************************
* @brief Load json file and read data in json file
*
* @param filePath - path to json file
*
* @return data in json file
***************************************************************/
static json LoadJsonFile(const std::string& filePath);

/****************************************************************
* @brief Overloaded operator + to add two matrices
*
* @param j - json library
*
* @param key - keyword from json file
*
* @param defaultValue - default value if json file does not contain any value
*
* @return Vec2(x,y)
*
* @return defaultValue
***************************************************************/
static Vec2 GetVec2(const json& j, const std::string& key, const Vec2& defaultValue = Vec2(0.0f, 0.0f));

/****************************************************************
* @brief Get windows initialization data
*
* @return windowInit
***************************************************************/
static WindowInit GetWindowInit();

private:
	static json jsonData;
	static WindowInit windowInit;

};

#endif // SERIALIZATION_H