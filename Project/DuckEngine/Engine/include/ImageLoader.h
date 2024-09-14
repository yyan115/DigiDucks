#pragma once


#include <string>
#include <GL/glew.h>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API ImageLoader
{
public:
    static GLuint LoadTexture(const std::string& filePath);
};

