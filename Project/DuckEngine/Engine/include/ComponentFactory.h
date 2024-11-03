/******************************************************************************/
/*!
\file       ComponentFactory.h
\author     Jovan Chua 2301244
\par        c.shengkaijovan@digipen.edu
\date       October 3 2024
\brief      Declaration of the ComponentFactory class responsible for adding components to entities based on JSON data.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include <string>
#include "Serialization.h"
#include "DuckEngine.h" 

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief The ComponentFactory class responsible for adding components to entities based on JSON data.
       It reads component data from a JSON object and adds various components to the entities.
*************************************************************************/
class DUCKENGINE_API ComponentFactory
{
public:
    static void AddComponentsToEntity(Entity* entity, const nlohmann::json& componentsData);
    static void SaveComponentsToJson(int entityID, json& componentsArray);
    static std::shared_ptr<Component> CreateComponentFromJson(const nlohmann::json& componentJson);
};

