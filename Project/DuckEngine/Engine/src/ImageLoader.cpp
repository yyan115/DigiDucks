/******************************************************************************/
/*!
\file       ImageLoader.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the ImageLoader class, which provides functionality
            for loading textures and sprite sheets from image files. It uses
            the STB image library to load images and OpenGL to upload texture
            data.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ImageLoader.h"
#include <iostream>
#include "GL/glew.h"

/************************************************************************
@brief Loads a texture from the specified file path using the STB image library.
       The texture is then uploaded to OpenGL and returned as a texture ID.
@param filePath The path to the texture file.
@return The OpenGL texture ID for the loaded texture, or 0 if loading failed.
*************************************************************************/
GLuint ImageLoader::LoadTexture(const std::string& filePath)
{
    // Flip the image vertically before loading
    stbi_set_flip_vertically_on_load(true); // Add this line

    // Load the image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        std::cerr << "STB Reason: " << stbi_failure_reason() << std::endl;
        return 0;
    }

    // Generate and bind a new texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Determine format based on number of channels
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    else {
        std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
        stbi_image_free(data);
        glDeleteTextures(1, &textureID);
        return 0;
    }

    // Upload the image data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

/************************************************************************
@brief Loads a sprite sheet from the specified file path, splits it into
       individual sprites based on the given width and height, and uploads
       them to OpenGL as separate textures.
@param filePath The path to the sprite sheet file.
@param spriteWidth The width of each individual sprite in the sheet.
@param spriteHeight The height of each individual sprite in the sheet.
@return A vector of OpenGL texture IDs for each sprite, or an empty vector if
        loading failed.
*************************************************************************/
std::vector<GLuint> ImageLoader::LoadSpriteSheet(const std::string& filePath,
    int spriteWidth,
    int spriteHeight)
{

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Failed to load sprite sheet: " << filePath << std::endl;
        std::cerr << "STB Reason: " << stbi_failure_reason() << std::endl;
        return {};
    }

    // Calculate how many columns fit (should be 3).
    int numSpritesX = width / spriteWidth;

    // Force only one row, ignoring the rest
    int numSpritesY = 1;

    // (Optional) check leftover
    int leftoverX = width % spriteWidth;
    int leftoverY = height % spriteHeight;
    if (leftoverX != 0)
    {
        std::cerr << "Warning: Sprite sheet width has leftover "
            << leftoverX << " pixels not used.\n";
    }
    if (leftoverY < (height - spriteHeight))
    {
        std::cerr << "Warning: The sprite sheet is taller than 'spriteHeight'; "
            << "extra rows will be ignored.\n";
    }

    // Determine the OpenGL format
    GLenum format;
    if (nrChannels == 1)
    {
        format = GL_RED;
    }
    else if (nrChannels == 3)
    {
        format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
        format = GL_RGBA;
    }
    else
    {
        std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
        stbi_image_free(data);
        return {};
    }

    std::cout << "Sprite Sheet Dimensions: " << width << "x" << height
        << " | Frame Size: " << spriteWidth << "x" << spriteHeight << std::endl;
    std::cout << "Loading only: " << numSpritesX << "x" << numSpritesY
        << " = " << (numSpritesX * numSpritesY) << " frames total.\n";

    std::vector<GLuint> textures;
    textures.reserve(numSpritesX * numSpritesY);

    // Temporary buffer for one sub-sprite
    std::vector<unsigned char> spriteData(spriteWidth * spriteHeight * nrChannels);

    // Always load just the top row: y = 0
    for (int x = 0; x < numSpritesX; ++x)
    {
        // Copy from the "big" image to a sub-sprite buffer
        for (int row = 0; row < spriteHeight; ++row)
        {
            int srcRowOffset = (0 * spriteHeight + row) * width * nrChannels;
            int dstRowOffset = row * spriteWidth * nrChannels;

            memcpy(spriteData.data() + dstRowOffset,
                data + srcRowOffset + x * spriteWidth * nrChannels,
                spriteWidth * nrChannels);
        }

        // Generate OpenGL texture for the sub-sprite
        GLuint textureID = 0;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, format,
            spriteWidth, spriteHeight, 0,
            format, GL_UNSIGNED_BYTE, spriteData.data());

        glBindTexture(GL_TEXTURE_2D, 0);

        textures.push_back(textureID);

        std::cout << "Added texture ID: " << textureID
            << " for frame (" << x << ", 0)\n";
    }

    // Free the big image
    stbi_image_free(data);

    return textures;
}


float ImageLoader::GetTextureWidth(const std::string& filePath)
{
    int width, height, nrChannels;

    stbi_info(filePath.c_str(), &width, &height, &nrChannels);

    if (width == 0 || height == 0)
    {
        std::cerr << "Failed to get texture dimensions: " << filePath << std::endl;
        return 1.0f;
    }

    return static_cast<float>(width);
}

float ImageLoader::GetTextureHeight(const std::string& filePath)
{
    int width, height, nrChannels;

    stbi_info(filePath.c_str(), &width, &height, &nrChannels);

    if (width == 0 || height == 0)
    {
        std::cerr << "Failed to get texture dimensions: " << filePath << std::endl;
        return 1.0f;
    }

    return static_cast<float>(height);
}
