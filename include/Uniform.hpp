#ifndef Uniform_HPP
#define Uniform_HPP

#include <GL/glew.h>

class Uniform
{
protected:
    const int Location; // id uniforma w shader programie

public:
    Uniform(const Uniform &) = delete;
protected:
    Uniform(const GLuint program, const char *name); // chroniony konstruktor, aby nie dało się stworzyć instancji
};

class Uniform1Int : public Uniform
{
public:
    Uniform1Int(const GLuint program, const char *name);

    void Set(const int newValue0);
};

class Uniform2Int : public Uniform1Int
{
public:
    Uniform2Int(const GLuint program, const char *name);

    void Set(const int newValue0, const int newValue1);
};

#endif // Uniform_HPP
