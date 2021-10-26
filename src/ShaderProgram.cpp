#include "../include/ShaderProgram.hpp"
#include "../include/FileIO.hpp"

#include <GL/glew.h>
#include <stdexcept>

ShaderProgram::ShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath) // kompilujemy shadery i linkujemy je do programu
{
    // kod vertex shadera wykonywany dla każdego wierzchołka umieszczonego w buforze VBO
    char *vertexShaderCode = NULL;
    if (LoadText(vertexShaderPath, &vertexShaderCode) < 0)
        throw std::runtime_error(std::string("ShaderProgram::ShaderProgram; Failed to load vertex shader code from file ") + vertexShaderPath);
    // tworzymy nowy vertex shader w GPU i zapisujemy do niego uchwyt
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // przekazujemy kod do utworzonego shadera
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    free(vertexShaderCode);
    // kompilujemy shader
    glCompileShader(vertexShader);
    // sprawdzamy, czy kompilacja się powiodła
    CheckShaderCompileStatus(vertexShader);

    // kod fragment shadera wykonywany dla każdego piksela wyświetlanej figury (tutaj prostokąta)
    char *fragmentShaderCode = NULL;
    if (LoadText(fragmentShaderPath, &fragmentShaderCode) < 0)
        throw std::runtime_error(std::string("ShaderProgram::ShaderProgram; Failed to load fragment shader code from file ") + fragmentShaderPath);
    // tworzymy nowy fragment shader w GPU i zapisujemy do niego uchwyt
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // przekazujemy kod do utworzonego shadera
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    free(fragmentShaderCode);
    // kompilujemy shader
    glCompileShader(fragmentShader);
    // sprawdzamy, czy kompilacja się powiodła
    CheckShaderCompileStatus(fragmentShader);

    // tworzymy program, do którego linkujemy skompilowane shadery, i zapisujemy do niego uchwyt
    Handle = glCreateProgram();
    // sprawdzamy, czy program został prawidłowo stworzony
    if (Handle == 0)
        throw std::runtime_error("ShaderProgram::ShaderProgram; Failed to create shader program.");
    // przyłączamy vertex shader do programu
    glAttachShader(Handle, vertexShader);
    // przyłączamy fragment shader do programu
    glAttachShader(Handle, fragmentShader);
    // linkujemy przyłączone shadery
    glLinkProgram(Handle);
    CheckProgramLinkStatus(Handle);

    // usuwamy vertex shader po jego zlinkowaniu do programu
    glDeleteShader(vertexShader);
    // usuwamy fragment shader po jego zlinkowaniu do programu
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(Handle);
}

void ShaderProgram::UseOnCurrentThread()
{
    glUseProgram(Handle);
}

Uniform1Int* ShaderProgram::CreateUniform1Int(const char *name)
{
    return new Uniform1Int(Handle, name);
}

Uniform2Int* ShaderProgram::CreateUniform2Int(const char *name)
{
    return new Uniform2Int(Handle, name);
}

void ShaderProgram::CheckShaderCompileStatus(GLuint handle)
{
    int success;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE)
        return;
    char infoLog[512];
    glGetShaderInfoLog(handle, 512, NULL, infoLog);
    throw std::runtime_error(std::string("ShaderProgram::CheckShaderCompileStatus; Shader was not compiled successfully.\n") + infoLog);
}

void ShaderProgram::CheckProgramLinkStatus(GLuint handle)
{
    int success;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    if (success == GL_TRUE)
        return;
    char infoLog[512];
    glGetProgramInfoLog(handle, 512, NULL, infoLog);
    throw std::runtime_error(std::string("ShaderProgram::CheckProgramLinkStatus; Program was not linked successfully.\n") + infoLog);
}
