#include "ShaderManager.h"
#include "iostream"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::map<std::string, GLShader*> ShaderManager::shaders;

/// <summary>
/// Constructor for the GLShader class. Initializes the shader program ID to 0.
/// </summary>
GLShader::GLShader() : programID(0) {
    // Constructor
}

/// <summary>
/// Destructor for the GLShader class. Deletes the shader program and frees associated resources.
/// </summary>
GLShader::~GLShader() {
    DeleteProgram();
}

/// <summary>
/// Compiles the vertex and fragment shaders from the specified files, links them into a shader program,
/// and checks for compilation and linking errors.
/// </summary>
/// <param name="vertexShaderFile">Path to the vertex shader file.</param>
/// <param name="fragmentShaderFile">Path to the fragment shader file.</param>
/// <returns>Returns true if the shaders were compiled and linked successfully, false otherwise.</returns>
bool GLShader::CompileShaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    std::string vertexCode = ReadShaderCode(vertexShaderFile);
    std::string fragmentCode = ReadShaderCode(fragmentShaderFile);

    if (vertexCode.empty() || fragmentCode.empty()) {
        return false;
    }

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    if (vertexShader == 0 || fragmentShader == 0) {
        return false;
    }

    // Create the shader program
    programID = glCreateProgram();
    if (programID == 0) {
        std::cerr << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n";
        return false;
    }

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    // Link the program
    glLinkProgram(programID);

    // Check for linking errors
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        return false;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

/// <summary>
/// Compiles a shader of the specified type (vertex or fragment) from the given source code.
/// </summary>
/// <param name="shaderType">The type of shader (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).</param>
/// <param name="source">The source code of the shader.</param>
/// <returns>Returns the shader object ID as a GLuint if compilation is successful, 0 otherwise.</returns>
GLuint GLShader::CompileShader(GLenum shaderType, const std::string& source) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        std::cerr << "ERROR::SHADER::SHADER_CREATION_FAILED\n";
        return 0;
    }

    const GLchar* shaderCode = source.c_str();
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    if (!CheckCompileErrors(shader, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")) {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

/// <summary>
/// Reads the shader code from a file and returns it as a string.
/// </summary>
/// <param name="fileName">Path to the shader file.</param>
/// <returns>The shader code as a string. If the file could not be opened, returns an empty string.</returns>
std::string GLShader::ReadShaderCode(const std::string& fileName) {
    std::ifstream shaderFile;
    shaderFile.open(fileName);
    if (!shaderFile.is_open()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << fileName << "\n";
        return "";
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return shaderStream.str();
}

/// <summary>
/// Checks for compilation or linking errors in a shader or program and logs the error details.
/// </summary>
/// <param name="shader">The shader or program object to check.</param>
/// <param name="type">The type of object being checked (e.g., "VERTEX", "FRAGMENT", "PROGRAM").</param>
/// <returns>Returns true if there are no errors, false otherwise.</returns>
bool GLShader::CheckCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[512];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
            return false;
        }
    }
    return true;
}

/// <summary>
/// Activates the shader program for use in the current OpenGL context.
/// </summary>
void GLShader::Use() const {
    glUseProgram(programID);
}

/// <summary>
/// Deletes the shader program and frees associated resources.
/// </summary>
void GLShader::DeleteProgram() {
    if (programID != 0) {
        glDeleteProgram(programID);
        programID = 0;
    }
}

/// <summary>
/// Returns the handle of the compiled shader program.
/// </summary>
/// <returns>The shader program ID as a GLuint.</returns>
GLuint GLShader::GetProgram() const {
    return programID;
}

/// <summary>
/// Inserts a new shader program into the manager by compiling the vertex and fragment shaders from files.
/// </summary>
/// <param name="shaderName">The name of the shader program.</param>
/// <param name="vertexShaderFile">Path to the vertex shader file.</param>
/// <param name="fragmentShaderFile">Path to the fragment shader file.</param>
/// <returns>Returns true if the shader program was inserted successfully, false otherwise.</returns>
bool ShaderManager::InsertShader(const std::string& shaderName, const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    // Check if shader already exists
    if (shaders.find(shaderName) != shaders.end()) {
        std::cerr << "Shader with name '" << shaderName << "' already exists.\n";
        return false;
    }

    // Create a new shader
    GLShader* shader = new GLShader();
    if (!shader->CompileShaders(vertexShaderFile, fragmentShaderFile)) {
        std::cerr << "Failed to compile shader: " << shaderName << "\n";
        delete shader;
        return false;
    }

    // Insert the shader into the map
    shaders[shaderName] = shader;
    return true;
}

/// <summary>
/// Retrieves a shader program by its name.
/// </summary>
/// <param name="shaderName">The name of the shader program to retrieve.</param>
/// <returns>A pointer to the GLShader object, or nullptr if the shader is not found.</returns>
GLShader* ShaderManager::GetShader(const std::string& shaderName) {
    auto it = shaders.find(shaderName);
    if (it != shaders.end()) {
        return it->second;
    }
    else {
        std::cerr << "Shader with name '" << shaderName << "' not found.\n";
        return nullptr;
    }
}

/// <summary>
/// Deletes all shader programs managed by the ShaderManager and frees associated resources.
/// </summary>
void ShaderManager::DeleteAllShaders() {
    for (auto& pair : shaders) {
        pair.second->DeleteProgram();
        delete pair.second;
    }
    shaders.clear();
}

/// <summary>
/// Cleans up resources by deleting all shader programs and terminating the ShaderManager.
/// </summary>
void ShaderManager::Exit() {
    DeleteAllShaders();
}