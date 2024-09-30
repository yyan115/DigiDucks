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
    Entity(int id, const char* name = "");

    const char* GetName() const;
    void SetName(const char* name);
    bool IsName(const char* compareName) const;
    void Release();

    int entityID;

private:
    struct Impl;
    Impl* impl;
};