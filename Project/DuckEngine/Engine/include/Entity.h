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
	int EntityID;
	Entity(int id) : EntityID(id) {}
};
