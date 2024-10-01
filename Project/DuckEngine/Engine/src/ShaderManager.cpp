#include "ShaderManager.h"
#include "iostream"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

GLShader::GLShader() : programID(0) {
    // Constructor
}

GLShader::~GLShader() {
    DeleteProgram();
}

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

void GLShader::Use() const {
    glUseProgram(programID);
}

void GLShader::DeleteProgram() {
    if (programID != 0) {
        glDeleteProgram(programID);
        programID = 0;
    }
}

GLuint GLShader::GetProgram() const {
    return programID;
}

#include "ShaderManager.h"

// Initialize the static member
std::map<std::string, GLShader*> ShaderManager::shaders;

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

void ShaderManager::DeleteAllShaders() {
    for (auto& pair : shaders) {
        pair.second->DeleteProgram();
        delete pair.second;
    }
    shaders.clear();
}

void ShaderManager::Exit() {
    DeleteAllShaders();
}