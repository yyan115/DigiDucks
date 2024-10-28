/******************************************************************************/
/*!
\file       Entity.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the Entity class used in the game engine, representing
            an entity with a unique ID and optional name. It provides
            methods to retrieve and compare the entity's name.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/



#pragma once
#include <bitset>
#include <iostream>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief The Entity class represents an entity in the game engine, identified
       by a unique ID and optionally a name. It provides methods for
       retrieving and comparing the entity's name.
*************************************************************************/
class Entity
{
public:
    int entityID;
    std::string name;
    std::string prefabName;

    /************************************************************************
    @brief Constructor for the Entity class. Initializes the entity with a
        given ID and an optional name.
    @param id The unique identifier for the entity.
    @param toName Optional name for the entity (default is an empty string).
    *************************************************************************/
    DUCKENGINE_API Entity(int id, std::string toName = "") : entityID(id), name(toName) {}

    /************************************************************************
    @brief Checks if the entity's name matches a given name.
    @param compareName The name to compare with the entity's name.
    @return True if the names match, otherwise false.
    *************************************************************************/
    DUCKENGINE_API bool IsName(const std::string& compareName) const
    {
        return name == compareName;
    }


};