/******************************************************************************/
/*!
\file       ImageLoader.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the ImageLoader class, which provides static methods
            for loading textures and sprite sheets in the game engine.
            These methods allow for efficient texture management and
            loading of sprite sheets with multiple frames.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <string>
#include <vector>

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

typedef unsigned int GLuint;

/************************************************************************
@brief The ImageLoader class provides static methods for loading textures
       and sprite sheets from files. It supports loading single textures
       as well as splitting sprite sheets into individual frames.
*************************************************************************/
class DUCKENGINE_API ImageLoader
{
public:
    /************************************************************************
    @brief Loads a texture from the specified file path and returns the
           OpenGL texture ID for the loaded texture.
    @param filePath The path to the texture file.
    @return The OpenGL texture ID for the loaded texture.
    *************************************************************************/
    static GLuint LoadTexture(const std::string& filePath);

    /************************************************************************
    @brief Loads a sprite sheet from the specified file path, splits it
           into individual sprites based on the provided width and height,
           and returns a vector of OpenGL texture IDs for each sprite.
    @param filePath The path to the sprite sheet file.
    @param spriteWidth The width of each individual sprite in the sheet.
    @param spriteHeight The height of each individual sprite in the sheet.
    @return A vector of OpenGL texture IDs for each sprite in the sheet.
    *************************************************************************/
    static std::vector<GLuint> LoadSpriteSheet(const std::string& filePath, int spriteWidth, int spriteHeight);
    
    static float GetTextureWidth(const std::string& filePath);
    static float GetTextureHeight(const std::string& filePath);

};

