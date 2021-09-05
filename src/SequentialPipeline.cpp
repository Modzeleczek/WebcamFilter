#include "../include/SequentialPipeline.hpp"

SequentialPipeline::SequentialPipeline(ISource &source, InPlaceProcessor &ipp, ReturningProcessor &rp, ITarget &target) :
    SequentialSink(source, ipp, rp, target)
{}

SequentialPipeline::~SequentialPipeline() {}

void SequentialPipeline::Start()
{
    ReturningProcessor &RP = static_cast<ReturningProcessor&>(OOPP);
    Running = true;
    while (Running == true)
    {
        Source.DownloadFrame();
        IPP.ProcessFrame();
        RP.UploadFrame();
        RP.ProcessFrame();
        RP.DownloadFrame();
        Target.UploadFrame();
    }
}
