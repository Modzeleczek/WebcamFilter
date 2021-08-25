#ifndef Processor_HPP
#define Processor_HPP

#include "IBuffer.hpp"
#include "ISource.hpp"
#include "ITarget.hpp"

#include <stdint.h>

class Processor // klasa abstrakcyjna
{
protected:
    uint8_t *const Input;
    const int Width, Height;

    Processor(IBuffer *source);
    virtual ~Processor();

public:
    virtual void ProcessFrame() = 0;
};

#endif // Processor_HPP
