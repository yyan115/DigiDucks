#pragma once

#include <string>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif


class DUCKENGINE_API LevelManager
{
public:
    static void LoadLevel(const std::string& levelFile);
};