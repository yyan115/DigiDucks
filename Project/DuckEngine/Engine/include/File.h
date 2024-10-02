/******************************************************************************/
/*
\file:		File.h
\authors:	Tan Yan Kai
\par:		yankai.tan@digipen.edu


\brief:     Contains the declarations that are used to get file path and print file path

 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/

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
		
/****************************************************************
* @brief Gets current file path
* 
***************************************************************/
static std::string GetFilePath();

/****************************************************************
* @brief Prints current file path
*
***************************************************************/
static void PrintPath();


	};


#endif // FILE_H