#ifndef Pipeline_HPP
#define Pipeline_HPP

#include "OpenGLContext.hpp"
#include "ISource.hpp"
#include "InPlaceProcessor.hpp"
#include "OutOfPlaceProcessor.hpp"
#include "ITarget.hpp"

class Pipeline
{
protected:
    OpenGLContext &Context;
    ISource &Source;
    InPlaceProcessor &IPP;
    OutOfPlaceProcessor &OOPP;
    ITarget &Target;
    bool Running;

public:
    Pipeline(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target);
    Pipeline(const Pipeline &) = delete;
    virtual ~Pipeline();

    virtual void Start();
    void Stop();
};

#endif // Pipeline_HPP
