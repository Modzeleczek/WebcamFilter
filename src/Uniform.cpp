#include "../include/ShaderProgram.hpp"

#include <GL/glew.h>

Uniform::Uniform(const GLuint program, const char *name) :
    Location(glGetUniformLocation(program, name))
{}

Uniform1Int::Uniform1Int(const GLuint program, const char *name) :
    Uniform(program, name)
{}

void Uniform1Int::Set(const int newValue0)
{
    glUniform1i(Location, newValue0);
}

Uniform2Int::Uniform2Int(const GLuint program, const char *name) :
    Uniform1Int(program, name)
{}

void Uniform2Int::Set(const int newValue0, const int newValue1)
{
    glUniform2i(Location, newValue0, newValue1);
}
