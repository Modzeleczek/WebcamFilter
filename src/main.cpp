#include "../include/Parameters.hpp"

#include "../include/Webcam.hpp"
#include "../include/GenericSource.hpp"

#include "../include/VideoLoopback.hpp"
#include "../include/Display.hpp"
#include "../include/Terminal.hpp"

#include "../include/OpenGLContext.hpp"
#include "../include/Uniform.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/OutOfPlaceProcessor.hpp"
#include "../include/YUYV422ReturningProcessor.hpp"
#include "../include/YReturningProcessor.hpp"

#include "../include/InPlaceProcessor.hpp"
#include "../include/SequentialPipeline.hpp"
#include "../include/ConcurrentPipeline.hpp"

#include <signal.h>
#include <stdexcept>
#include <string.h>
#include <array>

Runner *runner;
void SigintHandler(int signo)
{
    (void)signo; // żeby gcc nie wyrzucał warninga, że parametr jest nieużywany
    runner->Stop();
    printf("received SIGINT\n");
}

int main(int argc, char **argv)
{
    sigset_t set;
    if (signal(SIGINT, SigintHandler) == SIG_ERR)
        throw std::runtime_error(std::string("main; signal; ") + strerror(errno));
    if (sigemptyset(&set) == -1)
        throw std::runtime_error(std::string("main; sigemptyset; ") + strerror(errno));
    if (sigaddset(&set, SIGINT) == -1)
        throw std::runtime_error(std::string("main; sigaddset; ") + strerror(errno));

    Parameters p(argc, argv);

    OpenGLContext context(p.OutputWidth, p.OutputHeight);
    context.UseOnCurrentThread();
    ISource *source;
    ITarget *target;
    OutOfPlaceProcessor *gpu;

    if (p.SourceType == Parameters::SourceType::Generic)
        source = new GenericSource(p.InputWidth, p.InputHeight);
    else if (p.SourceType == Parameters::SourceType::Webcam)
    {
        source = new Webcam(p.SourcePath, p.InputWidth, p.InputHeight);
        ((Webcam*)source)->StartStreaming();
    }
    InPlaceProcessor cpu(*source);

    ShaderProgram shaderProgram(p.VSPath, p.FSPath);
    shaderProgram.UseOnCurrentThread();
    std::array<Uniform*, 3> uniforms =
    {
        shaderProgram.CreateUniform1Int("texture1"),
        shaderProgram.CreateUniform2Int("inputSize"),
        shaderProgram.CreateUniform2Int("scale")
    };
    ((Uniform2Int*)uniforms[1])->Set(p.InputWidth - 1, p.InputHeight - 1);
    ((Uniform2Int*)uniforms[2])->Set(p.InputWidth / p.OutputWidth, p.InputHeight / p.OutputHeight);

    if (p.TargetType == Parameters::TargetType::Display)
    {
        target = new Display(context);
        gpu = new OutOfPlaceProcessor(*source);
        if (p.Concurrent == false)
            runner = new SequentialSink(*source, cpu, *gpu, *target);
        else
            runner = new ConcurrentSink(*source, cpu, *gpu, *target);
    }
    else if (p.TargetType == Parameters::TargetType::V4L2Loopback)
    {
        target = new VideoLoopback(p.TargetPath, p.OutputWidth, p.OutputHeight);
        gpu = new YUYV422ReturningProcessor(*source, *target);
        if (p.Concurrent == false)
            runner = new SequentialPipeline(*source, cpu, dynamic_cast<ReturningProcessor&>(*gpu), *target);
        else
            runner = new ConcurrentPipeline(*source, cpu, dynamic_cast<ReturningProcessor&>(*gpu), *target);
        ((VideoLoopback*)target)->StartStreaming();
    }
    else // na pewno p.TargetType == Parameters::TargetType::Terminal
    {
        target = new Terminal(p.OutputWidth, p.OutputHeight);
        gpu = new YReturningProcessor(*source, *target);
        if (p.Concurrent == false)
            runner = new SequentialPipeline(*source, cpu, dynamic_cast<ReturningProcessor&>(*gpu), *target);
        else
            runner = new ConcurrentPipeline(*source, cpu, dynamic_cast<ReturningProcessor&>(*gpu), *target);
    }
    runner->Start();
    delete source;
    for (size_t i = 0; i < uniforms.size(); ++i)
        delete uniforms[i];
    delete target;
    delete gpu;
    delete runner;
    return 0;
}
