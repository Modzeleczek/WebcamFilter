#ifndef Runner_HPP
#define Runner_HPP

#include "OpenGLContext.hpp"
#include "ISource.hpp"
#include "InPlaceProcessor.hpp"
#include "OutOfPlaceProcessor.hpp"
#include "ITarget.hpp"

class Runner // klasa abstrakcyjna
{
protected:
    OpenGLContext &Context;
    ISource &Source;
    InPlaceProcessor &IPP;
    OutOfPlaceProcessor &OOPP;
    bool Running;

protected:
    Runner(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp);
public:
    Runner(const Runner &) = delete;
    virtual ~Runner();

    virtual void Start() = 0;
    void Stop();
};

#endif // Runner_HPP
