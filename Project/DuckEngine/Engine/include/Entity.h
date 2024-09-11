#pragma once
#include <bitset>
#include <iostream>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

const size_t MAX_COMPONENTS = 100;

class DUCKENGINE_API Entity
{
public:
    int EntityID;
    std::bitset<MAX_COMPONENTS> ComponentMask;

    Entity(int id, std::bitset<MAX_COMPONENTS> mask = std::bitset<MAX_COMPONENTS>())
        : EntityID(id), ComponentMask(mask) {}
};