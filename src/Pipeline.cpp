#include "../include/Pipeline.hpp"

Pipeline::Pipeline(OpenGLContext &context, ISource &source, CPU &cpu, GPU &gpu, ITarget &target) :
    Context(context),
    Source(source),
    Cpu(cpu),
    Gpu(gpu),
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
        Cpu.ProcessFrame();
        Gpu.UploadFrame();
        Gpu.ProcessFrame();
        Gpu.DownloadFrame();
        Target.UploadFrame();
    }
}

void Pipeline::Stop()
{
    Running = false;
}
