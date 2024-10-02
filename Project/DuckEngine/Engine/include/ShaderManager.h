#pragma once

#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vector2.h"

#include "Color.h"

class GLShader {
public:
    GLShader();
    ~GLShader();

    // Compiles the vertex and fragment shaders from files
    bool CompileShaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

    // Uses the compiled shader program
    void Use() const;

    // Deletes the shader program
    void DeleteProgram();

    // Gets the program handle
    GLuint GetProgram() const;

private:
    // Compiles a shader of a given type from source code
    GLuint CompileShader(GLenum shaderType, const std::string& source);

    // Reads the shader code from a file
    std::string ReadShaderCode(const std::string& fileName);

    // Checks for compilation errors
    bool CheckCompileErrors(GLuint shader, const std::string& type);

    // Shader program handle
    GLuint programID;

};

class ShaderManager {
public:
    // Inserts a shader program into the manager
    static bool InsertShader(const std::string& shaderName, const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

    // Retrieves a shader program by name
    static GLShader* GetShader(const std::string& shaderName);

    // Deletes all shader programs
    static void DeleteAllShaders();

    static void Exit();

private:
    // Map of shader programs by their names
    static std::map<std::string, GLShader*> shaders;
};