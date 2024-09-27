#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "json.hpp"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>

using json = nlohmann::json;

struct Window_init {
	std::string title = "";
	GLint width = 0 ;
	GLint height = 0 ;

}; 

extern Window_init W_init;

class DUCKENGINE_API Serialization {

	public:

		static void Init_json();

	private:

		static json jsonData;
		
};

#endif // SERIALIZATION_H