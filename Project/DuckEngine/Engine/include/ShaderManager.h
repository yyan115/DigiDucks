/******************************************************************************/
/*!
\file       ShaderManager.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Declares the ShaderManager class, responsible for managing the
            loading, compiling, and usage of shader programs within the game
            engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector2.h"
#include "Color.h"

/// <summary>
/// Manages the compilation, usage, and deletion of OpenGL shaders.
/// Provides functionality for compiling vertex and fragment shaders from files, using the shader program, and deleting it.
/// </summary>
class GLShader {
public:
    /// <summary>
    /// Constructor for the GLShader class. Initializes the shader program ID.
    /// </summary>
    GLShader();

    /// <summary>
    /// Destructor for the GLShader class. Cleans up the shader program resources.
    /// </summary>
    ~GLShader();

    /// <summary>
    /// Compiles the vertex and fragment shaders from the provided files and links them into a shader program.
    /// </summary>
    /// <param name="vertexShaderFile">Path to the vertex shader file.</param>
    /// <param name="fragmentShaderFile">Path to the fragment shader file.</param>
    /// <returns>Returns true if the shaders were compiled and linked successfully, false otherwise.</returns>
    bool CompileShaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

    /// <summary>
    /// Activates the shader program for use in the current OpenGL context.
    /// </summary>
    void Use() const;

    /// <summary>
    /// Deletes the shader program and frees associated resources.
    /// </summary>
    void DeleteProgram();

    /// <summary>
    /// Returns the handle of the compiled shader program.
    /// </summary>
    /// <returns>The shader program ID as a GLuint.</returns>
    GLuint GetProgram() const;

private:
    /// <summary>
    /// Compiles a shader of the specified type (vertex or fragment) from the given source code.
    /// </summary>
    /// <param name="shaderType">The type of shader (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).</param>
    /// <param name="source">The source code of the shader.</param>
    /// <returns>Returns the shader object ID as a GLuint if compilation is successful, 0 otherwise.</returns>
    GLuint CompileShader(GLenum shaderType, const std::string& source);

    /// <summary>
    /// Reads the shader code from a file and returns it as a string.
    /// </summary>
    /// <param name="fileName">Path to the shader file.</param>
    /// <returns>The shader code as a string.</returns>
    std::string ReadShaderCode(const std::string& fileName);

    /// <summary>
    /// Checks for compilation or linking errors in a shader or program.
    /// </summary>
    /// <param name="shader">The shader or program object to check.</param>
    /// <param name="type">The type of object being checked (e.g., "VERTEX", "FRAGMENT", "PROGRAM").</param>
    /// <returns>Returns true if there are no errors, false otherwise.</returns>
    bool CheckCompileErrors(GLuint shader, const std::string& type);

    /// <summary>
    /// The OpenGL shader program handle.
    /// </summary>
    GLuint programID;
};

/// <summary>
/// Manages multiple OpenGL shaders by name. Provides functionality to load, retrieve, and delete shader programs.
/// </summary>
class ShaderManager {
public:
    /// <summary>
    /// Inserts a new shader program into the manager by compiling the vertex and fragment shaders from files.
    /// </summary>
    /// <param name="shaderName">The name of the shader program.</param>
    /// <param name="vertexShaderFile">Path to the vertex shader file.</param>
    /// <param name="fragmentShaderFile">Path to the fragment shader file.</param>
    /// <returns>Returns true if the shader program was inserted successfully, false otherwise.</returns>
    static bool InsertShader(const std::string& shaderName, const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

    /// <summary>
    /// Retrieves a shader program by its name.
    /// </summary>
    /// <param name="shaderName">The name of the shader program to retrieve.</param>
    /// <returns>A pointer to the GLShader object, or nullptr if the shader is not found.</returns>
    static GLShader* GetShader(const std::string& shaderName);

    /// <summary>
    /// Deletes all shader programs managed by the ShaderManager.
    /// </summary>
    static void DeleteAllShaders();

    /// <summary>
    /// Cleans up resources and terminates the ShaderManager.
    /// </summary>
    static void Exit();

private:
    /// <summary>
    /// A map storing shader programs by their names.
    /// </summary>
    static std::map<std::string, GLShader*> shaders;
};
