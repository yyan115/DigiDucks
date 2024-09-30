#pragma once
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <string.h>
#include <filesystem>
#include <algorithm>


	///	Simple wrapper for dealing with file paths, extensions, root name, etc.
	class DUCKENGINE_API FilePath
	{
	public:
		
		static std::string GetFilePath();
		static void PrintPath();
		static void SetPath();


	};


#endif // FILE_H