#include "../include/Pipeline.hpp"

Pipeline::Pipeline(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target) :
    Context(context),
    Source(source),
    IPP(ipp),
    OOPP(oopp),
    Target(target),
    Running(false)
{}

Pipeline::~Pipeline() {}

void Pipeline::Start()
{
    Running = true;
    while (Running == true)
    {
        Source.DownloadFrame();
        IPP.ProcessFrame();
        OOPP.UploadFrame();
        OOPP.ProcessFrame();
        OOPP.DownloadFrame();
        Target.UploadFrame();
    }
}

void Pipeline::Stop()
{
    Running = false;
}
