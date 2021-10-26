#ifndef ShaderProgram_HPP
#define ShaderProgram_HPP

#include "Uniform.hpp"

#include <GL/glew.h>

class ShaderProgram
{
    GLuint Handle; // id programu nadane przez GPU przy jego tworzeniu

public:
    ShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
    ~ShaderProgram();

    void UseOnCurrentThread();
    Uniform1Int* CreateUniform1Int(const char *name);
    Uniform2Int* CreateUniform2Int(const char *name);
private:
    void CheckShaderCompileStatus(GLuint handle);
    void CheckProgramLinkStatus(GLuint handle);
};

#endif // ShaderProgram_HPP
