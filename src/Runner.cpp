#include "../include/Runner.hpp"

Runner::Runner(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp) :
    Context(context),
    Source(source),
    IPP(ipp),
    OOPP(oopp),
    Running(false)
{}

Runner::~Runner() {}

void Runner::Stop()
{
    Running = false;
}
