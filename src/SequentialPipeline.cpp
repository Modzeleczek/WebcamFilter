#include "../include/SequentialPipeline.hpp"

SequentialPipeline::SequentialPipeline(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target) :
    SequentialSink(context, source, ipp, oopp),
    Target(target)
{}

SequentialPipeline::~SequentialPipeline() {}

void SequentialPipeline::Start()
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
