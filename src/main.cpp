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
#include <pthread.h>
#include <unistd.h>

bool Running;
void SigintHandler(int signo);

pthread_mutex_t Mutex1, Mutex2;
pthread_cond_t Cond1, Cond2;
bool FrameReadyToBeProcessed = false, FrameReadyToBeDownloaded = false;

const int width = 160 * 4; // * 1, 2, 4, 5, 8
const int height = 120 * 4; // * 1, 2, 4, 5, 6

struct Items
{
    Webcam *w;
    CPU *c;
    GPU *g;
    VideoLoopback *v;
    GLFWwindow *win;
};

void* Download(void* a)
{
    Items *i = (Items*)a;
    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("Download; glfwInit");
    glfwMakeContextCurrent(i->win);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        throw std::runtime_error(std::string("Download; glewInit; ") + (const char*)glewGetErrorString(err));

    Webcam &webcam = *(i->w);
    CPU &cpu = *(i->c);
    GPU &gpu = *(i->g);
    webcam.StartStreaming();
    while (Running == true)
    {
        webcam.DownloadFrame();
        cpu.ProcessFrame();
        printf("1\n");

        pthread_mutex_lock(&Mutex1);
        FrameReadyToBeProcessed = false;
        gpu.UploadFrame();
        printf("2\n");
        FrameReadyToBeProcessed = true;
        pthread_cond_signal(&Cond1);
        pthread_mutex_unlock(&Mutex1);
    }
    webcam.StopStreaming();
    return NULL;
}

void* Process(void* a)
{
    Items *i = (Items*)a;
    GPU &gpu = *(i->g);
    while (Running == true)
    {
        pthread_mutex_lock(&Mutex1);
        if (FrameReadyToBeProcessed == false)
            pthread_cond_wait(&Cond1, &Mutex1);
        pthread_mutex_lock(&Mutex2);
        FrameReadyToBeDownloaded = false;
        gpu.ProcessFrame();
        printf("3\n");
        FrameReadyToBeDownloaded = true;
        pthread_cond_signal(&Cond2);
        pthread_mutex_unlock(&Mutex2);
        pthread_mutex_unlock(&Mutex1);
        usleep(5000);
    }
    return NULL;
}

void* Upload(void* a)
{
    Items *i = (Items*)a;
    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("Upload; glfwInit");
    glfwMakeContextCurrent(i->win);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        throw std::runtime_error(std::string("Upload; glewInit; ") + (const char*)glewGetErrorString(err));

    VideoLoopback &videoLoopback = *(i->v);
    GPU &gpu = *(i->g);
    videoLoopback.StartStreaming();
    while (Running == true)
    {
        pthread_mutex_lock(&Mutex2);
        if (FrameReadyToBeDownloaded == false)
            pthread_cond_wait(&Cond2, &Mutex2);
        gpu.DownloadFrame();
        printf("4\n");
        pthread_mutex_unlock(&Mutex2);

        videoLoopback.UploadFrame();
        printf("5\n");
    }
    videoLoopback.StopStreaming();
    return NULL;
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

    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("main; glfwInit");
    // tworzymy okno o szerokości WIDTH i wysokości HEIGHT; musi być stworzone dokładnie takie okno; w przeciwnym razie GPU nie renderuje; glfwCreateWindow oprócz okna tworzy też kontekst OpenGLa, który jest ustawiany w innych wątkach za pomocą glfwMakeContextCurrent
    GLFWwindow *window = glfwCreateWindow(width, height, "ESC, close or SIGINT to quit", NULL, NULL);
    if (window == NULL)
        throw std::runtime_error("main; glfwCreateWindow");
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        throw std::runtime_error(std::string("InitGLEW; glewInit; ") + (const char*)glewGetErrorString(err));
    if (glewIsSupported("GL_VERSION_3_3") == 0)
        throw std::runtime_error("OpenGL 3.3 unsupported");

    pthread_mutex_init(&Mutex1, NULL);
    pthread_mutex_init(&Mutex2, NULL);
    pthread_cond_init(&Cond1, NULL);
    pthread_cond_init(&Cond2, NULL);

    Running = true;

    Webcam webcam("/dev/video0", width, height);
    VideoLoopback videoLoopback("/dev/video2", width, height);
    CPU cpu(&webcam);
    GPU gpu(&webcam, &videoLoopback, "camera_shaders/rectangle.vert", "camera_shaders/identity.frag");

    Items i = { &webcam, &cpu, &gpu, &videoLoopback, window };

    pthread_t downloader, uploader;
    pthread_create(&downloader, NULL, Download, &i);
    pthread_create(&uploader, NULL, Upload, &i);

    Process(&i);

    pthread_join(downloader, NULL);
    pthread_join(uploader, NULL);

    glfwWindowShouldClose(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void SigintHandler(int signo)
{
    (void)signo; // żeby gcc nie wyrzucał warninga, że parametr jest nieużywany
    Running = false;
    printf("received SIGINT\n");
}
