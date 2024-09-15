#pragma once

#include <string>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

typedef unsigned int GLuint;

class DUCKENGINE_API ImageLoader
{
public:
    static GLuint LoadTexture(const std::string& filePath);
};

