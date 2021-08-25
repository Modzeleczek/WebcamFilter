#include "../include/Webcam.hpp"
#include "../include/VideoLoopback.hpp"
#include "../include/GPU.hpp"
#include "../include/CPU.hpp"
#include "../include/Pipeline.hpp"
#include "../include/ConcurrentPipeline.hpp"

#include <stdio.h>
#include <signal.h>
#include <stdexcept>
#include <string.h>

Pipeline *program;
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
    Webcam webcam("/dev/video0", width, height);
    VideoLoopback videoLoopback("/dev/video2", width, height);
    OpenGLContext context(width, height);
    context.UseOnCurrentThread();
    CPU cpu(&webcam);
    GPU gpu(&webcam, &videoLoopback, "camera_shaders/rectangle.vert", "camera_shaders/identity.frag");

    webcam.StartStreaming();
    videoLoopback.StartStreaming();
    ConcurrentPipeline p(context, webcam, cpu, gpu, videoLoopback);
    program = &p;
    program->Start();
    webcam.StopStreaming();
    videoLoopback.StopStreaming();
    return 0;
}
