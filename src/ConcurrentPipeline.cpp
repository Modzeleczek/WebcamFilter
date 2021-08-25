#include "../include/ConcurrentPipeline.hpp"

// #define info(number, letter) (printf("%i - %c\n", number, letter))
#define info(number, letter) // no-operation

ConcurrentPipeline::ConcurrentPipeline(OpenGLContext &context, ISource &source, CPU &cpu, GPU &gpu, ITarget &target) :
    Pipeline(context, source, cpu, gpu, target)
{
    pthread_mutex_init(&Mutex1, NULL);
    pthread_mutex_init(&Mutex2, NULL);
    pthread_cond_init(&Cond1, NULL);
    pthread_cond_init(&Cond2, NULL);
}

ConcurrentPipeline::~ConcurrentPipeline() {}

void ConcurrentPipeline::Start()
{
    FrameReadyToBeProcessed = false;
    FrameReadyToBeDownloaded = false;
    Running = true;
    pthread_t downloader, uploader;
    pthread_create(&downloader, NULL, Download, this);
    pthread_create(&uploader, NULL, Upload, this);
    Process();
    pthread_join(downloader, NULL);
    pthread_join(uploader, NULL);
}

void* ConcurrentPipeline::Download(void* caller)
{
    ConcurrentPipeline &mp = *((ConcurrentPipeline*)caller);
    mp.Context.UseOnCurrentThread();
    while (mp.Running == true)
    {
        info(1, 's');
        mp.Source.DownloadFrame();
        mp.Cpu.ProcessFrame();
        info(1, 'e');

        pthread_mutex_lock(&mp.Mutex1);
        mp.FrameReadyToBeProcessed = false;
        info(2, 's');
        mp.Gpu.UploadFrame();
        info(2, 'e');
        mp.FrameReadyToBeProcessed = true;
        pthread_cond_signal(&mp.Cond1);
        pthread_mutex_unlock(&mp.Mutex1);
    }
    return NULL;
}

void ConcurrentPipeline::Process()
{
    while (Running == true)
    {
        pthread_mutex_lock(&Mutex1);
        if (FrameReadyToBeProcessed == false)
            pthread_cond_wait(&Cond1, &Mutex1);
        pthread_mutex_lock(&Mutex2);
        FrameReadyToBeDownloaded = false;
        info(3, 's');
        Gpu.ProcessFrame();
        info(3, 'e');
        FrameReadyToBeProcessed = false;
        FrameReadyToBeDownloaded = true;
        pthread_cond_signal(&Cond2);
        pthread_mutex_unlock(&Mutex2);
        pthread_mutex_unlock(&Mutex1);
    }
}

void* ConcurrentPipeline::Upload(void* caller)
{
    ConcurrentPipeline &mp = *((ConcurrentPipeline*)caller);
    mp.Context.UseOnCurrentThread();
    while (mp.Running == true)
    {
        pthread_mutex_lock(&mp.Mutex2);
        if (mp.FrameReadyToBeDownloaded == false)
            pthread_cond_wait(&mp.Cond2, &mp.Mutex2);
        info(4, 's');
        mp.Gpu.DownloadFrame();
        info(4, 'e');
        mp.FrameReadyToBeDownloaded = false;
        pthread_mutex_unlock(&mp.Mutex2);

        info(5, 's');
        mp.Target.UploadFrame();
        info(5, 'e');
    }
    return NULL;
}
