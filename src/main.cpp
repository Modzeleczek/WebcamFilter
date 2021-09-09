#include "../include/Webcam.hpp"
#include "../include/GenericSource.hpp"
#include "../include/VideoLoopback.hpp"
#include "../include/Display.hpp"
#include "../include/OpenGLContext.hpp"
#include "../include/OutOfPlaceProcessor.hpp"
#include "../include/InPlaceProcessor.hpp"
#include "../include/SequentialPipeline.hpp"
#include "../include/ConcurrentPipeline.hpp"

#include <stdio.h>
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

int main()
{
    sigset_t set;
    if (signal(SIGINT, SigintHandler) == SIG_ERR)
        throw std::runtime_error(std::string("main; signal; ") + strerror(errno));
    if (sigemptyset(&set) == -1)
        throw std::runtime_error(std::string("main; sigemptyset; ") + strerror(errno));
    if (sigaddset(&set, SIGINT) == -1)
        throw std::runtime_error(std::string("main; sigaddset; ") + strerror(errno));

    const int width = 160 * 4; // * 1, 2, 4, 5, 8
    const int height = 120 * 4; // * 1, 2, 4, 5, 6
    OpenGLContext context(width, height);
    context.UseOnCurrentThread();

    Webcam source("/dev/video0", width, height);
    // GenericSource source(width, height);

    VideoLoopback target("/dev/video2", width, height);
    // Display target(context);

    InPlaceProcessor cpu(&source);
    ReturningProcessor gpu(&source, &target, "camera_shaders/rectangle.vert", "camera_shaders/solid_color.frag");
    // OutOfPlaceProcessor gpu(&source, "display_shaders/rectangle.vert", "display_shaders/binary.frag");

    source.StartStreaming();
    target.StartStreaming();
    ConcurrentPipeline p(source, cpu, gpu, target);
    // ConcurrentSink p(source, cpu, gpu, target);
    program = &p;
    program->Start();
    source.StopStreaming();
    target.StopStreaming();
    return 0;
}
