#include "../include/Runner.hpp"

Runner::Runner(ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target) :
    Source(source),
    IPP(ipp),
    OOPP(oopp),
    Running(false),
    Target(target)
{}

Runner::~Runner() {}

void Runner::Stop()
{
    Running = false;
}
