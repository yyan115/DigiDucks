/******************************************************************************/
/*
\file:      Serialization.cpp
\authors:	Tan Yan Kai, yankai.tan, 2301312 (100%)
\par:	    yankai.tan@digipen.edu
 

\brief:     Contains the definitions that are used to read json files for initialization
 
 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/

#include "Serialization.h"

json Serialization::jsonData;
WindowInit Serialization::windowInit;

/****************************************************************
* @brief Open and read json file data for windows initialization
*
* @param filepath - path to json file
*
***************************************************************/
void Serialization::InitJson(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::cout << filePath << std::endl;
    if (!file.is_open())
    {
        std::cerr << "Could not open the file:" << std::endl;  // Print the full file path
        return;
    }
    file >> jsonData;
    file.close();

    // Extract window initialization data
    windowInit.title = jsonData.value("title", "Untitled Game");
    windowInit.width = jsonData.value("width", 800);
    windowInit.height = jsonData.value("height", 600);
}

/****************************************************************
* @brief Load json file and read data in json file
*
* @param filePath - path to json file
*
* @return data in json file
***************************************************************/
json Serialization::LoadJsonFile(const std::string& filePath)
{
    json data;
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Could not open the file: " << filePath << std::endl;
    }
    else
    {
        file >> data;
        file.close();
    }
    return data;
}

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
Vec2 Serialization::GetVec2(const json& j, const std::string& key, const Vec2& defaultValue)
{
    if (j.contains(key))
    {
        float x = j[key]["x"].get<float>();
        float y = j[key]["y"].get<float>();
        return Vec2(x, y);
    }
    return defaultValue;
}

/****************************************************************
* @brief Get windows initialization data
*
* @return windowInit
***************************************************************/
WindowInit Serialization::GetWindowInit()
{
    return windowInit;
}
