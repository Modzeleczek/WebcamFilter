#ifndef Runner_HPP
#define Runner_HPP

#include "ISource.hpp"
#include "InPlaceProcessor.hpp"
#include "OutOfPlaceProcessor.hpp"
#include "ITarget.hpp"

class Runner // klasa abstrakcyjna
{
protected:
    ISource &Source;
    InPlaceProcessor &IPP;
    OutOfPlaceProcessor &OOPP;
    bool Running;
    ITarget &Target;

protected:
    Runner(ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target);
public:
    Runner(const Runner &) = delete;
    virtual ~Runner();

    virtual void Start() = 0;
    void Stop();
};

#endif // Runner_HPP
