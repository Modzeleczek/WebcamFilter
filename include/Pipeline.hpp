#ifndef Pipeline_HPP
#define Pipeline_HPP

#include "OpenGLContext.hpp"
#include "ISource.hpp"
#include "CPU.hpp"
#include "GPU.hpp"
#include "ITarget.hpp"

class Pipeline
{
protected:
    OpenGLContext &Context;
    ISource &Source;
    CPU &Cpu;
    GPU &Gpu;
    ITarget &Target;
    bool Running;

public:
    Pipeline(OpenGLContext &context, ISource &source, CPU &cpu, GPU &gpu, ITarget &target);
    Pipeline(const Pipeline &) = delete;
    virtual ~Pipeline();

    virtual void Start();
    void Stop();
};

#endif // Pipeline_HPP
