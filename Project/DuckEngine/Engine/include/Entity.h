#pragma once
#include <bitset>
#include <iostream>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif


class DUCKENGINE_API Entity
{
public:
    int entityID;
    std::string name;

    Entity(int id, std::string toName = "") : entityID(id), name(toName) {}
    
    std::string GetName()
    {
        return name;
    }

    bool IsName(const std::string& compareName) const
    {
        return name == compareName;
    }


};