#pragma once
#include <bitset>
#include <iostream>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif


class Entity
{
public:
    int entityID;
    std::string name;

    DUCKENGINE_API Entity(int id, std::string toName = "") : entityID(id), name(toName) {}

    DUCKENGINE_API std::string GetName()
    {
        return name;
    }

    DUCKENGINE_API bool IsName(const std::string& compareName) const
    {
        return name == compareName;
    }


};