#include "../include/ConcurrentPipeline.hpp"

#include <unistd.h>

ConcurrentPipeline::ConcurrentPipeline(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target) :
    ConcurrentSink(context, source, ipp, oopp),
    Target(target)
{
    pthread_mutex_init(&Mutex2, NULL);
    pthread_cond_init(&Cond2, NULL);
}

ConcurrentPipeline::~ConcurrentPipeline() {}

void ConcurrentPipeline::Start()
{
    FrameReadyForOOPP = false;
    FrameReadyForTarget = false;
    Running = true;
    pthread_t downloader, uploader;
    pthread_create(&downloader, NULL, Download, this);
    pthread_create(&uploader, NULL, Upload, this);
    Process();
    pthread_join(downloader, NULL);
    pthread_join(uploader, NULL);
}

#undef DEBUG
#ifdef DEBUG
    #include <stdio.h>
    #define operationInfo(number, letter) (printf("%i - %c\n", number, letter))
    #define threadEndInfo(name) (printf("%s thread ended\n", name))
#else
    #define operationInfo(number, letter) // no-operation
    #define threadEndInfo(name) // no-operation
#endif

void ConcurrentPipeline::Process()
{
    while (Running == true)
    {
        pthread_mutex_lock(&Mutex1);
        if (FrameReadyForOOPP == false)
            pthread_cond_wait(&Cond1, &Mutex1);
        operationInfo(2, 's');
        OOPP.UploadFrame();
        operationInfo(2, 'e');
        FrameReadyForOOPP = false;
        pthread_cond_signal(&Cond1); // jak wyżej
        pthread_mutex_unlock(&Mutex1);

        operationInfo(3, 's');
        OOPP.ProcessFrame();
        operationInfo(3, 'e');

        pthread_mutex_lock(&Mutex2);
        FrameReadyForTarget = false;
        operationInfo(4, 's');
        OOPP.DownloadFrame();
        operationInfo(4, 'e');
        FrameReadyForTarget = true;
        pthread_cond_signal(&Cond2);
        pthread_mutex_unlock(&Mutex2);
    }
    threadEndInfo("Process");
    // ustawiamy flagę i sygnalizujemy zmienną warunkową, aby wątek Download się nie zablokował
    FrameReadyForOOPP = false;
    pthread_cond_signal(&Cond1);
    // ustawiamy flagę i sygnalizujemy zmienną warunkową, aby wątek Upload się nie zablokował
    FrameReadyForTarget = true;
    pthread_cond_signal(&Cond2);
}

void* ConcurrentPipeline::Upload(void* caller)
{
    ConcurrentPipeline &mp = *((ConcurrentPipeline*)caller);
    while (mp.Running == true)
    {
        pthread_mutex_lock(&mp.Mutex2);
        if (mp.FrameReadyForTarget == false)
            pthread_cond_wait(&mp.Cond2, &mp.Mutex2);
        operationInfo(5, 's');
        mp.Target.UploadFrame();
        operationInfo(5, 'e');
        mp.FrameReadyForTarget = false;
        pthread_mutex_unlock(&mp.Mutex2);
    }
    threadEndInfo("Upload");
    return NULL;
}
