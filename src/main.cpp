#include "../include/Webcam.hpp"
#include "../include/VideoLoopback.hpp"
#include "../include/GPU.hpp"
#include "../include/CPU.hpp"

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <signal.h>
#include <math.h>

#include <stdexcept>
#include <string.h>

GLFWwindow *window;
void SigintHandler(int signo);
void KeyPressed(GLFWwindow *window, int key, int scancode, int action, int mods);

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

    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("main; glfwInit");
    // tworzymy okno o szerokości WIDTH i wysokości HEIGHT; musi być stworzone dokładnie takie okno i musi ono być zmaksymalizowane podczas działania programu; w przeciwnym razie GPU nie renderuje
    window = glfwCreateWindow(width, height, "ESC, close or SIGINT to quit", NULL, NULL);
    if (window == NULL)
        throw std::runtime_error("main; glfwCreateWindow");

    // ustawiamy funkcję obsługi zdarzeń - wciśnięć przycisków klawiatury
    glfwSetKeyCallback(window, KeyPressed);

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        throw std::runtime_error(std::string("main; glewInit; ") + (const char*)glewGetErrorString(err));
    if (glewIsSupported("GL_VERSION_3_3") == 0)
        throw std::runtime_error("OpenGL 3.3 unsupported");

    Webcam webcam("/dev/video0", width, height);
    VideoLoopback videoLoopback("/dev/video2", width, height);
    CPU cpu(&webcam);
    GPU gpu(&webcam, &videoLoopback, "camera_shaders/rectangle.vert", "camera_shaders/identity.frag");

    webcam.StartStreaming();
    videoLoopback.StartStreaming();
    // rozpoczynamy pętlę główną
    while (glfwWindowShouldClose(window) == 0)
    {
        glfwPollEvents();

        if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
        {
            perror("main; sigprocmask SIG_BLOCK");
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        // w tym momencie bufor wejściowy jest w przestrzeni użytkownika, czyli nie jest w incoming queue ani w outgoing queue urządzenia wejściowego ani wyjściowego
        else 
        {
            webcam.DownloadFrame();
            cpu.ProcessFrame();
            gpu.ProcessFrame();
            videoLoopback.UploadFrame();
            if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1)
            {
                perror("main; sigprocmask SIG_UNBLOCK");
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }
    }
    webcam.StopStreaming();
    videoLoopback.StopStreaming();
    return 0;
}

void SigintHandler(int signo)
{
    (void)signo; // żeby gcc nie wyrzucał warninga, że parametr jest nieużywany
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    printf("received SIGINT\n");
}

void KeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode; (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
