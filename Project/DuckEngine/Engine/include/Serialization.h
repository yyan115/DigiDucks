#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

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
	GLint width = 0 ;
	GLint height = 0 ;

}; 

struct GameObjectData 
{
	Vec2 position;
	Vec2 velocity;
	Vec2 scale;
	bool isStatic;
};


extern WindowInit W_init;

class DUCKENGINE_API Serialization {

	public:
		Serialization();
		~Serialization();
		static void InitJson(const char* filePath);
		static json LoadJsonFile(const char* filePath);
		static Vec2 GetVec2(const json& j, const char* key, const Vec2& defaultValue = Vec2(0.0f, 0.0f));
		static WindowInit GetWindowInit();

	private:
		struct Impl;           // Forward declaration of the Impl struct
		static Impl* impl;      // Raw pointer to the implementation struct
	//private:
	//	static json jsonData;
	//	static WindowInit windowInit;
		
};

#endif // SERIALIZATION_H