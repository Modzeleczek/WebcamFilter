#include "../include/Parameters.hpp"
#include "../include/Webcam.hpp"
#include "../include/GenericSource.hpp"
#include "../include/VideoLoopback.hpp"
#include "../include/Display.hpp"
#include "../include/OpenGLContext.hpp"
#include "../include/OutOfPlaceProcessor.hpp"
#include "../include/InPlaceProcessor.hpp"
#include "../include/SequentialPipeline.hpp"
#include "../include/ConcurrentPipeline.hpp"

#include <signal.h>
#include <stdexcept>
#include <string.h>

Runner *program;
void SigintHandler(int signo)
{
    (void)signo; // żeby gcc nie wyrzucał warninga, że parametr jest nieużywany
    program->Stop();
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

    OpenGLContext context(p.Width, p.Height);
    context.UseOnCurrentThread();
    ISource *source;
    ITarget *target;
    OutOfPlaceProcessor *gpu;
    if (p.SourcePath == NULL)
        source = new GenericSource(p.Width, p.Height);
    else
    {
        source = new Webcam(p.SourcePath, p.Width, p.Height);
        ((Webcam*)source)->StartStreaming();
    }
    InPlaceProcessor cpu(source);
    if (p.TargetPath == NULL)
    {
        target = new Display(context);
        gpu = new OutOfPlaceProcessor(source, p.VSPath, p.FSPath);
        if (p.Concurrent == false)
            program = new SequentialSink(*source, cpu, *gpu, *target);
        else
            program = new ConcurrentSink(*source, cpu, *gpu, *target);
    }
    else
    {
        target = new VideoLoopback(p.TargetPath, p.Width, p.Height);
        gpu = new ReturningProcessor(source, target, p.VSPath, p.FSPath);
        if (p.Concurrent == false)
            program = new SequentialPipeline(*source, cpu, dynamic_cast<ReturningProcessor&>(*gpu), *target);
        else
            program = new ConcurrentPipeline(*source, cpu, dynamic_cast<ReturningProcessor&>(*gpu), *target);
        ((VideoLoopback*)target)->StartStreaming();
    }
    program->Start();
    delete source;
    delete target;
    delete gpu;
    delete program;
    return 0;
}
